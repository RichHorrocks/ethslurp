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
#include "basenode.h"

//--------------------------------------------------------------------------------
ghRuntimeClass CBaseNode::classCBaseNode;
static CBuiltIn _biBaseNode(&CBaseNode::classCBaseNode, "CBaseNode", sizeof(CBaseNode), NULL, NULL, NO_SCHEMA);

//--------------------------------------------------------------------------------
CBaseNode::CBaseNode(void)
{
	Init();
}

//--------------------------------------------------------------------------------
CBaseNode::~CBaseNode(void)
{
	ASSERT(m_refCount==0);
}

//--------------------------------------------------------------------------------
void CBaseNode::Init(void)
{
	m_refCount = 0;
	m_deleted  = FALSE;
	m_schema = NO_SCHEMA;
	m_showing = TRUE;
	pParent = NULL;
}

//--------------------------------------------------------------------------------
void CBaseNode::Copy(const CBaseNode& bn)
{
	m_refCount = bn.m_refCount;
	m_deleted  = bn.m_deleted;
	m_schema = bn.m_schema;
	m_showing = bn.m_showing;
	pParent = NULL;
}

//--------------------------------------------------------------------------------
void CBaseNode::Reference(void)
{
	m_refCount++;
}

//--------------------------------------------------------------------------------
SFInt32 CBaseNode::Dereference(void)
{
	ASSERT(m_RefCount>0);
	m_refCount--;
	return (m_refCount==0);
}

//--------------------------------------------------------------------------------
SFBool CBaseNode::isReferenced(void) const
{
	return (m_refCount);
}

//--------------------------------------------------------------------------------
SFBool CBaseNode::isDeleted(void) const
{
	return (m_deleted);
}

//--------------------------------------------------------------------------------
void CBaseNode::setDeleted(SFBool del)
{
	m_deleted = del;
}

//--------------------------------------------------------------------------------
SFInt32 CBaseNode::getSchema(void) const
{
	return (m_schema);
}

//--------------------------------------------------------------------------------
SFBool CBaseNode::isShowing(void) const
{
	return (m_showing);
}

//--------------------------------------------------------------------------------
void CBaseNode::setSchema(SFBool val)
{
	m_schema = val;
}

//--------------------------------------------------------------------------------
void CBaseNode::setShowing(SFBool val)
{
	m_showing = val;
}

//--------------------------------------------------------------------------------
SFBool CBaseNode::isKindOf(const ghRuntimeClass* pClass) const
{
	ghRuntimeClass* pClassThis = getRuntimeClass();
	return pClassThis->IsDerivedFrom(pClass);
}

//--------------------------------------------------------------------------------
ghRuntimeClass* CBaseNode::getRuntimeClass() const
{
	return &CBaseNode::classCBaseNode;
}

//--------------------------------------------------------------------------------
char *CBaseNode::parseCSV(char *s, SFInt32& nFields, const SFString *fields)
{
	nFields = 0;

	typedef enum { OUTSIDE=0, INSIDE } parseState;
	parseState state = OUTSIDE;

	char *fieldVal=NULL;
	while (*s)
	{
		switch (state)
		{
			case OUTSIDE:
				if (*s=='\"')
				{
					state = INSIDE;
					fieldVal=s+1;

				} else if (*s=='\n')
				{
					finishParse();
					return (s+1);
				}
				s++;
				break;

			case INSIDE:
				if (*s == '\"')
				{
					*s = '\0';
					if (!this->setValueByName(fields[nFields++], fieldVal))
					{
					//	fprintf(stderr,"Bad field name %s. Quitting...", (const char*)fields[nFields-1]);
					//	return NULL;
					}
					fieldVal = NULL;
					state = OUTSIDE;

				}
				s++;
				break;
		}
	}
	finishParse();
	return NULL;
}

