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
#include "transaction.h"
#include "account.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CTransaction, CBaseNode, NO_SCHEMA);

//---------------------------------------------------------------------------
void CTransaction::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const
{
	if (!isShowing())
		return;

	SFString fmt = (fmtIn.IsEmpty() ? defaultFormat() : fmtIn);
	if (handleCustomFormat(ctx, fmt, data))
		return;

	CTransactionNotify dn(this);
	while (!fmt.IsEmpty())
		ctx << getNextChunk(fmt, nextTransactionChunk, &dn);
}

//---------------------------------------------------------------------------
SFString toNice(const SFString& in)
{
	SFString input = in;

	SFInt32 cnt=0;
	CStringExportContext ctx;
	ctx << input.Left(10) << "<br>\n";  input = input.Mid(10,1000000);
	while (!input.IsEmpty())
	{
		ctx << "[" << cnt++ << "]: " << input.Left(64) << "<br>\n";
		input=input.Mid(64,100000);
	}
	return ctx.str;
}

//---------------------------------------------------------------------------
SFString nextTransactionChunk(const SFString& fieldIn, SFBool& force, const void *data)
{
	CTransactionNotify *tr = (CTransactionNotify*)data;
	const CTransaction *tra = tr->getDataPtr();

	// Now give customized code a chance to override
	SFString ret = nextTransactionChunk_custom(fieldIn, force, data);
	if (!ret.IsEmpty())
		return ret;
	
	switch (tolower(fieldIn[0]))
	{
		case 'b':
			if ( fieldIn % "blockHash" ) return tra->blockHash;
			if ( fieldIn % "blockNumber" ) return asString(tra->blockNumber);
			break;
		case 'c':
			if ( fieldIn % "confirmations" ) return asString(tra->confirmations);
			if ( fieldIn % "contractAddress" ) return tra->contractAddress;
			if ( fieldIn % "cumulativeGasUsed" ) return tra->cumulativeGasUsed;
			break;
		case 'f':
			if ( fieldIn % "from" ) return tra->from;
			break;
		case 'g':
			if ( fieldIn % "gas" ) return tra->gas;
			if ( fieldIn % "gasPrice" ) return tra->gasPrice;
			if ( fieldIn % "gasUsed" ) return tra->gasUsed;
			break;
		case 'h':
			if ( fieldIn % "handle" ) return asString(tra->handle);
			if ( fieldIn % "hash" ) return tra->hash;
			break;
		case 'i':
			if ( fieldIn % "input_nice" ) return toNice(tra->input);
			if ( fieldIn % "input" ) return tra->input;
			if ( fieldIn % "isError" ) return asString(tra->isError);
			if ( fieldIn % "isInternalTx" ) return asString(tra->isInternalTx);
			break;
		case 'n':
			if ( fieldIn % "nonce" ) return tra->nonce;
			break;
		case 't':
			if ( fieldIn % "timeStamp" ) return asString(tra->timeStamp);
			if ( fieldIn % "to" ) return tra->to;
			if ( fieldIn % "transactionIndex" ) return asString(tra->transactionIndex);
			break;
		case 'v':
			if ( fieldIn % "value" ) return tra->value;
			break;
	}
	
	// Finally, give the parent class a chance
	ret = nextBasenodeChunk(fieldIn, force, tra);
	if (!ret.IsEmpty())
		return ret;
	
	return "<span class=warning>Field not found: [{" + fieldIn + "}]</span>\n";
}

