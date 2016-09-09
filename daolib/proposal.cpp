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
 * This file was generated with makeClass. Edit only those parts inside
 * of 'EXISTING_CODE' tags.
 */
#include "proposal.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CProposal, CTransaction, NO_SCHEMA);

//---------------------------------------------------------------------------
void CProposal::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const
{
	if (!isShowing())
		return;

	SFString fmt = (fmtIn.IsEmpty() ? defaultFormat() : fmtIn);
	if (handleCustomFormat(ctx, fmt, data))
		return;

	CProposalNotify dn(this);
	while (!fmt.IsEmpty())
		ctx << getNextChunk(fmt, nextProposalChunk, &dn);
}

//---------------------------------------------------------------------------
SFString nextProposalChunk(const SFString& fieldIn, SFBool& force, const void *data)
{
	CProposalNotify *pr = (CProposalNotify*)data;
	const CProposal *pro = pr->getDataPtr();

	// Now give customized code a chance to override
	SFString ret = nextProposalChunk_custom(fieldIn, force, data);
	if (!ret.IsEmpty())
		return ret;

	switch (tolower(fieldIn[0]))
	{
		case 'a':
			if ( fieldIn % "amount" ) return pro->amount;
			break;
		case 'c':
			if ( fieldIn % "creator" ) return pro->creator;
			break;
		case 'd':
			if ( fieldIn % "data" ) return pro->data;
			if ( fieldIn % "debatePeriod" ) return asString(pro->debatePeriod);
			if ( fieldIn % "description" ) return pro->description;
			break;
		case 'i':
			if ( fieldIn % "isSplit" ) return asString(pro->isSplit);
			break;
		case 'p':
			if ( fieldIn % "proposalID" ) return padNum3(pro->proposalID);
			break;
		case 'r':
			if ( fieldIn % "recipient" ) return pro->recipient;
			break;
	}

	// Finally, give the parent class a chance
	CTransactionNotify dn(pro);
	ret = nextTransactionChunk(fieldIn, force, &dn);
	if (!ret.IsEmpty())
		return ret;

	return "<span class=warning>Field not found: [{" + fieldIn + "}]</span>\n";
}

//---------------------------------------------------------------------------------------------------
SFBool CProposal::setValueByName(const SFString& fieldName, const SFString& fieldValue)
{
	// EXISTING_CODE
	// EXISTING_CODE

	if (CTransaction::setValueByName(fieldName, fieldValue))
		return TRUE;

	switch (tolower(fieldName[0]))
	{
		case 'a':
			if ( fieldName % "amount" ) { amount = fieldValue; return TRUE; }
			break;
		case 'c':
			if ( fieldName % "creator" ) { creator = toLower(fieldValue); return TRUE; }
			break;
		case 'd':
			if ( fieldName % "data" ) { data = fieldValue; return TRUE; }
			if ( fieldName % "debatePeriod" ) { debatePeriod = toLong(fieldValue); return TRUE; }
			if ( fieldName % "description" ) { description = fieldValue; return TRUE; }
			break;
		case 'i':
			if ( fieldName % "isSplit" ) { isSplit = toBool(fieldValue); return TRUE; }
			break;
		case 'p':
			if ( fieldName % "proposalID" ) { proposalID = toLong(fieldValue); return TRUE; }
			break;
		case 'r':
			if ( fieldName % "recipient" ) { recipient = toLower(fieldValue); return TRUE; }
			break;
		default:
			break;
	}
	return FALSE;
}

//---------------------------------------------------------------------------------------------------
void CProposal::finishParse()
{
	// EXISTING_CODE
	// EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
void CProposal::Serialize(SFArchive& archive)
{
	CTransaction::Serialize(archive);

	if (archive.isReading())
	{
		archive >> proposalID;
		archive >> creator;
		archive >> recipient;
		archive >> amount;
		archive >> data;
		archive >> debatePeriod;
		archive >> description;
		archive >> isSplit;
		finishParse();
	} else
	{
		archive << proposalID;
		archive << creator;
		archive << recipient;
		archive << amount;
		archive << data;
		archive << debatePeriod;
		archive << description;
		archive << isSplit;

	}
}

//---------------------------------------------------------------------------
void CProposal::registerClass(void)
{
	static bool been_here=false;
	if (been_here) return;
	been_here=true;

	CTransaction::registerClass();

	SFInt32 fieldNum=1000;
	ADD_FIELD(CProposal, "schema",  T_NUMBER|TS_LABEL, ++fieldNum);
	ADD_FIELD(CProposal, "deleted", T_RADIO|TS_LABEL,  ++fieldNum);
	ADD_FIELD(CProposal, "handle", T_NUMBER|TS_LABEL,  ++fieldNum);
	ADD_FIELD(CProposal, "proposalID", T_NUMBER, ++fieldNum);
	ADD_FIELD(CProposal, "creator", T_TEXT, ++fieldNum);
	ADD_FIELD(CProposal, "recipient", T_TEXT, ++fieldNum);
	ADD_FIELD(CProposal, "amount", T_TEXT, ++fieldNum);
	ADD_FIELD(CProposal, "data", T_TEXT, ++fieldNum);
	ADD_FIELD(CProposal, "debatePeriod", T_NUMBER, ++fieldNum);
	ADD_FIELD(CProposal, "description", T_TEXT, ++fieldNum);
	ADD_FIELD(CProposal, "isSplit", T_RADIO, ++fieldNum);

	// Hide our internal fields, user can turn them on if they like
	HIDE_FIELD(CProposal, "schema");
	HIDE_FIELD(CProposal, "deleted");
	HIDE_FIELD(CProposal, "handle");

	// EXISTING_CODE
	// EXISTING_CODE
}

//---------------------------------------------------------------------------
int sortProposal(const SFString& f1, const SFString& f2, const void *rr1, const void *rr2)
{
	CProposal *g1 = (CProposal*)rr1;
	CProposal *g2 = (CProposal*)rr2;

	SFString v1 = g1->getValueByName(f1);
	SFString v2 = g2->getValueByName(f1);
	SFInt32 s = v1.Compare(v2);
	if (s || f2.IsEmpty())
		return (int)s;

	v1 = g1->getValueByName(f2);
	v2 = g2->getValueByName(f2);
	return (int)v1.Compare(v2);
}
int sortProposalByName(const void *rr1, const void *rr2) { return sortProposal("pr_Name", "", rr1, rr2); }
int sortProposalByID  (const void *rr1, const void *rr2) { return sortProposal("proposalID", "", rr1, rr2); }

//---------------------------------------------------------------------------
SFString nextProposalChunk_custom(const SFString& fieldIn, SFBool& force, const void *data)
{
	CProposalNotify *pr = (CProposalNotify*)data;
	const CProposal *pro = pr->getDataPtr();
	switch (tolower(fieldIn[0]))
	{
		// EXISTING_CODE
		// EXISTING_CODE
		default:
			break;
	}

#pragma unused(pr)
#pragma unused(pro)

	return EMPTY;
}

//---------------------------------------------------------------------------
SFBool CProposal::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const
{
	// EXISTING_CODE
	// EXISTING_CODE
	return FALSE;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
