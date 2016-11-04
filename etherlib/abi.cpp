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
#include "abi.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CAbi, CBaseNode, curVersion);

//---------------------------------------------------------------------------
void CAbi::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const
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

	CAbiNotify dn(this);
	while (!fmt.IsEmpty())
		ctx << getNextChunk(fmt, nextAbiChunk, &dn);
}

//---------------------------------------------------------------------------
SFString nextAbiChunk(const SFString& fieldIn, SFBool& force, const void *data)
{
	CAbiNotify *ab = (CAbiNotify*)data;
	const CAbi *abi = ab->getDataPtr();

	// Now give customized code a chance to override
	SFString ret = nextAbiChunk_custom(fieldIn, force, data);
	if (!ret.IsEmpty())
		return ret;

	switch (tolower(fieldIn[0]))
	{
		case 'a':
			if ( fieldIn % "abiByName" )
			{
				SFInt32 cnt = abi->abiByName.getCount();
				SFString ret = "[\n";
				for (int i=0;i<cnt;i++)
				{
					ret += abi->abiByName[i].Format();
					ret += ((i<cnt-1) ? ",\n" : "\n");
				}
				return ret+"    ]";
			}
			if ( fieldIn % "abiByEncoding" )
			{
				SFInt32 cnt = abi->abiByEncoding.getCount();
				SFString ret = "[\n";
				for (int i=0;i<cnt;i++)
				{
					ret += abi->abiByEncoding[i].Format();
					ret += ((i<cnt-1) ? ",\n" : "\n");
				}
				return ret+"    ]";
			}
			break;
	}

	// Finally, give the parent class a chance
	ret = nextBasenodeChunk(fieldIn, force, abi);
	if (!ret.IsEmpty())
		return ret;

	return "<span class=warning>Field not found: [{" + fieldIn + "}]</span>\n";
}

//---------------------------------------------------------------------------------------------------
SFBool CAbi::setValueByName(const SFString& fieldName, const SFString& fieldValue)
{
	// EXISTING_CODE
	// EXISTING_CODE

	switch (tolower(fieldName[0]))
	{
		case 'a':
			if ( fieldName % "abiByName" ) return TRUE;
			if ( fieldName % "abiByEncoding" ) return TRUE;
			break;
		default:
			break;
	}
	return FALSE;
}

