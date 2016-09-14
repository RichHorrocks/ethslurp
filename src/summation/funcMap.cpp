//------------------------------------------------------------------------------
#include "funcMap.h"

//------------------------------------------------------------------------------
CFunctionMap funcMap[] = {
	CFunctionMap("Admin",                 GRP_ADMIN,    "checkProposalCode",       FNC_CHECKPROPOSALCODE    ),
	CFunctionMap("Admin",                 GRP_ADMIN,    "halveMinQuorum",          FNC_HALVEMINQUORUM       ),
	CFunctionMap("Admin",                 GRP_ADMIN,    "newContract",             FNC_NEWCONTRACT          ),
	CFunctionMap("Other",                 GRP_OTHER,    "balanceOf",               FNC_BALANCEOF            ),
	CFunctionMap("Other",                 GRP_OTHER,    "retrieveDAOReward",       FNC_RETRIEVEDAOREWARD    ),
	CFunctionMap("Admin",                 GRP_ADMIN,    "executeProposal",         FNC_EXECUTEPROPOSAL      ),
	CFunctionMap("Other",                 GRP_OTHER,    "getMyReward",             FNC_GETMYREWARD          ),
	CFunctionMap("Other",                 GRP_OTHER,    "receiveEther",            FNC_RECEIVEETHER         ),
	CFunctionMap("Other",                 GRP_OTHER,    "transferWithoutReward",   FNC_TRANSFERWITHOUTREWARD),
	CFunctionMap("Other",                 GRP_OTHER,    "unblockMe",               FNC_UNBLOCKME            ),
	CFunctionMap("Proposals/Voting",      GRP_PROPOSAL, "newProposal (non-split)", FNC_NEWPROPOSAL_NONSPLIT ),
	CFunctionMap("Other",                 GRP_OTHER,    "refund",                  FNC_REFUND               ),
	CFunctionMap("Proposals/Voting",      GRP_PROPOSAL, "splitDAO",                FNC_SPLITDAO             ),
	CFunctionMap("Proposals/Voting",      GRP_PROPOSAL, "newProposal (split)",     FNC_NEWPROPOSAL_SPLIT    ),
	CFunctionMap("Proposals/Voting",      GRP_PROPOSAL, "vote",                    FNC_VOTE                 ),
	CFunctionMap("Creation/Direct",       GRP_CREATE,   "createTokenProxy",        FNC_CREATETOKENPROXY     ),
	CFunctionMap("Transfer/TransferFrom", GRP_TRANSFER, "transferFrom",            FNC_TRANSFERFROM         ),
	CFunctionMap("Approvals",             GRP_APPROVE,  "approve",                 FNC_APPROVE              ),
	CFunctionMap("Transfer/TransferFrom", GRP_TRANSFER, "transfer",                FNC_TRANSFER             ),
	CFunctionMap("Withdraw Contract",     GRP_WITHDRAW, "withdraw",                FNC_WITHDRAW             ),
	CFunctionMap("Withdraw Contract",     GRP_WITHDRAW, "trusteeWithdraw",         FNC_TRUSTEEWITHDRAW      ),
	CFunctionMap("Creation/Direct",       GRP_CREATE,   "function()",              FNC_FUNCTION             ),
	CFunctionMap("Creation/Direct",       GRP_CREATE,   " ",                       FNC_FUNCTION             ),
};
SFInt32 nItems = sizeof(funcMap) / sizeof(CFunctionMap);

//------------------------------------------------------------------------------
CFunctionMap *findByFuncName(const SFString& funcIn)
{
	SFString func = (funcIn == "" ? "function()" : funcIn);
	for (int i=0;i<nItems;i++)
		if (funcMap[i].funcName == func)
			return &funcMap[i];
	return NULL;
}

//------------------------------------------------------------------------------
CFunctionMap *findByFuncID(SFInt32 id)
{
	for (int i=0;i<nItems;i++)
		if (funcMap[i].funcID == id)
			return &funcMap[i];
	return NULL;
}

//------------------------------------------------------------------------------
CFunctionMap *findByGroupID(SFInt32 id)
{
	for (int i=0;i<nItems;i++)
		if (funcMap[i].groupID == id)
			return &funcMap[i];
	return NULL;
}

//------------------------------------------------------------------------------
SFInt32 getGroupIndex(const SFString& func)
{
	CFunctionMap *item = findByFuncName(func);
	if (item)
		return item->groupID;
	outErr << "Could not find function name '" << func << "'\n";
	return funcMap[nItems-1].groupID; //exit(0);
}

//------------------------------------------------------------------------------
SFString getGroupName(SFInt32 id)
{
	CFunctionMap *item = findByGroupID(id);
	if (item)
		return item->groupName;
	outErr << "Could not find group id '" << id << "'\n";
	return funcMap[nItems-1].groupName; //exit(0);
}

//------------------------------------------------------------------------------
SFInt32 getFunctionIndex(const SFString& func)
{
	CFunctionMap *item = findByFuncName(func);
	if (item)
		return item->funcID;
	outErr << "Could not find function name '" << func << "'\n";
	return funcMap[nItems-1].funcID; //exit(0);
}

//------------------------------------------------------------------------------
SFString getFunctionName(SFInt32 id)
{
	CFunctionMap *item = findByFuncID(id);
	if (item)
		return item->funcName;
	outErr << "Could not find func id '" << id << "'\n";
	return funcMap[nItems-1].funcName; //exit(0);
}

