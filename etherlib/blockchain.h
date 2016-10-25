#ifndef _BLOCKCHAIN_H_
#define _BLOCKCHAIN_H_
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
/*
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
#include "block.h"
#include "etherlib.h"

//--------------------------------------------------------------------------
class CBlockChain;
typedef SFArrayBase<CBlockChain>         CBlockChainArray;
typedef SFList<CBlockChain*>             CBlockChainList;
typedef CNotifyClass<const CBlockChain*> CBlockChainNotify;
typedef SFUniqueList<CBlockChain*>       CBlockChainListU;

//---------------------------------------------------------------------------
extern int sortBlockchain        (const SFString& f1, const SFString& f2, const void *rr1, const void *rr2);
extern int sortBlockchainByName  (const void *rr1, const void *rr2);
extern int sortBlockchainByID    (const void *rr1, const void *rr2);
extern int isDuplicateBlockchain (const void *rr1, const void *rr2);

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CBlockChain : public CBaseNode
{
public:
	CBlockArray blocks;

public:
					CBlockChain  (void);
					CBlockChain  (const CBlockChain& bl);
				   ~CBlockChain  (void);
	CBlockChain&	operator= 		(const CBlockChain& bl);

	DECLARE_NODE (CBlockChain);

	// EXISTING_CODE
	// EXISTING_CODE

protected:
	void			Clear      		(void);
	void			Init      		(void);
	void			Copy      		(const CBlockChain& bl);
	SFBool                  readBackLevel           (SFArchive& archive);

	// EXISTING_CODE
	// EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CBlockChain::CBlockChain(void)
{
	Init();
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBlockChain::CBlockChain(const CBlockChain& bl)
{
	// EXISTING_CODE
	// EXISTING_CODE
	Copy(bl);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CBlockChain::~CBlockChain(void)
{
	Clear();
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBlockChain::Clear(void)
{
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBlockChain::Init(void)
{
	CBaseNode::Init();

//	blocks = ??; /* unknown type: CBlockArray */

	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBlockChain::Copy(const CBlockChain& bl)
{
	Clear();

	CBaseNode::Copy(bl);
	blocks = bl.blocks;

	// EXISTING_CODE
	// EXISTING_CODE
	finishParse();
}

//--------------------------------------------------------------------------
inline CBlockChain& CBlockChain::operator=(const CBlockChain& bl)
{
	Copy(bl);
	// EXISTING_CODE
	// EXISTING_CODE
	return *this;
}

//---------------------------------------------------------------------------
inline SFString CBlockChain::getValueByName(const SFString& fieldName) const
{
	// EXISTING_CODE
	// EXISTING_CODE
	return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
extern SFString nextBlockchainChunk(const SFString& fieldIn, SFBool& force, const void *data);

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CBlockChainArray);
IMPLEMENT_ARCHIVE_LIST(CBlockChainList);

//---------------------------------------------------------------------------
extern SFString nextBlockchainChunk_custom(const SFString& fieldIn, SFBool& force, const void *data);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

#endif
