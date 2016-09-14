//------------------------------------------------------------------------------
#include "accumulator.h"

//------------------------------------------------------------------------------
extern SFInt32 getBucket     (const SFTime& timeIn, SFInt32& nBuckets);
extern SFInt32 getPeriodIndex(const SFTime& timeIn);

//------------------------------------------------------------------------------
SFInt32 byFuncGrpPerDay    [N_COUNTERS][MAX_BUCKETS];
SFInt32 byFuncNamePerDay   [N_COUNTERS][MAX_BUCKETS];
SFInt32 byFuncNamePerPeriod[N_COUNTERS][MAX_BUCKETS];
SFTime  labels                         [MAX_BUCKETS];

//------------------------------------------------------------------------------
SFInt32 curDay;
SFInt32 nBuckets=0;

//------------------------------------------------------------------------------
SFTime endOfPer;

//------------------------------------------------------------------------------
void accumulator(CTransaction *trans, SFBool byDay)
{
	if (trans->isError)
		return;

	static SFBool initialized=FALSE;
	if (!initialized)
	{
		memset(byFuncGrpPerDay,      0, sizeof(byFuncGrpPerDay));
		memset(byFuncNamePerDay,     0, sizeof(byFuncNamePerDay));
		memset(byFuncNamePerPeriod,  0, sizeof(byFuncNamePerPeriod));
		initialized = TRUE;
	}

	while (trans->m_transDate > endOfPer)
	{
		endOfPer = AddOneDay(endOfPer);
		curDay   = getBucket(endOfPer, nBuckets);
		labels[curDay] = endOfPer;
	}

	SFString func = trans->function;
	func = nextTokenClear(func,'|');

	// counters for functional group by day
	byFuncGrpPerDay[getGroupIndex(func)][curDay]++;
	byFuncGrpPerDay[getGroupIndex(func)][MAX_BUCKETS-1]++; // store the total

	// counters for functional name by day
	byFuncNamePerDay[getFunctionIndex(func)-FNC_FIRST][curDay]++;
	byFuncNamePerDay[getFunctionIndex(func)-FNC_FIRST][MAX_BUCKETS-1]++; // store the total

	// counters for function by time period
	byFuncNamePerPeriod[getFunctionIndex(func)][getPeriodIndex(endOfPer)]++;
}

//------------------------------------------------------------------------------
void groupChart(const CChartData& chart)
{
	SFInt32 nAxes = 2;
	SFInt32 nSeries=0;
	SFInt32  start = GRP_FIRST;
	SFInt32  end   = GRP_LAST;

	SFInt32 firstBucket=1000000,lastBucket=-1;
	SFString dateLine = chart.getDateLine(firstBucket, lastBucket);

	SFInt32 largest[2]={0,25};
	SFInt32 smallest[2]={0,5};
	CStringExportContext dataCtx;

	for (SFInt32 i=start;i<end;i++) // we do not show OTHER group
	{
		if (i==GRP_OTHER || i==GRP_ADMIN)
			continue;

		if (byFuncGrpPerDay[i][MAX_BUCKETS-1] > 25)
		{ // only show functions with non-zero total
			SFString name = getGroupName(i);
			dataCtx << "\t/" << "/ " << name << "\n";
			dataCtx << "\t[ ";
			for (SFInt32 j=firstBucket;j<lastBucket;j++)
			{
				SFInt32 value = byFuncGrpPerDay[i][j];
				smallest[0]=MIN(smallest[0],value);
				largest[0]=MAX(largest[0],value);
				dataCtx << value << ",";
			}
			dataCtx << " ],\n\n";
			((CChartData*)&chart)->seriesNames[nSeries++] = name; // order matters
		}
	}
	dataCtx << chart.getPriceLine(labels[firstBucket], labels[lastBucket], 0);
	*((SFInt32*)&chart.nSeries)=nSeries;

	SFString output = chart.resolveChart(dataCtx.str, dateLine, nAxes, largest, smallest);
	stringToAsciiFile(chart.objectName+"Chart"+chart.nameSuffix+".js", output);
}

