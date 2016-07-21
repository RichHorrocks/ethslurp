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
#include "slurp.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CSlurp, CBaseNode, NO_SCHEMA);

//---------------------------------------------------------------------------
void CSlurp::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const
{
	if (!isShowing())
		return;

	SFString fmt = (fmtIn.IsEmpty() ? defaultFormat() : fmtIn);
	if (handleCustomFormat(ctx, fmt, data))
		return;

	CSlurpNotify dn(this);
	while (!fmt.IsEmpty())
		ctx << getNextChunk(fmt, nextSlurpChunk, &dn);
}

//---------------------------------------------------------------------------
SFString nextSlurpChunk(const SFString& fieldIn, SFBool& force, const void *data)
{
	CSlurpNotify *sl = (CSlurpNotify*)data;
	const CSlurp *slu = sl->getDataPtr();

	// Now give customized code a chance to override
	SFString ret = nextSlurpChunk_custom(fieldIn, force, data);
	if (!ret.IsEmpty())
		return ret;
	
	switch (tolower(fieldIn[0]))
	{
		case 'a':
			if ( fieldIn % "addr" ) return slu->addr;
			break;
		case 'd':
			if ( fieldIn % "displayString" ) return slu->displayString;
			break;
		case 'h':
			if ( fieldIn % "handle" ) return asString(slu->handle);
			if ( fieldIn % "header" ) return slu->header;
			break;
		case 'l':
			if ( fieldIn % "lastPage" ) return asString(slu->lastPage);
			if ( fieldIn % "lastBlock" ) return asString(slu->lastBlock);
			break;
		case 'n':
			if ( fieldIn % "nVisible" ) return asString(slu->nVisible);
			break;
		case 'p':
			if ( fieldIn % "pageSize" ) return asString(slu->pageSize);
			break;
		case 't':
			if ( fieldIn % "transactions" )
			{
				SFString ret = "\n";
				for (int i=0;i<slu->transactions.getCount();i++)
					ret += slu->transactions[i].Format();
				return ret;
			}
			break;
	}
	
	// Finally, give the parent class a chance
	ret = nextBasenodeChunk(fieldIn, force, slu);
	if (!ret.IsEmpty())
		return ret;
	
	return "<span class=warning>Field not found: [{" + fieldIn + "}]</span>\n";
}

//---------------------------------------------------------------------------------------------------
SFBool CSlurp::setValueByName(const SFString& fieldName, const SFString& fieldValue)
{
	// EXISTING_CODE
	// EXISTING_CODE

	switch (tolower(fieldName[0]))
	{
		case 'a':
			if ( fieldName % "addr" ) { addr = fieldValue; return TRUE; }
			break;
		case 'd':
			if ( fieldName % "displayString" ) { displayString = fieldValue; return TRUE; }
			break;
		case 'h':
			if ( fieldName % "handle" ) { handle = toLong(fieldValue); return TRUE; }
			if ( fieldName % "header" ) { header = fieldValue; return TRUE; }
			break;
		case 'l':
			if ( fieldName % "lastPage" ) { lastPage = toLong(fieldValue); return TRUE; }
			if ( fieldName % "lastBlock" ) { lastBlock = toLong(fieldValue); return TRUE; }
			break;
		case 'n':
			if ( fieldName % "nVisible" ) { nVisible = toLong(fieldValue); return TRUE; }
			break;
		case 'p':
			if ( fieldName % "pageSize" ) { pageSize = toBool(fieldValue); return TRUE; }
			break;
		case 't':
			if ( fieldName % "transactions" ) return TRUE;
			break;
		default:
			break;
	}
	return FALSE;
}