//--------------------------------------------------------------------------------
char *CBaseNode::parseText(char *s, SFInt32& nFields, const SFString *fields)
{
	nFields = 0;
	char *fieldVal=s;
	while (s && *s)
	{
		switch (*s)
		{
		case '\r':
			break;
		case '\t':
			*s = '\0';
			this->setValueByName(fields[nFields++], fieldVal);
			fieldVal = s+1;
			break;
		case '\n':
			*s = '\0';
			this->setValueByName(fields[nFields++], fieldVal);
			finishParse();
			return s+1;
		}
		s++;
	}
	this->setValueByName(fields[nFields++], fieldVal);
	finishParse();
	return NULL;
}

//--------------------------------------------------------------------------------
char *CBaseNode::parseJson(char *s)
{
	SFInt32 nFields=0;
	return parseJson(s, nFields);
}

//SFString tbs;
//--------------------------------------------------------------------------------
char *CBaseNode::parseJson(char *s, SFInt32& nFields)
{
//	tbs+="\t";
//	printf("--------------------------\n%s\n-----------------------------\n", s);
//	fflush(stdout);
//	printf("--------------------------\n%s\n-----------------------------\n", (const char*)SFString(s).Mid(SFString(s).Find("{"),300));
//	printf("--------------------------\n%s\n-----------------------------\n", (const char*)SFString(s).Right(300));
	typedef enum { OUTSIDE=0, IN_NAME, IN_VALUE } parseState;
	parseState state = OUTSIDE;

	char *fieldName=NULL;
	char *fieldVal=NULL;
	while (s && *s)
	{
		switch (state)
		{
		case OUTSIDE:
			if (*s=='{')
				state = IN_NAME;
			s++;
			break;

		case IN_NAME:
			if (!fieldName)
			{
				fieldName = s;

			} else if (*s == ':')
			{
				state = IN_VALUE;
				*s = '\0';
//printf("fn: %-10.10s fv: %-40.40s ---> %-60.60s\n" , fieldName, fieldVal, (s+1));
			}
			s++;
			break;

		case IN_VALUE:
			fieldVal = s;
			if (*s=='[') // array skip to end of array
			{
				fieldVal++;
				s++;
				bool done = false;int lev=1;
				while (s && *s && !done)
				{
					if (*s == '[')
					{
						lev++;

					} else if (lev==1 && *s == ']')
					{
						done=true;
						s--; // remove the closing bracket

					} else if (*s == ']')
					{
						lev--;
					}
					s++;
				}
			} else
			{
				bool done = false;int lev=1;
				while (s && *s && !done)
				{
					if (*s == '{')
					{
						lev++;

					} else if (lev==1 && (*s == ',' || *s == '}'))
					{
						done=true;
						s--;

					} else if (*s == '}')
					{
						lev--;
					}
					s++;
				}
			}
			*s = '\0';
//			printf("%sfn: %-20.20s fv: %-60.60s ---> %-40.40s\n" , (const char*)tbs,fieldName, fieldVal, (s+1));fflush(stdout);
			nFields += this->setValueByName(fieldName, fieldVal);
			fieldName = NULL;
			fieldVal = NULL;
			state = IN_NAME;
			s++;
			if (*s && *s==',')
				s++;
			if (s && *s && (*s=='{'||*s==']'))
			{
				finishParse();
//				tbs.Replace("\t","");
				return s;
			}
			break;
		}
	}
//	tbs.Replace("\t","");
	finishParse();
	return NULL;
}

//--------------------------------------------------------------------------------
char *cleanUpJson(char *s)
{
	if (!s)
		return s;
	char *l = s, *start = s;
	while (*s)
	{
		if (!isWhiteSpace(*s) && *s != '\"') // zap all the white space and quotes
		{
			*l = *s;
			l++;
		}
		s++;
	}
	*l = '\0';
	return start;
}

#include "sfarchive.h"
//---------------------------------------------------------------------------
SFBool CBaseNode::SerializeHeader(SFArchive& archive)
{
	archive.pParent = this; // sets this value for items stored in lists or arrays -- read only
	if (archive.m_isReading)
	{
		SFString str;
		archive >> m_deleted;
		archive >> m_schema;
		archive >> m_showing;
		archive >> str; ASSERT(str == getRuntimeClass()->getClassNamePtr());
	} else
	{
		if (m_deleted && !archive.writeDeleted())
			return FALSE;
		archive << m_deleted;
		archive << m_schema;
		archive << m_showing;
		archive << getRuntimeClass()->getClassNamePtr();
	}
	return TRUE;
}

