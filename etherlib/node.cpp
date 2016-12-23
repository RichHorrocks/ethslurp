#include "node.h"

static SFString source;

//-------------------------------------------------------------------------
CURL *getCurl(bool cleanup=false)
{
	static CURL *curl = NULL;
	static struct curl_slist *headers = NULL;
	if (!curl)
	{
		curl = curl_easy_init();
		if (!curl)
		{
			fprintf(stderr, "Curl failed to initialize. Quitting...\n");
			exit(0);
		}

		headers = curl_slist_append(headers, "Content-Type: application/json");

		if (source == "infura")
		{
			// we have to use Infura
			headers = curl_slist_append(headers, "Infura-Ethereum-Preferred-Client: parity");
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
			curl_easy_setopt(curl, CURLOPT_URL,        "https:/""/mainnet.infura.io/WkavvX9Hk5tvp34LhN7W");

		} else
		{
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
			curl_easy_setopt(curl, CURLOPT_URL,        "http:/""/localhost:8545");
		}

	} else if (cleanup)
	{
		if (headers)
			curl_slist_free_all(headers);
		if (curl)
			curl_easy_cleanup(curl);
		headers = NULL;
		curl = NULL;
		return NULL;
	}

	return curl;
}

//-------------------------------------------------------------------------
void setDataSource(const SFString& sourceIn)
{
	source = sourceIn;
}

//-------------------------------------------------------------------------
void etherlib_init(const SFString& sourceIn)
{
	source = sourceIn;

	CBlock       :: registerClass();
	CTransaction :: registerClass();
	CReceipt     :: registerClass();
	CTrace       :: registerClass();
	CLogEntry    :: registerClass();
	CStructLog   :: registerClass();

	HIDE_FIELD(CBlock,       "author");
	HIDE_FIELD(CBlock,       "sealFields");
	HIDE_FIELD(CTransaction, "isError");
	HIDE_FIELD(CTransaction, "isInternalTx");
	HIDE_FIELD(CTransaction, "toContract");
	HIDE_FIELD(CTransaction, "timeStamp");
	HIDE_FIELD(CTransaction, "date");
	HIDE_FIELD(CTransaction, "ether");
	UNHIDE_FIELD(CTransaction, "r");
	UNHIDE_FIELD(CTransaction, "s");
	UNHIDE_FIELD(CTransaction, "v");

	// initialize curl
	getCurl();
}

//-------------------------------------------------------------------------
void etherlib_cleanup(void)
{
	// cleanup curl
	getCurl(true);
}

extern size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
//-------------------------------------------------------------------------
// Use 'curl' to make an arbitrary rpc call
//-------------------------------------------------------------------------
SFString callRPC(const SFString& method, const SFString& params, const SFString& id, bool isJson=true)
{
	SFString thePost, result;

	thePost += "{";
	thePost +=  quote("jsonrpc") + ":"  + quote("2.0")  + ",";
	thePost +=  quote("method")  + ":"  + quote(method) + ",";
	thePost +=  quote("params")  + ":"  + params        + ",";
	thePost +=  quote("id")      + ":"  + quote(id);
	thePost += "}";

//outErr << "\n" << SFString('-',80) << "\n";
//outErr << thePost << "\n";
//outErr << SFString('-',80) << "\n";
//outErr.Flush();

	curl_easy_setopt(getCurl(), CURLOPT_POSTFIELDS,    (const char*)thePost);
	curl_easy_setopt(getCurl(), CURLOPT_POSTFIELDSIZE, thePost.GetLength());

	curl_easy_setopt(getCurl(), CURLOPT_WRITEDATA,     &result);
	curl_easy_setopt(getCurl(), CURLOPT_WRITEFUNCTION, write_callback);

	CURLcode res = curl_easy_perform(getCurl());
	if (res != CURLE_OK)
	{
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		exit(0);
	}

	if (isJson)
	{
		// The nodes return {jsonrpc:2.0,result:{...the block},id:1}, so if it's there, strip it off
		if (countOf('{',result)>1)
		{
			result = result.Mid(1,result.GetLength());
			result.ReplaceReverse("}","");
			nextTokenClearReverse(result,'}');

		} else
		{
			outErr << "Invalid json result: " << result << ". Quitting...\n";outErr.Flush();
			exit(0);
		}
//outScreen << "\n" << SFString('-',80) << "\n";
//outScreen << thePost << "\n";
//outScreen << SFString('-',60) << "\n";
//outScreen << ret << "\n";
//outScreen.Flush();
	}
	return result;
}

