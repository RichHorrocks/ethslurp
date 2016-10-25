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
	else if (            timeIn < rec_end               ) return PER_RECOVERY;
	return PER_OVERTIME;
}
