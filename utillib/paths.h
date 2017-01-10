#ifndef _PATHS_H_
#define _PATHS_H_
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 1999, 2016 by Great Hill Corporation.
 * All Rights Reserved
 *
 *------------------------------------------------------------------------*/

#include "sffile.h"
#include "sffolder.h"
#include "visitor.h"

//-----------------------------------------------------------------------------------------------
extern SFInt32  countFilesInFolder (const SFString& path,                                   SFInt32 options=F_DEFAULT);
extern SFInt32  getFilesInFolder   (const SFString& path, SFInt32& nFiles, SFString *files, SFInt32 options=F_DEFAULT);
extern SFString listFilesInFolder  (const SFString& path,                                   SFInt32 options=F_DEFAULT);
extern SFInt32  forAllItemsInFolder(const SFString& path, APPLYFUNC func, void *data, SFInt32 options);

#include <glob.h>
#include <iostream>
#include <fstream>
#include <curl/curl.h>

//----------------------------------------------------------------------------------
typedef bool (*VISITOR)(const SFString& str, void *data);

//------------------------------------------------------------------
inline int globErrFunc(const char *epath, int eerrno)
{
	perror(epath);
	printf("%d:%s\n",eerrno,epath);
	return 0;
}

//----------------------------------------------------------------------------------
inline void forAllFiles(const SFString& mask, VISITOR func, void *data )
{
	glob_t globBuf;
	glob( (const char *)mask, GLOB_MARK, globErrFunc, &globBuf);
	SFBool done=FALSE;
	for (uint i=0;i<globBuf.gl_pathc&&!done;i++)
		if (!(func)(globBuf.gl_pathv[i], data))
			done=TRUE;
	globfree( &globBuf );
}

//-------------------------------------------------------------------------
#define quote(a) (SFString("\"") + a + "\"")

#endif
