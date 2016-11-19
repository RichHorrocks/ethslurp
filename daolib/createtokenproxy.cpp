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
#include "createtokenproxy.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CCreateTokenProxy, CTransaction, curVersion);

//---------------------------------------------------------------------------
void CCreateTokenProxy::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const
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

	CCreateTokenProxyNotify dn(this);
	while (!fmt.IsEmpty())
		ctx << getNextChunk(fmt, nextCreatetokenproxyChunk, &dn);
}

//---------------------------------------------------------------------------
SFString nextCreatetokenproxyChunk(const SFString& fieldIn, SFBool& force, const void *data)
{
	CCreateTokenProxyNotify *cr = (CCreateTokenProxyNotify*)data;
	const CCreateTokenProxy *cre = cr->getDataPtr();

	// Now give customized code a chance to override
	SFString ret = nextCreatetokenproxyChunk_custom(fieldIn, force, data);
	if (!ret.IsEmpty())
		return ret;

	switch (tolower(fieldIn[0]))
	{
		case 't':
			if ( fieldIn % "tokenHolder" ) return cre->tokenHolder;
			break;
	}

	// Finally, give the parent class a chance
	CTransactionNotify dn(cre);
	ret = nextTransactionChunk(fieldIn, force, &dn);
	if (!ret.IsEmpty())
		return ret;

	return "<span class=warning>Field not found: [{" + fieldIn + "}]</span>\n";
}

//---------------------------------------------------------------------------------------------------
SFBool CCreateTokenProxy::setValueByName(const SFString& fieldName, const SFString& fieldValue)
{
	// EXISTING_CODE
	// EXISTING_CODE

	if (CTransaction::setValueByName(fieldName, fieldValue))
		return TRUE;

	switch (tolower(fieldName[0]))
	{
		case 't':
			if ( fieldName % "tokenHolder" ) { tokenHolder = toLower(fieldValue); return TRUE; }
			break;
		default:
			break;
	}
	return FALSE;
}

//---------------------------------------------------------------------------------------------------
void CCreateTokenProxy::finishParse()
{
	// EXISTING_CODE
	// EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
void CCreateTokenProxy::Serialize(SFArchive& archive)
{
	CTransaction::Serialize(archive);

	if (archive.isReading())
	{
		archive >> tokenHolder;
		finishParse();
	} else
	{
		archive << tokenHolder;

	}
}

//---------------------------------------------------------------------------
void CCreateTokenProxy::registerClass(void)
{
	static bool been_here=false;
	if (been_here) return;
	been_here=true;

	CTransaction::registerClass();

	SFInt32 fieldNum=1000;
	ADD_FIELD(CCreateTokenProxy, "schema",  T_NUMBER|TS_LABEL, ++fieldNum);
	ADD_FIELD(CCreateTokenProxy, "deleted", T_BOOL|TS_LABEL,  ++fieldNum);
	ADD_FIELD(CCreateTokenProxy, "tokenHolder", T_TEXT, ++fieldNum);

	// Hide our internal fields, user can turn them on if they like
	HIDE_FIELD(CCreateTokenProxy, "schema");
	HIDE_FIELD(CCreateTokenProxy, "deleted");

	// EXISTING_CODE
	// EXISTING_CODE
}

//---------------------------------------------------------------------------
int sortCreatetokenproxy(const SFString& f1, const SFString& f2, const void *rr1, const void *rr2)
{
	CCreateTokenProxy *g1 = (CCreateTokenProxy*)rr1;
	CCreateTokenProxy *g2 = (CCreateTokenProxy*)rr2;

	SFString v1 = g1->getValueByName(f1);
	SFString v2 = g2->getValueByName(f1);
	SFInt32 s = v1.Compare(v2);
	if (s || f2.IsEmpty())
		return (int)s;

	v1 = g1->getValueByName(f2);
	v2 = g2->getValueByName(f2);
	return (int)v1.Compare(v2);
}
int sortCreatetokenproxyByName(const void *rr1, const void *rr2) { return sortCreatetokenproxy("cr_Name", "", rr1, rr2); }
int sortCreatetokenproxyByID  (const void *rr1, const void *rr2) { return sortCreatetokenproxy("createtokenproxyID", "", rr1, rr2); }

//---------------------------------------------------------------------------
SFString nextCreatetokenproxyChunk_custom(const SFString& fieldIn, SFBool& force, const void *data)
{
	return EMPTY;
}

//---------------------------------------------------------------------------
SFBool CCreateTokenProxy::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const
{
	// EXISTING_CODE
	// EXISTING_CODE
	return FALSE;
}

//---------------------------------------------------------------------------
SFBool CCreateTokenProxy::readBackLevel(SFArchive& archive)
{
	SFBool done=FALSE;
	// EXISTING_CODE
	// EXISTING_CODE
	return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
