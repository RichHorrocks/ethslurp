#ifndef _NODE_H_
#define _NODE_H_

#include "etherlib.h"

//-----------------------------------------------------------------------
extern SFBool   readOneBlock_fromJson   (      CBlock& block,   const SFString& fileName);
extern SFBool   readOneBlock_fromBinary (      CBlock& block,   const SFString& fileName);
extern void     writeToJson             (const CBaseNode& node, const SFString& fileName);
extern void     writeToBinary           (const CBaseNode& node, const SFString& fileName);
extern SFBool   verifyBlock             (const CBlock& block,   const SFString& fileName, SFBool fail=true);

//-------------------------------------------------------------------------
extern SFString compileSolidity         (const SFString& sol, const SFString& id="");
extern SFString getClientVersion        (const SFString& id="");
extern SFUint32 getLatestBlock          (const SFString& id="");

//-------------------------------------------------------------------------
extern bool     getCode                 (CAccount& acct,      const SFString& addr, const SFString& id="");

//-------------------------------------------------------------------------
extern bool     getBlock                (CBlock& block,       SFUint32 num,         const SFString& id="");
extern bool     getTransaction          (CTransaction& trans, const SFString& hash, const SFString& id="");
extern bool     getReceipt              (CReceipt& receipt,   const SFString& hash, const SFString& id="");
extern bool     getLogEntry             (CLogEntry& log,      const SFString& hash, const SFString& id="");
extern bool     getTraces               (CTrace& traces,      const SFString& hash, const SFString& id="");

//-------------------------------------------------------------------------
extern SFString getStoragePath          (const SFString& path, SFUint32 num);
extern SFString getBinaryPath           (const SFString& path, SFUint32 num);
inline void     setStoragePath          (const SFString& path) { getStoragePath(path,0); }
extern void     setDataSource           (const SFString& source);

//-------------------------------------------------------------------------
extern void     etherlib_init           (const SFString& client);
extern void     etherlib_cleanup        (void);

#endif
