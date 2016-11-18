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
#include "logentry.h"
#include "etherlib.h"
//---------------------------------------------------------------------------
IMPLEMENT_NODE(CLogEntry, CBaseNode, curVersion);

//---------------------------------------------------------------------------
void CLogEntry::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const
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

	CLogEntryNotify dn(this);
	while (!fmt.IsEmpty())
		ctx << getNextChunk(fmt, nextLogentryChunk, &dn);
}

//---------------------------------------------------------------------------
SFString nextLogentryChunk(const SFString& fieldIn, SFBool& force, const void *data)
{
	CLogEntryNotify *lo = (CLogEntryNotify*)data;
	const CLogEntry *log = lo->getDataPtr();

	// Now give customized code a chance to override
	SFString ret = nextLogentryChunk_custom(fieldIn, force, data);
	if (!ret.IsEmpty())
		return ret;

	switch (tolower(fieldIn[0]))
	{
		case 'a':
			if ( fieldIn % "address" ) return log->address;
			break;
		case 'b':
			if ( fieldIn % "blockHash" ) return log->blockHash;
			if ( fieldIn % "blockNumber" ) return "0x"+asHex8(log->blockNumber);
			break;
		case 'd':
			if ( fieldIn % "data" ) return log->data;
			break;
		case 'l':
			if ( fieldIn % "logIndex" ) return "0x"+asHex8(log->logIndex);
			break;
		case 't':
			if ( fieldIn % "topics" )
			{
				SFInt32 cnt = log->topics.getCount();
				SFString ret;
				for (int i=0;i<cnt;i++)
				{
					ret += "\"" + log->topics[i] + "\"";
					if (i<cnt-1) ret += ", ";
				}
				return ret;
			}
			if ( fieldIn % "transactionHash" ) return log->transactionHash;
			if ( fieldIn % "transactionIndex" ) return "0x"+asHex8(log->transactionIndex);
			break;
	}

	// Finally, give the parent class a chance
	ret = nextBasenodeChunk(fieldIn, force, log);
	if (!ret.IsEmpty())
		return ret;

	return "<span class=warning>Field not found: [{" + fieldIn + "}]</span>\n";
}

//---------------------------------------------------------------------------------------------------
SFBool CLogEntry::setValueByName(const SFString& fieldName, const SFString& fieldValue)
{
	// EXISTING_CODE
	if (fieldName % "topics")
        {
		SFString str = fieldValue;
		while (!str.IsEmpty())
		{
			SFString topic = nextTokenClear(str,',');
			topics[topics.getCount()] = topic;
		}
		return TRUE;
	}
	// EXISTING_CODE

	switch (tolower(fieldName[0]))
	{
		case 'a':
			if ( fieldName % "address" ) { address = toLower(fieldValue); return TRUE; }
			break;
		case 'b':
			if ( fieldName % "blockHash" ) { blockHash = toLower(fieldValue); return TRUE; }
			if ( fieldName % "blockNumber" ) { blockNumber = thing(fieldValue); return TRUE; }
			break;
		case 'd':
			if ( fieldName % "data" ) { data = fieldValue; return TRUE; }
			break;
		case 'l':
			if ( fieldName % "logIndex" ) { logIndex = thing(fieldValue); return TRUE; }
			break;
		case 't':
			if ( fieldName % "topics" ) return TRUE;
			if ( fieldName % "transactionHash" ) { transactionHash = toLower(fieldValue); return TRUE; }
			if ( fieldName % "transactionIndex" ) { transactionIndex = thing(fieldValue); return TRUE; }
			break;
		default:
			break;
	}
	return FALSE;
}

//---------------------------------------------------------------------------------------------------
void CLogEntry::finishParse()
{
	// EXISTING_CODE
	// EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
void CLogEntry::Serialize(SFArchive& archive)
{
	if (!SerializeHeader(archive))
		return;

	if (archive.isReading())
	{
		archive >> address;
		archive >> blockHash;
		archive >> blockNumber;
		archive >> data;
		archive >> logIndex;
		archive >> topics;
		archive >> transactionHash;
		archive >> transactionIndex;
		finishParse();
	} else
	{
		archive << address;
		archive << blockHash;
		archive << blockNumber;
		archive << data;
		archive << logIndex;
		archive << topics;
		archive << transactionHash;
		archive << transactionIndex;

	}
}

//---------------------------------------------------------------------------
void CLogEntry::registerClass(void)
{
	static bool been_here=false;
	if (been_here) return;
	been_here=true;

	SFInt32 fieldNum=1000;
	ADD_FIELD(CLogEntry, "schema",  T_NUMBER|TS_LABEL, ++fieldNum);
	ADD_FIELD(CLogEntry, "deleted", T_RADIO|TS_LABEL,  ++fieldNum);
	ADD_FIELD(CLogEntry, "address", T_TEXT, ++fieldNum);
	ADD_FIELD(CLogEntry, "blockHash", T_TEXT, ++fieldNum);
	ADD_FIELD(CLogEntry, "blockNumber", T_NUMBER, ++fieldNum);
	ADD_FIELD(CLogEntry, "data", T_TEXT, ++fieldNum);
	ADD_FIELD(CLogEntry, "logIndex", T_NUMBER, ++fieldNum);
	ADD_FIELD(CLogEntry, "topics", T_TEXT|TS_ARRAY, ++fieldNum);
	ADD_FIELD(CLogEntry, "transactionHash", T_TEXT, ++fieldNum);
	ADD_FIELD(CLogEntry, "transactionIndex", T_NUMBER, ++fieldNum);

	// Hide our internal fields, user can turn them on if they like
	HIDE_FIELD(CLogEntry, "schema");
	HIDE_FIELD(CLogEntry, "deleted");

	// EXISTING_CODE
	// EXISTING_CODE
}

//---------------------------------------------------------------------------
int sortLogentry(const SFString& f1, const SFString& f2, const void *rr1, const void *rr2)
{
	CLogEntry *g1 = (CLogEntry*)rr1;
	CLogEntry *g2 = (CLogEntry*)rr2;

	SFString v1 = g1->getValueByName(f1);
	SFString v2 = g2->getValueByName(f1);
	SFInt32 s = v1.Compare(v2);
	if (s || f2.IsEmpty())
		return (int)s;

	v1 = g1->getValueByName(f2);
	v2 = g2->getValueByName(f2);
	return (int)v1.Compare(v2);
}
int sortLogentryByName(const void *rr1, const void *rr2) { return sortLogentry("lo_Name", "", rr1, rr2); }
int sortLogentryByID  (const void *rr1, const void *rr2) { return sortLogentry("logentryID", "", rr1, rr2); }

//---------------------------------------------------------------------------
SFString nextLogentryChunk_custom(const SFString& fieldIn, SFBool& force, const void *data)
{
	return EMPTY;
}

//---------------------------------------------------------------------------
SFBool CLogEntry::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const
{
	// EXISTING_CODE
	// EXISTING_CODE
	return FALSE;
}

//---------------------------------------------------------------------------
SFBool CLogEntry::readBackLevel(SFArchive& archive)
{
	SFBool done=FALSE;
	// EXISTING_CODE
	// EXISTING_CODE
	return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