//------------------------------------------------------------------------------
void functionChart(const CChartData& chart)
{
	chart.sortFunctionMap();
	SFInt32 nAxes = countOf(':',chart.series)+1;

	SFInt32  nSeries=0;

	SFInt32 firstBucket=1000000,lastBucket=-1;
	SFString dateLine = chart.getDateLine(firstBucket, lastBucket);

	SFInt32 largest[2]={0,25};
	SFInt32 smallest[2]={0,5};
	CStringExportContext dataCtx;

	// Now we create the function table
	CAccumulator *accums = new CAccumulator[(FNC_LAST-FNC_FIRST)*2]; // More than we need
	if (accums)
	{
		SFInt32 nAccums=0;
		for (SFInt32 i=0;i<FNC_LAST-FNC_FIRST;i++)
		{
			accums[nAccums].map = &funcMap[i];
			for (SFInt32 j=firstBucket;j<lastBucket;j++)
			{
				SFInt32 index = funcMap[i].funcID;
				index = getFunctionIndex(funcMap[i].funcName)-FNC_FIRST;
				SFInt32 value = byFuncNamePerDay[index][j];
				accums[nAccums].values[j-firstBucket+1] = value;
				accums[nAccums].values[IN_TOTO]        += value;
			}
			nAccums++;
		}

		for (int i=0;i<nAccums;i++)
		{
			if (accums[i].map->showing)
			{
				SFString name = accums[i].map->funcName;
				dataCtx << "\t/" << "/ " << name << "\n";
				dataCtx << "\t[ ";
				for (SFInt32 j=firstBucket;j<lastBucket;j++)
				{
					SFInt32 idex = j-firstBucket+1;
					SFInt32 value = accums[i].values[idex];
					smallest[0]=MIN(smallest[0],value);
					largest[0]=MAX(largest[0],value);
					dataCtx << value << ",";
				}
				dataCtx << " ],\n\n";
				((CChartData*)&chart)->seriesNames[nSeries++] = name; // order matters
			}
		}
		delete [] accums;
	}
	dataCtx << chart.getPriceLine(labels[firstBucket], labels[lastBucket], 0);
	*((SFInt32*)&chart.nSeries)=nSeries;

	SFString output = chart.resolveChart(dataCtx.str, dateLine, nAxes, largest, smallest);
	stringToAsciiFile(chart.objectName+"Chart"+chart.nameSuffix+".js", output);
}

//------------------------------------------------------------------------------
SFInt32 getBucket(const SFTime& timeIn, SFInt32& nBuckets)
{
	SFTime t  = timeIn;
	SFTime fD = firstDate;
	SFInt32 nBucket = ( t - fD ).getTotalDays();
	nBuckets = MAX(nBuckets,nBucket+1);
	return nBucket;
}

//------------------------------------------------------------------------------
SFInt32 getPeriodIndex(const SFTime& timeIn)
{
		 if ( isInRange( timeIn, earliestDate, cre_end) ) return PER_CREATION;
	else if ( isInRange( timeIn, cre_end,      opp_end) ) return PER_OPERATIONAL;
	else if ( isInRange( timeIn, opp_end,      hck_end) ) return PER_POSTHACK;
	return PER_RECOVERY;
}

//------------------------------------------------------------------------------
void generateTable(const CChartData& chart)
{
	// Now we create the function table
	CAccumulator accums[(FNC_LAST-FNC_FIRST)*2]; // More than we need
	SFInt32 nAccums=0;
	for (int i=FNC_FIRST;i<FNC_LAST;i++)
	{
		accums[nAccums].map = findByFuncID(i);
		for (int j=PER_FIRST;j<PER_LAST;j++)
		{
			accums[nAccums].values[j-PER_FIRST+1] = byFuncNamePerPeriod[i][j];
			accums[nAccums].values[IN_TOTO]      += byFuncNamePerPeriod[i][j];
		}
		nAccums++;
	}
	qsort(accums, nAccums, sizeof(CAccumulator), sortByFrequency);
	
	SFString rows;
	for (int i=0;i<nAccums;i++)
	{
		SFString row = STR_FUNC_ROW;
		row.Replace("{T}",    (!(i%2)?"even":"odd"));
		row.Replace("{ORD}",   asString(i+1));
		row.Replace("{GRP}",   accums[i].map->groupName);
		row.Replace("{FNC}",   accums[i].map->funcName);
		row.Replace("{CNT}",   asDelimited(accums[i].values[IN_TOTO]));
		row.Replace("{C_CNT}", asDelimited(accums[i].values[1]));
		row.Replace("{O_CNT}", asDelimited(accums[i].values[2]));
		row.Replace("{P_CNT}", asDelimited(accums[i].values[3]));
		row.Replace("{R_CNT}", asDelimited(accums[i].values[4]));
		rows += row;
	}
	
	SFString output = STR_FUNC_TABLE;
	output.Replace("[{ROWS}]",rows);
	stringToAsciiFile(chart.objectName+"Table.jade", output);
}
