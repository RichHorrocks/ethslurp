#ifndef _LOGENTRY_H_
#define _LOGENTRY_H_
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
#include "ethtypes.h"

//--------------------------------------------------------------------------
class CLogEntry;
typedef SFArrayBase<CLogEntry>         CLogEntryArray;
typedef SFList<CLogEntry*>             CLogEntryList;
typedef CNotifyClass<const CLogEntry*> CLogEntryNotify;
typedef SFUniqueList<CLogEntry*>       CLogEntryListU;

//---------------------------------------------------------------------------
extern int sortLogentry        (const SFString& f1, const SFString& f2, const void *rr1, const void *rr2);
extern int sortLogentryByName  (const void *rr1, const void *rr2);
extern int sortLogentryByID    (const void *rr1, const void *rr2);
extern int isDuplicateLogentry (const void *rr1, const void *rr2);

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CLogEntry : public CBaseNode
{
public:
	SFAddress address;
	SFHash blockHash;
	SFInt32 blockNumber;
	SFString data;
	SFInt32 logIndex;
	SFStringArray topics;
	SFHash transactionHash;
	SFInt32 transactionIndex;

public:
					CLogEntry  (void);
					CLogEntry  (const CLogEntry& lo);
				   ~CLogEntry  (void);
	CLogEntry&	operator= 		(const CLogEntry& lo);

	DECLARE_NODE (CLogEntry);

	// EXISTING_CODE
	// EXISTING_CODE

protected:
	void			Clear      		(void);
	void			Init      		(void);
	void			Copy      		(const CLogEntry& lo);
	SFBool                  readBackLevel           (SFArchive& archive);

	// EXISTING_CODE
	// EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CLogEntry::CLogEntry(void)
{
	Init();
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CLogEntry::CLogEntry(const CLogEntry& lo)
{
	// EXISTING_CODE
	// EXISTING_CODE
	Copy(lo);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CLogEntry::~CLogEntry(void)
{
	Clear();
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLogEntry::Clear(void)
{
	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLogEntry::Init(void)
{
	CBaseNode::Init();

	address = EMPTY;
	blockHash = EMPTY;
	blockNumber = 0;
	data = EMPTY;
	logIndex = 0;
//	topics = ??; /* unknown type: SFStringArray */
	transactionHash = EMPTY;
	transactionIndex = 0;

	// EXISTING_CODE
	// EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CLogEntry::Copy(const CLogEntry& lo)
{
	Clear();

	CBaseNode::Copy(lo);
	address = lo.address;
	blockHash = lo.blockHash;
	blockNumber = lo.blockNumber;
	data = lo.data;
	logIndex = lo.logIndex;
	topics = lo.topics;
	transactionHash = lo.transactionHash;
	transactionIndex = lo.transactionIndex;

	// EXISTING_CODE
	// EXISTING_CODE
	finishParse();
}

//--------------------------------------------------------------------------
inline CLogEntry& CLogEntry::operator=(const CLogEntry& lo)
{
	Copy(lo);
	// EXISTING_CODE
	// EXISTING_CODE
	return *this;
}

//---------------------------------------------------------------------------
inline SFString CLogEntry::getValueByName(const SFString& fieldName) const
{
	// EXISTING_CODE
	// EXISTING_CODE
	return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
extern SFString nextLogentryChunk(const SFString& fieldIn, SFBool& force, const void *data);

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CLogEntryArray);
IMPLEMENT_ARCHIVE_LIST(CLogEntryList);

//---------------------------------------------------------------------------
extern SFString nextLogentryChunk_custom(const SFString& fieldIn, SFBool& force, const void *data);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

#endif
