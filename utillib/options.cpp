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
#include "basetypes.h"
#include "sfstring.h"
#include "database.h"
#include "exportcontext.h"
#include "sffile.h"
#include "options.h"

SFString programName;
//--------------------------------------------------------------------------------
SFBool COptions::prepareArguments(int argc, const char *argv[])
{
	programName = SFFile(argv[0]).getFilename();
	if (argc==1)
		return usage();

	SFInt32 nArgs=0;
	SFString *args = new SFString[argc+2];

	SFBool hasStdIn = FALSE;
	for (int i=1;i<argc;i++)
	{
		SFString arg = argv[i];
		while (!arg.IsEmpty())
		{
			SFString opt = expandOption(arg); // handles case of -rf for example
			if (opt=="-")
				hasStdIn=TRUE;
			else
				args[nArgs++] = opt;
		}
	}

	SFString stdInCmds;
        if (hasStdIn)
        {
                // reading from stdin, expect only a list of addresses, one per line.
                char c = (char)getchar();
                while (c != EOF)
                {
                        stdInCmds += c;
                        c = (char)getchar();
                }
		if (!stdInCmds.endsWith("\n"))
			stdInCmds += "\n";
	}

	SFString cmdFileName=EMPTY;
	for (int i=0;i<nArgs;i++)
	{
		SFString arg = args[i];
		if (arg.startsWith("--file:"))
		{
			cmdFileName = arg.Substitute("--file:",EMPTY);
			cmdFileName.Replace("~/",getHomeFolder());
			if (!SFos::fileExists(cmdFileName))
			{
				if (args) delete [] args;
				return usage("--file: '" + cmdFileName + "' not found. Quitting.");
			}

		} else if (arg == "-h" || arg == "-help" || arg == "--help")
		{
			if (args) delete [] args;
			return usage();

		} else if (arg.startsWith("-v") || arg.startsWith("-verbose"))
		{
			verbose = TRUE;
			arg.Replace("-verbose",EMPTY);
			arg.Replace("-v",      EMPTY);
			arg.Replace(":",       EMPTY);
			if (!arg.IsEmpty())
				verbose = toLong(arg);

		} else if (arg=="-t" || arg=="-test")
		{
			// During testing, we send all output (including error messages)
			// to the screen so it can be re-directed to a file
			outErr = outScreen;
			isTesting = TRUE;
		}
	}

	// If we have a command file, we will use it, if not we will creat one and pretend we have one.
	commandList = EMPTY;
	fromFile = FALSE;
	if (cmdFileName.IsEmpty())
	{
		for (int i=0;i<nArgs;i++)
			commandList += (args[i] + " ");
		commandList += '\n';

	} else
	{
		fromFile = TRUE;
		SFString contents =  asciiFileToString(cmdFileName).Substitute("\t"," ").
								    Substitute("-v","").Substitute("-h","" ).
								    Substitute("-t","").Substitute("  "," ");
		while (!contents.IsEmpty())
		{
			SFString command = StripAny(nextTokenClear(contents, '\n'),"\t\r\n ");
			if (!command.IsEmpty() && !command.startsWith(";")) // ignore comments
				commandList += (command+"\n");
		}
	}
	commandList += stdInCmds;
	commandList.ReplaceAll(" \n","\n");

	if (args) delete [] args;
	return 1;
}

//--------------------------------------------------------------------------------
int usage(const SFString& errMsg)
{
	outErr << "\n" << (!errMsg.IsEmpty() ? "  " + errMsg + "\n\n" : "");
	outErr << "  Usage:   " + programName + " " << options() << "\n";
	outErr << purpose();
	outErr << descriptions() << "\n";
	outErr << COptions::msg;
	return FALSE;
}

//--------------------------------------------------------------------------------
CParams::CParams( const SFString& nameIn, const SFString& descr )
{
	SFString name = nameIn;

	description = descr;
	if (!name.IsEmpty())
	{
		shortName   = name.Left(2);
		if (name.GetLength()>2)
			longName  = name;
		if (name.Contains("{"))
		{
			name.Replace("{","|{");
			nextTokenClear(name,'|');
			shortName += name;

		} else if (name.Contains(":"))
		{
			nextTokenClear(name,':');
			shortName += name[0];
			longName = "-" + name;
		}
	}
}

