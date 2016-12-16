#include "node.h"

static string client;

//-------------------------------------------------------------------------
void padLeft(string& str, const size_t num, const char paddingChar=' ')
{
	if (num > str.size())
		str.insert(0,num-str.size(),paddingChar);
}

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

		if (client == "parity")
		{
			// we have to use Infura
			headers = curl_slist_append(headers, "Infura-Ethereum-Preferred-Client: parity");
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
			curl_easy_setopt(curl, CURLOPT_URL,        "https:/""/mainnet.infura.io/WkavvX9Hk5tvp34LhN7W");

		} else if (client == "geth14")
		{
			// we have to use Infura to get an old version of geth
			headers = curl_slist_append(headers, "Infura-Ethereum-Preferred-Client: geth");
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
		return NULL;
	}

	return curl;
}

//-------------------------------------------------------------------------
void etherlib_init(const string& clientIn)
{
	client = clientIn;

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

	if (client == "geth14")
	{
		HIDE_FIELD(CBlock, "mixHash");
		HIDE_FIELD(CTransaction, "r");
		HIDE_FIELD(CTransaction, "s");
		HIDE_FIELD(CTransaction, "v");
		HIDE_FIELD(CReceipt, "logsBloom");
		HIDE_FIELD(CBlock, "nonce");
		HIDE_FIELD(CReceipt, "from");
		HIDE_FIELD(CReceipt, "root");
		HIDE_FIELD(CReceipt, "to");

	} else if (client == "parity")
	{
//		HIDE_FIELD(CBlock, "nonce");
//		HIDE_FIELD(CReceipt, "from");
//		HIDE_FIELD(CReceipt, "root");
//		HIDE_FIELD(CReceipt, "to");
	} else
	{
		// geth15 is default format
	}

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
string callRPC(const string& method, const string& params, const string& id, bool isJson=true)
{
	string thePost, result;

	thePost += "{";
	thePost +=  quote("jsonrpc") + ":"  + quote("2.0")  + ",";
	thePost +=  quote("method")  + ":"  + quote(method) + ",";
	thePost +=  quote("params")  + ":"  + params        + ",";
	thePost +=  quote("id")      + ":"  + quote(id);
	thePost += "}";

//outErr << "\n" << SFString('-',80) << "\n";
//outErr << thePost.c_str() << "\n";
//outErr << SFString('-',80) << "\n";
//outErr.Flush();

	curl_easy_setopt(getCurl(), CURLOPT_POSTFIELDS,    thePost.c_str());
	curl_easy_setopt(getCurl(), CURLOPT_POSTFIELDSIZE, thePost.length());

	curl_easy_setopt(getCurl(), CURLOPT_WRITEDATA,     &result);
	curl_easy_setopt(getCurl(), CURLOPT_WRITEFUNCTION, write_callback);

	CURLcode res = curl_easy_perform(getCurl());
	if (res != CURLE_OK)
	{
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		exit(0);
	}

	const char *s = result.c_str();
	if (isJson)
	{
		// The nodes return {jsonrpc:2.0,result:{...the block},id:1}, so if it's there, strip it off
		SFString ret = s;
		if (countOf('{',ret)>1)
		{
			char *start = strstr(&s[1],"{");
//XX_A(start);
			start[strlen(start)-1] = '\0';
//XX_A(start);
			ret = start;
//XX_A(ret);
			nextTokenClearReverse(ret,'}');
//XX_A(ret);
			return (const char*)ret;

		} else
		{
			outErr << "Invalid json result: " << s << ". Quitting...\n";outErr.Flush();
			exit(0);
		}
//outScreen << "\n" << SFString('-',80) << "\n";
//outScreen << thePost.c_str() << "\n";
//outScreen << SFString('-',60) << "\n";
//outScreen << ret << "\n";
//outScreen.Flush();
	}
	return "";
}

//-------------------------------------------------------------------------
bool getObjectViaRPC(CBaseNode &node, const string& method, const string& params, const string& id)
{
	string ret = callRPC(method, params, id);
	node.parseJson((char*)ret.c_str());
	return true;
}

//-------------------------------------------------------------------------
string getLatestBlock(const string& id)
{
//	curl -X POST --data '{"jsonrpc":"2.0","method":"eth_getBlockByNumber","params":["0x1b4", true],"id":1}'
	return callRPC("web3_clientVersion", "[]", id, false);
}

#define getBinPath(a) getBinaryPath("",num)

//-------------------------------------------------------------------------
bool getBlock(CBlock& block, const string& numIn, const string& id)
{
	SFString num = (const char*)numIn.c_str();
	if (SFos::fileExists(getBinPath(num)))
	{
		UNHIDE_FIELD(CTransaction, "receipt");
		UNHIDE_FIELD(CTransaction, "traces");
		return readOneBlock_fromBinary(block, getBinPath(num));

	} else
	{
		HIDE_FIELD(CTransaction, "receipt");
		HIDE_FIELD(CTransaction, "traces");
		getObjectViaRPC(block, "eth_getBlockByNumber", "["+quote(numIn)+",true]", id);
		// if there are no transactions, we're done
		if (!block.transactions.getCount())
			return true;

		// There are transactions, but we only have the hashes. Pick up the rest of the transaction
		for (int i=0;i<block.transactions.getCount();i++)
		{
			UNHIDE_FIELD(CTransaction, "receipt");
			UNHIDE_FIELD(CTransaction, "traces");
			CReceipt receipt;
			getReceipt(receipt, string((const char*)block.transactions[i].hash), string((const char*)block.number) + "." + to_string(i));
			block.transactions[i].receipt = receipt;
//			CTrace traces;
//			getTraces(traces, string((const char*)block.transactions[i].hash), string((const char*)block.number) + "." + to_string(i));
//			block.transactions[i].trace = traces;
		}
	}
	return true;
}

//-------------------------------------------------------------------------
bool getBlock(CBlock& block, int num, const string& id)
{
	return getBlock(block,to_string(num),id);
}

//-------------------------------------------------------------------------
bool getTransaction(CTransaction& trans, const string& hash, const string& id)
{
	string h = hash.substr(2,hash.length());padLeft(h,64,'0');
	getObjectViaRPC(trans, "eth_getTransactionByHash", "[\"0x" + h +"\"]", id);
	return true;
}

//-------------------------------------------------------------------------
bool getReceipt(CReceipt& receipt, const string& hash, const string& id)
{
	string h = hash.substr(2,hash.length());padLeft(h,64,'0');
	getObjectViaRPC(receipt, "eth_getTransactionReceipt", "[\"0x" + h + "\"]", id);
	return true;
}

//-------------------------------------------------------------------------
bool getTraces(CTrace& traces, const string& hash, const string& id)
{
	string h = hash.substr(2,hash.length());padLeft(h,64,'0');
	getObjectViaRPC(traces, "debug_traceTransaction", "[\"0x" + h +"\"]", id);
	return true;
}

//-------------------------------------------------------------------------
string getClientVersion(const string& id)
{
	return callRPC("web3_clientVersion", "[]", id, false);
}

//-------------------------------------------------------------------------
string compileSolidity(const string& sol, const string& id)
{
	return callRPC("eth_compileSolidity", "[\"contract test { function multiply(uint a) returns(uint d) {   return a * 7;   } }\"]", id, false);
}

//-------------------------------------------------------------------------
bool getCode(CAccount& acct, const string& addr, const string& id)
{
	string a = addr.substr(2,addr.length());padLeft(a,40,'0');
	string ret = callRPC("eth_getCode", "[\"0x" + a +"\",latest]", id);
	return ret.length()!=0;
}

//-------------------------------------------------------------------------
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	string result;
	int cnt=0;
	for (int i=0;i<nmemb;i++)
	{
		if (ptr[i]!='\n' && ptr[i]!='\r' && ptr[i]!=' ' && ptr[i]!='\"')
		{
			result += ptr[i];
			cnt++;
		}
	}
	result[cnt] = '\0';

	// store it away in the caller's string
	(*(string*)userdata) += result;

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
void verifyOneBlock(const SFString& reference, const SFString& proposal)
{
	SFString source = asciiFileToString(reference);
	SFString dest   = asciiFileToString(proposal);
	source.Replace(dest,"");
	if (!source.IsEmpty())
	{
		outErr << "File: " << reference << " file is different to " << proposal << "\n";
		outErr << "---------source:\n";
		outErr << reference << "\n" << asciiFileToString( reference ).Substitute("\n"," ") << "\n";
		outErr.Flush();
		stringToAsciiFile("./dir1/file.json", source);
		outErr << "---------dest:\n";
		outErr << proposal << "\n" << asciiFileToString( proposal   ).Substitute("\n"," ") << "\n";
		outErr.Flush();
		stringToAsciiFile("./dir2/file.json", dest);
		exit(0);
	}
}


//-------------------------------------------------------------------------
SFString getStoragePath(const SFString& pathIn, const SFString& num)
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
	SFString full = padLeft(num,9,'0');
	char fileName[200];
	bzero(fileName,sizeof(fileName));
	sprintf(fileName, (const char*)(path+loc), (const char*)full.Left(2), (const char*)full.Mid(2,2), (const char*)full.Mid(4,2), (const char*)full);
	return &fileName[0];
}

//-------------------------------------------------------------------------
SFString getBinaryPath(const SFString& path, const SFString& num)
{
	SFString ret = getStoragePath(path,num);
	ret.Replace(".json",".bin");
	ret.Replace("/00.1/","/blocks/00/");
	ret.Replace("/00/",  "/blocks/00/");
	return ret;
}
