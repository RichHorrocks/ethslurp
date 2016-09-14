//------------------------------------------------------------------------------
#include "charts.h"

//------------------------------------------------------------------------------
extern SFInt32 nBuckets;
extern SFTime  labels[];

//------------------------------------------------------------------------------
extern SFString getAxis( SFInt32 id, SFBool opp, SFInt32 min, SFInt32 max, SFInt32 step, SFBool show );

//------------------------------------------------------------------------------
SFString CChartData::resolveChart(const SFString& data, const SFString& dates, SFInt32 nAxes, SFInt32 largest[], SFInt32 smallest[]) const
{
#define nearest200(a) (int)(((int)(((a)+(2*200))/200))*200)
	SFString axes;
	for (int i=0;i<nAxes;i++)
	{
		SFInt32 max = nearest200(largest[i]);
		if (i>0) max = largest[i];
		axes += getAxis(i>0?1:0,i>0,smallest[i],max,max/4,axisRange);
	}
	//	if (chart.showPrice())
	//		axes += getAxis(1,true,5,25,5,chart.axisRange); // mulitple of four, so above division results in matching yAxis markers
	
	SFString output = STR_THECHART;
	output.Replace   ("[DATE_ARRAY]",   dates);
	output.Replace   ("[VALUE_ARRAYS]", data);
	output.Replace   ("[PT_WID]",       asString(colWid));
	output.Replace   ("[OBJECT_NAME]",  objectName);
	output.Replace   ("[CHART_TITLE]",  title);
	output.Replace   ("[CHART_SUBTIT]", subTitle);
	output.Replace   ("[Y_AXES]",       yAxes);
	output.Replace   ("[LEGEND]",       getLegend());
	output.Replace   ("[SERIES_NAMES]", getNames());
	output.Replace   ("[SERIES]",       getSeries());
	output.Replace   ("[SUFIXES]",      getSuffixes());
	output.Replace   ("[TYPES]",        getTypes());
	output.Replace   ("[AXES]",         axes);
	output.Replace   ("[STACKING]",     stacking.IsEmpty()?"":"stacking:'"+stacking+"',");
	output.ReplaceAll("{S}",            nameSuffix);
	return output;
}

//------------------------------------------------------------------------------
void CChartData::sortFunctionMap(void) const
{
	SFString list = series;
	list.Replace("|price:0","");
	list.Replace("|price","");
	list.Replace("|price:1","");
	
	// set them all so we know which ones were set explicitly
	for (int i=0;i<nItems;i++)
	{
		funcMap[i].sortOrder = -1;
		funcMap[i].showing = FALSE;
	}
	
	// now, assign the ones the user wants to assign
	SFInt32 cnt=0;
	while (!list.IsEmpty())
	{
		SFString name = nextTokenClear(list,'|');
		CFunctionMap *item = findByFuncName(name);
		if (item)
		{
			item->sortOrder = cnt++;
			item->showing = TRUE;
		}
	}
	
	// now, set each unassigned sortOrder to not less than the last sorted item
	for (int i=0;i<nItems;i++)
	{
		if (funcMap[i].sortOrder==-1)
		{
			funcMap[i].sortOrder = cnt++;
			funcMap[i].showing = FALSE;
		}
	}
	
	// now, sort it
	qsort(funcMap, nItems, sizeof(CFunctionMap), sortBySortOrder);
}

//------------------------------------------------------------------------------
SFString CChartData::getDateLine(SFInt32& firstBucket, SFInt32& lastBucket) const
{
	CStringExportContext dateCtx;
	dateCtx << "\t[ ";
	for (int i=0;i<nBuckets;i++)
	{
		if (isInRange(labels[i],start,stop))
		{
			firstBucket = MIN(i,firstBucket);
			lastBucket =  MAX(i,lastBucket);
			dateCtx << "'" << labels[i].Format(dateFmt) << "',";
		}
	}
	dateCtx << " ],";
	
	// otherwise, we miss one
	lastBucket++;
	return dateCtx.str;
}