//---------------------------------------------------------------------------------------------------
void CAbi::finishParse()
{
	// EXISTING_CODE
	// EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
void CAbi::Serialize(SFArchive& archive)
{
	if (!SerializeHeader(archive))
		return;

	if (archive.isReading())
	{
		archive >> abiByName;
		archive >> abiByEncoding;
		finishParse();
	} else
	{
		archive << abiByName;
		archive << abiByEncoding;

	}
}

//---------------------------------------------------------------------------
void CAbi::registerClass(void)
{
	static bool been_here=false;
	if (been_here) return;
	been_here=true;

	SFInt32 fieldNum=1000;
	ADD_FIELD(CAbi, "schema",  T_NUMBER|TS_LABEL, ++fieldNum);
	ADD_FIELD(CAbi, "deleted", T_RADIO|TS_LABEL,  ++fieldNum);
	ADD_FIELD(CAbi, "abiByName", T_TEXT|TS_ARRAY, ++fieldNum);
	ADD_FIELD(CAbi, "abiByEncoding", T_TEXT|TS_ARRAY, ++fieldNum);

	// Hide our internal fields, user can turn them on if they like
	HIDE_FIELD(CAbi, "schema");
	HIDE_FIELD(CAbi, "deleted");

	// EXISTING_CODE
	// EXISTING_CODE
}

//---------------------------------------------------------------------------
int sortAbi(const SFString& f1, const SFString& f2, const void *rr1, const void *rr2)
{
	CAbi *g1 = (CAbi*)rr1;
	CAbi *g2 = (CAbi*)rr2;

	SFString v1 = g1->getValueByName(f1);
	SFString v2 = g2->getValueByName(f1);
	SFInt32 s = v1.Compare(v2);
	if (s || f2.IsEmpty())
		return (int)s;

	v1 = g1->getValueByName(f2);
	v2 = g2->getValueByName(f2);
	return (int)v1.Compare(v2);
}
int sortAbiByName(const void *rr1, const void *rr2) { return sortAbi("ab_Name", "", rr1, rr2); }
int sortAbiByID  (const void *rr1, const void *rr2) { return sortAbi("abiID", "", rr1, rr2); }

//---------------------------------------------------------------------------
SFString nextAbiChunk_custom(const SFString& fieldIn, SFBool& force, const void *data)
{
	return EMPTY;
}

//---------------------------------------------------------------------------
SFBool CAbi::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const
{
	// EXISTING_CODE
	// EXISTING_CODE
	return FALSE;
}

//---------------------------------------------------------------------------
SFBool CAbi::readBackLevel(SFArchive& archive)
{
	SFBool done=FALSE;
	// EXISTING_CODE
	// EXISTING_CODE
	return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------
int sortFuncTableByName(const void *ob1, const void *ob2)
{
	CFunction *p1 = (CFunction*)ob1;
	CFunction *p2 = (CFunction*)ob2;
	return (int)p2->name.Compare(p1->name);
}

//---------------------------------------------------------------------------
int sortFuncTableByEncoding(const void *ob1, const void *ob2)
{
	CFunction *p1 = (CFunction*)ob1;
	CFunction *p2 = (CFunction*)ob2;
	return (int)p2->encoding.Compare(p1->encoding);
}

//---------------------------------------------------------------------------
#if 1 //NEW_CODE
int findByName(const void *rr1, const void *rr2)
{
	CFunction *f1 = (CFunction *)rr1;
	CFunction *f2 = (CFunction *)rr2;
	return (int)f2->name.Compare(f1->name);
}
#endif

//---------------------------------------------------------------------------
int findByEncoding(const void *rr1, const void *rr2)
{
	CFunction *f1 = (CFunction *)rr1;
	CFunction *f2 = (CFunction *)rr2;
	return (int)f2->encoding.Compare(f1->encoding);
}

//---------------------------------------------------------------------------
#if 1 //NEW_CODE
CFunction *CAbi::findFunctionByName(const SFString& name)
{
	CFunction search;
	search.name = name;
	return abiByName.Find(&search,findByName);
}
#endif

//---------------------------------------------------------------------------
CFunction *CAbi::findFunctionByEncoding(const SFString& enc)
{
	CFunction search;
	search.encoding = enc;
	return abiByEncoding.Find(&search,findByEncoding);
}

//---------------------------------------------------------------------------
void CAbi::clearABI(void)
{
	abiByName.Clear();
	abiByEncoding.Clear();
}

//#define VERY_NEW_CODE
#ifdef VERY_NEW_CODE
extern "C" {
	const char*get_encoded_abi(const char*path);
	void free_encoded_abi(const char* abi);
}
#endif

//---------------------------------------------------------------------------
void CAbi::loadABI(const SFString& addr)
{
	// Already loaded?
#if 1 //NEW_CODE
	if (abiByName.getCount())
#else
	if (abiByEncoding.getCount())
#endif
		return;

extern SFString configPath(const SFString& part);
	SFString abiFilename = 	configPath("abis/"+addr+".json");
	if (!SFos::fileExists(abiFilename))
		return;

#ifdef VERY_NEW_CODE
//outErr << "----------------------------------------------------\n";
const char* encodedABI = get_encoded_abi((const char*)abiFilename);
outErr << "\tThe rust string: " << encodedABI << "\n";
printf("\t0x%llx\n", (long long)encodedABI);outErr.Flush();
free_encoded_abi(encodedABI);
//outErr << "----------------------------------------------------\n";
#endif

#if 0
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
				SFString cmd = ethabi + " encode function \"" + abiFilename + "\" " + func.name.Substitute("(","\\(").Substitute(")","\\)"); // when we call ethabi, we want the full function declaration (if it's present)
				SFString result = SFos::doCommand(cmd);
				func.encoding = result;
				func.name = nextTokenClear(func.name,'('); // when we search, we only want the name
#if 1 //NEW_CODE
				abiByName[abiByName.getCount()] = func;
#endif
				abiByEncoding[abiByEncoding.getCount()] = func;
			}
		}
	}

#if 1 //NEW_CODE
	abiByName.Sort(sortFuncTableByName);
#endif
	abiByEncoding.Sort(sortFuncTableByEncoding);
#if 1 //NEW_CODE
	for (int i=0;i<abiByName.getCount();i++)
		if (abiByName[i].type == "function" && verbose)
			outErr << abiByName[i].Format("[\"{NAME}|][{ENCODING}\"]").Substitute("\n"," ") << "\n";
#else
	for (int i=0;i<abiByEncoding.getCount();i++)
		if (abiByEncoding[i].type == "function" && verbose)
			outErr << abiByEncoding[i].Format("[\"{NAME}|][{ENCODING}\"]").Substitute("\n"," ") << "\n";
#endif
#endif
}
// EXISTING_CODE
