#include "Utility.h"


long LittleToBigEndianLong(long longWord)
{
	long a0, a1, a2, a3;

	a0 = (longWord & 0x000000FF) << 24;
	a1 = (longWord & 0x0000FF00) << 8;
	a2 = (longWord & 0x00FF0000) >> 8;
	a3 = (longWord & 0xFF000000) >> 24;
	
	return a0 | a1 | a2 | a3;
}

short LittleToBigEndianShort(short word)
{
	short a0, a1;

	a0 = (word & 0x00FF) << 8;
	a1 = (word & 0xFF00) >> 8;

	return a0 | a1;
}