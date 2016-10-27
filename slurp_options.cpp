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
#include "daolib.h"
#include "slurp_options.h"

//---------------------------------------------------------------------------------------------------
CParams params[] =
{
	CParams("~addr",	"the address of the account or contract to slurp" ),
	CParams("-archive",	"filename of output (stdout otherwise)" ),
	CParams("-blocks",	"export records in block range (:0[:max])" ),
	CParams("-dates",	"export records in date range (:yyyymmdd[hhmmss][:yyyymmdd[hhmmss]])" ),
	CParams("-max",		"maximum transactions to slurp (:250000)" ),
	CParams("-name",	"name this address" ),
	CParams("-rerun",	"re-run the most recent slurp" ),
	CParams("-fmt",		"pretty print, optionally add ':txt,' ':csv,' or ':html'" ),
	CParams("-income",	"include income transactions only" ),
	CParams("-expense",	"include expenditures only" ),
	CParams("-open",	"open the configuration file for editing" ),
	CParams("-list",	"list previously slurped addresses in cache" ),
	CParams("@--sleep",	"sleep for :x seconds" ),
	CParams("@--func",	"display only --func:functionName records" ),
	CParams("@--errFilt",	"display only non-error transactions or :errsOnly" ),
	CParams("@--reverse",	"display results sorted in reverse chronological order (chronological by default)" ),
	CParams("-clear",	"clear all previously cached slurps" ),
	CParams( "",		"Fetches data off the Ethereum blockchain for an arbitrary account or smart contract. Optionally formats the output to your specification.\n" ),
};
SFInt32 nParams = sizeof(params) / sizeof(CParams);

