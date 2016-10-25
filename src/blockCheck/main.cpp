#define STEP_3
#define PART_A
#include <iostream>
#include <fstream>
#include <string>
#include <curl/curl.h>
using namespace std;
//-------------------------------------------------------------------------
#define max(a,b) ((a>b)?a:b)
extern void   curl_init          (void);
extern void   curl_clean         (void);
extern string getBlock           (long blockNum);

#include "etherlib.h"
#include "stdio.h"
class CBlockSpan
{
public:
	long start;
	long stop;
	CBlockSpan(void) { start=stop=0; }
};
CBlockSpan spans[1000000];
long nSpans=0;

#ifdef STEP_3
int main(int argc, const char *argv[])
{
	FILE *fp = fopen("./file.bin",asciiReadOnly);
	if (fp)
	{
		fread(&nSpans,sizeof(long),1,fp);
		cout << nSpans << "\n";
		fread(spans,sizeof(long)*nSpans,1,fp);
		fclose(fp);
	}
	for (int i=0;i<nSpans;i++)
	{
		cout << i << ": " << spans[i].start << " : " << spans[i].stop << "\n"; cout.flush();
		if (spans[i].start==0&&spans[i].stop==0)
			getchar();
	}
}

#endif





#ifdef STEP_2
int main(int argc, const char *argv[])
{
	SFString contents = asciiFileToString("./blocks_with_transactions.txt");
	while (!contents.IsEmpty())
	{
		spans[nSpans].start = toLong(nextTokenClear(contents,'\n'));
		spans[nSpans].stop  = toLong(nextTokenClear(contents,'\n'));
		nSpans++;
		cout << nSpans << "\r";cout.flush();
	}
	cout << nSpans << "\n";cout.flush();
	FILE *fp = fopen("./file.bin", asciiWriteCreate);
	if (fp)
	{
		fwrite(&nSpans, sizeof(long), 1, fp);
		fwrite(spans, sizeof(CBlockSpan)*nSpans, 1, fp);
		fclose(fp);
	}
}
#endif









#ifdef STEP_1
extern void getBlocks(long start, long stop, bool dump);
int main(int argc, const char *argv[])
{
	curl_init();

#ifdef PART_A
	FILE *fp = fopen("./file.bin",asciiReadOnly);
	if (fp)
	{
		fread(&nSpans,sizeof(long),1,fp);
		fread(spans,sizeof(long)*nSpans,1,fp);
		fclose(fp);
	}
	for (int s=0;s<nSpans/2;s++)
		getBlocks(spans[s].start, spans[s].stop, true);
#endif
#ifdef PART_B
	unsigned long start = 0;
	unsigned long stop  = 2482355;
	getBlocks(start,stop,false);
#endif
	curl_clean();
	return 0;
}

//-------------------------------------------------------------------------
void getBlocks(long start, long stop, bool dump)
{
	bool had=true;
	for (long i=start;i<stop;i++)
	{
		if (dump)
		{
			bool has = getBlock(i).find("\"transactions\":[],") == string::npos;
			if (!(i%20))
			{
				if (has) { cout << "block " << i << " has no transactions\r"; cout.flush(); } //cout.flush(); exit(0); }
				else { cout << "block " << i << " has transactions\r"; cout.flush(); exit(0); }
			}
		} else
		{
			bool has = getBlock(i).find("\"transactions\":[],") != string::npos;
			if (has != had)
			{ cout << i << "\n"; cout.flush(); }
			had = has;
		}
	}
}

//-------------------------------------------------------------------------
extern CURL  *curlPtr       ( bool cleanup=false );
extern size_t write_callback( char *ptr, size_t size, size_t nmemb, void *userdata );
extern string callRPC       ( const string& method, const string& params, const string& id="");
#define quote(a) (string("\"") + a + "\"")

//-------------------------------------------------------------------------
string getBlock(long num)
{
	return callRPC("eth_getBlockByNumber", "["+quote(to_string(num))+",false]", to_string(num));
}

//-------------------------------------------------------------------------
string callRPC(const string& method, const string& params, const string& id)
{
	string thePost;
	thePost += "{";
	thePost +=  quote("jsonrpc") + ":"  + quote("2.0")  + ",";
	thePost +=  quote("method")  + ":"  + quote(method) + ",";
	thePost +=  quote("params")  + ":"  + params        + ",";
	thePost +=  quote("id")      + ":"  + quote(id);
	thePost += "}";

	curl_easy_setopt(curlPtr(), CURLOPT_POSTFIELDS,    thePost.c_str());
	curl_easy_setopt(curlPtr(), CURLOPT_POSTFIELDSIZE, thePost.length());

	string result;
	curl_easy_setopt(curlPtr(), CURLOPT_WRITEDATA,     &result);
	curl_easy_setopt(curlPtr(), CURLOPT_WRITEFUNCTION, write_callback);

	CURLcode res = curl_easy_perform(curlPtr());
	if (res != CURLE_OK)
	{
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		exit(0);
	}

	// Remove the json result wrapper so we get a clean block
	result.erase(0,1);                // skip over first bracket
	result.erase(0,result.find("{")); // clear to the next bracket
	result.erase(result.end()-1);     // erase the last bracket
	return result;
}

//-------------------------------------------------------------------------
// 'curl' calls this function as each portion of the request is processed.
// As we accumulate the string, we strip newlines (\n) and return (\r)
//-------------------------------------------------------------------------
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	string tmpStr;
	int cnt=0;
	for (int i=0;i<nmemb;i++)
	{
		if (ptr[i]!='\n' && ptr[i]!='\r')
		{
			tmpStr += ptr[i];
			cnt++;
		}
	}
	tmpStr[cnt] = '\0';

	// store it away in the caller's string
	(*(string*)userdata) += tmpStr;

	// we handeled everything, tell curl to send more if there is any
	return size*nmemb;
}

//-------------------------------------------------------------------------
CURL *curlPtr(bool cleanup)
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
		headers = curl_slist_append(headers,       "Content-Type: application/json");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_URL,        "http:/""/localhost:8545"); // 'geth' or 'parity' must be serving RPC here

	} else if (cleanup)
	{
		if ( headers ) curl_slist_free_all(headers);
		if ( curl    ) curl_easy_cleanup  (curl);
		curl = NULL;
	}

	return curl;
}

//-------------------------------------------------------------------------
void curl_init(void)
{
	curlPtr();
}

//-------------------------------------------------------------------------
void curl_clean(void)
{
	curlPtr(true);
}
CParams params[] = {};
SFInt32 nParams=0;
#endif