//------------------------------------------------------------------------------
SFString CChartData::getPriceLine(const SFTime& start, const SFTime& end, SFInt32 hour) const
{
	if (!showPrice())
		return "";

	CStringExportContext priceCtx;
	priceCtx << "\t/" << "/ Price of ether in dollars\n";
	priceCtx << "\t[ ";
	for (int i=0;i<quotes.getCount();i++)
	{
		if (isInRange(quotes[i].date, start, end))
		{
			if (quotes[i].date.GetHour() == hour && quotes[i].date.GetMinute() == 0)
			{
				priceCtx << quotes[i].Format("[{CLOSE}],");
			}
		}
	}
	priceCtx << " ],\n";
	return priceCtx.str;
}

//------------------------------------------------------------------------------
SFString CChartData::getLegend(void) const
{
	return (legend?STR_CHART_LEGEND:"");
}

//------------------------------------------------------------------------------
SFString CChartData::getNames(void) const
{
	SFString ret;
	for (int i=0;i<nSeries;i++)
		ret += ("'" + seriesNames[i] + "',");
	if (showPrice())
		ret += ("'ETH Price (USD)',");
	return ret;
}

//---------------------------------------------------------------------------------------------------
SFString CChartData::getSeries(void) const
{
	SFString ret;
	// produce JS code for nSeries plus one for the price line
	for (int i=0;i<nSeries;i++)
		ret += SFString(STR_CHART_SERIES_1).Substitute("{N}",asString(i));
	if (showPrice())
		ret += SFString(STR_CHART_SERIES_2).Substitute("{N}",asString(nSeries));
	return ret + "\n";
}

//---------------------------------------------------------------------------------------------------
SFString CChartData::getSuffixes(void) const
{
	SFString ret;
	for (int i=0;i<nSeries;i++)
		ret += "'',";
	if (showPrice())
		ret += "'USD',";
	return ret;
}

//---------------------------------------------------------------------------------------------------
SFString CChartData::getTypes(void) const
{
	SFString types=seriesTypes;
	if (!seriesTypes.Contains("|")) // do we have to build it?
	{
		for (int i=0;i<nSeries;i++)
			types += "|"+seriesTypes;
	}

	SFString ret;
	for (int i=0;i<nSeries;i++)
		ret += "'" + nextTokenClear(types,'|') + "',";
	if (showPrice())
		ret += "'spline',";
	return ret;
}

//---------------------------------------------------------------------------------------------------
SFString getAxis(SFInt32 id, SFBool opp, SFInt32 min, SFInt32 max, SFInt32 step, SFBool range)
{
	SFString ret = STR_CHART_AXES;
	ret.ReplaceAll("{N}",        asString(id));
	ret.ReplaceAll("[{OPP}]",    (opp?"true":"false"));
	ret.ReplaceAll("[{BOUNDS}]", (range ? ",endOnTick:false,min:"+asString(min)+",max:"+asString(max)+",tickInterval:"+asString(step) : ""));
	return ret;
}

