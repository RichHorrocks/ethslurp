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
#include "structlog.h"
#include "etherlib.h"
//---------------------------------------------------------------------------
IMPLEMENT_NODE(CStructLog, CBaseNode, curVersion);

//---------------------------------------------------------------------------
void CStructLog::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const
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

	CStructLogNotify dn(this);
	while (!fmt.IsEmpty())
		ctx << getNextChunk(fmt, nextStructlogChunk, &dn);
}

//---------------------------------------------------------------------------
SFString nextStructlogChunk(const SFString& fieldIn, SFBool& force, const void *data)
{
	CStructLogNotify *st = (CStructLogNotify*)data;
	const CStructLog *str = st->getDataPtr();

	// Now give customized code a chance to override
	SFString ret = nextStructlogChunk_custom(fieldIn, force, data);
	if (!ret.IsEmpty())
		return ret;

	switch (tolower(fieldIn[0]))
	{
		case 'd':
			if ( fieldIn % "depth" ) return asString(str->depth);
			break;
		case 'e':
			if ( fieldIn % "error" ) return asString(str->error);
			break;
		case 'g':
			if ( fieldIn % "gas" ) return asString(str->gas);
			if ( fieldIn % "gasCost" ) return asString(str->gasCost);
			break;
		case 'm':
			if ( fieldIn % "memory" )
			{
				SFInt32 cnt = str->memory.getCount();
				if (!cnt) return EMPTY;
				SFString ret = "[\n";
				for (int i=0;i<cnt;i++)
				{
					ret += ("\"" + str->memory[i] + "\"");
					ret += ((i<cnt-1) ? ",\n" : "\n");
				}
				return ret+"    ]";
			}
			break;
		case 'o':
			if ( fieldIn % "op" ) return str->op;
			break;
		case 'p':
			if ( fieldIn % "pc" ) return asString(str->pc);
			break;
		case 's':
			if ( fieldIn % "stack" )
			{
				SFInt32 cnt = str->stack.getCount();
				if (!cnt) return EMPTY;
				SFString ret = "[\n";
				for (int i=0;i<cnt;i++)
				{
					ret += ("\"" + str->stack[i] + "\"");
					ret += ((i<cnt-1) ? ",\n" : "\n");
				}
				return ret+"    ]";
			}
			if ( fieldIn % "storage" )
			{
/*
				SFInt32 cnt = str->storage.getCount();
				if (!cnt) return EMPTY;
				SFString ret = "[\n";
				for (int i=0;i<cnt;i++)
				{
					ret += str->storage[i];
					ret += ((i<cnt-1) ? ",\n" : "\n");
				}
				return ret+"    ]";
*/
				return "";
			}
			break;
	}

	// Finally, give the parent class a chance
	ret = nextBasenodeChunk(fieldIn, force, str);
	if (!ret.IsEmpty())
		return ret;

	return "<span class=warning>Field not found: [{" + fieldIn + "}]</span>\n";
}

//---------------------------------------------------------------------------------------------------
SFBool CStructLog::setValueByName(const SFString& fieldName, const SFString& fieldValue)
{
	// EXISTING_CODE
#define LIGHT
#ifdef LIGHT
	if (fieldName % "memory" || fieldName % "stack" || fieldName % "storage")
		return TRUE;
#else
	if (fieldName % "memory")
	{
//		outScreen << "MEMORY: " << fieldValue << "\n";
		if (fieldValue=="null")
			return TRUE;
		SFString val = fieldValue.Left(fieldValue.GetLength()-1);
		while (!val.IsEmpty())
			memory[memory.getCount()] = nextTokenClear(val,',');

	} else if (fieldName % "stack")
	{
//		outScreen << "STACK: " << fieldValue << "\n";
		if (fieldValue=="]")
			return TRUE;
		SFString val = fieldValue.Left(fieldValue.GetLength()-1);
		while (!val.IsEmpty())
			stack[stack.getCount()] = nextTokenClear(val,',');

	} else if (fieldName % "storage")
	{
//		outScreen << "STORAGE: " << fieldValue << "\n";
		if (fieldValue=="{}")
			return TRUE;
		SFString val = fieldValue.Substitute("{","").Substitute("}","");
		while (!val.IsEmpty())
			storage[storage.getCount()] = nextTokenClear(val,',');
	}
#endif
	// EXISTING_CODE

	switch (tolower(fieldName[0]))
	{
		case 'd':
			if ( fieldName % "depth" ) { depth = toLong(fieldValue); return TRUE; }
			break;
		case 'e':
			if ( fieldName % "error" ) { error = toBool(fieldValue); return TRUE; }
			break;
		case 'g':
			if ( fieldName % "gas" ) { gas = toLong(fieldValue); return TRUE; }
			if ( fieldName % "gasCost" ) { gasCost = toLong(fieldValue); return TRUE; }
			break;
		case 'm':
			if ( fieldName % "memory" ) return TRUE;
			break;
		case 'o':
			if ( fieldName % "op" ) { op = fieldValue; return TRUE; }
			break;
		case 'p':
			if ( fieldName % "pc" ) { pc = toLong(fieldValue); return TRUE; }
			break;
		case 's':
			if ( fieldName % "stack" ) return TRUE;
			if ( fieldName % "storage" ) return TRUE;
			break;
		default:
			break;
	}
	return FALSE;
}