//---------------------------------------------------------------------------------------------------
SFBool CTransaction::setValueByName(const SFString& fieldName, const SFString& fieldValue)
{
	// EXISTING_CODE
	if ( fieldName % "timeStamp" )
	{
		timeStamp = toLong(fieldValue);
		m_transDate = dateFromTimeStamp(timeStamp);
		return TRUE;
	}
	// EXISTING_CODE

	switch (tolower(fieldName[0]))
	{
		case 'b':
			if ( fieldName % "blockHash" ) { blockHash = fieldValue; return TRUE; }
			if ( fieldName % "blockNumber" ) { blockNumber = toLong(fieldValue); return TRUE; }
			break;
		case 'c':
			if ( fieldName % "confirmations" ) { confirmations = toLong(fieldValue); return TRUE; }
			if ( fieldName % "contractAddress" ) { contractAddress = fieldValue; return TRUE; }
			if ( fieldName % "cumulativeGasUsed" ) { cumulativeGasUsed = fieldValue; return TRUE; }
			break;
		case 'f':
			if ( fieldName % "from" ) { from = toLower(fieldValue); return TRUE; }
			break;
		case 'g':
			if ( fieldName % "gas" ) { gas = fieldValue; return TRUE; }
			if ( fieldName % "gasPrice" ) { gasPrice = fieldValue; return TRUE; }
			if ( fieldName % "gasUsed" ) { gasUsed = fieldValue; return TRUE; }
			break;
		case 'h':
			if ( fieldName % "handle" ) { handle = toLong(fieldValue); return TRUE; }
			if ( fieldName % "hash" ) { hash = fieldValue; return TRUE; }
			break;
		case 'i':
			if ( fieldName % "input" ) { input = fieldValue; return TRUE; }
			if ( fieldName % "isError" ) { isError = toBool(fieldValue); return TRUE; }
			if ( fieldName % "isInternalTx" ) { isInternalTx = toBool(fieldValue); return TRUE; }
			break;
		case 'n':
			if ( fieldName % "nonce" ) { nonce = fieldValue; return TRUE; }
			break;
		case 't':
			if ( fieldName % "timeStamp" ) { timeStamp = toLong(fieldValue); return TRUE; }
			if ( fieldName % "to" ) { to = toLower(fieldValue); return TRUE; }
			if ( fieldName % "transactionIndex" ) { transactionIndex = toLong(fieldValue); return TRUE; }
			break;
		case 'v':
			if ( fieldName % "value" ) { value = fieldValue; return TRUE; }
			break;
		default:
			break;
	}
	return FALSE;
}