//--------------------------------------------------------------------------------
SFString options(void)
{
	SFString required;

	CStringExportContext ctx;
	ctx << "[";
	for (int i=0;i<nParams;i++)
	{
		if (params[i].shortName.startsWith('~'))
		{
			required += (" " + params[i].longName.Mid(1));

		} else if (params[i].shortName.startsWith('@'))
		{
			// invisible option
			
		} else if (!params[i].shortName.IsEmpty())
		{
			ctx << params[i].shortName << "|";
		}
	}
	if (COptions::useTesting)
		ctx << "-t|";
	if (COptions::useVerbose)
		ctx << "-v|";
	ctx << "-h]";
	ctx << required;

	return ctx.str;
}

//--------------------------------------------------------------------------------
SFString purpose(void)
{
	SFString purpose;
	for (int i=0;i<nParams;i++)
		if (params[i].shortName.IsEmpty())
			purpose += ("\n           " + params[i].description);

	CStringExportContext ctx;
	if (!purpose.IsEmpty())
	{
		purpose.Replace("\n           ",EMPTY);
		ctx << "  Purpose: " << purpose.Substitute("\n","\n           ") << "\n";
	}
	ctx << "  Where:\n";
	return ctx.str;
}

//--------------------------------------------------------------------------------
SFString descriptions(void)
{
	SFString required;

	CStringExportContext ctx;
	for (int i=0;i<nParams;i++)
	{
		if (params[i].shortName.startsWith('~'))
		{
			required += ("\t" + padRight(params[i].longName,22) + params[i].description).Substitute("~",EMPTY) + " (required)\n";

		} else if (params[i].shortName.startsWith('@'))
		{
			// invisible option

		} else if (!params[i].shortName.IsEmpty())
		{
			ctx << "\t" << padRight(params[i].shortName,3) << padRight((params[i].longName.IsEmpty() ? "" : " (or "+params[i].longName+")"),18) << params[i].description << "\n";
		}
	}
	ctx.str = (required + ctx.str);
	if (COptions::useTesting)
		ctx << "\t" << "-t  (or -test)       generate intermediary files but do not execute the commands\n";
	if (COptions::useVerbose)
		ctx << "\t" << "-v  (or -verbose)    set verbose level. Follow with a number to set level (-v0 for silent)\n";
	ctx << "\t" << "-h  (or -help)       display this help screen\n";
	return ctx.str;
}

//--------------------------------------------------------------------------------
SFString expandOption(SFString& arg)
{
	SFString ret = arg;

	// Not an option
	if (!arg.startsWith('-') || arg.startsWith("--"))
	{
		arg=EMPTY;
		return ret;
	}

	// Stdin case
	if (arg=="-")
	{
		arg=EMPTY;
		return ret;
	}

	// Single option
	if (arg.GetLength()==2)
	{
		arg=EMPTY;
		return ret;
	}

	// One of the range commands. These must be alone on
	// the line (this is a bug for -rf:txt for example)
	if (arg.Contains(":"))
	{
		arg=EMPTY;
		return ret;
	}

	// This is a ganged-up option. We need to pull it apart by returning
	// the first two chars, and saving the rest for later.
	ret = arg.Left(2);
	arg = "-"+arg.Mid(2,1000);
	return ret;
}

//--------------------------------------------------------------------------------
int sortParams(const void *c1, const void *c2)
{
	CParams *p1 = (CParams*)c1;
	CParams *p2 = (CParams*)c2;
	if (p1->shortName=="-h")
		return 1;
	else if (p2->shortName=="-h")
		return -1;
	return (int)p1->shortName.Compare(p2->shortName);
}

//--------------------------------------------------------------------------------
SFString COptions::msg = EMPTY;
SFBool COptions::useVerbose = TRUE;
SFBool COptions::useTesting = TRUE;

//--------------------------------------------------------------------------------
SFBool verbose  = FALSE;
SFBool isTesting = FALSE;

//--------------------------------------------------------------------------------
CFileExportContext   outScreen;
CErrorExportContext  outErr_internal;
CFileExportContext&  outErr = outErr_internal;

//---------------------------------------------------------------------------------------------------
SFString configPath(const SFString& part)
{
	return getHomeFolder() + "." + programName + (isTesting?".test":EMPTY) + "/" + part;
}

//---------------------------------------------------------------------------------------------------
SFString cachePath(const SFString& part)
{
	return configPath("slurps/") + part;
}
