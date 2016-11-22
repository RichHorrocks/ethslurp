#ifndef _FIELDDATA_H
#define _FIELDDATA_H
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
#include "list.h"

class CConfig;

#define NOT_A_FIELD	1
//typedef SFBool (*SELFUNC)(SFInt32& nVals, SFAttribute *attrs, CSelectionData *data);

//-------------------------------------------------------------------------
class CFieldData
{
private:
	// saved in file
	SFString m_groupName;
	SFString m_fieldName;
	SFString m_prompt;
	SFString m_defStr;
	SFBool   m_required;
	SFBool   m_export;
	SFInt32  m_minPerms;
	SFBool   m_access;
	SFString m_helpText;

	// not saved in file
	SFInt32  m_fieldID;
	SFInt32  m_defInt;
	SFInt32  m_type;
	SFInt32  m_editSize;
	SFInt32  m_maxSize;
    SFString m_value;
	SFBool   m_resolved;
	SFString m_script;
	SFBool   m_disabled;
	SFString m_problem;
	SFBool   m_hidden;
	SFBool   m_noRow;
	SFBool   m_showHidden;
	SFInt32  m_extraInt;
	SFString m_extraStr;
//	SELFUNC  m_selFunc;
	void    *m_dataPtr;

public:
	CFieldData (void);

	CFieldData (const CFieldData& data)
		{
			Copy(data);
		}

	CFieldData (const SFString& group, const SFString& fieldName, const SFString& prompt, const SFString& value, SFInt32 type, SFBool required, SFInt32 fieldID);
	CFieldData (const SFString& group, const SFString& fieldName, const SFString& prompt, SFInt32 value,         SFInt32 type, SFBool required, SFInt32 fieldID)
	{ *this = CFieldData(group, fieldName, prompt, asString(value), type, required, fieldID); }

	CFieldData (const SFString& in);

	CFieldData& operator=(const CFieldData& data)
		{
			Copy(data);
			return *this;
		}

	SFBool operator==(const CFieldData& data)
		{
			if ( m_groupName   != data.m_groupName) return FALSE;
			if ( m_fieldName   != data.m_fieldName) return FALSE;
			if ( m_prompt      != data.m_prompt   ) return FALSE;
			if ( m_defStr      != data.m_defStr   ) return FALSE;
			if ( m_required    != data.m_required ) return FALSE;
			if ( m_export      != data.m_export   ) return FALSE;
			if ( m_minPerms    != data.m_minPerms ) return FALSE;
			if ( m_access      != data.m_access   ) return FALSE;
			if ( m_helpText    != data.m_helpText ) return FALSE;
			if ( m_fieldID     != data.m_fieldID  ) return FALSE;

			ASSERT( m_defInt     == data.m_defInt   );
			ASSERT( m_type       == data.m_type     );
			ASSERT( m_editSize   == data.m_editSize );
			ASSERT( m_maxSize    == data.m_maxSize  );
			ASSERT( m_value      == data.m_value    );
			ASSERT( m_resolved   == data.m_resolved );
			ASSERT( m_disabled   == data.m_disabled );
			ASSERT( m_problem    == data.m_problem  );
			ASSERT( m_noRow      == data.m_noRow    );
			//ASSERT( m_showHidden == data.m_showHidden );
			//ASSERT( m_hidden   == data.m_hidden   );
			//ASSERT( m_script   == data.m_script   );
			//ASSERT( m_extraInt == data.m_extraInt );
			//ASSERT( m_extraStr == data.m_extraStr );
//			//ASSERT( m_selFunc  == data.m_selFunc  );
			//ASSERT( m_dataPtr  == data.m_dataPtr  );

			return TRUE;
		}

	SFBool operator!=(const CFieldData& data)
		{
			return !operator==(data);
		}

	~CFieldData(void)
		{
		}

	//--------------------------------------------------------------------------------
	void   setSizes (void);

	SFString getFieldName   (void) const
	{
		if (m_fieldName.Contains('|'))
			return m_fieldName.Left(m_fieldName.Find("|"));
		return m_fieldName;
	}
	SFBool isObject(void) const;
	SFBool isArray(void) const;
	SFString getControl   (void) const;

