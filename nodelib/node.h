#ifndef _NODE_H_
#define _NODE_H_

#include "etherlib.h"
#include <glob.h>
#include <iostream>
#include <fstream>
#include <string>
#include <curl/curl.h>

//----------------------------------------------------------------------------------
typedef SFBool (*VISITOR)(const SFString& str);

//------------------------------------------------------------------
inline int globErrFunc(const char *epath, int eerrno)
{
	perror(epath);
	printf("%d:%s\n",eerrno,epath);
	return 0;
}

//----------------------------------------------------------------------------------
inline void forAllFiles(const SFString& mask, VISITOR func )
{
	glob_t globBuf;
	glob( (const char *)mask, GLOB_MARK, globErrFunc, &globBuf);
	SFBool done=FALSE;
	for (int i=0;i<globBuf.gl_pathc&&!done;i++)
		if (!(func)(globBuf.gl_pathv[i]))
			done=TRUE;
	globfree( &globBuf );
}

//-------------------------------------------------------------------------
#define quote(a) (string("\"") + a + "\"")
#define max(a,b) ((a>b)?a:b)

//-----------------------------------------------------------------------
extern SFBool readOneBlock_fromJson   (      CBlock& block, const SFString& fileName);
extern SFBool readOneBlock_fromBinary (      CBlock& block, const SFString& fileName);
extern void   writeOneBlock_toJson    (const CBlock& block, const SFString& fileName);
extern void   writeOneBlock_toBinary  (const CBlock& block, const SFString& fileName);
extern void   verifyOneBlock          (const SFString& reference, const SFString& proposal);

//-------------------------------------------------------------------------
extern string getClientVersion (const string& id="");
extern string compileSolidity  (const string& sol, const string& id="");

//-------------------------------------------------------------------------
class CAccount;
extern bool   getCode          (CAccount& acct,      const string& addr, const string& id="");
extern bool   getBlock         (CBlock& block,       int num,            const string& id="");
extern bool   getTransaction   (CTransaction& trans, const string& hash, const string& id="");
extern bool   getReceipt       (CReceipt& receipt,   const string& hash, const string& id="");
extern bool   getLogEntry      (CLogEntry& log,      const string& hash, const string& id="");
extern bool   getTraces        (CTrace& traces,      const string& hash, const string& id="");

//-------------------------------------------------------------------------
extern void writeToJson(const CBaseNode& node, const SFString& fileName);
extern void   etherlib_init    (const string& client);
extern void   etherlib_cleanup (void);

//-------------------------------------------------------------------------
extern SFString getStoragePath(const SFString& path, const SFString& num);
extern SFString getBinaryPath (const SFString& path, const SFString& num);
inline void     setStoragePath(const SFString& path) { getStoragePath(path,"0"); }

#endif