//---------------------------------------------------------------------------
static CExportOptions expC;
CExportOptions& expContext(void)
{
	return expC;
}

//---------------------------------------------------------------------------
void incIndent(void)
{
	expC.lev++;
}

//---------------------------------------------------------------------------
void decIndent(void)
{
	expC.lev--;
}

//---------------------------------------------------------------------------
SFString indent(void)
{
	return SFString(expC.tab,expC.spcs*expC.lev);
}

extern SFString decBigNum(const SFString& str);
//--------------------------------------------------------------------------------
SFString CBaseNode::toJson(void) const
{
	CExportOptions& opts = expContext();

	const CFieldList *fieldList = getRuntimeClass()->GetFieldList();
	if (!fieldList)
	{
		fprintf(stderr,"No fieldList in %s. Did you register the class? Quitting...\n", (const char*)getRuntimeClass()->m_ClassName);
		exit(0);
	}

	SFString newLine(opts.nl);
	SFString ret;

	SFBool first=true;
	if (!expContext().noFrst)
		ret += indent();
	expContext().noFrst=false;
	ret += "{";
	LISTPOS lPos = fieldList->SFList<CFieldData *>::GetHeadPosition();
	while (lPos)
	{
		incIndent();
		CFieldData *fld = fieldList->GetNextItem(lPos);
		SFString val = getValueByName(fld->getFieldName());
		if (!fld->isHidden() && (!val.IsEmpty() || fld->isArray()))
		{
			if (!first) ret += ",\n"; else ret += "\n";
			first=false;
			ret += indent();
			ret += "\"" + fld->getFieldName() + "\"";
			ret += ": ";
			if (fld->isArray() && val.IsEmpty())
			{
				ret += "[]";

			} else if (fld->isArray())
			{
				ret += "[\n" + val + indent() + "]";

			} else if (fld->isObject())
			{
				ret += val;

			} else if (fld->getFieldType() == T_NUMBER || fld->getFieldType() == T_QNUMBER)
			{
				ret += "\"" + decBigNum(val) + "\"";

			} else
			{
				ret += "\"" + val + "\"";
			}
		}
		decIndent();
	}
	ret += "\n";
	ret += indent() + "}";
	return ret;
}

#include "biglib.h"
SFString decBigNum(const SFString& str)
{
	string s = (const char*)((str.startsWith("0x")?str.Mid(2,1000):str));
	SFString ret = to_string(hex2BigUint(s)).c_str();
	SFInt32 len = ret.GetLength();
	     if (len>29) ret = ret.Left(1) + "." + StripTrailing(ret.Mid(1,10000),'0') + "e+29";
	else if (len>28) ret = ret.Left(1) + "." + StripTrailing(ret.Mid(1,10000),'0') + "e+28";
	else if (len>27) ret = ret.Left(1) + "." + StripTrailing(ret.Mid(1,10000),'0') + "e+27";
	else if (len>26) ret = ret.Left(1) + "." + StripTrailing(ret.Mid(1,10000),'0') + "e+26";
	else if (len>25) ret = ret.Left(1) + "." + StripTrailing(ret.Mid(1,10000),'0') + "e+25";
	else if (len>24) ret = ret.Left(1) + "." + StripTrailing(ret.Mid(1,10000),'0') + "e+24";
	else if (len>23) ret = ret.Left(1) + "." + StripTrailing(ret.Mid(1,10000),'0') + "e+23";
	else if (len>22) ret = ret.Left(1) + "." + StripTrailing(ret.Mid(1,10000),'0') + "e+22";
	else if (len>21) ret = ret.Left(1) + "." + StripTrailing(ret.Mid(1,10000),'0') + "e+21";
	ret.Replace(".e+","e+");
	return ret;
}
