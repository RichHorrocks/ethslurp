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
#include "dapp.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CDapp, CBaseNode, curVersion);

//---------------------------------------------------------------------------
void CDapp::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const
{
	if (!isShowing())
		return;

	if (fmtIn.IsEmpty())
	{
		ctx << toJson();
		return;
	}

	SFString fmt = fmtIn;
	if (handleCustomFormat(ctx, fmt, data))
		return;

	CDappNotify dn(this);
	while (!fmt.IsEmpty())
		ctx << getNextChunk(fmt, nextDappChunk, &dn);
}

//---------------------------------------------------------------------------
SFString nextDappChunk(const SFString& fieldIn, SFBool& force, const void *data)
{
	CDappNotify *da = (CDappNotify*)data;
	const CDapp *dap = da->getDataPtr();

	// Now give customized code a chance to override
	SFString ret = nextDappChunk_custom(fieldIn, force, data);
	if (!ret.IsEmpty())
		return ret;

	switch (tolower(fieldIn[0]))
	{
		case 'd':
			if ( fieldIn % "dappName" ) return dap->dappName;
			if ( fieldIn % "description" ) return dap->description;
			break;
		case 'g':
			if ( fieldIn % "gitHub" ) return dap->gitHub;
			break;
		case 'l':
			if ( fieldIn % "license" ) return dap->license;
			if ( fieldIn % "last_update" ) return dap->last_update;
			break;
		case 'p':
			if ( fieldIn % "people" ) return dap->people;
			break;
		case 'r':
			if ( fieldIn % "reddit" ) return dap->reddit;
			break;
		case 's':
			if ( fieldIn % "site" ) return dap->site;
			if ( fieldIn % "status" ) return dap->status;
			break;
		case 't':
			if ( fieldIn % "tags" ) return dap->tags;
			break;
	}

	// Finally, give the parent class a chance
	ret = nextBasenodeChunk(fieldIn, force, dap);
	if (!ret.IsEmpty())
		return ret;

	return "<span class=warning>Field not found: [{" + fieldIn + "}]</span>\n";
}

//---------------------------------------------------------------------------------------------------
SFBool CDapp::setValueByName(const SFString& fieldName, const SFString& fieldValue)
{
	// EXISTING_CODE
	// EXISTING_CODE

	switch (tolower(fieldName[0]))
	{
		case 'd':
			if ( fieldName % "dappName" ) { dappName = fieldValue; return TRUE; }
			if ( fieldName % "description" ) { description = fieldValue; return TRUE; }
			break;
		case 'g':
			if ( fieldName % "gitHub" ) { gitHub = fieldValue; return TRUE; }
			break;
		case 'l':
			if ( fieldName % "license" ) { license = fieldValue; return TRUE; }
			if ( fieldName % "last_update" ) { last_update = fieldValue; return TRUE; }
			break;
		case 'p':
			if ( fieldName % "people" ) { people = fieldValue; return TRUE; }
			break;
		case 'r':
			if ( fieldName % "reddit" ) { reddit = fieldValue; return TRUE; }
			break;
		case 's':
			if ( fieldName % "site" ) { site = fieldValue; return TRUE; }
			if ( fieldName % "status" ) { status = fieldValue; return TRUE; }
			break;
		case 't':
			if ( fieldName % "tags" ) { tags = fieldValue; return TRUE; }
			break;
		default:
			break;
	}
	return FALSE;
}

//---------------------------------------------------------------------------------------------------
void CDapp::finishParse()
{
	// EXISTING_CODE
	// EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
void CDapp::Serialize(SFArchive& archive)
{
	if (!SerializeHeader(archive))
		return;

	if (archive.isReading())
	{
		archive >> dappName;
		archive >> description;
		archive >> site;
		archive >> gitHub;
		archive >> reddit;
		archive >> people;
		archive >> tags;
		archive >> license;
		archive >> status;
		archive >> last_update;
		finishParse();
	} else
	{
		archive << dappName;
		archive << description;
		archive << site;
		archive << gitHub;
		archive << reddit;
		archive << people;
		archive << tags;
		archive << license;
		archive << status;
		archive << last_update;

	}
}

//---------------------------------------------------------------------------
void CDapp::registerClass(void)
{
	static bool been_here=false;
	if (been_here) return;
	been_here=true;

	SFInt32 fieldNum=1000;
	ADD_FIELD(CDapp, "schema",  T_NUMBER|TS_LABEL, ++fieldNum);
	ADD_FIELD(CDapp, "deleted", T_RADIO|TS_LABEL,  ++fieldNum);
	ADD_FIELD(CDapp, "dappName", T_TEXT, ++fieldNum);
	ADD_FIELD(CDapp, "description", T_TEXT, ++fieldNum);
	ADD_FIELD(CDapp, "site", T_TEXT, ++fieldNum);
	ADD_FIELD(CDapp, "gitHub", T_TEXT, ++fieldNum);
	ADD_FIELD(CDapp, "reddit", T_TEXT, ++fieldNum);
	ADD_FIELD(CDapp, "people", T_TEXT, ++fieldNum);
	ADD_FIELD(CDapp, "tags", T_TEXT, ++fieldNum);
	ADD_FIELD(CDapp, "license", T_TEXT, ++fieldNum);
	ADD_FIELD(CDapp, "status", T_TEXT, ++fieldNum);
	ADD_FIELD(CDapp, "last_update", T_TEXT, ++fieldNum);

	// Hide our internal fields, user can turn them on if they like
	HIDE_FIELD(CDapp, "schema");
	HIDE_FIELD(CDapp, "deleted");

	// EXISTING_CODE
	// EXISTING_CODE
}

//---------------------------------------------------------------------------
int sortDapp(const SFString& f1, const SFString& f2, const void *rr1, const void *rr2)
{
	CDapp *g1 = (CDapp*)rr1;
	CDapp *g2 = (CDapp*)rr2;

	SFString v1 = g1->getValueByName(f1);
	SFString v2 = g2->getValueByName(f1);
	SFInt32 s = v1.Compare(v2);
	if (s || f2.IsEmpty())
		return (int)s;

	v1 = g1->getValueByName(f2);
	v2 = g2->getValueByName(f2);
	return (int)v1.Compare(v2);
}
int sortDappByName(const void *rr1, const void *rr2) { return sortDapp("da_Name", "", rr1, rr2); }
int sortDappByID  (const void *rr1, const void *rr2) { return sortDapp("dappID", "", rr1, rr2); }

//---------------------------------------------------------------------------
SFString nextDappChunk_custom(const SFString& fieldIn, SFBool& force, const void *data)
{
	CDappNotify *da = (CDappNotify*)data;
	const CDapp *dap = da->getDataPtr();
	switch (tolower(fieldIn[0]))
	{
		// EXISTING_CODE
		// EXISTING_CODE
		default:
			break;
	}

#pragma unused(da)
#pragma unused(dap)

	return EMPTY;
}

//---------------------------------------------------------------------------
SFBool CDapp::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const
{
	// EXISTING_CODE
	// EXISTING_CODE
	return FALSE;
}

//---------------------------------------------------------------------------
SFBool CDapp::readBackLevel(SFArchive& archive)
{
	SFBool done=FALSE;
	// EXISTING_CODE
	// EXISTING_CODE
	return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
