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
#include "utillib.h"
#include "rlp_options.h"

//---------------------------------------------------------------------------------------------------
CParams params[] =
{
	CParams("~input",	"the input to convert (by default will be converted to hex)" ),
	CParams("-decode",	"assumes input is hex and converts to ascii" ),
	CParams("-encode",	"assumes input is ascii and converts to hex (the defaul)" ),
	CParams( "",		"Convert ascii to hex and visa versa.\n" ),
};
SFInt32 nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
SFBool CRLPOptions::parseArguments(SFString& command)
{
	Init();
	while (!command.IsEmpty())
	{
		SFString arg = nextTokenClear(command,' ');
		if (arg == "-e" || arg == "-encode")
		{
			ascii2Hex = TRUE;
			hex2Ascii = FALSE;

		} else if (arg == "-d" || arg == "-decode")
		{
			ascii2Hex = FALSE;
			hex2Ascii = TRUE;

                } else if (arg.startsWith('-'))
                {
                        if (arg != "-t" && arg != "-h")
                        {
                                return usage("Invalid option: " + arg);
                        }

		} else
		{
			if (!input.IsEmpty())
				input += " ";
			input += arg;
		}
		
		if (arg.startsWith("0x"))
		{
			ascii2Hex = FALSE;
			hex2Ascii = TRUE;
		}
	}
	return TRUE;
}

//---------------------------------------------------------------------------------------------------
void CRLPOptions::Init(void)
{
	outScreen.setOutput(stdout); // so we know where it is at the start of each run
	ascii2Hex = TRUE;
	hex2Ascii = FALSE;
	input = EMPTY;
}

//---------------------------------------------------------------------------------------------------
CRLPOptions::CRLPOptions(void)
{
	COptions::msg = EMPTY;
	COptions::useVerbose = FALSE;
	COptions::useTesting = TRUE;
	Init();
}
