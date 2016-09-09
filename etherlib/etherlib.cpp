#include "etherlib.h"

//---------------------------------------------------------
extern char  toAscii1(short v);
extern short toHex1  (char c);
//---------------------------------------------------------
SFString compressHash(const SFString& hashIn)
{
#ifndef COMPRESS
	return hashIn;
#else
	SFString hashOut;
	char *s = (char *)(const char*)hashIn;
	s+=2; // skip '0x'
	while (*s)
	{
		short t = toHex1(*s);
		short b = toHex1(*(s+1));
		if (t == 0 && b == 0)
		{
			hashOut = hashIn; // don't compress if the string contains '00'
			break;
		}
		hashOut += (unsigned char)(t * 16 + b);
		s += 2;
	}
	return hashOut;
#endif
}

//---------------------------------------------------------
SFString uncompressHash(const SFString& hashIn)
{
#ifndef COMPRESS
	return hashIn;
#else
	if (hashIn.startsWith("0x"))
		return hashIn;

	SFString hashOut = "0x";
	char *s = (char *)(const char*)hashIn;
	while (*s)
	{
		unsigned char c = (unsigned char)*s;
		unsigned char t = (unsigned char)((c&0xf0)>>4);
		unsigned char b = (unsigned char)(c-(t*16));
		hashOut += toAscii1(t); // do not add these together, add them separately
		hashOut += toAscii1(b);
		s++;
	}
	return hashOut;
#endif
}

#ifdef COMPRESS
//---------------------------------------------------------
short toHex1(char c)
{
	switch(tolower(c))
	{
	case '0': return 0;
	case '1': return 1;
	case '2': return 2;
	case '3': return 3;
	case '4': return 4;
	case '5': return 5;
	case '6': return 6;
	case '7': return 7;
	case '8': return 8;
	case '9': return 9;
	case 'a': return 10;
	case 'b': return 11;
	case 'c': return 12;
	case 'd': return 13;
	case 'e': return 14;
	case 'f': return 15;
	}
	outScreen << "bad input: " << c << "\n";
//	exit(0);
	return 0;
}

//---------------------------------------------------------
char toAscii1(short v)
{
	switch(v)
	{
	case 0:  return '0';
	case 1:  return '1';
	case 2:  return '2';
	case 3:  return '3';
	case 4:  return '4';
	case 5:  return '5';
	case 6:  return '6';
	case 7:  return '7';
	case 8:  return '8';
	case 9:  return '9';
	case 10: return 'a';
	case 11: return 'b';
	case 12: return 'c';
	case 13: return 'd';
	case 14: return 'e';
	case 15: return 'f';
	}
	outScreen << "bad input: " << v << "\n";
//	exit(0);
	return 'X';
}
#endif
