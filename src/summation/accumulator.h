#ifndef _H_ACCUMLATOR_
#define _H_ACCUMLATOR_

//-----------------------------------------------------------
#include "tables.h"
#include "charts.h"
#include "funcMap.h"

//-----------------------------------------------------------
class CAccumulator
{
public:
	const CFunctionMap *map;
	      SFInt32       values[MAX_BUCKETS];

	CAccumulator(void) { memset(values,0,sizeof(values)); map = NULL; }
};

//-----------------------------------------------------------
inline int sortByFrequency(const void *v1, const void *v2)
{
	CAccumulator *a1 = (CAccumulator*)v1;
	CAccumulator *a2 = (CAccumulator*)v2;
	SFInt32 ret = a2->values[0] - a1->values[0];
	if (ret)
		return (int)ret;
	ret = a1->map->groupName.Compare(a2->map->groupName);
	if (ret)
		return (int)ret;
	return (int)a1->map->funcName.Compare(a2->map->funcName);
}

//------------------------------------------------------------------------------
extern const SFTime firstDate;
extern const SFTime lastDate;
extern const SFTime cre_end;
extern const SFTime opp_end;
extern const SFTime hck_end;
extern const SFTime rec_end;

//------------------------------------------------------------------------------
extern SFTime endOfPer;

//------------------------------------------------------------------------------
extern void accumulator   ( CTransaction *trans, SFBool byDay=TRUE );
extern void groupChart    ( const CChartData& chart );
extern void functionChart ( const CChartData& chart );
extern void generateTable ( const CChartData& chart );

#endif
