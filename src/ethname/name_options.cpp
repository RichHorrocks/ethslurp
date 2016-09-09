/*--------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2016 Great Hill Corporation

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
--------------------------------------------------------------------------------*/
#include "etherlib.h"
#include "name_options.h"

//---------------------------------------------------------------------------------------------------
CParams params[] =
{
	CParams("~term [name]",	"search for 'term' in either name or address. If 'name' is present, term \n\t\t\t     is assumed to be an address. In this case, both must match" ),
	CParams("-all",		"search 'source' field as well name and address" ),
	CParams("-count",	"print only the count of the number of matches" ),
	CParams("-list",        "list all names" ),
	CParams("-matchCase",	"matches must agree in case (the default is to ignore case)" ),
	CParams("-open",	"open the name database for editing" ),
	CParams("-write",	"write the name/address pair to database. Will prompt for an optional source" ),
	CParams( "",		"Find a name given an Ethereum address, or find an address given a name.\n" ),
};
SFInt32 nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
SFBool CNameOptions::parseArguments(SFString& command)
{
	Init();
	while (!command.IsEmpty())
	{
		SFBool needWrite=FALSE;
		SFString arg = nextTokenClear(command,' ');
		if (arg == "-a" || arg == "-all")
		{
			all = TRUE;

		} else if (arg == "-c" || arg == "-count")
		{
			count = TRUE;

		} else if (arg == "-l" || arg == "-list")
		{
			list = TRUE;

		} else if (arg == "-m" || arg == "-matchCase")
		{
			matchCase = TRUE;

		} else if (arg == "-o" || arg == "-open")
		{
			// open command stuff
			system("pico " + DATA_FILE);
			exit(0);

		} else if (arg == "-w" || arg == "-write")
		{
			needWrite = TRUE;

                } else if (arg.startsWith('-'))
                {
                        if (arg != "-t" && arg != "-h")
                        {
                                return usage("Invalid option: " + arg);
                        }

		} else
		{
			     if (!addr.IsEmpty() && !name.IsEmpty() && !source.IsEmpty())
				return usage("Invalid option: " + arg);
			else if (!addr.IsEmpty() && !name.IsEmpty())
				source = arg;
			else if (!addr.IsEmpty())
				name = arg;
			else
				addr = arg;
		}
		
		if (needWrite && (name.IsEmpty() || addr.IsEmpty() || source.IsEmpty()))
			return usage("If -w is given, all three of 'addr', 'name', and 'source' must be given");
	}
	return TRUE;
}

//---------------------------------------------------------------------------------------------------
void CNameOptions::Init(void)
{
	outScreen.setOutput(stdout); // so we know where it is at the start of each run
	addr = EMPTY;
	name = EMPTY;
	source = EMPTY;
	all = FALSE;
	write = FALSE;
	matchCase = FALSE;
	list = FALSE;
}

//---------------------------------------------------------------------------------------------------
CNameOptions::CNameOptions(void)
{
	COptions::msg = EMPTY;
	COptions::useVerbose = FALSE;
	COptions::useTesting = FALSE;
	Init();
}
