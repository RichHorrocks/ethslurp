#include "ethfunc.h"

//-----------------------------------------------------------------------
int main(int argc, const char *argv[])
{
	loadData();
	if (accounts)
	{
		for (int i=0;i<nAbis;i++)
			out << accounts[i].Format() << "\n";
		delete [] accounts;
	}
	return 0;
}

//-----------------------------------------------------------------------
void loadData(void)
{
	SFInt32 nFiles=0;
	SFos::listFiles(nFiles, NULL, DATA_PATH);
	if (nFiles)
	{
		SFString *files = new SFString[nFiles];
		SFos::listFiles(nFiles, files, DATA_PATH);
		for (int i=0;i<nFiles;i++)
			out << files[i] << "\n";
		delete [] files;
	}
}

//-----------------------------------------------------------------------
CAccountAbi *accounts=NULL;
SFInt32 nAbis=0;

CParams params[] =
{
        CParams( "", "xxx.\n" ),
};
SFInt32 nParams = sizeof(params) / sizeof(CParams);

CFileExportContext out;

