/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 1999, 2016 by Great Hill Corporation.
 * All Rights Reserved
 *
 *------------------------------------------------------------------------*/
#include "basetypes.h"

#include "paths.h"

//----------------------------------------------------------------------------------
SFFile::SFFile(void)
{
	Init();
}

//----------------------------------------------------------------------------------
SFFile::SFFile(const SFString& fn)
{
	Init();
	input = fn;
	if (fn.endsWith('/'))
	{
		path = fn;
	} else
	{
		path = fn.Left(fn.ReverseFind('/')+1);
		fileName = fn;
		fileName.Replace(path, EMPTY);
	}
}

//----------------------------------------------------------------------------------
SFFile::SFFile(const SFFile& file)
{
	Copy(file);
}

//----------------------------------------------------------------------------------
SFFile& SFFile::operator=(const SFFile& file)
{
	Clear();
	Copy(file);
	return *this;
}

//----------------------------------------------------------------------------------
SFFile::~SFFile(void)
{
}

//----------------------------------------------------------------------------------
SFString SFFile::getInput(void) const
{
	return input;
}

//----------------------------------------------------------------------------------
SFString SFFile::getFilename(void) const
{
	return fileName;
}

//----------------------------------------------------------------------------------
SFString SFFile::getPath(void) const
{
	return path;
}

//----------------------------------------------------------------------------------
SFInt32 SFFile::getSize(void) const
{
	if (size == -1)
		((SFFile*)this)->size = SFos::fileSize(input);
	return size;
}

//----------------------------------------------------------------------------------
SFInt32 SFFile::getType(void) const
{
	if (type == -1)
		((SFFile*)this)->type = 1;
	return type;
}

//----------------------------------------------------------------------------------
void SFFile::setType(SFInt32 t)
{
	type = t;
}

//----------------------------------------------------------------------------------
void SFFile::Report(CExportContext& ctx)
{
	SFString fn = getInput().Mid(getInput().ReverseFind('/')+1);
	if (type == 2)
		ctx << fn << "--------------------------------------------\n";
	else
		ctx << fn << "\t" << getSize() << "\t" << getType() << "\n";
}

//----------------------------------------------------------------------------------
void SFFile::Init(void)
{
	type = -1;
	size = -1;
}

//----------------------------------------------------------------------------------
void SFFile::Clear(void)
{
	path = fileName = input = EMPTY;
	type = size = 0;
}

//----------------------------------------------------------------------------------
void SFFile::Copy(const SFFile& file)
{
	path     = file.path;
	fileName = file.fileName;
	input    = file.input;
	type     = file.type;
	size     = file.size;
}