//-------------------------------------------------------------------------
bool getObjectViaRPC(CBaseNode &node, const SFString& method, const SFString& params, const SFString& id)
{
	SFString ret = callRPC(method, params, id);
	node.parseJson((char *)(const char*)ret);
	return true;
}

//-------------------------------------------------------------------------
SFUint32 getLatestBlock(const SFString& id)
{
	CBlock block;
	getObjectViaRPC(block, "eth_getBlockByNumber", "[\"latest\",true]", id);
	SFString n = block.number;
	SFUint32 ret = toLong(n);
	if (n.startsWith("0x"))
		ret = hex2Long(n);
	return ret;
}

#define getBinPath(a) getBinaryPath("",num)
#define getStorePath(a) getStoragePath("",num)

//-------------------------------------------------------------------------
bool getBlock(CBlock& block, SFUint32 num, const SFString& id)
{
	if ((source == "json" || source == "binary") && SFos::fileExists(getBinPath(num)))
	{
		UNHIDE_FIELD(CTransaction, "receipt");
		UNHIDE_FIELD(CTransaction, "traces");
		if (source == "json")
			return readOneBlock_fromJson(block, getStorePath(num));
		return readOneBlock_fromBinary(block, getBinPath(num));

	} else
	{
		HIDE_FIELD(CTransaction, "receipt");
		HIDE_FIELD(CTransaction, "traces");
		getObjectViaRPC(block, "eth_getBlockByNumber", "["+quote(asString(num))+",true]", id);
		// if there are no transactions, we're done
		if (!block.transactions.getCount())
		{
			writeToBinary(block, getBinPath(num));
			return true;
		}

		// There are transactions, but we only have the hashes. Pick up the rest of the transaction
		for (int i=0;i<block.transactions.getCount();i++)
		{
			UNHIDE_FIELD(CTransaction, "receipt");
			UNHIDE_FIELD(CTransaction, "traces");
			CReceipt receipt;
			getReceipt(receipt, SFString((const char*)block.transactions[i].hash), SFString((const char*)block.number) + "." + asString(i));
			block.transactions[i].receipt = receipt;
//			CTrace traces;
//			getTraces(traces, string((const char*)block.transactions[i].hash), string((const char*)block.number) + "." + asString(i));
//			block.transactions[i].trace = traces;
		}
		writeToBinary(block, getBinPath(num));
	}

	return true;
}

//-------------------------------------------------------------------------
bool getTransaction(CTransaction& trans, const SFString& hash, const SFString& id)
{
	SFString h = hash.Mid(2,hash.GetLength());padLeft(h,64,'0');
	getObjectViaRPC(trans, "eth_getTransactionByHash", "[\"0x" + h +"\"]", id);
	return true;
}

//-------------------------------------------------------------------------
bool getReceipt(CReceipt& receipt, const SFString& hash, const SFString& id)
{
	SFString h = hash.Mid(2,hash.GetLength());padLeft(h,64,'0');
	getObjectViaRPC(receipt, "eth_getTransactionReceipt", "[\"0x" + h + "\"]", id);
	return true;
}

//-------------------------------------------------------------------------
bool getTraces(CTrace& traces, const SFString& hash, const SFString& id)
{
	SFString h = hash.Mid(2,hash.GetLength());padLeft(h,64,'0');
	getObjectViaRPC(traces, "debug_traceTransaction", "[\"0x" + h +"\"]", id);
	return true;
}

//-------------------------------------------------------------------------
SFString getClientVersion(const SFString& id)
{
	return callRPC("web3_clientVersion", "[]", id, false);
}

//-------------------------------------------------------------------------
bool getCode(CAccount& acct, const SFString& addr, const SFString& id)
{
	SFString a = addr.Mid(2,addr.GetLength());padLeft(a,40,'0');
	SFString ret = callRPC("eth_getCode", "[\"0x" + a +"\",latest]", id);
	return ret.GetLength()!=0;
}

