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
#include "block.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CBlock, CBaseNode, NO_SCHEMA);

//---------------------------------------------------------------------------
void CBlock::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const
{
	if (!isShowing())
		return;

	SFString fmt = (fmtIn.IsEmpty() ? defaultFormat() : fmtIn);
	if (handleCustomFormat(ctx, fmt, data))
		return;

	CBlockNotify dn(this);
	while (!fmt.IsEmpty())
		ctx << getNextChunk(fmt, nextBlockChunk, &dn);
}

//---------------------------------------------------------------------------
SFString nextBlockChunk(const SFString& fieldIn, SFBool& force, const void *data)
{
	CBlockNotify *bl = (CBlockNotify*)data;
	const CBlock *blo = bl->getDataPtr();

	// Now give customized code a chance to override
	SFString ret = nextBlockChunk_custom(fieldIn, force, data);
	if (!ret.IsEmpty())
		return ret;

	switch (tolower(fieldIn[0]))
	{
		case 'd':
			if ( fieldIn % "difficulty" ) return blo->difficulty;
			break;
		case 'e':
			if ( fieldIn % "extraData" ) return blo->extraData;
			break;
		case 'g':
			if ( fieldIn % "gasLimit" ) return blo->gasLimit;
			if ( fieldIn % "gasUsed" ) return blo->gasUsed;
			break;
		case 'h':
			if ( fieldIn % "handle" ) return asString(blo->handle);
			if ( fieldIn % "hash" ) return blo->hash;
			break;
		case 'l':
			if ( fieldIn % "logsBloom" ) return blo->logsBloom;
			break;
		case 'm':
			if ( fieldIn % "miner" ) return blo->miner;
			break;
		case 'n':
			if ( fieldIn % "nonce" ) return blo->nonce;
			if ( fieldIn % "number" ) return blo->number;
			break;
		case 'p':
			if ( fieldIn % "parentHash" ) return blo->parentHash;
			break;
		case 'r':
			if ( fieldIn % "receiptRoot" ) return blo->receiptRoot;
			break;
		case 's':
			if ( fieldIn % "sha3Uncles" ) return blo->sha3Uncles;
			if ( fieldIn % "size" ) return blo->size;
			if ( fieldIn % "stateRoot" ) return blo->stateRoot;
			break;
		case 't':
			if ( fieldIn % "timestamp" ) return blo->timestamp;
			if ( fieldIn % "totalDifficulty" ) return blo->totalDifficulty;
			if ( fieldIn % "transactions" )
			{
				SFString ret = "\n";
				for (int i=0;i<blo->transactions.getCount();i++)
					ret += blo->transactions[i].Format().Substitute("\n","\n\t");
				return ret;
			}
			if ( fieldIn % "transactionsRoot" ) return blo->transactionsRoot;
			break;
		case 'u':
			if ( fieldIn % "uncles" )
			{
				return EMPTY;
//				SFStringArray not returned
			}
			break;
	}

	// Finally, give the parent class a chance
	ret = nextBasenodeChunk(fieldIn, force, blo);
	if (!ret.IsEmpty())
		return ret;

	return "<span class=warning>Field not found: [{" + fieldIn + "}]</span>\n";
}

