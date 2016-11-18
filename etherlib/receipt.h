#ifndef _RECEIPT_H_
#define _RECEIPT_H_
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
#include "logentry.h"

//--------------------------------------------------------------------------
class CReceipt;
typedef SFArrayBase<CReceipt>         CReceiptArray;
typedef SFList<CReceipt*>             CReceiptList;
typedef CNotifyClass<const CReceipt*> CReceiptNotify;
typedef SFUniqueList<CReceipt*>       CReceiptListU;

//---------------------------------------------------------------------------
extern int sortReceipt        (const SFString& f1, const SFString& f2, const void *rr1, const void *rr2);
extern int sortReceiptByName  (const void *rr1, const void *rr2);
extern int sortReceiptByID    (const void *rr1, const void *rr2);
extern int isDuplicateReceipt (const void *rr1, const void *rr2);

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CReceipt : public CBaseNode
{
public:
	SFHash blockHash;
	SFInt32 blockNumber;
	SFAddress contractAddress;
	SFInt32 cumulativeGasUsed;
	SFAddress from;
	SFInt32 gasUsed;
	CLogEntryArray logs;
	SFString logsBloom;
	SFString root;
	SFAddress to;
	SFHash transactionHash;
	SFInt32 transactionIndex;

public:
					CReceipt  (void);
					CReceipt  (const CReceipt& re);
				   ~CReceipt  (void);
	CReceipt&	operator= 		(const CReceipt& re);

	DECLARE_NODE (CReceipt);

	// EXISTING_CODE
	// EXISTING_CODE

protected:
	void			Clear      		(void);
	void			Init      		(void);
	void			Copy      		(const CReceipt& re);
	SFBool                  readBackLevel           (SFArchive& archive);

	// EXISTING_CODE
	// EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CReceipt::CReceipt(void)
{
	Init();
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CReceipt::CReceipt(const CReceipt& re)
{
	// EXISTING_CODE
	// EXISTING_CODE
	Copy(re);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CReceipt::~CReceipt(void)
{
	Clear();
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReceipt::Clear(void)
{
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReceipt::Init(void)
{
	CBaseNode::Init();

	blockHash = EMPTY;
	blockNumber = 0;
	contractAddress = EMPTY;
	cumulativeGasUsed = 0;
	from = EMPTY;
	gasUsed = 0;
//	logs = ??; /* unknown type: CLogEntryArray */
	logsBloom = EMPTY;
	root = EMPTY;
	to = EMPTY;
	transactionHash = EMPTY;
	transactionIndex = 0;

	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CReceipt::Copy(const CReceipt& re)
{
	Clear();

	CBaseNode::Copy(re);
	blockHash = re.blockHash;
	blockNumber = re.blockNumber;
	contractAddress = re.contractAddress;
	cumulativeGasUsed = re.cumulativeGasUsed;
	from = re.from;
	gasUsed = re.gasUsed;
	logs = re.logs;
	logsBloom = re.logsBloom;
	root = re.root;
	to = re.to;
	transactionHash = re.transactionHash;
	transactionIndex = re.transactionIndex;

	// EXISTING_CODE
	// EXISTING_CODE
	finishParse();
}

//--------------------------------------------------------------------------
inline CReceipt& CReceipt::operator=(const CReceipt& re)
{
	Copy(re);
	// EXISTING_CODE
	// EXISTING_CODE
	return *this;
}

//---------------------------------------------------------------------------
inline SFString CReceipt::getValueByName(const SFString& fieldName) const
{
	// EXISTING_CODE
	// EXISTING_CODE
	return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
extern SFString nextReceiptChunk(const SFString& fieldIn, SFBool& force, const void *data);

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CReceiptArray);
IMPLEMENT_ARCHIVE_LIST(CReceiptList);

//---------------------------------------------------------------------------
extern SFString nextReceiptChunk_custom(const SFString& fieldIn, SFBool& force, const void *data);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

#endif
