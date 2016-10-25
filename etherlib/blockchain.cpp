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
#include "blockchain.h"
#include "account.h"
//---------------------------------------------------------------------------
IMPLEMENT_NODE(CBlockChain, CBaseNode, curVersion);

//---------------------------------------------------------------------------
void CBlockChain::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const
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

	CBlockChainNotify dn(this);
	while (!fmt.IsEmpty())
		ctx << getNextChunk(fmt, nextBlockchainChunk, &dn);
}

//---------------------------------------------------------------------------
SFString nextBlockchainChunk(const SFString& fieldIn, SFBool& force, const void *data)
{
	CBlockChainNotify *bl = (CBlockChainNotify*)data;
	const CBlockChain *blo = bl->getDataPtr();

	// Now give customized code a chance to override
	SFString ret = nextBlockchainChunk_custom(fieldIn, force, data);
	if (!ret.IsEmpty())
		return ret;

	switch (tolower(fieldIn[0]))
	{
		case 'b':
			if ( fieldIn % "blocks" )
			{
				SFInt32 cnt = blo->blocks.getCount();
				SFString ret = "[\n";
				for (int i=0;i<cnt;i++)
				{
					ret += blo->blocks[i].Format();
					ret += ((i<cnt-1) ? ",\n" : "\n");
				}
				return ret+"    ]";
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
SFBool CBlockChain::setValueByName(const SFString& fieldName, const SFString& fieldValue)
{
	// EXISTING_CODE
	// EXISTING_CODE

	switch (tolower(fieldName[0]))
	{
		case 'b':
			if ( fieldName % "blocks" ) return TRUE;
			break;
		default:
			break;
	}
	return FALSE;
}

//---------------------------------------------------------------------------------------------------
void CBlockChain::finishParse()
{
	// EXISTING_CODE
	// EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
void CBlockChain::Serialize(SFArchive& archive)
{
	if (!SerializeHeader(archive))
		return;

	if (archive.isReading())
	{
		archive >> blocks;
		finishParse();
	} else
	{
		archive << blocks;

	}
}

//---------------------------------------------------------------------------
void CBlockChain::registerClass(void)
{
	static bool been_here=false;
	if (been_here) return;
	been_here=true;

	SFInt32 fieldNum=1000;
	ADD_FIELD(CBlockChain, "schema",  T_NUMBER|TS_LABEL, ++fieldNum);
	ADD_FIELD(CBlockChain, "deleted", T_RADIO|TS_LABEL,  ++fieldNum);
	ADD_FIELD(CBlockChain, "blocks", T_TEXT|TS_ARRAY, ++fieldNum);

	// Hide our internal fields, user can turn them on if they like
	HIDE_FIELD(CBlockChain, "schema");
	HIDE_FIELD(CBlockChain, "deleted");

	// EXISTING_CODE
	// EXISTING_CODE
}

//---------------------------------------------------------------------------
int sortBlockchain(const SFString& f1, const SFString& f2, const void *rr1, const void *rr2)
{
	CBlockChain *g1 = (CBlockChain*)rr1;
	CBlockChain *g2 = (CBlockChain*)rr2;

	SFString v1 = g1->getValueByName(f1);
	SFString v2 = g2->getValueByName(f1);
	SFInt32 s = v1.Compare(v2);
	if (s || f2.IsEmpty())
		return (int)s;

	v1 = g1->getValueByName(f2);
	v2 = g2->getValueByName(f2);
	return (int)v1.Compare(v2);
}
int sortBlockchainByName(const void *rr1, const void *rr2) { return sortBlockchain("bl_Name", "", rr1, rr2); }
int sortBlockchainByID  (const void *rr1, const void *rr2) { return sortBlockchain("blockchainID", "", rr1, rr2); }

//---------------------------------------------------------------------------
SFString nextBlockchainChunk_custom(const SFString& fieldIn, SFBool& force, const void *data)
{
	CBlockChainNotify *bl = (CBlockChainNotify*)data;
	const CBlockChain *blo = bl->getDataPtr();
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
SFBool CBlockChain::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const
{
	// EXISTING_CODE
	// EXISTING_CODE
	return FALSE;
}

//---------------------------------------------------------------------------
SFBool CBlockChain::readBackLevel(SFArchive& archive)
{
	SFBool done=FALSE;
	// EXISTING_CODE
	// EXISTING_CODE
	return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
