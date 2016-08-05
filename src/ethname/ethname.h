#ifndef _ETHNAME_H_
#define _ETHNAME_H_

#include "etherlib.h"
#include "daolib.h"
#include "name_options.h"

class CAccountName : public CAccount
{
public:
	SFString source;
	SFString name;
	CAccountName() { }
	CAccountName(SFString& nameIn)
	{
		source = nameIn;
		addr = toLower(nextTokenClear(source,'\t'));
		name = nextTokenClear(source,'\t');
	}
	SFString Format12(void)
	{
#define F(a) (SFString("`") + (SFString(#a) + "`: `" + a + "` ")).Substitute("`","\"")
		return ("{" + F(addr) + ", " + F(name) + ", " + F(source) + "}").Substitute(" ,",",");
	}
        SFBool Match(const SFString& s1, const SFString& s2, const SFString& s3, SFBool matchCase, SFBool all);
};

extern CAccountName *accounts;
extern SFInt32 nNames;
extern CFileExportContext out;
extern void loadData(void);
extern SFInt32 countOf(const SFString& addr);
extern SFString showName(const SFString& s1, const SFString& s2, const SFString& s3, SFBool match, SFBool all);
extern SFString setName(const SFString& addr, const SFString& name);

#endif
