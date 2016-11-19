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
#include "basetypes.h"

#include "fielddata.h"

//-------------------------------------------------------------------------
void CFieldList::Release(void) const
{
	((CFieldList*)this)->m_group = EMPTY;
}

//-------------------------------------------------------------------------
LISTPOS CFieldList::GetFirstItem(const SFString& filter) const
{
	if (filter != nullString)
		((CFieldList*)this)->m_group = filter;
	return SFList<CFieldData*>::GetHeadPosition();
};

//-------------------------------------------------------------------------
CFieldData *CFieldList::GetNextItem(LISTPOS& rPosition) const
{
	CFieldData *data = SFList<CFieldData*>::GetNext(rPosition);
	if (m_group.IsEmpty())
		return data;

	while (rPosition && !(data->getGroupName() % m_group))
		data = SFList<CFieldData*>::GetNext(rPosition);

	return data;
}

//-------------------------------------------------------------------------
const CFieldData *CFieldList::getFieldByID(SFInt32 id) const
{
	if (id == NOT_A_FIELD)
		return NULL;

	LISTPOS lPos = GetFirstItem();
	while (lPos)
	{
		CFieldData *field = GetNextItem(lPos);
		if (field->getFieldID() == id)
			return field;
	}

	//ASSERT(0); // should never happen
	return NULL;
}

//-------------------------------------------------------------------------
const CFieldData *CFieldList::getFieldByName(const SFString& fieldString) const
{
	SFString fieldName = fieldString; fieldName = nextTokenClear(fieldName, '|'); // the input may contain more than just fieldName

	const CFieldData *field = NULL;
	LISTPOS lPos = GetFirstItem();
	while (lPos)
	{
		CFieldData *current = GetNextItem(lPos);
		if (current->getFieldName() % fieldName)
		{
			field = current;
			continue;
		}
	}

	if (!field)
	{
		// always return something
		static const CFieldData non_field = CFieldData("|unknown");
		field = &non_field;
		((CFieldData*)field)->setFieldID(NOT_A_FIELD);
	}

	return field;
}
