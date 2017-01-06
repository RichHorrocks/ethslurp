#ifndef _RPCRESULT_H_
#define _RPCRESULT_H_
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

//--------------------------------------------------------------------------
class CRPCResult;
typedef SFArrayBase<CRPCResult>         CRPCResultArray;
typedef SFList<CRPCResult*>             CRPCResultList;
typedef CNotifyClass<const CRPCResult*> CRPCResultNotify;
typedef SFUniqueList<CRPCResult*>       CRPCResultListU;

//---------------------------------------------------------------------------
extern int sortRpcresult        (const SFString& f1, const SFString& f2, const void *rr1, const void *rr2);
extern int sortRpcresultByName  (const void *rr1, const void *rr2);
extern int sortRpcresultByID    (const void *rr1, const void *rr2);
extern int isDuplicateRpcresult (const void *rr1, const void *rr2);

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CRPCResult : public CBaseNode
{
public:
	SFString jsonrpc;
	SFString result;
	SFString id;

public:
					CRPCResult  (void);
					CRPCResult  (const CRPCResult& rp);
				   ~CRPCResult  (void);
	CRPCResult&	operator= 		(const CRPCResult& rp);

	DECLARE_NODE (CRPCResult);

	// EXISTING_CODE
	// EXISTING_CODE

protected:
	void			Clear      		(void);
	void			Init      		(void);
	void			Copy      		(const CRPCResult& rp);
	SFBool                  readBackLevel           (SFArchive& archive);

	// EXISTING_CODE
	// EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CRPCResult::CRPCResult(void)
{
	Init();
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CRPCResult::CRPCResult(const CRPCResult& rp)
{
	// EXISTING_CODE
	// EXISTING_CODE
	Copy(rp);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CRPCResult::~CRPCResult(void)
{
	Clear();
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRPCResult::Clear(void)
{
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRPCResult::Init(void)
{
	CBaseNode::Init();

	jsonrpc = EMPTY;
	result = EMPTY;
	id = EMPTY;

	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CRPCResult::Copy(const CRPCResult& rp)
{
	Clear();

	CBaseNode::Copy(rp);
	jsonrpc = rp.jsonrpc;
	result = rp.result;
	id = rp.id;

	// EXISTING_CODE
	// EXISTING_CODE
	finishParse();
}

//--------------------------------------------------------------------------
inline CRPCResult& CRPCResult::operator=(const CRPCResult& rp)
{
	Copy(rp);
	// EXISTING_CODE
	// EXISTING_CODE
	return *this;
}

//---------------------------------------------------------------------------
inline SFString CRPCResult::getValueByName(const SFString& fieldName) const
{
	// EXISTING_CODE
	// EXISTING_CODE
	return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
extern SFString nextRpcresultChunk(const SFString& fieldIn, SFBool& force, const void *data);

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CRPCResultArray);
IMPLEMENT_ARCHIVE_LIST(CRPCResultList);

//---------------------------------------------------------------------------
extern SFString nextRpcresultChunk_custom(const SFString& fieldIn, SFBool& force, const void *data);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

#endif
