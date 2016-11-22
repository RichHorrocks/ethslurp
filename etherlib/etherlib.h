#ifndef _ETHER_LIB_
#define _ETHER_LIB_
//#define CONVERT_TO_PARITY
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
#include "ethtypes.h"
#include "parameter.h"
#include "function.h"
#include "block.h"
#include "abi.h"
#include "transaction.h"
#include "account.h"
#include "blockchain.h"
#include "receipt.h"
#include "logentry.h"
#include "webapi.h"
#include "dapp.h"
#include "pricequote.h"
#include "trace.h"

//------------------------------------------------------------------------------
extern SFString compressHash  (const SFString& hashIn);
extern SFString uncompressHash(const SFString& hashIn);

//---------------------------------------------------------------------------------------------------
inline SFUint32 thing(const SFString& str)
{
//outErr << str << " : " << toLong(str) << " : " << (SFUint32)hex2Long(str) << "\n";
	return str.startsWith("0x") ? (SFUint32)hex2Long(str) : toLong(str);
}

inline SFString outThing(SFUint32 n)
{
//	return "0x"+asHex8(n);
//outErr << "outThing: " << n << " : " << "0x"+asHex8(n) << " : " << asStringU(n) << "\n";
	return asStringU(n);
}

#ifdef CONVERT_TO_PARITY
inline SFString lengthen(const SFString& strIn)
{
	static SFInt32 len = SFString("0x0b047f8d31897baaed7ac1bd3bd2576537cc7aec0c67bb054ac708599bf03a33").GetLength();
	SFString ret = strIn;
	while (ret.GetLength() < len)
		ret.Replace("0x","0x0");
	return ret;
}
#else
#define lengthen(a) (a)
#endif

#endif
