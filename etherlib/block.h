#ifndef _BLOCK_H_
#define _BLOCK_H_
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
#include "abilib.h"
#include "transaction.h"

//--------------------------------------------------------------------------
class CBlock;
typedef SFArrayBase<CBlock>         CBlockArray;
typedef SFList<CBlock*>             CBlockList;
typedef CNotifyClass<const CBlock*> CBlockNotify;
typedef SFUniqueList<CBlock*>       CBlockListU;

//---------------------------------------------------------------------------
extern int sortBlock        (const SFString& f1, const SFString& f2, const void *rr1, const void *rr2);
extern int sortBlockByName  (const void *rr1, const void *rr2);
extern int sortBlockByID    (const void *rr1, const void *rr2);
extern int isDuplicateBlock (const void *rr1, const void *rr2);

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CBlock : public CBaseNode
{
public:
	SFAddress author;
	SFString difficulty;
	SFString extraData;
	SFString gasLimit;
	SFString gasUsed;
	SFHash hash;
	SFString logsBloom;
	SFHash mixHash;
	SFAddress miner;
	SFString nonce;
	SFString number;
	SFHash parentHash;
	SFString receiptRoot;
	SFString receiptsRoot;
	SFStringArray sealFields;
	SFString sha3Uncles;
	SFString size;
	SFString stateRoot;
	SFString timestamp;
	SFString totalDifficulty;
	CTransactionArray transactions;
	SFString transactionsRoot;
	SFStringArray uncles;

public:
			CBlock  (void);
			CBlock  (const CBlock& bl);
			~CBlock  (void);
	CBlock&		operator=(const CBlock& bl);

	DECLARE_NODE (CBlock);

	// EXISTING_CODE
	SFUint32 firstTrans;
	SFUint32 nTrans;
	// EXISTING_CODE

protected:
	void			Clear		(void);
	void			Init		(void);
	void			Copy		(const CBlock& bl);
	SFBool                  readBackLevel	(SFArchive& archive);

	// EXISTING_CODE
	// EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CBlock::CBlock(void)
{
	Init();
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CBlock::CBlock(const CBlock& bl)
{
	// EXISTING_CODE
	// EXISTING_CODE
	Copy(bl);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CBlock::~CBlock(void)
{
	Clear();
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBlock::Clear(void)
{
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBlock::Init(void)
{
	CBaseNode::Init();

	author = EMPTY;
	difficulty = EMPTY;
	extraData = EMPTY;
	gasLimit = EMPTY;
	gasUsed = EMPTY;
	hash = EMPTY;
	logsBloom = EMPTY;
	mixHash = EMPTY;
	miner = EMPTY;
	nonce = EMPTY;
	number = EMPTY;
	parentHash = EMPTY;
	receiptRoot = EMPTY;
	receiptsRoot = EMPTY;
//	sealFields = ??; /* unknown type: SFStringArray */
	sha3Uncles = EMPTY;
	size = EMPTY;
	stateRoot = EMPTY;
	timestamp = EMPTY;
	totalDifficulty = EMPTY;
//	transactions = ??; /* unknown type: CTransactionArray */
	transactionsRoot = EMPTY;
//	uncles = ??; /* unknown type: SFStringArray */

	// EXISTING_CODE
	firstTrans = 0;
	nTrans = 0;
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CBlock::Copy(const CBlock& bl)
{
	Clear();

	CBaseNode::Copy(bl);
	author = bl.author;
	difficulty = bl.difficulty;
	extraData = bl.extraData;
	gasLimit = bl.gasLimit;
	gasUsed = bl.gasUsed;
	hash = bl.hash;
	logsBloom = bl.logsBloom;
	mixHash = bl.mixHash;
	miner = bl.miner;
	nonce = bl.nonce;
	number = bl.number;
	parentHash = bl.parentHash;
	receiptRoot = bl.receiptRoot;
	receiptsRoot = bl.receiptsRoot;
	sealFields = bl.sealFields;
	sha3Uncles = bl.sha3Uncles;
	size = bl.size;
	stateRoot = bl.stateRoot;
	timestamp = bl.timestamp;
	totalDifficulty = bl.totalDifficulty;
	transactions = bl.transactions;
	transactionsRoot = bl.transactionsRoot;
	uncles = bl.uncles;

	// EXISTING_CODE
	firstTrans = bl.firstTrans;
	nTrans = bl.nTrans;
	// EXISTING_CODE
	finishParse();
}

//--------------------------------------------------------------------------
inline CBlock& CBlock::operator=(const CBlock& bl)
{
	Copy(bl);
	// EXISTING_CODE
	// EXISTING_CODE
	return *this;
}

//---------------------------------------------------------------------------
inline SFString CBlock::getValueByName(const SFString& fieldName) const
{
	// EXISTING_CODE
	// EXISTING_CODE
	return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
extern SFString nextBlockChunk(const SFString& fieldIn, SFBool& force, const void *data);

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CBlockArray);
IMPLEMENT_ARCHIVE_LIST(CBlockList);

//---------------------------------------------------------------------------
extern SFString nextBlockChunk_custom(const SFString& fieldIn, SFBool& force, const void *data);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

#endif
