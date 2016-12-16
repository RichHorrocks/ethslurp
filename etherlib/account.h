#ifndef _ACCOUNT_H_
#define _ACCOUNT_H_
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
#include "etherlib.h"
#include "transaction.h"

//--------------------------------------------------------------------------
class CAccount;
typedef SFArrayBase<CAccount>         CAccountArray;
typedef SFList<CAccount*>             CAccountList;
typedef CNotifyClass<const CAccount*> CAccountNotify;
typedef SFUniqueList<CAccount*>       CAccountListU;

//---------------------------------------------------------------------------
extern int sortAccount        (const SFString& f1, const SFString& f2, const void *rr1, const void *rr2);
extern int sortAccountByName  (const void *rr1, const void *rr2);
extern int sortAccountByID    (const void *rr1, const void *rr2);
extern int isDuplicateAccount (const void *rr1, const void *rr2);

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CAccount : public CBaseNode
{
public:
	SFAddress addr;
	SFString header;
	SFString displayString;
	SFBool pageSize;
	SFInt32 lastPage;
	SFInt32 lastBlock;
	SFInt32 nVisible;
	CTransactionArray transactions;

public:
					CAccount  (void);
					CAccount  (const CAccount& ac);
				   ~CAccount  (void);
	CAccount&	operator= 		(const CAccount& ac);

	DECLARE_NODE (CAccount);

	// EXISTING_CODE
	SFInt32 deleteNotShowing(void);
	CAbi abi;
	// EXISTING_CODE

protected:
	void			Clear      		(void);
	void			Init      		(void);
	void			Copy      		(const CAccount& ac);
	SFBool                  readBackLevel           (SFArchive& archive);

	// EXISTING_CODE
	// EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAccount::CAccount(void)
{
	Init();
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAccount::CAccount(const CAccount& ac)
{
	// EXISTING_CODE
	// EXISTING_CODE
	Copy(ac);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAccount::~CAccount(void)
{
	Clear();
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccount::Clear(void)
{
	// EXISTING_CODE
#if 1 //NEW_CODE
	abi.abiByName.Clear();
#endif
	abi.abiByEncoding.Clear();
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccount::Init(void)
{
	CBaseNode::Init();

	addr = EMPTY;
	header = EMPTY;
	displayString = EMPTY;
	pageSize = 0;
	lastPage = 0;
	lastBlock = -1;
	nVisible = 0;
//	transactions = ??; /* unknown type: CTransactionArray */

	// EXISTING_CODE
//	abi = ??; /* unknown type: CAbi */
//	abi.abiByName.Clear();
//	abi.abiByEncoding.Clear();
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAccount::Copy(const CAccount& ac)
{
	Clear();

	CBaseNode::Copy(ac);
	addr = ac.addr;
	header = ac.header;
	displayString = ac.displayString;
	pageSize = ac.pageSize;
	lastPage = ac.lastPage;
	lastBlock = ac.lastBlock;
	nVisible = ac.nVisible;
	transactions = ac.transactions;

	// EXISTING_CODE
	abi = ac.abi;
	abi.abiByName = ac.abi.abiByName;
	abi.abiByEncoding = ac.abi.abiByEncoding;
	// EXISTING_CODE
	finishParse();
}

//--------------------------------------------------------------------------
inline CAccount& CAccount::operator=(const CAccount& ac)
{
	Copy(ac);
	// EXISTING_CODE
	// EXISTING_CODE
	return *this;
}

//---------------------------------------------------------------------------
inline SFString CAccount::getValueByName(const SFString& fieldName) const
{
	// EXISTING_CODE
	// EXISTING_CODE
	return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
extern SFString nextAccountChunk(const SFString& fieldIn, SFBool& force, const void *data);

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CAccountArray);
IMPLEMENT_ARCHIVE_LIST(CAccountList);

//---------------------------------------------------------------------------
extern SFString nextAccountChunk_custom(const SFString& fieldIn, SFBool& force, const void *data);

//---------------------------------------------------------------------------
// EXISTING_CODE
extern SFBool verbose;
extern SFBool isTesting;
extern CFileExportContext& outErr;
#define REP_FREQ   11
#define REP_INFREQ 563
// EXISTING_CODE

#endif