	SFString getGroupName (void) const { return m_groupName; }
	SFString getPrompt    (void) const { return m_prompt;    }
	SFString getDefStr    (void) const { return m_defStr;    }
	SFInt32  getDefInt    (void) const { return m_defInt;    }
	SFBool   isRequired   (void) const { return m_required;  }
	SFInt32  getFieldType (void) const { return m_type;      }
	SFInt32  getMinPerms  (void) const { return m_minPerms;  }
	SFInt32  getEditSize  (void) const { return m_editSize;  }
	SFInt32  getMaxSize   (void) const { return m_maxSize;   }
	SFString getValue     (void) const { return m_value;     }
	SFInt32  getValueI    (void) const { return toLong(m_value); }
	SFBool   isExport     (void) const { return m_export;    }
	SFBool   getAccess    (void) const { return m_access;    }
	SFString getHelpText  (void) const { return m_helpText;  }
	SFInt32  getFieldID   (void) const { return m_fieldID;   }
	SFBool   isResolved   (void) const { return m_resolved;  }
	SFString getScript    (void) const { return m_script;    }
//	SELFUNC  getSelFunc   (void) const { return m_selFunc;   }
	void    *getDataPtr   (void) const { return m_dataPtr;   }
	SFBool   getNoRow     (void) const { return m_noRow;     }
	SFBool   getShowHidden(void) const { return m_showHidden;}
	SFString getProblem   (void) const { return m_problem;   }
	SFBool   isDisabled   (void) const { return m_disabled;  }
	SFInt32  getExtraInt  (void) const { return m_extraInt;  }
	SFString getExtraStr  (void) const { return m_extraStr;  }
	SFBool   isHidden     (void) const { return m_hidden;    }

	SFBool   isFieldVisible(SFInt32 userPerm) const;

	void     setFieldName (const SFString& fieldName) { m_fieldName = fieldName;    }
	void     setGroupName (const SFString& groupName) { m_groupName = groupName;    }
	void     setPrompt    (const SFString& prompt)    { m_prompt    = prompt;       }
	void     setRequired  (SFBool required)           { m_required  = required;     }
	void     setMinPerms  (SFInt32 minPerm)           { m_minPerms  = minPerm;      }
	void     setValue     (const SFString& val)       { m_value     = val;          }
	void     setValue     (SFInt32 val)               { m_value     = asString(val);}
	void     setExport    (SFBool exp)                { m_export    = exp;          }
	void     setAccess    (SFBool priv)               { m_access    = priv;         }
	void     setHelpText  (const SFString& ht)        { m_helpText  = ht;           }
	void     setResolved  (SFBool on)                 { m_resolved  = on;           }
	void     setDisabled  (SFBool disable)            { m_disabled  = disable;      }
	void     setHidden    (SFBool hide)               { m_hidden    = hide;         }
//	void     setSelFunc   (SELFUNC func)              { m_selFunc   = func;         }
	void     setDataPtr   (void *data)                { m_dataPtr   = data;         }
	void     setNoRow     (SFBool noRow)              { m_noRow     = noRow;        }
	void     setShowHidden(SFBool showHidden)         { m_showHidden= showHidden;   }
	void     setEditSize  (SFInt32 edit)              { m_editSize  = edit;         }
	void     setMaxsize   (SFInt32 max)               { m_maxSize   = max;          }
	void     setExtraInt  (SFInt32 ext)               { m_extraInt  = ext;          }
	void     setExtraStr  (const SFString& ext)       { m_extraStr  = ext;          }
	void     setScript    (const SFString& scr)       { m_script    = scr;          }
	void     setFieldID   (SFInt32 id);
	void     setFieldType (SFInt32 type)
		{
			m_type = type;
			setSizes();
		}

	void     setProblem   (const SFString& prob);
	void     setLabel(const SFString& label)
		{
			m_script = EMPTY;
			addScript("<label>" + label + "</label>");
		}
	void     addScript(const SFString& script)
		{
			m_script += script;
		}
	void     setDefStr    (const SFString& defStr)
		{
			m_defStr = defStr;
			m_defInt = toLong(m_defStr);
		}

