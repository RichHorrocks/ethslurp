#ifndef MINI_BLOCK
#define MINI_BLOCK

class CMiniBlock
{
public:
        SFUint32 number;
        SFUint32 timestamp;
        SFUint32 firstTrans;
        SFUint32 nTrans;

	CMiniBlock(CBlock *block) {
		bzero(this, sizeof(CMiniBlock));
		number     = block->number.startsWith("0x")    ? hex2Long(block->number)    : toLong(block->number);
		timestamp  = block->timestamp.startsWith("0x") ? hex2Long(block->timestamp) : toLong(block->timestamp);
                firstTrans = block->firstTrans;
                nTrans     = block->nTrans;
        }

	void toBlock(CBlock& block) {
		block.number     = asString(number);
		block.timestamp  = asString(timestamp);
		block.firstTrans = firstTrans;
		block.nTrans     = nTrans;
		return;
	}

	SFString Format(void) {
		CStringExportContext ctx;
		ctx << "number: " << number << " timestamp: " << timestamp << " firstTrans: " << firstTrans << " nTrans: " << nTrans;
		return ctx.str;
	}

	CMiniBlock() { }
};

class CMiniTrans
{
public:
	SFUint32 index;
	char     from [41];
	char     to   [41];
	char     value[41];

	CMiniTrans(CTransaction *t)
	{
		bzero(this, sizeof(CMiniTrans));
		index = t->transactionIndex;
		strncpy(from,  (const char*)t->from.Mid(2,1000), 40);from [40]='\0';
		strncpy(to,    (const char*)t->to.Mid(2,1000),   40);to   [40]='\0';
		strncpy(value, (const char*)t->value,            40);value[40]='\0';
	}

	void toTrans(CTransaction& trans)
	{
		trans.transactionIndex = index;
		trans.from  = from;
		trans.to    = to;
		trans.value = value;
		return;
	}

	SFString Format(void) {
		CStringExportContext ctx;
		ctx << "index: " << index << " from: " << from << " to: " << to << " value: " << value;
		return ctx.str;
	}

	CMiniTrans() {}
};

#endif