//-------------------------------------------------------------------------
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	unsigned long pos = 0;
	for (unsigned long i=0;i<(nmemb*size);i++)
	{
		if (ptr[i]!='\n' && ptr[i]!='\r' && ptr[i]!=' ' && ptr[i]!='\"')
			ptr[pos++] = ptr[i];
	}
	ptr[pos] = '\0';

	// store it away in the caller's string
	(*(SFString*)userdata) += ptr;

	// we handeled everything, tell curl to keep going
	return size*nmemb;
}

//-----------------------------------------------------------------------
void writeToJson(const CBaseNode& node, const SFString& fileName)
{
	std::ofstream out(fileName);
	SFString fmt = node.Format();
	fmt.ReplaceAll("\"to\": \"0x\"","\"to\": null");
	out << fmt << "\n";
	out.close();
}

//-----------------------------------------------------------------------
void writeToBinary(const CBaseNode& node, const SFString& fileName)
{
	SFArchive archive(FALSE, curVersion);
	if (archive.Lock(fileName, binaryWriteCreate, LOCK_CREATE))
	{
		((CBlock *)&node)->Serialize(archive);
		archive.Close();
	}
}

//-----------------------------------------------------------------------
SFBool readOneBlock_fromBinary(CBlock& block, const SFString& fileName)
{
	SFArchive archive(TRUE, curVersion);
	if (archive.Lock(fileName, binaryReadOnly, LOCK_NOWAIT))
	{
		block.Serialize(archive);
		archive.Close();
		return !block.number.IsEmpty();
	}
	return FALSE;
}

//-----------------------------------------------------------------------
SFBool readOneBlock_fromJson(CBlock& block, const SFString& fileName)
{
	SFString contents = asciiFileToString(fileName);
	if (contents.Contains("null"))
	{
		contents.ReplaceAll("null", "\"0x\"");
		stringToAsciiFile(fileName, contents);
	}
	if (!contents.endsWith('\n'))
	{
		stringToAsciiFile(fileName, contents+"\n");
	}
	char *p = cleanUpJson((char *)(const char*)contents);
	SFInt32 nFields=0;
	block.parseJson(p,nFields);
	return nFields;
}

//-----------------------------------------------------------------------
SFBool verifyBlock(const CBlock& block, const SFString& path, SFBool fail)
{
	SFString source = block.Format()+"\n";
	source.ReplaceAll("\"to\": \"0x\"","\"to\": null");
	SFString dest   = asciiFileToString(path);
	source.Replace(dest,"");
	if (!source.IsEmpty() && fail)
	{
		outErr << "File: " << getStoragePath("",toLong(block.number)) << " file is different to " << path << "\n";
		outErr << "---------source:\n";
		outErr << getStoragePath("",toLong(block.number)) << "\n" << source.Substitute("\n"," ") << "\n";
		outErr.Flush();
		stringToAsciiFile("./dir1/file.json", source);
		outErr << "---------dest:\n";
		outErr << path << "\n" << dest.Substitute("\n"," ") << "\n";
		outErr.Flush();
		stringToAsciiFile("./dir2/file.json", dest);
		exit(0);

	} else if (!source.IsEmpty())
	{
		outErr << "Block " << block.number << " differs\n";
		outErr.Flush();

	} else if (!fail)
	{
		outErr << "Block " << block.number << " checks\n";
		outErr.Flush();
	}

	return TRUE;
}

//-------------------------------------------------------------------------
SFString getStoragePath(const SFString& pathIn, SFUint32 num)
{
	static SFString lastPath;
	SFString path = pathIn;
	if (path.IsEmpty())
		path = lastPath;
	else
		lastPath = path;

	if (path.IsEmpty())
	{
		// If it's still empty, we're in trouble.
		outScreen << "You must set a path first. Quitting...\n";
		exit(0);
	}

	SFString loc = "%s/%s/%s/%s.json";
	SFString full = padLeft(asString(num),9,'0');
	char fileName[200];
	bzero(fileName,sizeof(fileName));
	sprintf(fileName, (const char*)(path+loc), (const char*)full.Left(2), (const char*)full.Mid(2,2), (const char*)full.Mid(4,2), (const char*)full);
	return &fileName[0];
}

//-------------------------------------------------------------------------
SFString getBinaryPath(const SFString& path, SFUint32 num)
{
	SFString ret = getStoragePath(path,num);
	ret.Replace(".json",".bin");
	ret.Replace("/00.1/","/blocks/00/");
	ret.Replace("/00/",  "/blocks/00/");
	return ret;
}
