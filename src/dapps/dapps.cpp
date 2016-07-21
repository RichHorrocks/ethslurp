#include "dapps.h"

//-----------------------------------------------------------------------
CDapp *dapps;
SFInt32 nDapps=0;

//-----------------------------------------------------------------------
int main(int argc, const char *argv[])
{
	loadData();
#if 0
	CNameOptions options;

	if (!options.prepareArguments(argc, argv))
		return 0;

	loadData();
	while (!options.commandList.IsEmpty())
	{
		SFString command = nextTokenClear(options.commandList, '\n');
		options.parseArguments(command);
		out << showName(options.addr, options.name, options.source, options.matchCase, options.all);
		if (options.write)
			out << "Writing is not implemented.\n";
		out.Flush();
	}
#endif
	if (dapps)
		delete [] dapps;
	return 0;
}

//-----------------------------------------------------------------------
void loadData(void)
{
	if (nDapps>0||dapps!=NULL)
		return;

	SFString contents = asciiFileToString(DATA_FILE);
	SFInt32 nLines = countOf('\n',contents);
	dapps = new CDapp[nLines];
	if (!dapps)
	{
		outErr << "Could not allocate memory for " << nLines << " dapps. Quitting...\n";
		exit(0);
	}

	SFString fields[] = { "dappName", "description", "site", "gitHub", "reddit", "people", "tags", "license", "status", "last_update", };
	SFInt32 nFields = sizeof(fields) / sizeof(SFString);
	while (!contents.IsEmpty())
	{
		SFString line = nextTokenClear(contents,'\n');
		if (!countOf('\t',line))
			outErr << "Line " << line << " does not contain two tabs.\n";
		dapps[nDapps++].parseText(line, nFields, fields);
	}
}

#if 0
//-----------------------------------------------------------------------
SFInt32 countOf(const SFString& addr)
{
	SFInt32 cnt=0;
	for (int i=0;i<nNames;i++)
		if (accounts[i].addr % addr)
			cnt++;
	return cnt;
}

//-----------------------------------------------------------------------
SFString showName(const SFString& s1, const SFString& s2, const SFString& s3, SFBool matchCase, SFBool all)
{
	SFString ret;
	for (int i=0;i<nNames;i++)
		if (accounts[i].Match(s1, s2, s3, matchCase, all))
			ret += (accounts[i].Format12()+"\n");
	return ret;
}

//-----------------------------------------------------------------------
SFString setName(const SFString& addr, const SFString& name)
{
	CStringExportContext ctx;
	ctx << "setName(" << addr << "," << name << ");\n";
	return ctx.str;
}

//-----------------------------------------------------------------------
CAccountName *accounts=NULL;
SFInt32 nNames=0;
#endif

//-----------------------------------------------------------------------
CFileExportContext out;

