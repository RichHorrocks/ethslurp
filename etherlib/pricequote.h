#ifndef _PRICEQUOTE_H_
#define _PRICEQUOTE_H_
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
#include "utillib.h"

//--------------------------------------------------------------------------
class CPriceQuote;
typedef SFArrayBase<CPriceQuote>         CPriceQuoteArray;
typedef SFList<CPriceQuote*>             CPriceQuoteList;
typedef CNotifyClass<const CPriceQuote*> CPriceQuoteNotify;
typedef SFUniqueList<CPriceQuote*>       CPriceQuoteListU;

//---------------------------------------------------------------------------
extern int sortPricequote        (const SFString& f1, const SFString& f2, const void *rr1, const void *rr2);
extern int sortPricequoteByName  (const void *rr1, const void *rr2);
extern int sortPricequoteByID    (const void *rr1, const void *rr2);
extern int isDuplicatePricequote (const void *rr1, const void *rr2);

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CPriceQuote : public CBaseNode
{
public:
	SFInt32 timeStamp;
	float open;
	float high;
	float low;
	float close;
	float quoteVolume;
	float volume;
	float weightedAvg;

public:
					CPriceQuote  (void);
					CPriceQuote  (const CPriceQuote& pr);
				   ~CPriceQuote  (void);
	CPriceQuote&	operator= 		(const CPriceQuote& pr);

	DECLARE_NODE (CPriceQuote);

	// EXISTING_CODE
	SFTime date;
	// EXISTING_CODE

protected:
	void			Clear      		(void);
	void			Init      		(void);
	void			Copy      		(const CPriceQuote& pr);
	SFBool                  readBackLevel           (SFArchive& archive);

	// EXISTING_CODE
	// EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CPriceQuote::CPriceQuote(void)
{
	Init();
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CPriceQuote::CPriceQuote(const CPriceQuote& pr)
{
	// EXISTING_CODE
	// EXISTING_CODE
	Copy(pr);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CPriceQuote::~CPriceQuote(void)
{
	Clear();
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPriceQuote::Clear(void)
{
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPriceQuote::Init(void)
{
	CBaseNode::Init();

	timeStamp = 0;
	open = 0.0;
	high = 0.0;
	low = 0.0;
	close = 0.0;
	quoteVolume = 0.0;
	volume = 0.0;
	weightedAvg = 0.0;

	// EXISTING_CODE
	date = earliestDate;
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CPriceQuote::Copy(const CPriceQuote& pr)
{
	Clear();

	CBaseNode::Copy(pr);
	timeStamp = pr.timeStamp;
	open = pr.open;
	high = pr.high;
	low = pr.low;
	close = pr.close;
	quoteVolume = pr.quoteVolume;
	volume = pr.volume;
	weightedAvg = pr.weightedAvg;

	// EXISTING_CODE
	date = pr.date;
	// EXISTING_CODE
	finishParse();
}

//--------------------------------------------------------------------------
inline CPriceQuote& CPriceQuote::operator=(const CPriceQuote& pr)
{
	Copy(pr);
	// EXISTING_CODE
	// EXISTING_CODE
	return *this;
}

//---------------------------------------------------------------------------
inline SFString CPriceQuote::getValueByName(const SFString& fieldName) const
{
	// EXISTING_CODE
	// EXISTING_CODE
	return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
extern SFString nextPricequoteChunk(const SFString& fieldIn, SFBool& force, const void *data);

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CPriceQuoteArray);
IMPLEMENT_ARCHIVE_LIST(CPriceQuoteList);

//---------------------------------------------------------------------------
extern SFString nextPricequoteChunk_custom(const SFString& fieldIn, SFBool& force, const void *data);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

#endif
