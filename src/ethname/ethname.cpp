#include "ethname.h"

//-----------------------------------------------------------------------
int main(int argc, const char *argv[])
{
	CNameOptions options;
	if (!options.prepareArguments(argc, argv))
		return 0;

	loadData();
	while (!options.commandList.IsEmpty())
	{
		SFString command = nextTokenClear(options.commandList, '\n');
		options.parseArguments(command);
		if (options.list)
		{
			for (int i=0;i<nNames;i++)
				out << accounts[i].Format12() << "\n";
			exit(0);
		}
		out << showName(options.addr, options.name, options.source, options.matchCase, options.all);
		if (options.write)
			out << "Writing is not implemented.\n";
		out.Flush();
	}
	if (accounts)
		delete [] accounts;

	return 0;
}

//-----------------------------------------------------------------------
void loadData(void)
{
	if (nNames>0||accounts!=NULL)
		return;

	SFString contents = asciiFileToString(DATA_FILE);

	SFInt32 nLines = countOf('\n',contents);
	accounts = new CAccountName[nLines];
	if (!accounts)
	{
		outErr << "Could not allocate memory for " << nLines << " account names. Quitting...\n";
		exit(0);
	}

	while (!contents.IsEmpty())
	{
		SFString line = nextTokenClear(contents,'\n');
		if (!countOf('\t',line))
			outErr << "Line " << line << " does not contain two tabs.\n";
		accounts[nNames++] = CAccountName(line);
	}
}

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

//-----------------------------------------------------------------------
CFileExportContext out;

//---------------------------------------------------------------------------
SFBool CAccountName::Match(const SFString& s1, const SFString& s2, const SFString& s3, SFBool matchCase, SFBool all)
{
	SFBool m11 = ( matchCase ?   addr.Contains(s1) :   addr.ContainsI(s1) );
	SFBool m12 = ( matchCase ?   name.Contains(s1) :   name.ContainsI(s1) );
	SFBool m13 = ( matchCase ? source.Contains(s1) : source.ContainsI(s1) );
	SFBool m2  = ( matchCase ?   name.Contains(s2) :   name.ContainsI(s2) );
	SFBool m3  = ( matchCase ? source.Contains(s3) : source.ContainsI(s3) );

	if (!s1.IsEmpty() && !s2.IsEmpty() && !s3.IsEmpty())
		return m11 && m2 && m3; // all three must match
	
	if (!s1.IsEmpty() && !s2.IsEmpty())
		return m11 && m2; // addr and name must both match

	if (s1.IsEmpty())
		return FALSE; // nothing matches

	// We have only s1
	return (all ? m11 || m12 || m13 : m11 || m12);
}

