#ifndef _FUNCTION_MAP_H_
#define _FUNCTION_MAP_H_

//------------------------------------------------------------------------------
#include "etherlib.h"

//------------------------------------------------------------------------------
class CFunctionMap
{
public:
	SFString groupName;
	SFInt32  groupID;
	SFString funcName;
	SFInt32  funcID;
	SFInt32  sortOrder;
	SFBool   showing;
	CFunctionMap(void) { groupID = funcID = sortOrder = -1; showing = FALSE; }
	CFunctionMap(const SFString& gn, SFInt32 gID, const SFString& fn, SFInt32 fID)
		{ groupName=gn; groupID=gID; funcName=fn; funcID=fID; showing=FALSE; sortOrder=funcID; }
};

//------------------------------------------------------------------------------
extern CFunctionMap funcMap[];
extern SFInt32 nItems;

//------------------------------------------------------------------------------
extern CFunctionMap *findByFuncName(const SFString& funcIn);
extern CFunctionMap *findByFuncID  (SFInt32 id);
extern CFunctionMap *findByGroupID (SFInt32 id);

//------------------------------------------------------------------------------
extern SFInt32 getGroupIndex   (const SFString& func);
extern SFInt32 getFunctionIndex(const SFString& func);

//------------------------------------------------------------------------------
extern SFString getGroupName   (SFInt32 gID);
extern SFString getFunctionName(SFInt32 fID);

//------------------------------------------------------------------------------
// Be very careful about changing these values as the code depends on them sometimes
#define IN_TOTO                   0

#define GRP_WITHDRAW              1 // start at '1' since we store totals in '0'
#define GRP_TRANSFER              2
#define GRP_APPROVE               3
#define GRP_PROPOSAL              4
#define GRP_CREATE                5
#define GRP_OTHER                 6
#define GRP_ADMIN                 7
#define GRP_FIRST                 (GRP_WITHDRAW)
#define GRP_LAST                  (GRP_ADMIN+1)

#define PER_FIRST                 GRP_LAST
#define PER_CREATION              (PER_FIRST)
#define PER_OPERATIONAL           9
#define PER_POSTHACK              10
#define PER_RECOVERY              11
#define PER_LAST                  (PER_RECOVERY+1)

#define FNC_CHECKPROPOSALCODE     12
#define FNC_HALVEMINQUORUM        13
#define FNC_NEWCONTRACT           14
#define FNC_BALANCEOF             15
#define FNC_RETRIEVEDAOREWARD     16
#define FNC_EXECUTEPROPOSAL       17
#define FNC_GETMYREWARD           18
#define FNC_RECEIVEETHER          19
#define FNC_TRANSFERWITHOUTREWARD 20
#define FNC_UNBLOCKME             21
#define FNC_NEWPROPOSAL_NONSPLIT  22
#define FNC_REFUND                23
#define FNC_SPLITDAO              24
#define FNC_NEWPROPOSAL_SPLIT     25
#define FNC_VOTE                  26
#define FNC_CREATETOKENPROXY      27
#define FNC_TRANSFERFROM          28
#define FNC_APPROVE               29
#define FNC_TRANSFER              30
#define FNC_WITHDRAW              31
#define FNC_TRUSTEEWITHDRAW       32
#define FNC_FUNCTION              33
#define FNC_FIRST                 (FNC_CHECKPROPOSALCODE)
#define FNC_LAST                  (FNC_FUNCTION+1)

#define N_COUNTERS		  (FNC_LAST)

//------------------------------------------------------------------------------
#define DAYS_PER_YEAR 		  365
#define HOURS_PER_DAY 		  24
#define MAX_YEARS     		  5
#define MAX_BUCKETS               (MAX_YEARS * DAYS_PER_YEAR * HOURS_PER_DAY)

#endif
