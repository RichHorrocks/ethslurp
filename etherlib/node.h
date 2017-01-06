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
extern void     etherlib_init           (const SFString& client);
extern void     etherlib_cleanup        (void);

//-------------------------------------------------------------------------
extern void     setDataSource           (const SFString& source);

//-------------------------------------------------------------------------
extern void     setStorageRoot          (const SFString& path);
extern SFString getJsonFilename1        (SFUint32 num);
extern SFString getBinaryFilename1      (SFUint32 num);
extern SFString getJsonPath1            (SFUint32 num);
extern SFString getBinaryPath1          (SFUint32 num);

//-------------------------------------------------------------------------
typedef bool (*BLOCKVISITFUNC)(CBlock& block, void *data);
typedef bool (*TRANSVISITFUNC)(CTransaction& trans, void *data);

//-------------------------------------------------------------------------
extern bool forEveryBlockOnDisc          (BLOCKVISITFUNC func, void *data, SFUint32 start=0, SFUint32 count=(SFUint32)-1);
extern bool forEveryEmptyBlockOnDisc     (BLOCKVISITFUNC func, void *data, SFUint32 start=0, SFUint32 count=(SFUint32)-1);
extern bool forEveryNonEmptyBlockOnDisc  (BLOCKVISITFUNC func, void *data, SFUint32 start=0, SFUint32 count=(SFUint32)-1);
extern bool forEveryNonEmptyBlockInMemory(BLOCKVISITFUNC func, void *data, SFUint32 start=0, SFUint32 count=(SFUint32)-1);

//-------------------------------------------------------------------------
extern bool forEveryTransaction          (TRANSVISITFUNC func, void *data, SFUint32 start=0, SFUint32 count=(SFUint32)-1);
extern bool forEveryTransactionTo        (TRANSVISITFUNC func, void *data, SFUint32 start=0, SFUint32 count=(SFUint32)-1);
extern bool forEveryTransactionFrom      (TRANSVISITFUNC func, void *data, SFUint32 start=0, SFUint32 count=(SFUint32)-1);

//-----------------------------------------------------------------------
inline SFUint32 numFromFilename(const SFString& fileNameIn)
{
        SFString str = fileNameIn.Substitute(".bin","");
        return toLong(nextTokenClearReverse(str,'/'));
}

#endif