//---------------------------------------------------------------------------------------------------
void CStructLog::finishParse()
{
	// EXISTING_CODE
	// EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
void CStructLog::Serialize(SFArchive& archive)
{
	if (!SerializeHeader(archive))
		return;

	if (archive.isReading())
	{
		archive >> depth;
		archive >> error;
		archive >> gas;
		archive >> gasCost;
		archive >> memory;
		archive >> op;
		archive >> pc;
		archive >> stack;
		archive >> storage;
		finishParse();
	} else
	{
		archive << depth;
		archive << error;
		archive << gas;
		archive << gasCost;
		archive << memory;
		archive << op;
		archive << pc;
		archive << stack;
		archive << storage;

	}
}

//---------------------------------------------------------------------------
void CStructLog::registerClass(void)
{
	static bool been_here=false;
	if (been_here) return;
	been_here=true;

	SFInt32 fieldNum=1000;
	ADD_FIELD(CStructLog, "schema",  T_NUMBER|TS_LABEL, ++fieldNum);
	ADD_FIELD(CStructLog, "deleted", T_BOOL|TS_LABEL,  ++fieldNum);
	ADD_FIELD(CStructLog, "depth", T_NUMBER, ++fieldNum);
	ADD_FIELD(CStructLog, "error", T_BOOL, ++fieldNum);
	ADD_FIELD(CStructLog, "gas", T_NUMBER, ++fieldNum);
	ADD_FIELD(CStructLog, "gasCost", T_NUMBER, ++fieldNum);
	ADD_FIELD(CStructLog, "memory", T_TEXT|TS_ARRAY, ++fieldNum);
	ADD_FIELD(CStructLog, "op", T_TEXT, ++fieldNum);
	ADD_FIELD(CStructLog, "pc", T_NUMBER, ++fieldNum);
	ADD_FIELD(CStructLog, "stack", T_TEXT|TS_ARRAY, ++fieldNum);
	ADD_FIELD(CStructLog, "storage", T_TEXT|TS_ARRAY, ++fieldNum);

	// Hide our internal fields, user can turn them on if they like
	HIDE_FIELD(CStructLog, "schema");
	HIDE_FIELD(CStructLog, "deleted");

	// EXISTING_CODE
	// EXISTING_CODE
}

//---------------------------------------------------------------------------
int sortStructlog(const SFString& f1, const SFString& f2, const void *rr1, const void *rr2)
{
	CStructLog *g1 = (CStructLog*)rr1;
	CStructLog *g2 = (CStructLog*)rr2;

	SFString v1 = g1->getValueByName(f1);
	SFString v2 = g2->getValueByName(f1);
	SFInt32 s = v1.Compare(v2);
	if (s || f2.IsEmpty())
		return (int)s;

	v1 = g1->getValueByName(f2);
	v2 = g2->getValueByName(f2);
	return (int)v1.Compare(v2);
}
int sortStructlogByName(const void *rr1, const void *rr2) { return sortStructlog("st_Name", "", rr1, rr2); }
int sortStructlogByID  (const void *rr1, const void *rr2) { return sortStructlog("structlogID", "", rr1, rr2); }

//---------------------------------------------------------------------------
SFString nextStructlogChunk_custom(const SFString& fieldIn, SFBool& force, const void *data)
{
	return EMPTY;
}

//---------------------------------------------------------------------------
SFBool CStructLog::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const
{
	// EXISTING_CODE
	// EXISTING_CODE
	return FALSE;
}

//---------------------------------------------------------------------------
SFBool CStructLog::readBackLevel(SFArchive& archive)
{
	SFBool done=FALSE;
	// EXISTING_CODE
	// EXISTING_CODE
	return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
