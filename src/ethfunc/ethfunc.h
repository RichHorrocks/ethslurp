#ifndef _ETHABI_H_
#define _ETHABI_H_

#include "etherlib.h"
#include "daolib.h"

class CAccountAbi : public CAccount
{
public:
	SFString source;
	SFString abi;
	CAccountAbi() { }
	CAccountAbi(SFString& abiIn)
	{
		source = abiIn;
		addr = toLower(nextTokenClear(source,'\t'));
		abi = nextTokenClear(source,'\t');
	}
	SFString Format12(void)
	{
#define F(a) (SFString("`") + (SFString(#a) + "`: `" + a + "` ")).Substitute("`","\"")
		return ("{" + F(addr) + ", " + F(abi) + ", " + F(source) + "}").Substitute(" ,",",");
	}
        SFBool Match(const SFString& s1, const SFString& s2, const SFString& s3, SFBool matchCase, SFBool all);
};

extern CAccountAbi *accounts;
extern SFInt32 nAbis;
extern CFileExportContext out;
extern void loadData(void);

#define DATA_PATH SFString("/Users/jrush/.ethslurp/abis/*.json")

#endif
