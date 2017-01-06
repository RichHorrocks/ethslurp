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
#include "rpcresult.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CRPCResult, CBaseNode, curVersion);

//---------------------------------------------------------------------------
void CRPCResult::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const
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

	CRPCResultNotify dn(this);
	while (!fmt.IsEmpty())
		ctx << getNextChunk(fmt, nextRpcresultChunk, &dn);
}

//---------------------------------------------------------------------------
SFString nextRpcresultChunk(const SFString& fieldIn, SFBool& force, const void *data)
{
	CRPCResultNotify *rp = (CRPCResultNotify*)data;
	const CRPCResult *rpc = rp->getDataPtr();

	// Now give customized code a chance to override
	SFString ret = nextRpcresultChunk_custom(fieldIn, force, data);
	if (!ret.IsEmpty())
		return ret;

	switch (tolower(fieldIn[0]))
	{
		case 'i':
			if ( fieldIn % "id" ) return rpc->id;
			break;
		case 'j':
			if ( fieldIn % "jsonrpc" ) return rpc->jsonrpc;
			break;
		case 'r':
			if ( fieldIn % "result" ) return rpc->result;
			break;
	}

	// Finally, give the parent class a chance
	ret = nextBasenodeChunk(fieldIn, force, rpc);
	if (!ret.IsEmpty())
		return ret;

	return "<span class=warning>Field not found: [{" + fieldIn + "}]</span>\n";
}

//---------------------------------------------------------------------------------------------------
SFBool CRPCResult::setValueByName(const SFString& fieldName, const SFString& fieldValue)
{
	// EXISTING_CODE
	// EXISTING_CODE

	switch (tolower(fieldName[0]))
	{
		case 'i':
			if ( fieldName % "id" ) { id = fieldValue; return TRUE; }
			break;
		case 'j':
			if ( fieldName % "jsonrpc" ) { jsonrpc = fieldValue; return TRUE; }
			break;
		case 'r':
			if ( fieldName % "result" ) { result = fieldValue; return TRUE; }
			break;
		default:
			break;
	}
	return FALSE;
}

//---------------------------------------------------------------------------------------------------
void CRPCResult::finishParse()
{
	// EXISTING_CODE
	// EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
void CRPCResult::Serialize(SFArchive& archive)
{
	if (!SerializeHeader(archive))
		return;

	if (archive.isReading())
	{
		archive >> jsonrpc;
		archive >> result;
		archive >> id;
		finishParse();
	} else
	{
		archive << jsonrpc;
		archive << result;
		archive << id;

	}
}

//---------------------------------------------------------------------------
void CRPCResult::registerClass(void)
{
	static bool been_here=false;
	if (been_here) return;
	been_here=true;

	SFInt32 fieldNum=1000;
	ADD_FIELD(CRPCResult, "schema",  T_NUMBER|TS_LABEL, ++fieldNum);
	ADD_FIELD(CRPCResult, "deleted", T_BOOL|TS_LABEL,  ++fieldNum);
	ADD_FIELD(CRPCResult, "jsonrpc", T_TEXT, ++fieldNum);
	ADD_FIELD(CRPCResult, "result", T_TEXT, ++fieldNum);
	ADD_FIELD(CRPCResult, "id", T_TEXT, ++fieldNum);

	// Hide our internal fields, user can turn them on if they like
	HIDE_FIELD(CRPCResult, "schema");
	HIDE_FIELD(CRPCResult, "deleted");

	// EXISTING_CODE
	// EXISTING_CODE
}

//---------------------------------------------------------------------------
int sortRpcresult(const SFString& f1, const SFString& f2, const void *rr1, const void *rr2)
{
	CRPCResult *g1 = (CRPCResult*)rr1;
	CRPCResult *g2 = (CRPCResult*)rr2;

	SFString v1 = g1->getValueByName(f1);
	SFString v2 = g2->getValueByName(f1);
	SFInt32 s = v1.Compare(v2);
	if (s || f2.IsEmpty())
		return (int)s;

	v1 = g1->getValueByName(f2);
	v2 = g2->getValueByName(f2);
	return (int)v1.Compare(v2);
}
int sortRpcresultByName(const void *rr1, const void *rr2) { return sortRpcresult("rp_Name", "", rr1, rr2); }
int sortRpcresultByID  (const void *rr1, const void *rr2) { return sortRpcresult("rpcresultID", "", rr1, rr2); }

//---------------------------------------------------------------------------
SFString nextRpcresultChunk_custom(const SFString& fieldIn, SFBool& force, const void *data)
{
	CRPCResultNotify *rp = (CRPCResultNotify*)data;
	const CRPCResult *rpc = rp->getDataPtr();
	switch (tolower(fieldIn[0]))
	{
		// EXISTING_CODE
		// EXISTING_CODE
		default:
			break;
	}

#pragma unused(rp)
#pragma unused(rpc)

	return EMPTY;
}

//---------------------------------------------------------------------------
SFBool CRPCResult::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const
{
	// EXISTING_CODE
	// EXISTING_CODE
	return FALSE;
}

//---------------------------------------------------------------------------
SFBool CRPCResult::readBackLevel(SFArchive& archive)
{
	SFBool done=FALSE;
	// EXISTING_CODE
	// EXISTING_CODE
	return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
