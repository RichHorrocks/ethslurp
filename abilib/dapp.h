#ifndef _DAPP_H_
#define _DAPP_H_
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
class CDapp;
typedef SFArrayBase<CDapp>         CDappArray;
typedef SFList<CDapp*>             CDappList;
typedef CNotifyClass<const CDapp*> CDappNotify;
typedef SFUniqueList<CDapp*>       CDappListU;

//---------------------------------------------------------------------------
extern int sortDapp        (const SFString& f1, const SFString& f2, const void *rr1, const void *rr2);
extern int sortDappByName  (const void *rr1, const void *rr2);
extern int sortDappByID    (const void *rr1, const void *rr2);
extern int isDuplicateDapp (const void *rr1, const void *rr2);

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CDapp : public CBaseNode
{
public:
	SFString dappName;
	SFString description;
	SFString site;
	SFString gitHub;
	SFString reddit;
	SFString people;
	SFString tags;
	SFString license;
	SFString status;
	SFString last_update;

public:
					CDapp  (void);
					CDapp  (const CDapp& da);
				   ~CDapp  (void);
	CDapp&	operator= 		(const CDapp& da);

	DECLARE_NODE (CDapp);

	// EXISTING_CODE
	// EXISTING_CODE

protected:
	void			Clear      		(void);
	void			Init      		(void);
	void			Copy      		(const CDapp& da);
	SFBool                  readBackLevel           (SFArchive& archive);

	// EXISTING_CODE
	// EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CDapp::CDapp(void)
{
	Init();
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CDapp::CDapp(const CDapp& da)
{
	// EXISTING_CODE
	// EXISTING_CODE
	Copy(da);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CDapp::~CDapp(void)
{
	Clear();
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDapp::Clear(void)
{
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDapp::Init(void)
{
	CBaseNode::Init();

	dappName = EMPTY;
	description = EMPTY;
	site = EMPTY;
	gitHub = EMPTY;
	reddit = EMPTY;
	people = EMPTY;
	tags = EMPTY;
	license = EMPTY;
	status = EMPTY;
	last_update = EMPTY;

	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CDapp::Copy(const CDapp& da)
{
	Clear();

	CBaseNode::Copy(da);
	dappName = da.dappName;
	description = da.description;
	site = da.site;
	gitHub = da.gitHub;
	reddit = da.reddit;
	people = da.people;
	tags = da.tags;
	license = da.license;
	status = da.status;
	last_update = da.last_update;

	// EXISTING_CODE
	// EXISTING_CODE
	finishParse();
}

//--------------------------------------------------------------------------
inline CDapp& CDapp::operator=(const CDapp& da)
{
	Copy(da);
	// EXISTING_CODE
	// EXISTING_CODE
	return *this;
}

//---------------------------------------------------------------------------
inline SFString CDapp::getValueByName(const SFString& fieldName) const
{
	// EXISTING_CODE
	// EXISTING_CODE
	return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
extern SFString nextDappChunk(const SFString& fieldIn, SFBool& force, const void *data);

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CDappArray);
IMPLEMENT_ARCHIVE_LIST(CDappList);

//---------------------------------------------------------------------------
extern SFString nextDappChunk_custom(const SFString& fieldIn, SFBool& force, const void *data);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

#endif
