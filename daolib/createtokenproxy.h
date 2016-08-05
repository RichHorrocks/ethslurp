#ifndef _CREATETOKENPROXY_H_
#define _CREATETOKENPROXY_H_
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
#include "utillib.h"
#include "etherlib.h"
#include "transaction.h"

//--------------------------------------------------------------------------
class CCreateTokenProxy;
typedef SFArrayBase<CCreateTokenProxy>         CCreateTokenProxyArray;
typedef SFList<CCreateTokenProxy*>             CCreateTokenProxyList;
typedef CNotifyClass<const CCreateTokenProxy*> CCreateTokenProxyNotify;
typedef SFUniqueList<CCreateTokenProxy*>       CCreateTokenProxyListU;

//---------------------------------------------------------------------------
extern int sortCreatetokenproxy        (const SFString& f1, const SFString& f2, const void *rr1, const void *rr2);
extern int sortCreatetokenproxyByName  (const void *rr1, const void *rr2);
extern int sortCreatetokenproxyByID    (const void *rr1, const void *rr2);
extern int isDuplicateCreatetokenproxy (const void *rr1, const void *rr2);

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CCreateTokenProxy : public CTransaction
{
public:
	SFString tokenHolder;

public:
					CCreateTokenProxy  (void);
					CCreateTokenProxy  (const CCreateTokenProxy& cr);
				   ~CCreateTokenProxy  (void);
	CCreateTokenProxy&	operator= 		(const CCreateTokenProxy& cr);

	DECLARE_NODE (CCreateTokenProxy);

	// EXISTING_CODE
	// EXISTING_CODE

protected:
	void			Clear      		(void);
	void			Init      		(void);
	void			Copy      		(const CCreateTokenProxy& cr);

	// EXISTING_CODE
	// EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CCreateTokenProxy::CCreateTokenProxy(void)
{
	Init();
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CCreateTokenProxy::CCreateTokenProxy(const CCreateTokenProxy& cr)
{
	// EXISTING_CODE
	// EXISTING_CODE
	Copy(cr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CCreateTokenProxy::~CCreateTokenProxy(void)
{
	Clear();
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCreateTokenProxy::Clear(void)
{
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCreateTokenProxy::Init(void)
{
	CTransaction::Init();

	tokenHolder = EMPTY;

	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CCreateTokenProxy::Copy(const CCreateTokenProxy& cr)
{
	Clear();

	CTransaction::Copy(cr);
	tokenHolder = cr.tokenHolder;

	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CCreateTokenProxy& CCreateTokenProxy::operator=(const CCreateTokenProxy& cr)
{
	Copy(cr);
	// EXISTING_CODE
	// EXISTING_CODE
	return *this;
}

//---------------------------------------------------------------------------
inline SFString CCreateTokenProxy::getValueByName(const SFString& fieldName) const
{
	// EXISTING_CODE
	// EXISTING_CODE
	return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
inline SFInt32 CCreateTokenProxy::getHandle(void) const
{
	// EXISTING_CODE
	// EXISTING_CODE
	return handle;
}

//---------------------------------------------------------------------------
extern SFString nextCreatetokenproxyChunk(const SFString& fieldIn, SFBool& force, const void *data);

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CCreateTokenProxyArray);
IMPLEMENT_ARCHIVE_LIST(CCreateTokenProxyList);

//---------------------------------------------------------------------------
extern SFString nextCreatetokenproxyChunk_custom(const SFString& fieldIn, SFBool& force, const void *data);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

#endif