//---------------------------------------------------------------------------------------------------
SFBool CBlock::setValueByName(const SFString& fieldName, const SFString& fieldValue)
{
	// EXISTING_CODE
	if (fieldName % "transactions")
	{
		char *p = (char *)(const char*)fieldValue;
		while (p && *p)
		{
			CTransaction trans;SFInt32 nFields=0;
			trans.timeStamp = toLong(timestamp);
			p = trans.parseJson(p,nFields);
			if (nFields)
				transactions[transactions.getCount()] = trans;
		}
		return TRUE;
#ifdef COMPRESS
	} else if (fieldName % "logsBloom")
	{
const char* STR_ZEROS=
"0x"
"0000000000000000000000000000000000000000"
"0000000000000000000000000000000000000000"
"0000000000000000000000000000000000000000"
"0000000000000000000000000000000000000000"
"0000000000000000000000000000000000000000"
"0000000000000000000000000000000000000000"
"0000000000000000000000000000000000000000"
"0000000000000000000000000000000000000000"
"0000000000000000000000000000000000000000"
"0000000000000000000000000000000000000000"
"0000000000000000000000000000000000000000"
"0000000000000000000000000000000000000000"
"0000000000000000000000000000000";
		logsBloom = ((fieldValue == STR_ZEROS) ? "0x" : fieldValue);
		return TRUE;
#endif
	}
	// EXISTING_CODE

	switch (tolower(fieldName[0]))
	{
		case 'd':
			if ( fieldName % "difficulty" ) { difficulty = fieldValue; return TRUE; }
			break;
		case 'e':
			if ( fieldName % "extraData" ) { extraData = fieldValue; return TRUE; }
			break;
		case 'g':
			if ( fieldName % "gasLimit" ) { gasLimit = fieldValue; return TRUE; }
			if ( fieldName % "gasUsed" ) { gasUsed = fieldValue; return TRUE; }
			break;
		case 'h':
			if ( fieldName % "handle" ) { handle = toLong(fieldValue); return TRUE; }
			if ( fieldName % "hash" ) { hash = fieldValue; return TRUE; }
			break;
		case 'l':
			if ( fieldName % "logsBloom" ) { logsBloom = fieldValue; return TRUE; }
			break;
		case 'm':
			if ( fieldName % "miner" ) { miner = fieldValue; return TRUE; }
			break;
		case 'n':
			if ( fieldName % "nonce" ) { nonce = fieldValue; return TRUE; }
			if ( fieldName % "number" ) { number = fieldValue; return TRUE; }
			break;
		case 'p':
			if ( fieldName % "parentHash" ) { parentHash = fieldValue; return TRUE; }
			break;
		case 'r':
			if ( fieldName % "receiptRoot" ) { receiptRoot = fieldValue; return TRUE; }
			break;
		case 's':
			if ( fieldName % "sha3Uncles" ) { sha3Uncles = fieldValue; return TRUE; }
			if ( fieldName % "size" ) { size = fieldValue; return TRUE; }
			if ( fieldName % "stateRoot" ) { stateRoot = fieldValue; return TRUE; }
			break;
		case 't':
			if ( fieldName % "timestamp" ) { timestamp = fieldValue; return TRUE; }
			if ( fieldName % "totalDifficulty" ) { totalDifficulty = fieldValue; return TRUE; }
			if ( fieldName % "transactions" ) return TRUE;
			if ( fieldName % "transactionsRoot" ) { transactionsRoot = fieldValue; return TRUE; }
			break;
		case 'u':
			if ( fieldName % "uncles" )
			break;
		default:
			break;
	}
	return FALSE;
}

