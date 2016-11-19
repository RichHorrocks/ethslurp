#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_
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
#include "utillib.h"
#include "ethtypes.h"
#include "function.h"
#include "receipt.h"
#include "trace.h"

//--------------------------------------------------------------------------
class CTransaction;
typedef SFArrayBase<CTransaction>         CTransactionArray;
typedef SFList<CTransaction*>             CTransactionList;
typedef CNotifyClass<const CTransaction*> CTransactionNotify;
typedef SFUniqueList<CTransaction*>       CTransactionListU;

//---------------------------------------------------------------------------
extern int sortTransaction        (const SFString& f1, const SFString& f2, const void *rr1, const void *rr2);
extern int sortTransactionByName  (const void *rr1, const void *rr2);
extern int sortTransactionByID    (const void *rr1, const void *rr2);
extern int isDuplicateTransaction (const void *rr1, const void *rr2);

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CTransaction : public CBaseNode
{
public:
	SFHash blockHash;
	SFInt32 blockNumber;
	SFString creates;
	SFInt32 confirmations;
	SFAddress contractAddress;
	SFString cumulativeGasUsed;
	SFAddress from;
	SFInt32 gas;
	SFString gasPrice;
	SFString gasUsed;
	SFHash hash;
	SFString input;
	SFBool isError;
	SFBool isInternalTx;
	SFInt32 nonce;
	SFHash r;
	SFString raw;
	SFHash s;
	SFInt32 timeStamp;
	SFAddress to;
	SFInt32 transactionIndex;
	SFHash v;
	SFString value;
	CReceipt receipt;
	CTrace trace;

public:
					CTransaction  (void);
					CTransaction  (const CTransaction& tr);
				   ~CTransaction  (void);
	CTransaction&	operator= 		(const CTransaction& tr);

	DECLARE_NODE (CTransaction);

	// EXISTING_CODE
	SFString inputToFunction(void) const;
	SFBool   isFunction(const SFString& func) const;
	SFString getAddrList(char delim='\t') const;
	SFString getAddrsFromInput(char delim='\t') const;
	CFunction *funcPtr;
	SFTime     m_transDate;
	SFString   function;
	double     ether;
	// EXISTING_CODE

protected:
	void			Clear      		(void);
	void			Init      		(void);
	void			Copy      		(const CTransaction& tr);
	SFBool                  readBackLevel           (SFArchive& archive);

	// EXISTING_CODE
	friend int sortTransactionsForWrite(const void *rr1, const void *rr2);
	// EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CTransaction::CTransaction(void)
{
	Init();
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CTransaction::CTransaction(const CTransaction& tr)
{
	// EXISTING_CODE
	// EXISTING_CODE
	Copy(tr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CTransaction::~CTransaction(void)
{
	Clear();
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransaction::Clear(void)
{
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransaction::Init(void)
{
	CBaseNode::Init();

	blockHash = EMPTY;
	blockNumber = 0;
	creates = EMPTY;
	confirmations = 0;
	contractAddress = EMPTY;
	cumulativeGasUsed = EMPTY;
	from = EMPTY;
	gas = 0;
	gasPrice = EMPTY;
	gasUsed = EMPTY;
	hash = EMPTY;
	input = EMPTY;
	isError = 0;
	isInternalTx = 0;
	nonce = 0;
	r = EMPTY;
	raw = EMPTY;
	s = EMPTY;
	timeStamp = 0;
	to = EMPTY;
	transactionIndex = 0;
	v = EMPTY;
	value = EMPTY;
//	receipt = ??; /* unknown type: CReceipt */
//	trace = ??; /* unknown type: CTrace */

	// EXISTING_CODE
	//SFString function
	m_transDate = earliestDate;
	funcPtr = NULL;
	ether = 0.0;
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CTransaction::Copy(const CTransaction& tr)
{
	Clear();

	CBaseNode::Copy(tr);
	blockHash = tr.blockHash;
	blockNumber = tr.blockNumber;
	creates = tr.creates;
	confirmations = tr.confirmations;
	contractAddress = tr.contractAddress;
	cumulativeGasUsed = tr.cumulativeGasUsed;
	from = tr.from;
	gas = tr.gas;
	gasPrice = tr.gasPrice;
	gasUsed = tr.gasUsed;
	hash = tr.hash;
	input = tr.input;
	isError = tr.isError;
	isInternalTx = tr.isInternalTx;
	nonce = tr.nonce;
	r = tr.r;
	raw = tr.raw;
	s = tr.s;
	timeStamp = tr.timeStamp;
	to = tr.to;
	transactionIndex = tr.transactionIndex;
	v = tr.v;
	value = tr.value;
	receipt = tr.receipt;
	trace = tr.trace;

	// EXISTING_CODE
	m_transDate = tr.m_transDate;
	funcPtr = tr.funcPtr;
	function = tr.function;
	ether = tr.ether;
	// EXISTING_CODE
	finishParse();
}

//--------------------------------------------------------------------------
inline CTransaction& CTransaction::operator=(const CTransaction& tr)
{
	Copy(tr);
	// EXISTING_CODE
	// EXISTING_CODE
	return *this;
}

//---------------------------------------------------------------------------
inline SFString CTransaction::getValueByName(const SFString& fieldName) const
{
	// EXISTING_CODE
	// EXISTING_CODE
	return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
extern SFString nextTransactionChunk(const SFString& fieldIn, SFBool& force, const void *data);

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CTransactionArray);
IMPLEMENT_ARCHIVE_LIST(CTransactionList);

//---------------------------------------------------------------------------
extern SFString nextTransactionChunk_custom(const SFString& fieldIn, SFBool& force, const void *data);

//---------------------------------------------------------------------------
// EXISTING_CODE
extern SFString toNice(const SFString& in);
extern int sortTransactionsForWrite(const void *rr1, const void *rr2);
//---------------------------------------------------------------------------
inline SFString fmtAddress(const SFString& addr)
{
        return addr.Left(8)+"..."+addr.Right(6);
}
// EXISTING_CODE

#endif
