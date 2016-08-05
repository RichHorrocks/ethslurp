#ifndef _ABI_H_
#define _ABI_H_
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
#include "function.h"
#include "parameter.h"

//--------------------------------------------------------------------------
class CAbi;
typedef SFArrayBase<CAbi>         CAbiArray;
typedef SFList<CAbi*>             CAbiList;
typedef CNotifyClass<const CAbi*> CAbiNotify;
typedef SFUniqueList<CAbi*>       CAbiListU;

//---------------------------------------------------------------------------
extern int sortAbi        (const SFString& f1, const SFString& f2, const void *rr1, const void *rr2);
#if 1 //NEW_CODE
extern int sortAbiByName  (const void *rr1, const void *rr2);
#endif
extern int sortAbiByID    (const void *rr1, const void *rr2);
extern int isDuplicateAbi (const void *rr1, const void *rr2);

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CAbi : public CBaseNode
{
public:
	SFInt32 handle;
#if 1 //NEW_CODE
	CFunctionArray abiByName;
#endif
	CFunctionArray abiByEncoding;

public:
					CAbi  (void);
					CAbi  (const CAbi& ab);
				   ~CAbi  (void);
	CAbi&	operator= 		(const CAbi& ab);

	DECLARE_NODE (CAbi);

	// EXISTING_CODE
        void loadABI(const SFString& addr);
#if 1 //NEW_CODE
        CFunction *findFunctionByName(const SFString& search);
#endif
        CFunction *findFunctionByEncoding(const SFString& search);
	// EXISTING_CODE

protected:
	void			Clear      		(void);
	void			Init      		(void);
	void			Copy      		(const CAbi& ab);

	// EXISTING_CODE
	// EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CAbi::CAbi(void)
{
	Init();
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAbi::CAbi(const CAbi& ab)
{
	// EXISTING_CODE
	// EXISTING_CODE
	Copy(ab);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CAbi::~CAbi(void)
{
	Clear();
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAbi::Clear(void)
{
	// EXISTING_CODE
#if 1 //NEW_CODE
	abiByName.Clear();
#endif
	abiByEncoding.Clear();
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAbi::Init(void)
{
	CBaseNode::Init();

	handle = 0;
//	abiByName = ??; /* unknown type: CFunctionArray */
//	abiByEncoding = ??; /* unknown type: CFunctionArray */

	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CAbi::Copy(const CAbi& ab)
{
	Clear();

	CBaseNode::Copy(ab);
	handle = ab.handle;
#if 1 //NEW_CODE
	abiByName = ab.abiByName;
#endif
	abiByEncoding = ab.abiByEncoding;

	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CAbi& CAbi::operator=(const CAbi& ab)
{
	Copy(ab);
	// EXISTING_CODE
	// EXISTING_CODE
	return *this;
}

//---------------------------------------------------------------------------
inline SFString CAbi::getValueByName(const SFString& fieldName) const
{
	// EXISTING_CODE
	// EXISTING_CODE
	return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
inline SFInt32 CAbi::getHandle(void) const
{
	// EXISTING_CODE
	// EXISTING_CODE
	return handle;
}

//---------------------------------------------------------------------------
extern SFString nextAbiChunk(const SFString& fieldIn, SFBool& force, const void *data);

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CAbiArray);
IMPLEMENT_ARCHIVE_LIST(CAbiList);

//---------------------------------------------------------------------------
extern SFString nextAbiChunk_custom(const SFString& fieldIn, SFBool& force, const void *data);

//---------------------------------------------------------------------------
// EXISTING_CODE
extern SFBool verbose;
extern SFBool isTesting;
extern CFileExportContext& outErr;
#define REP_FREQ   11
#define REP_INFREQ 563
// EXISTING_CODE

#endif
