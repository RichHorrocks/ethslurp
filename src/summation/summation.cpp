#include "summation.h"
#include "funcMap.h"
#include "options.h"
#include "accumulator.h"

//-----------------------------------------------------------------------------------
CPriceQuoteArray quotes;

//------------------------------------------------------------------------------
const SFTime firstDate(2016,4,28,9,0,0);
const SFTime lastDate (2016,9,1,0,0,0);

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
	chart.legend      = TRUE;
	chart.byGroup     = TRUE;
	chart.start       = firstDate;
	chart.stop        = lastDate;
	chart.seriesTypes = "areaspline";
	generateChart(chart);
	generateTable(chart);

	chart.objectName  = "detailCreation";
	chart.title       = "Detail of Creation Period by Function";
	chart.subTitle    = firstDate.Format(FMT_JSON) + " - " + cre_end.Format(FMT_JSON);
	chart.yAxes       = "'Number of Function Calls'";
	chart.legend      = FALSE;
	chart.byGroup     = FALSE;
	chart.start       = firstDate;
	chart.stop        = cre_end;
	chart.colWid      = 15;
	chart.showPrice   = FALSE;
	chart.seriesTypes = "column|areaspline|column";
	generateChart(chart);
	chart.nameSuffix  = "_1";
//	generateTable(chart);
	chart.nameSuffix  = "_2";
//	generateTable(chart);
	chart.nameSuffix  = "";

	chart.objectName  = "detailOperational";
	chart.nameSuffix  = "_1";
	chart.title       = "1st Day of Period";
	chart.subTitle    = AddOneDay(cre_end).Format(FMT_JSON);
	chart.start       = BOND(cre_end);
	chart.stop        = BOND(BOND(cre_end));
	chart.colWid      = 10;
	chart.seriesTypes = "column";
	generateChart(chart);
//	generateTable(chart);

	chart.objectName  = "detailOperational";
	chart.nameSuffix  = "_2";
	chart.title       = "Remaining Days of Operational Period All Functions";
	chart.subTitle    = AddOneDay(AddOneDay(cre_end)).Format(FMT_JSON) + " - " + opp_end.Format(FMT_JSON);
	chart.start       = BOND(BOND(cre_end));
	chart.stop        = opp_end;
	setSortOrder        ("vote|transfer|transferFrom|approve|newProposal (non-split)|new Proposal (split),splitDAO");
	chart.seriesTypes = "areaspline|column|column|column|column|column|column";
	generateChart(chart);
//	generateTable(chart);

	chart.objectName  = "detailPostHack";
	chart.nameSuffix  = "";
	chart.title       = "Detail of Post Hack Period by Function";
	chart.subTitle    = opp_end.Format(FMT_JSON) + " - " + hck_end.Format(FMT_JSON);
	chart.start       = opp_end;
	chart.stop        = hck_end;
	chart.seriesTypes = "column";
	generateChart(chart);
//	generateTable(chart);

	chart.objectName  = "detailRecovery";
	chart.title       = "Detail of Recovery Period by Function";
	chart.subTitle    = hck_end.Format(FMT_JSON) + " - " + rec_end.Format(FMT_JSON);
	chart.start       = hck_end;
	chart.stop        = rec_end;
	chart.colWid      = 15;
	setSortOrder        ("newProposal (split)|vote|transferFrom|approve|transfer|withdraw|function()");
	chart.seriesTypes = "areaspline|column|column|column|column|column|column";
	generateChart(chart);
//	generateTable(chart);

	outErr << "\tDone...                  \n";
}

//---------------------------------------------------------------------------------------------------
void loadData(const SFString& addr, CAccount& acct)
{
extern SFString programName;
	programName = "ethslurp";
	CTransaction::registerClass();

	SFString root = "/Users/jrush/.ethslurp/slurps/";
	acct.abi.loadABI(addr);

	outErr << "\tReading data...\r";
	outErr.Flush();
	SFArchive archive(TRUE, NO_SCHEMA);

	SFString fileName = root + addr + ".bin";
//	if (addr.Contains("0xbb9"))
//		fileName = "./bin/Creation.bin";
	if (archive.Lock(fileName, binaryReadOnly, LOCK_NOWAIT))
	{
		acct.Serialize(archive);
		archive.Close(); // already sorted
	}
}

//---------------------------------------------------------------------------------------------------
CParams params[]={};
SFInt32 nParams = sizeof(params) / sizeof(CParams);