//---------------------------------------------------------------------------------------------------
SFInt32 CSlurpOptions::parseArguments(SFString& command)
{
        while (!command.IsEmpty())
        {
                SFString arg = nextTokenClear(command,' ');
		if (arg == "-i" || arg == "-income")
		{
			if (expenseOnly)
				return usage("Only one of -income or -expense may be specified.");
			incomeOnly = TRUE;

		} else if (arg == "-e" || arg == "-expense")
		{
			if (incomeOnly)
				return usage("Only one of -income or -expense may be specified.");
			expenseOnly = TRUE;

		} else if (arg == "-f")
		{
			// -f by itself is json prettyPrint
			prettyPrint = TRUE;
			exportFormat = "json";

		} else if (arg.startsWith("-f"))
		{
			// any other -f has the format attached  or is invalid
			prettyPrint = TRUE;
			exportFormat = arg;
			SFString arg = nextTokenClear(exportFormat, ':');
			if (arg != "-f" && arg != "-fmt")
				return usage("Unknown parameter: " + arg);

		} else if (arg.startsWith("--func"))
		{
			funcFilter = arg.Substitute("--func:",EMPTY);

		} else if (arg.startsWith("--errFilt"))
		{
			errFilt = TRUE + arg.Contains(":errOnly");

		} else if (arg.startsWith("--reverse"))
		{
			reverseSort = TRUE;

		} else if (arg == "-l" || arg == "-list")
		{
			SFInt32 nFiles=0;
			SFos::listFilesOrFolders(nFiles, NULL, cachePath("*.*"));
			if (nFiles)
			{
				SFString *files = new SFString[nFiles];
				SFos::listFilesOrFolders(nFiles, files, cachePath("*.*"));
				for (int i=0;i<nFiles;i++)
					outScreen << files[i] << "\n";
				delete [] files;
			} else
			{
				outScreen << "No files found in cache folder '" << cachePath() << "'\n";
			}
			exit(0);

		} else if (arg == "-b")
		{
			return usage("Invalid option -b. This option must include :firstBlock or :first:lastBlock range.");

		} else if (arg.Left(3) == "-b:" || arg.Left(8) == "-blocks:")
		{
			arg = arg.Substitute("-b:",EMPTY).Substitute("-blocks:",EMPTY);
			firstBlock2Read = MAX(0,toLong(arg));
			if (arg.Contains(":"))
			{
				nextTokenClear(arg,':');
				lastBlock2Read = MAX(firstBlock2Read, toLong(arg));
			}

		} else if (arg == "-d")
		{
			return usage("Invalid option -d. This option must include :firstDate or :first:lastDate range.");

		} else if (arg.Left(3) == "-d:" || arg.Left(8) == "-dates:")
		{
			SFString lateStr = arg.Substitute("-d:",EMPTY).Substitute("-dates:",EMPTY);
			SFString earlyStr = nextTokenClear(lateStr,':');

			earlyStr.ReplaceAll("-","");
			lateStr.ReplaceAll("-","");

			if (!earlyStr.IsEmpty() && earlyStr.GetLength() != 8 && earlyStr.GetLength() != 14)
				return usage("Option -d: Invalid date format for startDate. Format must be either yyyymmdd or yyyymmddhhmmss.");

			if (!lateStr.IsEmpty() && lateStr.GetLength () != 8 && lateStr.GetLength () != 14)
				return usage("Option -d: Invalid date format for endDate. Format must be either yyyymmdd or yyyymmddhhmmss.");

			firstDate = snagDate(earlyStr, earliestDate, -1);
			lastDate  = snagDate(lateStr,  latestDate,    1);

		} else if (arg == "-r" || arg == "-rerun")
		{
			rerun = TRUE;

		} else if (arg.startsWith("--sleep:"))
		{
			nextTokenClearI(arg, ':');
			SFInt32 wait = toLong(arg);
			if (wait)
			{
				outErr << "Sleeping " << wait << " seconds\n";
				SFos::sleep(wait);
			}
			
		} else if (arg.startsWith("-m"))
		{
			SFString val = arg;
			SFString arg = nextTokenClear(val, ':');
			if (arg != "-m" && arg != "-max")
				return usage("Unknown parameter: " + arg);
			maxTransactions = toLong(val);

		} else if (arg.startsWith("-n"))
		{
			SFString val = arg;
			SFString arg = nextTokenClear(val, ':');
			if (arg != "-n" && arg != "-name")
				return usage("Unknown parameter: " + arg);
			name = val;

		} else if (arg.startsWith("-a"))
		{
			SFString fileName = arg.Substitute("-a:",EMPTY).Substitute("-archive:",EMPTY);
			if (fileName == "-a")
			{
				// -a is acceptable but only if we get a -name (or we have only already) checked during slurp since we don't have an address yet
				wantsArchive = TRUE;

			} else
			{
				SFFile file(fileName);
				if (!file.getPath().startsWith('/'))
					return usage("Archive file '" + arg + "' does not resolve to a full path. Use ./path/filename, ~/path/filename, or a fully qualified path.");
				archiveFile = file.getFullPath();
				wantsArchive = TRUE;
			}

		} else if (arg == "-o" || arg == "-open")
		{
			// open command stuff
			openFile = TRUE;
			if (isTesting)
				outScreen << "Testing only for open command:\n" << asciiFileToString(configPath("ethslurp.conf")) << "\n";
			else
				if (system("pico ~/.ethslurp/ethslurp.conf")) {}; // do not remove. The test just silences compiler warnings
			exit(0);

		} else if (arg == "-c" || arg == "-clear")
		{
			if (isTesting)
			{
				SFString unused1, unused2;
				SFos::removeFolder(cachePath(), unused1, unused2, TRUE);
				outErr << "Cached slurp files were cleared\n";
			} else
			{
				outErr << "Clearing the cache is not implemented. You may, if you wish, remove all\n";
				outErr << "files in ~/.ethslurp/slurps/ to acheive the same thing. Large contracts\n";
				outErr << "may take a very long time to re-download if you do.\n";
			}
			exit(1);

		} else
		{
			if (arg != "-v" && arg != "-t" && arg != "-h")
			{
				if (arg[0] == '-')
					return usage("Invalid option " + arg);
				addr = arg;
			}
		}
	}

	return TRUE;
}

//---------------------------------------------------------------------------------------------------
void CSlurpOptions::Init(void)
{
	prettyPrint     = FALSE;
	rerun           = FALSE;
	incomeOnly      = FALSE;
	expenseOnly     = FALSE;
	openFile        = FALSE;
	funcFilter      = EMPTY;
	errFilt         = FALSE;
	reverseSort     = FALSE;
	firstBlock2Read = 0;
	lastBlock2Read  = LONG_MAX;
	firstDate       = earliestDate;
	lastDate        = latestDate;
	maxTransactions = 250000;
	pageSize        = 5000;
	exportFormat    = "json";
	archiveFile     = EMPTY;
	wantsArchive    = FALSE;
	//addr;

	outScreen.setOutput(stdout); // so we know where it is at the start of each run
	output          = NULL;
}

//---------------------------------------------------------------------------------------------------
CSlurpOptions::CSlurpOptions(void)
{
	Init();
	msg = "  Portions of this software are Powered by Etherscan.io APIs\n\n";
}

//--------------------------------------------------------------------------------
CSlurpOptions::~CSlurpOptions(void)
{
	outScreen.setOutput(stdout); // flushes and clears archive file if any
	output=NULL;
}