//---------------------------------------------------------------------------------------------------
void CBlock::finishParse()
{
	// EXISTING_CODE
	// EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
void CBlock::Serialize(SFArchive& archive)
{
	if (!SerializeHeader(archive))
		return;

	if (archive.isReading())
	{
		SFString tmp;
		archive >> handle;
		ASSERT(handle==curVersion);
		archive >> difficulty;
		archive >> extraData;
		archive >> gasLimit;
		archive >> gasUsed;
		archive >> hash;
		archive >> logsBloom;
		{ archive >> tmp; miner = uncompressHash(tmp); }
		archive >> nonce;
		archive >> number;
		{ archive >> tmp; parentHash = uncompressHash(tmp); }
		{ archive >> tmp; receiptRoot = uncompressHash(tmp); }
		archive >> sha3Uncles;
		archive >> size;
		archive >> stateRoot;
		archive >> timestamp;
		archive >> totalDifficulty;
		archive >> transactions;
		{ archive >> tmp; transactionsRoot = uncompressHash(tmp); }
//		archive >> uncles;
		finishParse();
	} else
	{
		archive << curVersion;
		archive << difficulty;
		archive << extraData;
		archive << gasLimit;
		archive << gasUsed;
		archive << hash;
		archive << logsBloom;
		archive << compressHash(miner);
		archive << nonce;
		archive << number;
		archive << compressHash(parentHash);
		archive << compressHash(receiptRoot);
		archive << sha3Uncles;
		archive << size;
		archive << stateRoot;
		archive << timestamp;
		archive << totalDifficulty;
		archive << transactions;
		archive << compressHash(transactionsRoot);
//		archive << uncles;

	}
}

//---------------------------------------------------------------------------
void CBlock::registerClass(void)
{
	static bool been_here=false;
	if (been_here) return;
	been_here=true;

	SFInt32 fieldNum=1000;
	ADD_FIELD(CBlock, "schema",  T_NUMBER|TS_LABEL, ++fieldNum);
	ADD_FIELD(CBlock, "deleted", T_RADIO|TS_LABEL,  ++fieldNum);
	ADD_FIELD(CBlock, "handle", T_NUMBER|TS_LABEL,  ++fieldNum);
	ADD_FIELD(CBlock, "difficulty", T_TEXT, ++fieldNum);
	ADD_FIELD(CBlock, "extraData", T_TEXT, ++fieldNum);
	ADD_FIELD(CBlock, "gasLimit", T_TEXT, ++fieldNum);
	ADD_FIELD(CBlock, "gasUsed", T_TEXT, ++fieldNum);
	ADD_FIELD(CBlock, "hash", T_TEXT, ++fieldNum);
	ADD_FIELD(CBlock, "logsBloom", T_TEXT, ++fieldNum);
	ADD_FIELD(CBlock, "miner", T_TEXT, ++fieldNum);
	ADD_FIELD(CBlock, "nonce", T_TEXT, ++fieldNum);
	ADD_FIELD(CBlock, "number", T_TEXT, ++fieldNum);
	ADD_FIELD(CBlock, "parentHash", T_TEXT, ++fieldNum);
	ADD_FIELD(CBlock, "receiptRoot", T_TEXT, ++fieldNum);
	ADD_FIELD(CBlock, "sha3Uncles", T_TEXT, ++fieldNum);
	ADD_FIELD(CBlock, "size", T_TEXT, ++fieldNum);
	ADD_FIELD(CBlock, "stateRoot", T_TEXT, ++fieldNum);
	ADD_FIELD(CBlock, "timestamp", T_TEXT, ++fieldNum);
	ADD_FIELD(CBlock, "totalDifficulty", T_TEXT, ++fieldNum);
	ADD_FIELD(CBlock, "transactions", T_TEXT|TS_ARRAY, ++fieldNum);
	ADD_FIELD(CBlock, "transactionsRoot", T_TEXT, ++fieldNum);
	ADD_FIELD(CBlock, "uncles", T_TEXT|TS_ARRAY, ++fieldNum);

	// Hide our internal fields, user can turn them on if they like
	HIDE_FIELD(CBlock, "schema");
	HIDE_FIELD(CBlock, "deleted");
	HIDE_FIELD(CBlock, "handle");

	// EXISTING_CODE
	// EXISTING_CODE
}

//---------------------------------------------------------------------------
int sortBlock(const SFString& f1, const SFString& f2, const void *rr1, const void *rr2)
{
	CBlock *g1 = (CBlock*)rr1;
	CBlock *g2 = (CBlock*)rr2;

	SFString v1 = g1->getValueByName(f1);
	SFString v2 = g2->getValueByName(f1);
	SFInt32 s = v1.Compare(v2);
	if (s || f2.IsEmpty())
		return (int)s;

	v1 = g1->getValueByName(f2);
	v2 = g2->getValueByName(f2);
	return (int)v1.Compare(v2);
}
int sortBlockByName(const void *rr1, const void *rr2) { return sortBlock("bl_Name", "", rr1, rr2); }
int sortBlockByID  (const void *rr1, const void *rr2) { return sortBlock("blockID", "", rr1, rr2); }

//---------------------------------------------------------------------------
SFString nextBlockChunk_custom(const SFString& fieldIn, SFBool& force, const void *data)
{
	CBlockNotify *bl = (CBlockNotify*)data;
	const CBlock *blo = bl->getDataPtr();
	switch (tolower(fieldIn[0]))
	{
		// EXISTING_CODE
		// EXISTING_CODE
		default:
			break;
	}

#pragma unused(bl)
#pragma unused(blo)

	return EMPTY;
}

//---------------------------------------------------------------------------
SFBool CBlock::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const
{
	// EXISTING_CODE
	// EXISTING_CODE
	return FALSE;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