//------------------------------------------------------------------------------
const char *STR_THECHART=
"var objectName{S}    =   '#[OBJECT_NAME]Chart{S}';\n"
"var chartTitle{S}    =   '[CHART_TITLE]';\n"
"var chartSubTitle{S} =   '[CHART_SUBTIT]';\n"
"// One per axis\n"
"var yAxisTitles{S}   = [ [Y_AXES] ];\n"
"\n"
"// One per series\n"
"var seriesNames{S}   = [ [SERIES_NAMES] ];\n"
"var suffix{S}        = [ [SUFIXES] ];\n"
"var type{S}          = [ [TYPES] ];\n"
"\n"
"// X values\n"
"var xValues{S}       = [\n"
"[DATE_ARRAY]\n"
"	];\n"
"\n"
"// Y values, one per series\n"
"var yValues{S} = [\n"
"[VALUE_ARRAYS]\n"
"	];\n"
"\n"
"$(function () {\n"
"    $(objectName{S}).highcharts({\n"
"		plotOptions: {\n"
"			areaspline: { lineColor:'#666666',lineWidth:1,marker:{enabled:false,} },\n"
"			column:     { pointWidth:[PT_WID] },\n"
"			spline:     { color:'blue',lineWidth:1,marker:{enabled:false,} },\n"
"			series:     { [STACKING]animation: {duration: 400,} }\n"
"		},\n"
"		chart:       {zoomType:'xy'},\n"
"		title:       {text:chartTitle{S}},\n"
"		subtitle:    {text:chartSubTitle{S}},\n"
"		xAxis:      [{categories:xValues{S}[0],crosshair:true,}],\n"
"        yAxis:      [\n"
"[AXES]"
"		],\n"
"        series:     [\n"
"[SERIES]"
"		],\n"
"[LEGEND]"
"		tooltip:{shared:true},\n"
"    });\n"
"});\n"
"var yColors= Highcharts.getOptions().colors;\n";

//------------------------------------------------------------------------------
const char* STR_CHART_LEGEND=
"\t\tlegend:{\n"
"\t\t\tlayout:'vertical',\n"
"\t\t\tx:110,align:'left',\n"
"\t\t\ty:60,verticalAlign:'top',\n"
"\t\t\tfloating:true,\n"
"\t\t\tbackgroundColor:(Highcharts.theme && Highcharts.theme.legendBackgroundColor) || '#FFFFFF'\n"
"\t\t},\n";

//------------------------------------------------------------------------------
const char* STR_CHART_AXES=
"\t\t\t{ title:{text:yAxisTitles{S}[{N}],style:{color:yColors[{N}]}}, labels:{format:'{value} '+suffix{S}[{N}],style:{color:yColors[{N}]}},opposite:[{OPP}][{BOUNDS}] },\n";

//------------------------------------------------------------------------------
const char* STR_CHART_SERIES_1=
"\t\t\t{name:seriesNames{S}[{N}],data:yValues{S}[{N}],tooltip:{valueSuffix:suffix{S}[{N}]},type:type{S}[{N}],yAxis:0,color:yColors[{N}+1],},\n";

//------------------------------------------------------------------------------
const char* STR_CHART_SERIES_2=
"\t\t\t{name:seriesNames{S}[{N}],data:yValues{S}[{N}],tooltip:{valueSuffix:suffix{S}[{N}]},type:type{S}[{N}],yAxis:1,},\n";

//------------------------------------------------------------------------------
const char* STR_FUNC_TABLE=
"table#dataTables-example.table.table-striped.table-bordered.table-hover(width='100%')\n"
"\tthead\n"
"\t\ttr\n"
"\t\t\tth(colspan=\"4\")\n"
"\t\t\tth(colspan=\"4\") <center>Periods</center>\n"
"\t\ttr\n"
"\t\t\tth Ord\n"
"\t\t\tth Functional Group\n"
"\t\t\tth Function Name\n"
"\t\t\tth Total\n"
"\t\t\tth Creation\n"
"\t\t\tth Operational\n"
"\t\t\tth Post-Hack\n"
"\t\t\tth Recovery\n"
"\ttbody\n"
"[{ROWS}]";

//------------------------------------------------------------------------------
const char* STR_FUNC_ROW=
"\t\ttr.{T}\n"
"\t\t\ttd(align=\"right\") {ORD}\n"
"\t\t\ttd {GRP}\n"
"\t\t\ttd {FNC}\n"
"\t\t\ttd(align=\"right\") {CNT}\n"
"\t\t\ttd(align=\"right\") {C_CNT}\n"
"\t\t\ttd(align=\"right\") {O_CNT}\n"
"\t\t\ttd(align=\"right\") {P_CNT}\n"
"\t\t\ttd(align=\"right\") {R_CNT}\n";
