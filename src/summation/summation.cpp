#include "etherlib.h"

//------------------------------------------------------------------------------
extern void loadData(CAccount& acct);
extern const char* STR_HEADER;
extern const char* STR_TAIL;
extern SFTime snapTime(SFInt32 which, const SFTime& tmIn);
extern SFString getFileHeader(const SFString& type, const SFString& name);

//------------------------------------------------------------------------------
SFTime start(2016,04,26,00,00,00);
SFTime first(2016,04,25,23,59,59);

//------------------------------------------------------------------------------
#define MAX_BUCKETS 365*24*2

//------------------------------------------------------------------------------
#define ACCUMULATOR(_name,_fmt,_which) \
{ \
	static SFInt32 nSlots  = -1; \
	static SFInt32 counts  [MAX_BUCKETS]; \
	static SFTime  labels  [MAX_BUCKETS]; \
	static SFTime  curTime = first; \
\
	if (nSlots == -1) \
		memset(counts,  0, sizeof(counts)); \
\
	if (!trans) \
	{ \
		SFString dataName = SFString("all_by") + SFString(_name); \
		FILE *fp = fopen(dataName+".js",asciiWriteCreate); \
\
		CFileExportContext out; \
		out.setOutput(fp?fp:stdout); \
		out << getFileHeader("Line", dataName) << "\n"; \
		for (int i=0;i<MAX_BUCKETS;i++) \
			if (counts[i]>0) \
				out << "\t\t\t\t{ period:  '" << labels[i].Format(_fmt) << "', nTransactions: " << counts[i] << "},\n"; \
		out << STR_TAIL << "\n"; \
		fprintf(stderr,"Wrote %s\n", (const char*)(dataName+".js")); \
\
	} else \
	{ \
		if (trans->m_transDate > curTime) \
		{ \
			nSlots++; \
			curTime = snapTime(_which, trans->m_transDate); \
			labels[nSlots] = curTime; \
		} \
		counts[nSlots]++; \
	} \
}

//------------------------------------------------------------------------------
void hourlyAccumulator(CTransaction *trans)
ACCUMULATOR("Hour","%#m/%#d-%#H",0);

//------------------------------------------------------------------------------
void dailyAccumulator(CTransaction *trans)
ACCUMULATOR("Day","%#m/%#d/%Y",1);

//------------------------------------------------------------------------------
void weeklyAccumulator(CTransaction *trans)
ACCUMULATOR("Week","%#m/%#d",2);

//------------------------------------------------------------------------------
void monthlyAccumulator(CTransaction *trans)
ACCUMULATOR("Month","%B",3);

//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	CTransaction::registerClass();

	CAccount theAccount;
	loadData(theAccount);

	for (int i=0;i<theAccount.transactions.getCount();i++)
	{
		CTransaction trans = theAccount.transactions[i];
		if (!trans.isError)
		{
			hourlyAccumulator (&trans);
			dailyAccumulator  (&trans);
			weeklyAccumulator (&trans);
			monthlyAccumulator(&trans);
		}
	}
	hourlyAccumulator (NULL);
	dailyAccumulator  (NULL);
	weeklyAccumulator (NULL);
	monthlyAccumulator(NULL);
}

//---------------------------------------------------------------------------------------------------
void loadData(CAccount& acct)
{
extern SFString programName;
	programName = "ethslurp";
	CTransaction::registerClass();
	
	SFString root = "/Users/jrush/.ethslurp/slurps/";
//	SFString addr = "0x7cB57B5A97eAbe94205C07890BE4c1aD31E486A8";
	SFString addr = "0xbb9bc244d798123fde783fcc1c72d3bb8c189413";
	acct.abi.loadABI(addr);
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
SFTime snapTime(SFInt32 which, const SFTime& tmIn)
{
	if (which==0)
		return EOH(tmIn);
	else if (which==1)
		return EOD(tmIn);
	else if (which==2)
		return EOW(tmIn);
	else if (which==3)
		return EOM(tmIn);
	return tmIn;
}

//---------------------------------------------------------------------------------------------------
SFString getFileHeader(const SFString& type, const SFString& name)
{
	SFString ret = STR_HEADER;
	ret.Replace("[{TYPE}]", type);
	ret.Replace("[{NAME}]", name);
	return ret;
}

//---------------------------------------------------------------------------------------------------
const char* STR_HEADER=
"$(function() {\n"
"\n"
"	Morris.[{TYPE}]({\n"
"		element: '[{NAME}]',\n"
"		data:\n"
"			[\n";

//---------------------------------------------------------------------------------------------------
const char* STR_TAIL=
"			],\n"
"		xkey: 'period',\n"
"		ykeys: ['nTransactions'],\n"
"		labels: ['nTransactions'],\n"
"		pointSize: 0,\n"
"		lineWidth: 1,\n"
"		hideHover: 'auto',\n"
"		smooth: true,\n"
"		resize: true,\n"
"		parseTime: false\n"
"	});\n"
"});\n";
