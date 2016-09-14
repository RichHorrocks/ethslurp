#ifndef _CHARTS_H
#define _CHARTS_H

//-----------------------------------------------------------
#include "etherlib.h"
#include "funcMap.h"

//-----------------------------------------------------------
class CChartData
{
public:
	SFString objectName;
	SFString nameSuffix;
	SFString title;
	SFString subTitle;
	SFString yAxes;
	SFString seriesTypes;
	SFString dateFmt;
	SFString series;
	SFBool   legend;
	SFBool   axisRange;
	SFBool   byGroup;
	SFTime   start;
	SFTime   stop;
	SFInt32  colWid;
	SFString stacking;
	SFInt32  nSeries;
	SFString seriesNames[FNC_LAST-FNC_FIRST+1];

	         CChartData      ( void ) { legend=byGroup=FALSE; start=stop=earliestDate; axisRange=TRUE; colWid=10; dateFmt="%Y-%#m-%#d";nSeries=0; }

	SFBool   showPrice       ( void ) const { SFString l = series;l.ReplaceAll(":","|");return l.ContainsExact("price",'|'); }
	void     sortFunctionMap ( void ) const;
	SFString getLegend       ( void ) const;

	SFString getNames        ( void ) const;
	SFString getSeries       ( void ) const;
	SFString getSuffixes     ( void ) const;
	SFString getTypes        ( void ) const;

	SFString getDateLine     ( SFInt32& firstBucket, SFInt32& lastBucket) const;
	SFString getPriceLine    ( const SFTime& start, const SFTime& end, SFInt32 hour) const;

	SFString resolveChart    ( const SFString& data, const SFString& dates, SFInt32 nAxes, SFInt32 largest[], SFInt32 smallest[]) const;
};

//------------------------------------------------------------------------------
inline int sortBySortOrder(const void *v1, const void *v2)
{
	CFunctionMap *f1 = (CFunctionMap*)v1;
	CFunctionMap *f2 = (CFunctionMap*)v2;
	SFInt32 ret = f1->sortOrder - f2->sortOrder;
	if (ret)
		return (int)ret;
	return (int)(f1->funcID - f2->funcID);
}

//------------------------------------------------------------------------------
extern const char* STR_THECHART;
extern const char* STR_FUNC_TABLE;
extern const char* STR_FUNC_ROW;
extern const char* STR_CHART_AXES;
extern const char* STR_CHART_LEGEND;
extern const char* STR_CHART_SERIES_1;
extern const char* STR_CHART_SERIES_2;
extern const char* STR_HORIZ;
extern const char* STR_VERT;

//-----------------------------------------------------------------------------------
extern CPriceQuoteArray quotes;

#endif
