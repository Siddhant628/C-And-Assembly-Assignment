#include <stdlib.h>
#include <iostream>

#include "RandomData.h"

unsigned char GetRandomByte(int lowerRange, int upperRange)
{
	int range;
	unsigned char randomByte;
	
	range = upperRange - lowerRange;
	if (range > 255 || range < 0 || upperRange > 255 || lowerRange < 0)
	{
		std::cout << "Invalid parameters for generating a random byte.\n";
		return 0;
	}
	if (lowerRange == upperRange) 
	{
	//	std::cout << "Random Byte Returned: " << lowerRange << "  Range: " << lowerRange << " - " << upperRange << std::endl;
		return (char)lowerRange;
	}
	randomByte = rand() % (range+1);
	randomByte = randomByte + lowerRange;
	//std::cout << "Random Byte Returned: " << (int)randomByte << "  Range: " << lowerRange << " - " << upperRange << std::endl;
	return randomByte;
}

short GetRandomWord(int lowerRange, int upperRange)
{
	int range;
	short randomShort;

	range = upperRange - lowerRange;
	if (range > 65535 || range < 0 || upperRange > 65535 || lowerRange < 0)
	{
		std::cout << "Invalid parameters for generating a random short.\n";
		return 0;
	}
	if (lowerRange == upperRange)
	{
	//	std::cout << "Random Short Returned: " << lowerRange << "  Range: " << lowerRange << " - " << upperRange << std::endl;
		return (short)lowerRange;
	}
	randomShort = rand() % (range + 1);
	randomShort = randomShort + lowerRange;
	//std::cout << "Random Short Returned: " << (int)randomShort << "  Range: " << lowerRange << " - " << upperRange << std::endl;
	return randomShort;
}

long GetRandomLong(int lowerRange, int upperRange)
{
	int range;
	long randomLong;

	range = upperRange - lowerRange;
	if (range > 4294967296 || range < 0 || upperRange > 2147483647 || lowerRange < 0)
	{
		std::cout << "Invalid parameters for generating a random long.\n";
		return 0;
	}
	if (lowerRange == upperRange)
	{
	//	std::cout << "Random Long Returned: " << lowerRange << "  Range: " << lowerRange << " - " << upperRange << std::endl;
		return (long)lowerRange;
	}
	randomLong = rand() % (range + 1);
	randomLong = randomLong + lowerRange;
	//std::cout << "Random Long Returned: " << (int)randomLong << "  Range: " << lowerRange << " - " << upperRange << std::endl;
	return randomLong;
}