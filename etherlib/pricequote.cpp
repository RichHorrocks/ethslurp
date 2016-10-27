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
#include "pricequote.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CPriceQuote, CBaseNode, curVersion);

//---------------------------------------------------------------------------
void CPriceQuote::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const
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

	CPriceQuoteNotify dn(this);
	while (!fmt.IsEmpty())
		ctx << getNextChunk(fmt, nextPricequoteChunk, &dn);
}

//---------------------------------------------------------------------------
SFString nextPricequoteChunk(const SFString& fieldIn, SFBool& force, const void *data)
{
	CPriceQuoteNotify *pr = (CPriceQuoteNotify*)data;
	const CPriceQuote *pri = pr->getDataPtr();

	// Now give customized code a chance to override
	SFString ret = nextPricequoteChunk_custom(fieldIn, force, data);
	if (!ret.IsEmpty())
		return ret;

	switch (tolower(fieldIn[0]))
	{
		case 'c':
			if ( fieldIn % "close" ) return asStringF(pri->close);
			break;
		case 'h':
			if ( fieldIn % "high" ) return asStringF(pri->high);
			break;
		case 'l':
			if ( fieldIn % "low" ) return asStringF(pri->low);
			break;
		case 'o':
			if ( fieldIn % "open" ) return asStringF(pri->open);
			break;
		case 'q':
			if ( fieldIn % "quoteVolume" ) return asStringF(pri->quoteVolume);
			break;
		case 't':
			if ( fieldIn % "timeStamp" ) return asString(pri->timeStamp);
			break;
		case 'v':
			if ( fieldIn % "volume" ) return asStringF(pri->volume);
			break;
		case 'w':
			if ( fieldIn % "weightedAvg" ) return asStringF(pri->weightedAvg);
			break;
	}

	// Finally, give the parent class a chance
	ret = nextBasenodeChunk(fieldIn, force, pri);
	if (!ret.IsEmpty())
		return ret;

	return "<span class=warning>Field not found: [{" + fieldIn + "}]</span>\n";
}

//---------------------------------------------------------------------------------------------------
SFBool CPriceQuote::setValueByName(const SFString& fieldName, const SFString& fieldValue)
{
	// EXISTING_CODE
	if ( fieldName % "date" || fieldName % "timeStamp" )
	{
		timeStamp = toLong(fieldValue);
		date = dateFromTimeStamp(timeStamp);
		return TRUE;
	}
	// EXISTING_CODE

	switch (tolower(fieldName[0]))
	{
		case 'c':
			if ( fieldName % "close" ) { close = toFloat(fieldValue); return TRUE; }
			break;
		case 'h':
			if ( fieldName % "high" ) { high = toFloat(fieldValue); return TRUE; }
			break;
		case 'l':
			if ( fieldName % "low" ) { low = toFloat(fieldValue); return TRUE; }
			break;
		case 'o':
			if ( fieldName % "open" ) { open = toFloat(fieldValue); return TRUE; }
			break;
		case 'q':
			if ( fieldName % "quoteVolume" ) { quoteVolume = toFloat(fieldValue); return TRUE; }
			break;
		case 't':
			if ( fieldName % "timeStamp" ) { timeStamp = toLong(fieldValue); return TRUE; }
			break;
		case 'v':
			if ( fieldName % "volume" ) { volume = toFloat(fieldValue); return TRUE; }
			break;
		case 'w':
			if ( fieldName % "weightedAvg" ) { weightedAvg = toFloat(fieldValue); return TRUE; }
			break;
		default:
			break;
	}
	return FALSE;
}

