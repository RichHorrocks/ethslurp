#include "summation.h"

//------------------------------------------------------------------------------
//SFString addr = "0x7cB57B5A97eAbe94205C07890BE4c1aD31E486A8";
SFString addr = "0xbb9bc244d798123fde783fcc1c72d3bb8c189413";

//------------------------------------------------------------------------------
SFTime start(2015,01,01,00,00,00);
SFTime stop (2016,12,31,23,59,59);

//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	CTransaction::registerClass();

	CAccount theAccount;
	loadData(theAccount);

	for (int i=0;i<theAccount.transactions.getCount();i++)
	{
		CTransaction trans = theAccount.transactions[i];
		if (!trans.isError && (trans.m_transDate >= start && trans.m_transDate <= stop))
		{
			trans.finishParse();
			dailyAccumulator       (&trans);
			dailyFuncAccumulator   (&trans);
			creationFuncAccumulator(&trans);
			accountAccumulator     (&trans);
		}
	}
	dailyAccumulator       (NULL);
	dailyFuncAccumulator   (NULL);
	creationFuncAccumulator(NULL);
	accountAccumulator     (NULL);
}

//---------------------------------------------------------------------------------------------------
void loadData(CAccount& acct)
{
	CFileExportContext errOut(stderr);
extern SFString programName;
	programName = "ethslurp";
	CTransaction::registerClass();

	SFString root = "/Users/jrush/.ethslurp/slurps/";
	acct.abi.loadABI(addr);

	errOut << "\tReading data...\n";errOut.Flush();
	SFArchive archive(TRUE, NO_SCHEMA);
	if (archive.Lock(root + addr + ".bin", binaryReadOnly, LOCK_NOWAIT))
	{
		acct.Serialize(archive);
		archive.Close(); // already sorted
	}
}

//---------------------------------------------------------------------------------------------------
CParams params[]={};
SFInt32 nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
SFString getFileHeader(const SFString& header, const SFString& type, const SFString& name)
{
	SFString ret = header;
	ret.Replace("[{TYPE}]", type);
	ret.Replace("[{NAME}]", name);
	return ret;
}