	SFString toAscii      (void) const;

	SFBool Customize(const CConfig *config);

private:
	void Init(void)
		{
//	SFString m_groupName;
//	SFString m_fieldName;
//	SFString m_prompt;
//	SFString m_defStr;
//	SFString m_value;
			m_type       = 0;
			m_editSize   = 0;
			m_maxSize    = 0;
			m_defInt     = 0;
			m_required   = FALSE;
			m_export     = FALSE;
			m_minPerms   = 0; // every field is visible to every user by default
			m_access     = FALSE;
// SFString m_helpText;
			m_fieldID    = NOT_A_FIELD;
			m_resolved   = FALSE;
			m_disabled   = FALSE;
			m_hidden     = FALSE;
//	SFString m_script;
//  SFString m_problem;
//			m_selFunc    = NULL;
			m_dataPtr    = NULL;
			m_noRow      = FALSE;
			m_showHidden = TRUE;
			m_extraInt   = 0;
//  SFString m_extraStr;
	}

	void Copy(const CFieldData& data)
		{
			m_groupName = data.m_groupName;
			m_fieldName = data.m_fieldName;
			m_type      = data.m_type;
			m_editSize  = data.m_editSize;
			m_maxSize   = data.m_maxSize;
			m_prompt    = data.m_prompt;
			m_defStr    = data.m_defStr;
			m_defInt    = data.m_defInt;
			m_required  = data.m_required;
			m_minPerms  = data.m_minPerms;
			m_value     = data.m_value;
			m_export    = data.m_export;
			m_access    = data.m_access;
			m_helpText  = data.m_helpText;
			m_fieldID   = data.m_fieldID;
			m_resolved  = data.m_resolved;
			m_script    = data.m_script;
			m_disabled  = data.m_disabled;
			m_problem   = data.m_problem;
			m_hidden    = data.m_hidden;
//			m_selFunc   = data.m_selFunc;
			m_dataPtr   = data.m_dataPtr;
			m_noRow     = data.m_noRow;
			m_showHidden= data.m_showHidden;
			m_extraInt  = data.m_extraInt;
			m_extraStr  = data.m_extraStr;
		}
};

//-------------------------------------------------------------------------
class CFieldList : public SFList<CFieldData*>
{
private:
	SFString    m_group;

	// Hide these to force the use of the filtered accessors
	CFieldData *GetNext(LISTPOS& rPosition) const;
	CFieldData *GetHeadPosition(void) const;

public:
	                  CFieldList           (void) : SFList<CFieldData*>() {}
	      void        Release              (void) const;
	      LISTPOS     GetFirstItem         (const SFString& filter=nullString) const;
	      CFieldData *GetNextItem          (LISTPOS& rPosition) const;

	const CFieldData *getFieldByID         (SFInt32 id) const;
	const CFieldData *getFieldByName       (const SFString& name) const;
};

typedef SFArrayBase<CFieldData> CFieldArray;

#define TS_LABEL            (1<<10)
#define TS_DATE             (1<<12)
#define TS_ARRAY            (1<<14)
#define TS_NUMERAL          (1<<15)
#define TS_OBJECT           (1<<16)

#define T_DATE              (  4 | TS_DATE )
#define T_TIME              (  5 | TS_DATE )

#define T_TEXT              ( 10 )
#define T_BOOL              ( 12 )
#define T_NUMBER            ( 14 | TS_NUMERAL )
#define T_FLOAT             T_NUMBER
#define T_DOUBLE            T_NUMBER
#define T_QNUMBER           ( T_NUMBER | (1<<30))

//-------------------------------------------------------------------------
inline void CFieldData::setFieldID(SFInt32 id)
{
	m_fieldID = id;
}

//-------------------------------------------------------------------------
inline SFBool CFieldData::isArray(void) const
{
	return getFieldType()&TS_ARRAY;
}

//-------------------------------------------------------------------------
inline SFBool CFieldData::isObject(void) const
{
	return getFieldType()&TS_OBJECT;
}

#endif
