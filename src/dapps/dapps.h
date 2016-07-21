#ifndef _ETHNAME_H_
#define _ETHNAME_H_

#include "abilib.h"
#include "etherlib.h"
#include "dapps_options.h"

#if 0
class CAccountName : public CAccount
{
public:
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
};
extern CAccountName *accounts;
extern SFInt32 nNames;
#endif

extern CFileExportContext out;
extern void loadData(void);

#if 0
extern SFInt32 countOf(const SFString& addr);
extern SFString showName(const SFString& s1, const SFString& s2, const SFString& s3, SFBool match, SFBool all);
extern SFString setName(const SFString& addr, const SFString& name);
#endif

#endif