//---------------------------------------------------------------------------------------------------
void CTransaction::finishParse()
{
	// EXISTING_CODE
	m_transDate = dateFromTimeStamp(timeStamp);
	// EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
void CTransaction::Serialize(SFArchive& archive)
{
	if (!SerializeHeader(archive))
		return;

	if (archive.isReading())
	{
		archive >> handle;
		archive >> blockHash;
		archive >> blockNumber;
		archive >> confirmations;
		archive >> contractAddress;
		archive >> cumulativeGasUsed;
		archive >> from;
		archive >> gas;
		archive >> gasPrice;
		archive >> gasUsed;
		archive >> hash;
		archive >> input;
		archive >> isError;
		archive >> isInternalTx;
		archive >> nonce;
		archive >> timeStamp;
		archive >> to;
		archive >> transactionIndex;
		archive >> value;
		finishParse();
	} else
	{
		archive << handle;
		archive << blockHash;
		archive << blockNumber;
		archive << confirmations;
		archive << contractAddress;
		archive << cumulativeGasUsed;
		archive << from;
		archive << gas;
		archive << gasPrice;
		archive << gasUsed;
		archive << hash;
		archive << input;
		archive << isError;
		archive << isInternalTx;
		archive << nonce;
		archive << timeStamp;
		archive << to;
		archive << transactionIndex;
		archive << value;

	}
}

//---------------------------------------------------------------------------
void CTransaction::registerClass(void)
{
	static bool been_here=false;
	if (been_here) return;
	been_here=true;

	SFInt32 fieldNum=1000;
	ADD_FIELD(CTransaction, "schema",  T_NUMBER|TS_LABEL, ++fieldNum);
	ADD_FIELD(CTransaction, "deleted", T_RADIO|TS_LABEL,  ++fieldNum);
	ADD_FIELD(CTransaction, "handle", T_NUMBER|TS_LABEL,  ++fieldNum);
	ADD_FIELD(CTransaction, "blockHash", T_TEXT, ++fieldNum);
	ADD_FIELD(CTransaction, "blockNumber", T_NUMBER, ++fieldNum);
	ADD_FIELD(CTransaction, "confirmations", T_NUMBER, ++fieldNum);
	ADD_FIELD(CTransaction, "contractAddress", T_TEXT, ++fieldNum);
	ADD_FIELD(CTransaction, "cumulativeGasUsed", T_TEXT, ++fieldNum);
	ADD_FIELD(CTransaction, "from", T_TEXT, ++fieldNum);
	ADD_FIELD(CTransaction, "gas", T_TEXT, ++fieldNum);
	ADD_FIELD(CTransaction, "gasPrice", T_TEXT, ++fieldNum);
	ADD_FIELD(CTransaction, "gasUsed", T_TEXT, ++fieldNum);
	ADD_FIELD(CTransaction, "hash", T_TEXT, ++fieldNum);
	ADD_FIELD(CTransaction, "input", T_TEXT, ++fieldNum);
	ADD_FIELD(CTransaction, "isError", T_RADIO, ++fieldNum);
	ADD_FIELD(CTransaction, "isInternalTx", T_RADIO, ++fieldNum);
	ADD_FIELD(CTransaction, "nonce", T_TEXT, ++fieldNum);
	ADD_FIELD(CTransaction, "timeStamp", T_NUMBER, ++fieldNum);
	ADD_FIELD(CTransaction, "to", T_TEXT, ++fieldNum);
	ADD_FIELD(CTransaction, "transactionIndex", T_NUMBER, ++fieldNum);
	ADD_FIELD(CTransaction, "value", T_TEXT, ++fieldNum);

	// Hide our internal fields, user can turn them on if they like
	HIDE_FIELD(CTransaction, "schema");
	HIDE_FIELD(CTransaction, "deleted");
	HIDE_FIELD(CTransaction, "handle");

	// EXISTING_CODE
	ADD_FIELD(CTransaction, "date", T_DATE, ++fieldNum);
	ADD_FIELD(CTransaction, "ether", T_NUMBER, ++fieldNum);
	ADD_FIELD(CTransaction, "addr_list", T_TEXT, ++fieldNum);
	ADD_FIELD(CTransaction, "function", T_TEXT, ++fieldNum);
	ADD_FIELD(CTransaction, "recNum", T_NUMBER, ++fieldNum);

	// Hide fields we don't want to show by default
//	HIDE_FIELD(CTransaction, "date");
//	HIDE_FIELD(CTransaction, "ether");
	HIDE_FIELD(CTransaction, "addr_list");
	HIDE_FIELD(CTransaction, "confirmations");
	HIDE_FIELD(CTransaction, "recNum");
	HIDE_FIELD(CTransaction, "function");
//	HIDE_FIELD(CTransaction, "isInternalTx");
//	HIDE_FIELD(CTransaction, "isError");
	// EXISTING_CODE
}

//---------------------------------------------------------------------------
int sortTransaction(const SFString& f1, const SFString& f2, const void *rr1, const void *rr2)
{
	CTransaction *g1 = (CTransaction*)rr1;
	CTransaction *g2 = (CTransaction*)rr2;

	SFString v1 = g1->getValueByName(f1);
	SFString v2 = g2->getValueByName(f1);
	SFInt32 s = v1.Compare(v2);
	if (s || f2.IsEmpty())
		return (int)s;

	v1 = g1->getValueByName(f2);
	v2 = g2->getValueByName(f2);
	return (int)v1.Compare(v2);
}
int sortTransactionByName(const void *rr1, const void *rr2) { return sortTransaction("tr_Name", "", rr1, rr2); }
int sortTransactionByID  (const void *rr1, const void *rr2) { return sortTransaction("transactionID", "", rr1, rr2); }

#ifdef NEW_CODE
SFBool shortAddr=FALSE;
#endif
//---------------------------------------------------------------------------
SFString nextTransactionChunk_custom(const SFString& fieldIn, SFBool& force, const void *data)
{
	CTransactionNotify *tr = (CTransactionNotify*)data;
	const CTransaction *tra = tr->getDataPtr();
	switch (tolower(fieldIn[0]))
	{
		// EXISTING_CODE
		case 'a':
			if (fieldIn % "addr_list") return tra->getAddrList();
		case 'd':
			if (fieldIn % "date") return tra->m_transDate.Format("%Y-%m-%d %H:%M:%S UTC");
		case 'e':
			if ( fieldIn % "ether" )
			{
				// Make sure the wei number is at least 18 characters long. Once it is,
				// reverse it, put a decimal at the 18th position, reverse it back,
				// strip leading '0's except the tens digit.
				SFString ether = tra->value;
				if (ether.GetLength()<18)
					ether = padLeft(tra->value,18).Substitute(" ","0");
				ether.Reverse();
				ether = ether.Left(18) + "." + ether.Mid(18,1000);
				ether.Reverse();
				ether = StripLeading(ether, '0');
				if (ether.startsWith('.'))
					ether = "0" + ether;
				return ether;
			}
			break;
		case 'f':
			if ( fieldIn % "function" ) return tra->inputToFunction();
			break;
		case 'r':
			if ( fieldIn % "recNum" ) return asString(tra->pParent ? tra->pParent->getHandle() : 0);
			break;
#if 1 //def NEW_CODE
		case 's':
			if ( fieldIn % "sh_date" ) return tra->m_transDate.Format("%m/%d/16 %H:%M");
//			if ( fieldIn % "sh_function" ) { shortAddr=TRUE; SFString ret = tra->inputToFunction(); shortAddr=FALSE; return ret; }
			if ( fieldIn % "sh_from" ) return fmtAddress(tra->from);
			if ( fieldIn % "sh_to" ) return fmtAddress(tra->to);
			break;
#endif
		// EXISTING_CODE
		default:
			break;
	}
	
#pragma unused(tr)
#pragma unused(tra)

	return EMPTY;
}

//---------------------------------------------------------------------------
SFBool CTransaction::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const
{
	// EXISTING_CODE
	// EXISTING_CODE
	return FALSE;
}

// EXISTING_CODE
//---------------------------------------------------------------------------
int sortTransactionsForWrite(const void *rr1, const void *rr2)
{
	CTransaction *tr1 = (CTransaction*)rr1;
	CTransaction *tr2 = (CTransaction*)rr2;

	SFInt32 ret;
	ret = tr1->timeStamp - tr2->timeStamp;         if (ret!=0) return (int)ret;
	ret = tr1->from.Compare(tr2->from);            if (ret!=0) return (int)ret;
	ret = toLong(tr1->nonce) - toLong(tr2->nonce); if (ret!=0) return (int)ret;

	return (int)tr1->hash.Compare(tr2->hash);
}

//---------------------------------------------------------------------------
SFBool CTransaction::isFunction(const SFString& func) const
{
	if (func=="none")
	{
		SFString ret = inputToFunction();
		if (ret.ContainsAny("acghrstuv"))
			return FALSE;
		return (ret==" ");
	}

	return (funcPtr ? funcPtr->name == func : FALSE);
}

//------------------------------------------------------------------------------
#define grabPart(a,b)       StripLeading((a).Mid(64*(b),64),'0')
#define grabBigNum(a,b)     strtoull((const char*)grabPart(a,b),NULL,16)
#define toAddr(a,b)         "0x"+padLeft(grabPart(a,b),40,'0')
#define toAddrOld(a,b)      "0x"+grabPart(a,b)
#define toAscString(a,b)    hex2String("0x"+grabPart(a,b))
#define toBigNum(a,b)       asStringULL(grabBigNum(a,b))
#define toBigNum3(a,b)      padNum3(grabBigNum(a,b))
#define theRest(a,b)        (a).Mid(64*(b),(a).GetLength());
#define toVote(a,b)         (grabBigNum(a,b)?"Yea":"Nay")
#define toBoolean(a,b)      (grabBigNum(a,b)?"true":"false")
#define toBytes(a,b)        ((a).Mid(64*(b),64))
SFString parse(const SFString& params, int nItems, SFString *types)
{
	SFString ret;
	for (int item=0;item<nItems;item++)
	{
		SFString t = types[item];
		SFBool isDynamic = (t=="string" || t=="bytes" || t.Contains("[]"));
		SFString val;

#ifdef NEW_CODE
		     if ( t == "address"                    ) { val =          toAddr      (params,item); if (shortAddr) val = fmtAddress(val); }
#else
		     if ( t == "address"                    )   val =          toAddr      (params,item);
#endif
		else if ( t == "bool"                       )   val =          toBoolean   (params,item);
		else if ( t == "vote"                       )   val =          toVote      (params,item);
		else if ( t == "uint3"                      )   val =          toBigNum3   (params,item);
		else if ( t == "bytes256"                   )   val =          toAscString (params,item);
		else if ( t.Contains("int") &&   !isDynamic )   val =          toBigNum    (params,item);
		else if ( t.Contains("bytes") && !isDynamic )   val =          toBytes     (params,item); 
		else if ( isDynamic                         )   val = "off:" + toBigNum    (params,item);
		else                                            val = "unknown type: " + t;

		if (val.Contains("off:"))
		{
			long start = toLong(val.Substitute("off:","")) / 32;
			long len   = grabBigNum(params,start);
			if (t == "string")
				val = hex2String(params.Mid((start+1)*64,len*2)).Substitute("\n","\\n").Substitute("\r","");
			else
				val = "0x"+params.Mid((start+1)*64,len*2);
		}

//		if (transCount++<100) printf("%05ld-%03d: %-10.10s %s %s\n", transCount, item, (const char*)types[item], (const char*)params.Mid(item*64,64), (const char*)val);
		ret += ("|" + val);
	}
//	if (transCount<100) printf("\n");
	return ret;
}

/*
variable:
	if contains []
	if == bytes
	if == string

fixed:
	address		left	160	no
	bool		left	1	yes
	int8		left	8	yes
	int16		left	16	yes
	int32		left 	32	yes
	int256		left	256	yes
	uint8		left	8	yes
	uint16		left	16	yes
	uint32		left	32	yes
	uint256		left	256	yes
	bytes8		right	8	no
	bytes16		right	16	no
	bytes32		right	32	no

non-canonical:
	uint
*/

//---------------------------------------------------------------------------
SFString parseParams(const CTransaction* trans, const SFString& which, const SFString& params)
{
	if (which=="approve")
	{
		//function approve(address _spender, uint256 _amount) returns (bool success) {  discuss
		SFString items[] = { "address", "uint256", };
		int nItems = sizeof(items) / sizeof(SFString);
		return which + parse(params, nItems, items);

	} else if (which=="splitDAO")
	{
		//function splitDAO(uint _proposalID, address _newCurator)
		SFString items[] = { "uint3", "address", };
		int nItems = sizeof(items) / sizeof(SFString);
		return which + parse(params, nItems, items);

	} else if (which=="createTokenProxy")
	{
		//function createTokenProxy(address _tokenHolder)
		SFString items[] = { "address", };
		int nItems = sizeof(items) / sizeof(SFString);
		return which + parse(params, nItems, items);

	} else if (which=="getMyReward")
	{
		//function getMyReward()
		SFString items[] = { };
		int nItems = sizeof(items) / sizeof(SFString);
		return which + parse(params, nItems, items);
	
	} else if (which=="payOut")
	{
		//function payOut(address _recipient, uint _amount)
		SFString items[] = { "address", "uint256", };
		int nItems = sizeof(items) / sizeof(SFString);
		return which + parse(params, nItems, items);

	} else if (which=="receiveEther")
	{
		//function receiveEther()
		SFString items[] = { };
		int nItems = sizeof(items) / sizeof(SFString);
		return which + parse(params, nItems, items);
		return which;
	
	} else if (which=="changeDomain")
	{
		//function changeDomain( uint domain, uint expires, uint price, address transfer ) 
		SFString items[] = { "bytes256", "uint256", "uint256", "address", };
		int nItems = sizeof(items) / sizeof(SFString);
		return which + parse(params, nItems, items);

	} else if (which=="changeId")
	{
		//function changeId( uint domain, uint name, uint value )
		SFString items[] = { "bytes256", "bytes256", "uint256", };
		int nItems = sizeof(items) / sizeof(SFString);
		return which + parse(params, nItems, items);

	} else if (which=="getDomain")
	{
		//function getDomain( uint domain )
		SFString items[] = { "bytes256", };
		int nItems = sizeof(items) / sizeof(SFString);
		return which + parse(params, nItems, items);

	} else if (which=="getId")
	{
		//function getId( uint domain, uint id )
		SFString items[] = { "bytes256", "uint256", };
		int nItems = sizeof(items) / sizeof(SFString);
		return which + parse(params, nItems, items);

	} else if (which=="balanceOf")
	{
		//function balanceOf(addr _owner);
		SFString items[] = { "address", };
		int nItems = sizeof(items) / sizeof(SFString);
		return which + parse(params, nItems, items);

	} else if (which=="vote")
	{
		//function vote(uint _proposalID, bool _supportsProposal)
		SFString items[] = { "uint3", "vote", };
		int nItems = sizeof(items) / sizeof(SFString);
		return which + parse(params, nItems, items);
	
	} else if (which=="executeProposal")
	{
		//function executeProposal(uint _proposalID, bytes _transactionData)
		SFString items[] = { "uint3", "bytes", };
		int nItems = sizeof(items) / sizeof(SFString);
		return which + parse(params, nItems, items);

	} else if (which=="transferFrom")
	{
		//function transferFrom(address _from, address _to, uint256 _amount)
		SFString items[] = { "address", "address", "uint256", };
		int nItems = sizeof(items) / sizeof(SFString);
		return which + parse(params, nItems, items);

	} else if (which=="transfer")
	{
		//function transfer(address _to, uint256 _amount)
		SFString items[] = { "address", "uint256", };
		int nItems = sizeof(items) / sizeof(SFString);
		return which + parse(params, nItems, items);

	} else if (which=="newProposal")
	{
		//function newProposal(address _recipient,uint _amount,string _description,bytes _transactionData,uint _debatingPeriod,bool _newCurator)
		SFString items[] = { "address", "uint256", "string", "bytes", "uint256", "bool", };
		int nItems = sizeof(items) / sizeof(SFString);

		SFString type = (grabBigNum(trans->value,0)>0?" (non-split)":" (split)");
//		if (trans && trans->pParent && ((CAccount*)trans->pParent)->addr == recipient)
//			type.Replace(")", "-self)");
		return which + type + parse(params, nItems, items);
	}

	return which;
}

//---------------------------------------------------------------------------
SFString CTransaction::inputToFunction(void) const
{
	if (funcPtr)
		return parseParams(this, funcPtr->name, input.Mid(10,input.GetLength()));

	if (input.GetLength()<10)
		return " ";
#if 1
	switch (input[2])
	{
		case '0':
			if      (input.startsWith( "0x095ea7b3" )) return parseParams(this, "approve",                   input.Mid(10,input.GetLength()));
		     	else if (input.startsWith( "0x0221038a" )) return parseParams(this, "payOut",                    input.Mid(10,input.GetLength()));
		     	break;

		case '1':
			if      (input.startsWith( "0x1a7a98e2" )) return parseParams(this, "getDomain",                 input.Mid(10,input.GetLength()));
			break;

		case '2':
			if      (input.startsWith( "0x237e9492" )) return parseParams(this, "executeProposal",           input.Mid(10,input.GetLength()));
		     	else if (input.startsWith( "0x24fc65ed" )) return parseParams(this, "getId",                     input.Mid(10,input.GetLength()));
		     	else if (input.startsWith( "0x23b872dd" )) return parseParams(this, "transferFrom",              input.Mid(10,input.GetLength()));
		     	else if (input.startsWith( "0x2632bf20" )) return parseParams(this, "unblockMe",                 input.Mid(10,input.GetLength()));
		     	break;

		case '3':
			break;

		case '4':
			if      (input.startsWith( "0x4e10c3ee" )) return parseParams(this, "transferWithoutReward",     input.Mid(10,input.GetLength()));
			break;

		case '5':
			if      (input.startsWith( "0x590e1ae3" )) return parseParams(this, "refund",                    input.Mid(10,input.GetLength()));
			break;

		case '6':
			if      (input.startsWith( "0x612e45a3" )) return parseParams(this, "newProposal",               input.Mid(10,input.GetLength()));
			break;

		case '7':
			if      (input.startsWith( "0x78524b2e" )) return parseParams(this, "halveMinQuorum",            input.Mid(10,input.GetLength()));
			else if (input.startsWith( "0x75090ebf" )) return parseParams(this, "changeDomain",              input.Mid(10,input.GetLength()));
			else if (input.startsWith( "0x70a08231" )) return parseParams(this, "balanceOf",                 input.Mid(10,input.GetLength()));
			break;

		case '8':
			if      (input.startsWith( "0x82661dc4" )) return parseParams(this, "splitDAO",                  input.Mid(10,input.GetLength()));
			break;

		case '9':
			break;

		case 'a':
			if      (input.startsWith( "0xa1da2fb9" )) return parseParams(this, "retrieveDAOReward",         input.Mid(10,input.GetLength()));
			else if (input.startsWith( "0xa3912ec8" )) return parseParams(this, "receiveEther",              input.Mid(10,input.GetLength()));
		     	else if (input.startsWith( "0xa9059cbb" )) return parseParams(this, "transfer",                  input.Mid(10,input.GetLength()));
		     	break;

		case 'b':
			if      (input.startsWith( "0xbaac5300" )) return parseParams(this, "createTokenProxy",          input.Mid(10,input.GetLength()));
			break;

		case 'c':
			if      (input.startsWith( "0xc9d27afe" )) return parseParams(this, "vote",                      input.Mid(10,input.GetLength()));
			else if (input.startsWith( "0xcc9ae3f6" )) return parseParams(this, "getMyReward",               input.Mid(10,input.GetLength()));
		     	break;

		case 'd':
			if      (input.startsWith( "0xdbde1988" )) return parseParams(this, "transferFromWithoutReward", input.Mid(10,input.GetLength()));
			break;

		case 'e':
			if      (input.startsWith( "0xeceb2945" )) return parseParams(this, "checkProposalCode",         input.Mid(10,input.GetLength()));
			else if (input.startsWith( "0xeb1ff845" )) return parseParams(this, "changeId",                  input.Mid(10,input.GetLength()));
		     	break;

		case 'f':
			break;
	}

	if (pParent)
	{
#if 1 //NEW_CODE
		for (int i=0;i<((CAccount*)pParent)->abi.abiByName.getCount();i++)
			if (input.Mid(2,8) == ((CAccount*)pParent)->abi.abiByName[i].encoding)
				return ((CAccount*)pParent)->abi.abiByName[i].Format("[{NAME}]");
#else
		for (int i=0;i<((CAccount*)pParent)->abi.abiByEncoding.getCount();i++)
			if (input.Mid(2,8) == ((CAccount*)pParent)->abi.abiByEncoding[i].encoding)
				return ((CAccount*)pParent)->abi.abiByEncoding[i].Format("[{NAME}]");
#endif
	}
#endif

	return " ";
}

SFString CTransaction::getAddrsFromInput(char delim) const
{
	SFString ret;
	if (funcPtr && funcPtr->hasAddrs)
	{
		SFString func = parseParams(this, funcPtr->name, input.Mid(10,input.GetLength()));
		nextTokenClear(func,'|'); // remove function name
		SFInt32 cnt=0;
		while (!func.IsEmpty())
		{
			SFString val = nextTokenClear(func,'|');
			CParameter pp = funcPtr->inputs[cnt];
			if (pp.type == "address")
				ret += (val + delim);
			cnt++;
		}
	}
	return ret;
}

SFString CTransaction::getAddrList(char delim) const
{
	return Strip((from + delim + to + delim + getAddrsFromInput(delim)), delim);
}
// EXISTING_CODE