//---------------------------------------------------------------------------------------------------
void CSlurp::finishParse()
{
	// EXISTING_CODE
	// EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
void CSlurp::Serialize(SFArchive& archive)
{
	if (!SerializeHeader(archive))
		return;

	if (archive.isReading())
	{
		archive >> handle;
		archive >> addr;
		archive >> header;
		archive >> displayString;
		archive >> pageSize;
		archive >> lastPage;
		archive >> lastBlock;
		archive >> nVisible;
		archive >> transactions;
		finishParse();
	} else
	{
		archive << handle;
		archive << addr;
		archive << header;
		archive << displayString;
		archive << pageSize;
		archive << lastPage;
		archive << lastBlock;
		archive << nVisible;
		archive << transactions;

	}
}

//---------------------------------------------------------------------------
void CSlurp::registerClass(void)
{
	static bool been_here=false;
	if (been_here) return;
	been_here=true;

	SFInt32 fieldNum=1000;
	ADD_FIELD(CSlurp, "schema",  T_NUMBER|TS_LABEL, ++fieldNum);
	ADD_FIELD(CSlurp, "deleted", T_RADIO|TS_LABEL,  ++fieldNum);
	ADD_FIELD(CSlurp, "handle", T_NUMBER|TS_LABEL,  ++fieldNum);
	ADD_FIELD(CSlurp, "addr", T_TEXT, ++fieldNum);
	ADD_FIELD(CSlurp, "header", T_TEXT, ++fieldNum);
	ADD_FIELD(CSlurp, "displayString", T_TEXT, ++fieldNum);
	ADD_FIELD(CSlurp, "pageSize", T_RADIO, ++fieldNum);
	ADD_FIELD(CSlurp, "lastPage", T_NUMBER, ++fieldNum);
	ADD_FIELD(CSlurp, "lastBlock", T_NUMBER, ++fieldNum);
	ADD_FIELD(CSlurp, "nVisible", T_NUMBER, ++fieldNum);
	ADD_FIELD(CSlurp, "transactions", T_TEXT|TS_ARRAY, ++fieldNum);

	// Hide our internal fields, user can turn them on if they like
	HIDE_FIELD(CSlurp, "schema");
	HIDE_FIELD(CSlurp, "deleted");
	HIDE_FIELD(CSlurp, "handle");

	// EXISTING_CODE
	// EXISTING_CODE
}

//---------------------------------------------------------------------------
int sortSlurp(const SFString& f1, const SFString& f2, const void *rr1, const void *rr2)
{
	CSlurp *g1 = (CSlurp*)rr1;
	CSlurp *g2 = (CSlurp*)rr2;

	SFString v1 = g1->getValueByName(f1);
	SFString v2 = g2->getValueByName(f1);
	SFInt32 s = v1.Compare(v2);
	if (s || f2.IsEmpty())
		return (int)s;

	v1 = g1->getValueByName(f2);
	v2 = g2->getValueByName(f2);
	return (int)v1.Compare(v2);
}
int sortSlurpByName(const void *rr1, const void *rr2) { return sortSlurp("sl_Name", "", rr1, rr2); }
int sortSlurpByID  (const void *rr1, const void *rr2) { return sortSlurp("slurpID", "", rr1, rr2); }

//---------------------------------------------------------------------------
SFString nextSlurpChunk_custom(const SFString& fieldIn, SFBool& force, const void *data)
{
	CSlurpNotify *sl = (CSlurpNotify*)data;
	const CSlurp *slu = sl->getDataPtr();
	switch (tolower(fieldIn[0]))
	{
		// EXISTING_CODE
		case 'n':
			if ( fieldIn % "now" ) return (isTesting ? "TESTING_TIME" : Now().Format(FMT_DEFAULT));
			break;
		case 'r':
			if ( fieldIn % "records" ) return (slu->transactions.getCount() == 0 ? "No records" : "");
			break;
		// EXISTING_CODE
		default:
			break;
	}
	
#pragma unused(sl)
#pragma unused(slu)

	return EMPTY;
}

//---------------------------------------------------------------------------
SFBool CSlurp::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const
{
	// EXISTING_CODE
	// Split the format string into three parts: pre, post and records.
	// If no records, just process as normal. We do this because it's so slow
	// copying the records into a string, so we write it directly to the
	// export context. If there is no {RECORDS}, then just send handle it like normal
	if (!fmtIn.Contains("{RECORDS}") || transactions.getCount()==0)
	{
		SFString fmt = fmtIn;

		CSlurpNotify dn(this);
		while (!fmt.IsEmpty())
			ctx << getNextChunk(fmt, nextSlurpChunk, &dn);
	} else
	{
		SFString postFmt = fmtIn;
		postFmt.Replace("{RECORDS}","|");
		SFString preFmt = nextTokenClear(postFmt,'|');

		// We assume here that the token was properly formed. For the pre-text we
		// have to clear out the start '[', and for the post text we clear out the ']'
		preFmt.ReplaceReverse("[","");
		postFmt.Replace("]","");

		// We handle the display in three parts: pre, records, and post so as
		// to avoid building the entire record list into an ever-growing and
		// ever-slowing string
		CSlurpNotify dn(this);
		while (!preFmt.IsEmpty())
			ctx << getNextChunk(preFmt, nextSlurpChunk, &dn);
		SFInt32 cnt=0;
		for (int i=0;i<transactions.getCount();i++)
		{
			cnt += transactions[i].isShowing();
			if (cnt && !(cnt%REP_INFREQ))
			{
				outErr << "\tExporting record " << cnt << " of " << nVisible;
				outErr << (transactions.getCount()!=nVisible?" visible":"") << " records" << (isTesting?"\n":"\r"); outErr.Flush();
			}
			SFInt32 save = transactions[i].handle;
			((CSlurp*)this)->transactions[i].handle = transactions.getCount() - i;
			ctx << transactions[i].Format(displayString);
			((CSlurp*)this)->transactions[i].handle = save;
			if (cnt>=nVisible)
				break; // no need to keep spinning if we've shown them all
		}
		ctx << "\n";
		while (!postFmt.IsEmpty())
			ctx << getNextChunk(postFmt, nextSlurpChunk, &dn);
	}
	return TRUE;
	// EXISTING_CODE
	return FALSE;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
#define MAX_FUNCS 200
CFunction funcTable[MAX_FUNCS];
SFInt32 nFunctions=0;
//---------------------------------------------------------------------------
int sortFuncTable(const void *ob1, const void *ob2)
{
	CFunction *p1 = (CFunction*)ob1;
	CFunction *p2 = (CFunction*)ob2;
	return (int)p2->encoding.Compare(p1->encoding);
}

//---------------------------------------------------------------------------
void CSlurp::loadABI(void)
{
	// Already loaded?
	if (nFunctions)
		return;

extern SFString configPath(const SFString& part);
	SFString abiFilename = 	configPath("abis/"+addr+".json");
	if (!SFos::fileExists(abiFilename))
		return;

	outErr << "\tLoading abi file: " << abiFilename << "...\n";
	SFString contents = asciiFileToString(abiFilename);
	ASSERT(!contents.IsEmpty());

	char *p = cleanUpJson((char *)(const char*)contents);
	while (p && *p)
	{
		CFunction func;SFInt32 nFields=0;
		p = func.parseJson(p,nFields);
		if (nFields)
		{
			SFString ethabi = "/usr/local/bin/ethabi";
			if (!SFos::fileExists(ethabi))
			{
				outErr << "/usr/local/bin/ethabi command not found. Cannot parse functions.\n";
			} else if (func.type == "function")
			{
				SFString cmd = ethabi + " encode function \"" + abiFilename + "\" " + func.name;
				SFString result = SFos::doCommand(cmd);
				func.encoding = result;
				funcTable[nFunctions++] = func;
			}
		}
	}
	qsort(funcTable, nFunctions, sizeof(CFunction), sortFuncTable);
	for (int i=0;i<nFunctions;i++)
		if (funcTable[i].type == "function" && verbose)
			outErr << funcTable[i].Format().Substitute("\n"," ") << "\n";
}
// EXISTING_CODE
