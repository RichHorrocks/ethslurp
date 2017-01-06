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

//outScreen << "\n" << SFString('-',80) << "\n";
//outScreen << thePost << "\n";
//outScreen << SFString('-',60) << "\n";
//outScreen << result << "\n";
//outScreen.Flush();
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

//-------------------------------------------------------------------------
bool getBlock(CBlock& block, SFUint32 num, const SFString& id)
{
	if (source == "binary" || source == "nonemp")
	{
		UNHIDE_FIELD(CTransaction, "receipt");
		UNHIDE_FIELD(CTransaction, "traces");
		return readOneBlock_fromBinary(block, getBinaryFilename1(num));
	}

	if (source == "json")
	{
		UNHIDE_FIELD(CTransaction, "receipt");
		UNHIDE_FIELD(CTransaction, "traces");
		return readOneBlock_fromJson(block, getJsonFilename1(num));

	}

	HIDE_FIELD(CTransaction, "receipt");
	HIDE_FIELD(CTransaction, "traces");
	getObjectViaRPC(block, "eth_getBlockByNumber", "["+quote(asString(num))+",true]", id);

	// If there are no transactions, we're done
	if (!block.transactions.getCount())
	{
		// We only write binary if there are transactions
		//writeToBinary(block, getBinaryFilename1(num));
		writeToJson(block, getJsonFilename1(num)); //We've stopped writing JSON for now because of disc space
		return true;
	}

	// We have the transactions, but we also want the receipts
	for (int i=0;i<block.transactions.getCount();i++)
	{
		UNHIDE_FIELD(CTransaction, "receipt");
		CReceipt receipt;
		getReceipt(receipt, SFString((const char*)block.transactions[i].hash), SFString((const char*)block.number) + "." + asString(i));
		block.transactions[i].receipt = receipt;
//		UNHIDE_FIELD(CTransaction, "traces");
//		CTrace traces;
//		getTraces(traces, string((const char*)block.transactions[i].hash), string((const char*)block.number) + "." + asString(i));
//		block.transactions[i].trace = traces;
	}

	writeToBinary(block, getBinaryFilename1(num));
	writeToJson(block, getJsonFilename1(num)); //We've stopped writing JSON for now because of disc space
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
	SFString ret = callRPC("web3_clientVersion", "[]", id, false);
	CRPCResult generic;
	generic.parseJson((char *)(const char*)ret);
	return generic.result;
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
	if (SFos::establishFolder(fileName))
	{
		std::ofstream out(fileName);
		SFString fmt = node.Format();
		fmt.ReplaceAll("\"to\": \"0x\"","\"to\": null");
		out << fmt << "\n";
		out.close();
	}
}

