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
#include "vote.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CVote, CTransaction, curVersion);

//---------------------------------------------------------------------------
void CVote::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const
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

	CVoteNotify dn(this);
	while (!fmt.IsEmpty())
		ctx << getNextChunk(fmt, nextVoteChunk, &dn);
}

//---------------------------------------------------------------------------
SFString nextVoteChunk(const SFString& fieldIn, SFBool& force, const void *data)
{
	CVoteNotify *vo = (CVoteNotify*)data;
	const CVote *vot = vo->getDataPtr();

	// Now give customized code a chance to override
	SFString ret = nextVoteChunk_custom(fieldIn, force, data);
	if (!ret.IsEmpty())
		return ret;

	switch (tolower(fieldIn[0]))
	{
		case 'p':
			if ( fieldIn % "proposalID" ) return padNum3(vot->proposalID);
			break;
		case 'v':
			if ( fieldIn % "voter" ) return vot->voter;
			if ( fieldIn % "votedYes" ) return asString(vot->votedYes);
			break;
	}

	// Finally, give the parent class a chance
	CTransactionNotify dn(vot);
	ret = nextTransactionChunk(fieldIn, force, &dn);
	if (!ret.IsEmpty())
		return ret;

	return "<span class=warning>Field not found: [{" + fieldIn + "}]</span>\n";
}

//---------------------------------------------------------------------------------------------------
SFBool CVote::setValueByName(const SFString& fieldName, const SFString& fieldValue)
{
	// EXISTING_CODE
	if ( fieldName % "function" )
	{
		SFString val = fieldValue;
		SFString func = nextTokenClear(val,'|'); // skip
		SFString prop = nextTokenClear(val,'|');
		SFString vote = nextTokenClear(val,'|');
		setValueByName("proposalID", prop);
		setValueByName("votedYes",   vote == "Yea" ? "1" : "0");
		return TRUE;
	}
	// EXISTING_CODE

	if (CTransaction::setValueByName(fieldName, fieldValue))
		return TRUE;

	switch (tolower(fieldName[0]))
	{
		case 'p':
			if ( fieldName % "proposalID" ) { proposalID = toLong(fieldValue); return TRUE; }
			break;
		case 'v':
			if ( fieldName % "voter" ) { voter = toLower(fieldValue); return TRUE; }
			if ( fieldName % "votedYes" ) { votedYes = toBool(fieldValue); return TRUE; }
			break;
		default:
			break;
	}
	return FALSE;
}

//---------------------------------------------------------------------------------------------------
void CVote::finishParse()
{
	// EXISTING_CODE
	// EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
void CVote::Serialize(SFArchive& archive)
{
	CTransaction::Serialize(archive);

	if (archive.isReading())
	{
		archive >> voter;
		archive >> proposalID;
		archive >> votedYes;
		finishParse();
	} else
	{
		archive << voter;
		archive << proposalID;
		archive << votedYes;

	}
}

//---------------------------------------------------------------------------
void CVote::registerClass(void)
{
	static bool been_here=false;
	if (been_here) return;
	been_here=true;

	CTransaction::registerClass();

	SFInt32 fieldNum=1000;
	ADD_FIELD(CVote, "schema",  T_NUMBER|TS_LABEL, ++fieldNum);
	ADD_FIELD(CVote, "deleted", T_RADIO|TS_LABEL,  ++fieldNum);
	ADD_FIELD(CVote, "voter", T_TEXT, ++fieldNum);
	ADD_FIELD(CVote, "proposalID", T_NUMBER, ++fieldNum);
	ADD_FIELD(CVote, "votedYes", T_RADIO, ++fieldNum);

	// Hide our internal fields, user can turn them on if they like
	HIDE_FIELD(CVote, "schema");
	HIDE_FIELD(CVote, "deleted");

	// EXISTING_CODE
	// EXISTING_CODE
}

//---------------------------------------------------------------------------
int sortVote(const SFString& f1, const SFString& f2, const void *rr1, const void *rr2)
{
	CVote *g1 = (CVote*)rr1;
	CVote *g2 = (CVote*)rr2;

	SFString v1 = g1->getValueByName(f1);
	SFString v2 = g2->getValueByName(f1);
	SFInt32 s = v1.Compare(v2);
	if (s || f2.IsEmpty())
		return (int)s;

	v1 = g1->getValueByName(f2);
	v2 = g2->getValueByName(f2);
	return (int)v1.Compare(v2);
}
int sortVoteByName(const void *rr1, const void *rr2) { return sortVote("vo_Name", "", rr1, rr2); }
int sortVoteByID  (const void *rr1, const void *rr2) { return sortVote("voteID", "", rr1, rr2); }

//---------------------------------------------------------------------------
SFString nextVoteChunk_custom(const SFString& fieldIn, SFBool& force, const void *data)
{
	CVoteNotify *vo = (CVoteNotify*)data;
	const CVote *vot = vo->getDataPtr();
	switch (tolower(fieldIn[0]))
	{
		// EXISTING_CODE
		// EXISTING_CODE
		default:
			break;
	}

#pragma unused(vo)
#pragma unused(vot)

	return EMPTY;
}

//---------------------------------------------------------------------------
SFBool CVote::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const
{
	// EXISTING_CODE
	// EXISTING_CODE
	return FALSE;
}

//---------------------------------------------------------------------------
SFBool CVote::readBackLevel(SFArchive& archive)
{
	SFBool done=FALSE;
	// EXISTING_CODE
	// EXISTING_CODE
	return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
