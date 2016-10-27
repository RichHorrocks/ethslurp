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
#include "trace.h"
#include "etherlib.h"
//---------------------------------------------------------------------------
IMPLEMENT_NODE(CTrace, CBaseNode, curVersion);

//---------------------------------------------------------------------------
void CTrace::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const
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

	CTraceNotify dn(this);
	while (!fmt.IsEmpty())
		ctx << getNextChunk(fmt, nextTraceChunk, &dn);
}

//---------------------------------------------------------------------------
SFString nextTraceChunk(const SFString& fieldIn, SFBool& force, const void *data)
{
	CTraceNotify *tr = (CTraceNotify*)data;
	const CTrace *tra = tr->getDataPtr();

	// Now give customized code a chance to override
	SFString ret = nextTraceChunk_custom(fieldIn, force, data);
	if (!ret.IsEmpty())
		return ret;

	switch (tolower(fieldIn[0]))
	{
		case 'g':
			if ( fieldIn % "gas" ) return asString(tra->gas);
			break;
		case 'r':
			if ( fieldIn % "returnValue" ) return tra->returnValue;
			break;
		case 's':
			if ( fieldIn % "structLogs" )
			{
				SFInt32 cnt = tra->structLogs.getCount();
				if (!cnt) return EMPTY;
				SFString ret = "[\n";
				for (int i=0;i<cnt;i++)
				{
					ret += tra->structLogs[i].Format();
					ret += ((i<cnt-1) ? ",\n" : "\n");
				}
				return ret+"    ]";
			}
			break;
	}

	// Finally, give the parent class a chance
	ret = nextBasenodeChunk(fieldIn, force, tra);
	if (!ret.IsEmpty())
		return ret;

	return "<span class=warning>Field not found: [{" + fieldIn + "}]</span>\n";
}

//---------------------------------------------------------------------------------------------------
SFBool CTrace::setValueByName(const SFString& fieldName, const SFString& fieldValue)
{
	// EXISTING_CODE
	if (fieldName % "structLogs")
	{
		char *p = (char *)(const char*)fieldValue;
		while (p && *p)
		{
			CStructLog sl;SFInt32 nFields=0;
			p = sl.parseJson(p,nFields);
			structLogs[structLogs.getCount()] = sl;
		}
		return TRUE;
	}
	// EXISTING_CODE

	switch (tolower(fieldName[0]))
	{
		case 'g':
			if ( fieldName % "gas" ) { gas = toLong(fieldValue); return TRUE; }
			break;
		case 'r':
			if ( fieldName % "returnValue" ) { returnValue = fieldValue; return TRUE; }
			break;
		case 's':
			if ( fieldName % "structLogs" ) return TRUE;
			break;
		default:
			break;
	}
	return FALSE;
}

//---------------------------------------------------------------------------------------------------
void CTrace::finishParse()
{
	// EXISTING_CODE
	// EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
void CTrace::Serialize(SFArchive& archive)
{
	if (!SerializeHeader(archive))
		return;

	if (archive.isReading())
	{
		archive >> gas;
		archive >> returnValue;
		archive >> structLogs;
		finishParse();
	} else
	{
		archive << gas;
		archive << returnValue;
		archive << structLogs;

	}
}

//---------------------------------------------------------------------------
void CTrace::registerClass(void)
{
	static bool been_here=false;
	if (been_here) return;
	been_here=true;

	SFInt32 fieldNum=1000;
	ADD_FIELD(CTrace, "schema",  T_NUMBER|TS_LABEL, ++fieldNum);
	ADD_FIELD(CTrace, "deleted", T_RADIO|TS_LABEL,  ++fieldNum);
	ADD_FIELD(CTrace, "gas", T_NUMBER, ++fieldNum);
	ADD_FIELD(CTrace, "returnValue", T_TEXT, ++fieldNum);
	ADD_FIELD(CTrace, "structLogs", T_TEXT|TS_ARRAY, ++fieldNum);

	// Hide our internal fields, user can turn them on if they like
	HIDE_FIELD(CTrace, "schema");
	HIDE_FIELD(CTrace, "deleted");

	// EXISTING_CODE
	// EXISTING_CODE
}

//---------------------------------------------------------------------------
int sortTrace(const SFString& f1, const SFString& f2, const void *rr1, const void *rr2)
{
	CTrace *g1 = (CTrace*)rr1;
	CTrace *g2 = (CTrace*)rr2;

	SFString v1 = g1->getValueByName(f1);
	SFString v2 = g2->getValueByName(f1);
	SFInt32 s = v1.Compare(v2);
	if (s || f2.IsEmpty())
		return (int)s;

	v1 = g1->getValueByName(f2);
	v2 = g2->getValueByName(f2);
	return (int)v1.Compare(v2);
}
int sortTraceByName(const void *rr1, const void *rr2) { return sortTrace("tr_Name", "", rr1, rr2); }
int sortTraceByID  (const void *rr1, const void *rr2) { return sortTrace("traceID", "", rr1, rr2); }

//---------------------------------------------------------------------------
SFString nextTraceChunk_custom(const SFString& fieldIn, SFBool& force, const void *data)
{
	return EMPTY;
}

//---------------------------------------------------------------------------
SFBool CTrace::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const
{
	// EXISTING_CODE
	// EXISTING_CODE
	return FALSE;
}

//---------------------------------------------------------------------------
SFBool CTrace::readBackLevel(SFArchive& archive)
{
	SFBool done=FALSE;
	// EXISTING_CODE
	// EXISTING_CODE
	return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