//-----------------------------------------------------------------------
void writeToBinary(const CBaseNode& node, const SFString& fileName)
{
	if (SFos::establishFolder(fileName))
	{
		SFArchive archive(FALSE, curVersion);
		if (archive.Lock(fileName, binaryWriteCreate, LOCK_CREATE))
		{
			((CBlock *)&node)->Serialize(archive);
			archive.Close();
		}
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
		outErr << "File: " << getJsonFilename1(toLong(block.number)) << " file is different to " << path << "\n";
		outErr << "---------source:\n";
		outErr << getJsonFilename1(toLong(block.number)) << "\n" << source.Substitute("\n"," ") << "\n";
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

static SFString storagePath;
//-------------------------------------------------------------------------
void setStorageRoot(const SFString& path)
{
	storagePath = path;
	if (!storagePath.endsWith('/'))
		storagePath += "/";
	SFos::establishFolder(storagePath);
}

//-------------------------------------------------------------------------
static SFString getFilename_local(SFUint32 numIn, bool asPath, bool asJson)
{
	if (storagePath.IsEmpty())
	{
		outScreen << "You must set the storage path with setStorageRoot(path). Quitting...\n";
		exit(0);
	}

	char ret[512];
	bzero(ret,sizeof(ret));

	SFString num = padLeft(asString(numIn),9,'0');
	SFString fmt = (asPath ? "%s/%s/%s/" : "%s/%s/%s/%s");
	SFString fn  = (asPath ? "" : num + (asJson ? ".json" : ".bin"));

	sprintf(ret, (const char*)(storagePath+fmt), (const char*)num.Left(2), (const char*)num.Mid(2,2), (const char*)num.Mid(4,2), (const char*)fn);
	return ret;
}

//-------------------------------------------------------------------------
SFString getJsonFilename1(SFUint32 num)
{
	return getFilename_local(num, false, true);
}

//-------------------------------------------------------------------------
SFString getBinaryFilename1(SFUint32 num)
{
	SFString ret = getFilename_local(num, false, false);
	ret.Replace("/00/",  "/blocks/00/"); // can't use Substitute because it will change them all
	return ret;
}

//-------------------------------------------------------------------------
SFString getJsonPath1(SFUint32 num)
{
	return getFilename_local(num, true, true);
}

//-------------------------------------------------------------------------
SFString getBinaryPath1(SFUint32 num)
{
	SFString ret = getFilename_local(num, true, false);
	ret.Replace("/00/",  "/blocks/00/"); // can't use Substitute because it will change them all
	return ret;
}

SFString fullBlockIndex = getHomeFolder()+".ethslurp/scraper/fullBlocks.bin";
//-------------------------------------------------------------------------
bool forEveryBlockOnDisc(BLOCKVISITFUNC func, void *data, SFUint32 start, SFUint32 count)
{
	if (!func)
		return false;

	// Read every block from number start to start+count
	for (SFUint32 i=start;i<start+count;i++)
	{
		CBlock block;
		if (getBlock(block,i+start))
			if (!(*func)(block, data))
				return false;
	}
	return true;
}

//-------------------------------------------------------------------------
bool forEveryEmptyBlockOnDisc(BLOCKVISITFUNC func, void *data, SFUint32 start, SFUint32 count)
{
	if (!func)
		return false;

	// Read the non-empty block index file and spit it out only empty blocks
	FILE *fp = fopen(fullBlockIndex, binaryReadOnly);
	if (fp)
	{
		SFUint32 nItems = SFos::fileSize(fullBlockIndex) / sizeof(SFUint32) + 1;
		SFUint32 *contents = new SFUint32[nItems];
		fread(&contents[1], nItems-1, sizeof(SFUint32), fp);
		contents[0]=0;
		SFUint32 cnt=start;
		for (int i=1;i<nItems;i++)
		{
			while (cnt<contents[i])
			{
				CBlock block;
				if (getBlock(block,cnt))
					if (!(*func)(block, data))
						return false;
				cnt++;
			}
			cnt++;
		}
	}
	return true;
}

//-------------------------------------------------------------------------
bool forEveryNonEmptyBlockOnDisc(BLOCKVISITFUNC func, void *data, SFUint32 start, SFUint32 count)
{
	if (!func)
		return false;

	// Read the non-empty block index file and spit it out only non-empty blocks
	FILE *fp = fopen(fullBlockIndex, binaryReadOnly);
	if (fp)
	{
		SFUint32 nItems = SFos::fileSize(fullBlockIndex) / sizeof(SFUint32);
		SFUint32 *contents = new SFUint32[nItems];
		fread(contents, nItems, sizeof(SFUint32), fp);
		for (int i=0;i<nItems;i++)
		{
			if (inRange(contents[i], start, start+count-1))
			{
				CBlock block;
				if (getBlock(block,contents[i]))
					if (!(*func)(block, data))
						return false;
			}
		}
		delete [] contents;
		fclose(fp);
	}
	return true;
}

#define CLEANUP(ret) { if (blocks) delete [] blocks; if (trans) delete [] trans; if (fpBlocks) fclose(fpBlocks); if (fpTrans) fclose(fpTrans); return ret; }

//--------------------------------------------------------------------------
bool forEveryNonEmptyBlockInMemory(BLOCKVISITFUNC func, void *data, SFUint32 start, SFUint32 count)
{
	CMiniBlock *blocks    = NULL;
	FILE       *fpBlocks  = NULL;
	SFString    blockFile = getHomeFolder() + ".ethslurp/scraper/allBlocks.bin";

	CMiniTrans *trans     = NULL;
	FILE       *fpTrans   = NULL;
	SFString    transFile = getHomeFolder() + ".ethslurp/scraper/allTrans.bin";

	// First, we see if we can allocation enough space for the mini-block database
	SFUint32 nBlocks   = SFos::fileSize(blockFile) / sizeof(CMiniBlock);
	blocks = new CMiniBlock[nBlocks];
	if (!blocks)
	{
		cerr << "Could not allocate memory for the blocks (size needed: " << nBlocks << ").\n";
		CLEANUP(false);
	}
	bzero(blocks, sizeof(CMiniBlock)*(nBlocks));

	// Next, we try to open the mini-block database
	fpBlocks = fopen((const char*)blockFile, binaryReadOnly);
	if (!fpBlocks)
	{
		cerr << "Could not open the mini-block database: " << blockFile << ".\n";
		CLEANUP(false);
	}

	// Read the entire mini-block database into memory in one chunk
	size_t nRead = fread(blocks, sizeof(CMiniBlock), nBlocks, fpBlocks);
	if (nRead != nBlocks)
	{
		cerr << "Error encountered reading mini-blocks database.\n Quitting...";
		CLEANUP(false);
	}

	// Now, we see if we can allocation enough space for the mini-transaction database
	SFUint32 nTrans   = SFos::fileSize(transFile) / sizeof(CMiniTrans);
	trans = new CMiniTrans[nTrans];
	if (!trans)
	{
		cerr << "Could not allocate memory for the transactions (size needed: " << nTrans << ").\n";
		CLEANUP(false);
	}
	bzero(trans, sizeof(CMiniTrans)*(nTrans));

	// Next, we try to open the mini-transaction database
	fpTrans = fopen((const char*)transFile, binaryReadOnly);
	if (!fpTrans)
	{
		cerr << "Could not open the mini-transaction database: " << transFile << ".\n";
		CLEANUP(false);
	}

	// Read the entire mini-transaction database into memory in one chunk
	nRead = fread(trans, sizeof(CMiniTrans), nTrans, fpTrans);
	if (nRead != nTrans)
	{
		cerr << "Error encountered reading mini-transactions database.\n Quitting...";
		CLEANUP(false);
	}

	bool done=false;
	for (int i=0;i<nBlocks&&!done;i++)
	{
		if (inRange(blocks[i].number, start, start+count-1))
		{
			CBlock block;
			blocks[i].toBlock(block);
			for (SFUint32 tr=block.firstTrans;tr< block.firstTrans+block.nTrans; tr++)
			{
				CTransaction tt;
				trans[tr].toTrans(tt);
				block.transactions[block.transactions.getCount()] = tt;
			}
			if (!(*func)(block, data))
				CLEANUP(false);

		} else if (blocks[i].number >= start+count)
		{
			done=true;
		}
	}

	CLEANUP(true);
}

//-------------------------------------------------------------------------
bool forEveryTransaction(TRANSVISITFUNC func, void *data, SFUint32 start, SFUint32 count)
{
	if (!func)
		return false;
	return true;
}

//-------------------------------------------------------------------------
bool forEveryTransactionTo(TRANSVISITFUNC func, void *data, SFUint32 start, SFUint32 count)
{
	if (!func)
		return false;
	return true;
}

//-------------------------------------------------------------------------
bool forEveryTransactionFrom(TRANSVISITFUNC func, void *data, SFUint32 start, SFUint32 count)
{
	if (!func)
		return false;
	return true;
}
