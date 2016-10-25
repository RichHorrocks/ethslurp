//-----------------------------------------------------------------------------------
#include "summation.h"

//-----------------------------------------------------------------------------------
CPriceQuoteArray quotes;

//------------------------------------------------------------------------------
const SFTime firstDate (2016,4,28,9,0,0);
const SFTime lastDate  (2016,9,1,0,0,0);

//------------------------------------------------------------------------------
const SFTime cre_end = firstDate + SFTimeSpan(30,0,0,0);
const SFTime opp_end   (2016,6,17,3,34,48);
const SFTime hck_end   (2016,7,20,1,20,40);
const SFTime rec_end = lastDate;

//------------------------------------------------------------------------------
SFString addrs[] = {
"0xbb9bc244d798123fde783fcc1c72d3bb8c189413",
"0xbf4ed7b27f1d666546e30d74d50d173d20bca754",
};
SFInt32 nAddrs = sizeof (addrs) / sizeof(SFString);

//------------------------------------------------------------------------------
extern void   loadData  (const SFString& addr, CAccount& acct);
extern SFBool loadPrices(void);

//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	for (int a=0;a<nAddrs;a++)
	{
		endOfPer = firstDate + SFTimeSpan(1,0,0,0);
		CAccount theAccount;
		loadData(addrs[a], theAccount);
		for (int i=0;i<theAccount.transactions.getCount();i++)
		{
			if (!(i%30)) { outErr << "\tCollecting results..." << i << "\r";outErr.Flush();}
			CTransaction trans = theAccount.transactions[i];
			trans.finishParse();
			accumulator(&trans);
		}
		outErr << "\n";
	}
	loadPrices();

	outErr << "\tAccumulating results...\r";
	outErr.Flush();

	CChartData chart;
	chart.objectName  = "overview";
	chart.title       = "Lifetime by Functional Group";
	chart.subTitle    = firstDate.Format(FMT_JSON) + " - " + lastDate.Format(FMT_JSON) + "<br>Price source: Poloniex.com";
	chart.yAxes       = "'Transactions by Group','ETH Price in USD'";
	chart.legend      = 0;
	chart.start       = firstDate;
	chart.stop        = lastDate;
	chart.seriesTypes = "areaspline";
	chart.stacking    = "normal";
	chart.dateFmt     = "%m-%d";
	chart.axisRange   = FALSE;
	chart.series      = "|price:1";
	groupChart(chart);
	generateTable(chart);

	chart.objectName  = "detailCreation";
	chart.title       = "Detail of Creation Period by Function";
	chart.subTitle    = firstDate.Format(FMT_JSON) + " - " + cre_end.Format(FMT_JSON);
	chart.yAxes       = "'Number of Function Calls'";
	chart.start       = firstDate;
	chart.stop        = cre_end;
	chart.colWid      = 15;
	chart.stacking    = "";
//	chart.dateFmt     = "%Y-%#m-%#d";
	chart.axisRange   = TRUE;
	chart.series      = "createTokenProxy|approve|function()";
	chart.seriesTypes = "column|areaspline|column";
	functionChart(chart);
//	chart.nameSuffix  = "_1";
//	generateTable(chart);
//	chart.nameSuffix  = "_2";
//	generateTable(chart);
//	chart.nameSuffix  = "";

	chart.objectName  = "detailOperational";
	chart.nameSuffix  = "_1";
	chart.title       = "1st Day of Period";
	chart.subTitle    = AddOneDay(cre_end).Format(FMT_JSON);
	chart.start       = BOND(cre_end);
	chart.stop        = BOND(BOND(cre_end));
	chart.colWid      = 10;
	chart.series      = "vote|transferFrom|approve|transfer";
//	chart.series      = "vote|transferFrom|approve|transfer|function()|newProposal (non-split)|newProposal (split)|transferWithoutReward|getMyReward|retrieveDAOReward";
	chart.seriesTypes = "column";
	functionChart(chart);