//---------------------------------------------------------------------------------------------------
void CPriceQuote::finishParse()
{
	// EXISTING_CODE
	date = dateFromTimeStamp(timeStamp);
	// EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
void CPriceQuote::Serialize(SFArchive& archive)
{
	if (!SerializeHeader(archive))
		return;

	if (archive.isReading())
	{
		archive >> timeStamp;
		archive >> open;
		archive >> high;
		archive >> low;
		archive >> close;
		archive >> quoteVolume;
		archive >> volume;
		archive >> weightedAvg;
		finishParse();
	} else
	{
		archive << timeStamp;
		archive << open;
		archive << high;
		archive << low;
		archive << close;
		archive << quoteVolume;
		archive << volume;
		archive << weightedAvg;

	}
}

//---------------------------------------------------------------------------
void CPriceQuote::registerClass(void)
{
	static bool been_here=false;
	if (been_here) return;
	been_here=true;

	SFInt32 fieldNum=1000;
	ADD_FIELD(CPriceQuote, "schema",  T_NUMBER|TS_LABEL, ++fieldNum);
	ADD_FIELD(CPriceQuote, "deleted", T_RADIO|TS_LABEL,  ++fieldNum);
	ADD_FIELD(CPriceQuote, "timeStamp", T_NUMBER, ++fieldNum);
	ADD_FIELD(CPriceQuote, "open", T_FLOAT, ++fieldNum);
	ADD_FIELD(CPriceQuote, "high", T_FLOAT, ++fieldNum);
	ADD_FIELD(CPriceQuote, "low", T_FLOAT, ++fieldNum);
	ADD_FIELD(CPriceQuote, "close", T_FLOAT, ++fieldNum);
	ADD_FIELD(CPriceQuote, "quoteVolume", T_FLOAT, ++fieldNum);
	ADD_FIELD(CPriceQuote, "volume", T_FLOAT, ++fieldNum);
	ADD_FIELD(CPriceQuote, "weightedAvg", T_FLOAT, ++fieldNum);

	// Hide our internal fields, user can turn them on if they like
	HIDE_FIELD(CPriceQuote, "schema");
	HIDE_FIELD(CPriceQuote, "deleted");

	// EXISTING_CODE
	ADD_FIELD(CPriceQuote, "date", T_DATE, ++fieldNum);
	// EXISTING_CODE
}

//---------------------------------------------------------------------------
int sortPricequote(const SFString& f1, const SFString& f2, const void *rr1, const void *rr2)
{
	CPriceQuote *g1 = (CPriceQuote*)rr1;
	CPriceQuote *g2 = (CPriceQuote*)rr2;

	SFString v1 = g1->getValueByName(f1);
	SFString v2 = g2->getValueByName(f1);
	SFInt32 s = v1.Compare(v2);
	if (s || f2.IsEmpty())
		return (int)s;

	v1 = g1->getValueByName(f2);
	v2 = g2->getValueByName(f2);
	return (int)v1.Compare(v2);
}
int sortPricequoteByName(const void *rr1, const void *rr2) { return sortPricequote("pr_Name", "", rr1, rr2); }
int sortPricequoteByID  (const void *rr1, const void *rr2) { return sortPricequote("pricequoteID", "", rr1, rr2); }

//---------------------------------------------------------------------------
SFString nextPricequoteChunk_custom(const SFString& fieldIn, SFBool& force, const void *data)
{
	CPriceQuoteNotify *pr = (CPriceQuoteNotify*)data;
	const CPriceQuote *pri = pr->getDataPtr();
	switch (tolower(fieldIn[0]))
	{
		// EXISTING_CODE
		case 'd':
			if ( fieldIn % "date" ) return pri->date.Format(FMT_JSON);
			break;
		// EXISTING_CODE
		default:
			break;
	}

	return EMPTY;
}

//---------------------------------------------------------------------------
SFBool CPriceQuote::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const
{
	// EXISTING_CODE
	// EXISTING_CODE
	return FALSE;
}

//---------------------------------------------------------------------------
SFBool CPriceQuote::readBackLevel(SFArchive& archive)
{
	SFBool done=FALSE;
	// EXISTING_CODE
	// EXISTING_CODE
	return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
