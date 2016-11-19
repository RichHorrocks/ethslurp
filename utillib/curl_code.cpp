/*--------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2016 Great Hill Corporation

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
--------------------------------------------------------------------------------*/
#include "utillib.h"
#include "curl_code.h"
#include <curl/curl.h>

//-------------------------------------------------------------------------
CURL *getCurl_internal(bool cleanup=false)
{
        static CURL *curl = NULL;
        if (!curl) {

                curl = curl_easy_init();
                if (!curl)
                {
                        fprintf(stderr, "Curl failed to initialize. Quitting...\n");
                        exit(0);
                }

        } else if (cleanup) {

                if (curl)
                        curl_easy_cleanup(curl);
                return NULL;
        }
        return curl;
}

//-------------------------------------------------------------------------
size_t write_callback_internal(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	string result;
	size_t i=0;
	for (i=0;i<nmemb;i++)
		result += ptr[i];
	result[i] = '\0';

	// store it away in the caller's string
	(*(string*)userdata) += result;

	// we handeled everything, tell curl to keep going
	return size*nmemb;
}

//---------------------------------------------------------------------------------------------------
SFString urlToString(const SFString& url)
{
	if (url.IsEmpty())
	{
		getCurl_internal(true);
		return EMPTY;
	}

	string result;
	curl_easy_setopt(getCurl_internal(), CURLOPT_URL,           (const char*)url);
	curl_easy_setopt(getCurl_internal(), CURLOPT_WRITEDATA,     &result);
	curl_easy_setopt(getCurl_internal(), CURLOPT_WRITEFUNCTION, write_callback_internal);
        CURLcode res = curl_easy_perform(getCurl_internal());
        if (res != CURLE_OK)
        {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
                exit(0);
        }

	return result.c_str();
}

//-------------------------------------------------------------------------
class Cleanup
{
public:
	SFInt32 unused;
	Cleanup() { unused = 0; }
	~Cleanup() {
		urlToString();
	}
};
// destructor gets called on pragram exit to clear curl handle
static Cleanup cleanUp;