//	generateTable(chart);

	chart.objectName  = "detailOperational";
	chart.nameSuffix  = "_2";
	chart.title       = "Remaining Days of Operational Period - All Functions";
	chart.subTitle    = AddOneDay(AddOneDay(cre_end)).Format(FMT_JSON) + " - " + opp_end.Format(FMT_JSON);
	chart.start       = BOND(BOND(cre_end));
	chart.stop        = opp_end;
	chart.series      = "vote|transfer|transferFrom|approve|newProposal (non-split)|newProposal (split)|splitDAO";
	chart.seriesTypes = "areaspline|column|column|column|column|column|column";
	functionChart(chart);
//	generateTable(chart);

	chart.objectName  = "detailOperational";
	chart.nameSuffix  = "_3";
	chart.title       = "Voting, Proposals, and Splitting the DAO";
	chart.yAxes       = "'Number of Function Calls','Num Votes'";
	chart.series      = "vote|newProposal (non-split)|newProposal (split)|splitDAO";
	chart.seriesTypes = "areaspline|column|column|column";
	functionChart(chart);
//	generateTable(chart);

	chart.objectName  = "detailPostHack";
	chart.nameSuffix  = "";
	chart.title       = "Detail of Post Hack Period by Function";
	chart.subTitle    = opp_end.Format(FMT_JSON) + " - " + hck_end.Format(FMT_JSON);
	chart.yAxes       = "'Number of Function Calls'";
	chart.start       = opp_end;
	chart.stop        = hck_end;
	chart.series      = "newProposal (non-split)|splitDAO|newProposal (split)|vote|transferFrom|approve|transfer|function()";
	chart.seriesTypes = "column";
	functionChart(chart);
//	generateTable(chart);

	chart.objectName  = "detailRecovery";
	chart.title       = "Detail of Recovery Period by Function";
	chart.subTitle    = hck_end.Format(FMT_JSON) + " - " + rec_end.Format(FMT_JSON);
	chart.start       = hck_end;
	chart.stop        = rec_end;
	chart.colWid      = 15;
	chart.stacking    = "normal";
	chart.series      = "approve|transferFrom|transfer|withdraw";
	chart.seriesTypes = "areaspline|column|column|column";
	functionChart(chart);
//	generateTable(chart);

	outErr << "\tDone...                  \n";
}

//---------------------------------------------------------------------------------------------------
void loadData(const SFString& addr, CAccount& acct)
{
extern SFString programName;
	programName = "ethslurp";
	CTransaction::registerClass();

	outErr << "\tReading data...\r";
	outErr.Flush();

	SFString root = "/Users/jrush/.ethslurp/slurps/";
	SFString fileName = root + addr + ".bin";

	acct.abi.loadABI(addr);
	SFArchive archive(TRUE, NO_SCHEMA);
	if (archive.Lock(fileName, binaryReadOnly, LOCK_NOWAIT))
	{
		acct.Serialize(archive);
		archive.Close(); // already sorted
	}
}

//-----------------------------------------------------------------------------------
SFBool loadPrices(void)
{
	SFString cacheFile = cachePath("prices.bin").Substitute("/slurps/","/");
	if (!SFos::fileExists(cacheFile))
	{
		outErr << "Cache folder '" << cacheFile << "' does not exist. Quitting...\n";
		exit(0);
	}
	
	SFArchive archive(TRUE, NO_SCHEMA);
	if (!archive.Lock(cacheFile, binaryReadOnly, LOCK_NOWAIT))
	{
		outErr << "Could not open price database '" << cacheFile << "'. Quitting...\n";
		exit(0);
	}
	
	SFTime  lastRead;
	SFInt32 freq;
	
	archive >> freq;
	archive >> lastRead;
	archive >> quotes;
	archive.Close();
	
	//	outErr << "\tRead " << quotes.getCount() << " existing price quotes (lastRead: " << lastRead << ")\n";
	return TRUE;
}

//---------------------------------------------------------------------------------------------------
CParams params[]={};
SFInt32 nParams = sizeof(params) / sizeof(CParams);
