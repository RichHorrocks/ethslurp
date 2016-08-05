#ifndef _ETHSLURP_H_
#define _ETHSLURP_H_
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
#include "etherlib.h"
#include "daolib.h"
#include "slurp_options.h"

//---------------------------------------------------------------------------------------------------
class CSlurperApp
{
private:
	CVersion  version;
	CConfig   config;
	CWebAPI   api;

public:
	CAccount theAccount;

              CSlurperApp (void) : config(NULL) { };
             ~CSlurperApp (void) {  }

	SFBool    Initialize  (CSlurpOptions& options, SFString& message);
	SFBool    Slurp       (CSlurpOptions& options, SFString& message);
	SFBool    Filter      (CSlurpOptions& options, SFString& message);
	SFBool    Display     (CSlurpOptions& options, SFString& message);

private:
	void      buildDisplayStrings (CSlurpOptions& options);
	SFString  getFormatString     (CSlurpOptions& options, const SFString& name, SFBool ignoreBlank);
};

//--------------------------------------------------------------------------------
extern SFString getHomeFolder(void);

//---------------------------------------------------------------------------------------------------
extern SFBool establishFolders(CConfig& config, const SFString& vers);
extern void   findBlockRange  (const SFString& contents, SFInt32& minBlock, SFInt32& maxBlock);

//---------------------------------------------------------------------------------------------------
extern void reportTimes(const SFString& func, double start, double stop, double timeSpent);
#define START_TIMER()		PERF_START();
#define STOP_TIMER(func)	PERF_STOP(); reportTimes(func, timerStart, timerStop, timeSpent); 

#endif
