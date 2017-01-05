#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include <iomanip>
#include<stdio.h>
#include <math.h>

#include "RandomData.h"
#include "Utility.h"

int rangeOfNumberOfTypes[6];
int numberOfSections;


FILE *outputBinaryFile;
FILE *outputTextFile;

void GetUserInput(void);
void GetNumberOfSections(void);
void InitializeOutputFiles(void);
void HandleSections(void);
void CloseFiles(void);

int main()
{
	std::cout << std::setprecision(4) << std::fixed;
	GetUserInput();
	GetNumberOfSections();
	InitializeOutputFiles();
	HandleSections();
	CloseFiles();
	return 0;
}

// The range of number of bytes, shorts and longs are accessed from an input file
void GetUserInput()
{	
	FILE *userInputFile;
	userInputFile = fopen("RangeOfNumberOfTypes.txt", "r");
	
	// Check if user input file exists
	if (userInputFile == nullptr)
	{
		std::cout << "Error reading the user input file, program terminated.\n";
		exit(0);
	}
	
	// Scan and store data from the user input file
	for (int i = 0; i < 6; i++)
	{
		fscanf(userInputFile, "%d,", &rangeOfNumberOfTypes[i]);
	}
	
	fclose(userInputFile);
}
// Get user input on the number of sections for which the sets of data are to be generated 
void GetNumberOfSections()
{
	std::cout << "Enter the number of sections:\n";
	std::cin >> numberOfSections;
	std::cout << std::endl;
	// Check if the input is valid
	if (numberOfSections > 50 || numberOfSections < 0)
	{
		std::cout << "Number of sections must be within 0 to 50.\n";
		GetNumberOfSections();
	}
}
// Opens output files (binary files and text file) and writes the number of sections in it
void InitializeOutputFiles()
{

	// Open the binary file and write the number of sections to it
	long tempLong = numberOfSections;
	tempLong = LittleToBigEndianLong(tempLong);
	outputBinaryFile = fopen("OutputBinaryFile.txt", "wb");
	fwrite(&tempLong, sizeof(long), 1, outputBinaryFile);
	// Open the text file and write the number of sections to it
	outputTextFile = fopen("OutputTextFile.txt", "w");
	fprintf(outputTextFile, "User chose %d sections.\n\n", numberOfSections);

}
// Estimates data for all the sections and fills both the files with data generated
void HandleSections()
{
	unsigned char tempUnsignedChar;
	short tempShort;
	long tempLong;

	int sectionCounter = 0;
	short numberOfBytes, numberOfShorts, numberOfLongs;
	float byteAverage, shortAverage, longAverage;

	// Loop for each section
	while (sectionCounter < numberOfSections)
	{
		// Set values in input text file called RangeOfNumberOfTypes
		numberOfBytes = GetRandomWord(rangeOfNumberOfTypes[0], rangeOfNumberOfTypes[1]);		// Get a random word within the range as specified by the text input file
		numberOfShorts = GetRandomWord(rangeOfNumberOfTypes[2], rangeOfNumberOfTypes[3]);
		numberOfLongs = GetRandomWord(rangeOfNumberOfTypes[4], rangeOfNumberOfTypes[5]);
		// Check if the number of bytes are odd, in case they are, add 1 byte for padding at the end
		bool oddBytes = (numberOfBytes % 2 == 1);
		if (oddBytes) numberOfBytes++;
		// Write the number of bytes that need to be skipped to reach the next section
		tempShort = numberOfBytes + numberOfShorts * 2 + numberOfLongs * 4;
		tempShort = LittleToBigEndianShort(tempShort);
		fwrite(&tempShort, sizeof(short), 1, outputBinaryFile);
		// Write the number of bytes, shorts and longs required in output binary file
		tempShort = LittleToBigEndianShort(numberOfBytes);
		fwrite(&tempShort, sizeof(short), 1, outputBinaryFile);
		
		tempShort = LittleToBigEndianShort(numberOfShorts);
		fwrite(&tempShort, sizeof(short), 1, outputBinaryFile);

		tempShort = LittleToBigEndianShort(numberOfLongs);
		fwrite(&tempShort, sizeof(short), 1, outputBinaryFile);
		// Initialize values for average calculation
		byteAverage = 0;
		shortAverage = 0;
		longAverage = 0;
		// Check for ensuring that padding byte isn't included in estimating byte average
		if (oddBytes) numberOfBytes--;
		// Loop for byte average and writing the section's byte data to the file
		for (int i = 0; i < numberOfBytes; i++)
		{
			tempUnsignedChar = GetRandomByte(50, 150);
			byteAverage = byteAverage + tempUnsignedChar;
			fwrite(&tempUnsignedChar, sizeof(unsigned char), 1, outputBinaryFile);
		}
		// Add padding to binary file if there are odd number of bytes
		if (oddBytes)
		{
			tempUnsignedChar = 0;
			fwrite(&tempUnsignedChar, sizeof(unsigned char), 1, outputBinaryFile);
		}
		byteAverage = (float)byteAverage / numberOfBytes;
		std::cout << "\nThe average of bytes is: " << byteAverage << std::endl;
		// Loop for short average and writing the section's short data to the binary file
		for (int i = 0; i < numberOfShorts; i++)
		{
			tempShort = GetRandomWord(1000, 2000);
			shortAverage = shortAverage + tempShort;
			tempShort = LittleToBigEndianShort(tempShort);
			fwrite(&tempShort, sizeof(short), 1, outputBinaryFile);
		}
		shortAverage = (float)shortAverage / numberOfShorts;
		std::cout << "\nThe average of shorts is: " << shortAverage << std::endl;
		// Loop for long average
		for (int i = 0; i < numberOfLongs; i++)
		{
			tempLong = GetRandomLong(3000, 5000);
			longAverage = longAverage + tempLong;
			tempLong = LittleToBigEndianLong(tempLong);
			fwrite(&tempLong, sizeof(long), 1, outputBinaryFile);
		}
		longAverage = (float)longAverage / numberOfLongs;
		std::cout << "\nThe average of longs is: " << longAverage << std::endl;
		// Write the section information to the output text file
		fprintf(outputTextFile, "Section %d\n", sectionCounter+1);
		// Round the average values up to 2 decimal places
		byteAverage = round(byteAverage * 100) / 100;
		shortAverage = round(shortAverage * 100) / 100;
		longAverage = round(longAverage * 100) / 100;
		// Write the averages and number of data fields to output text file
		fprintf(outputTextFile, "%d bytes, average value %.2f\n%d shorts, average value %.2f\n%d longs, average value %.2f\n\n", numberOfBytes, byteAverage, numberOfShorts, shortAverage, numberOfLongs, longAverage);
		sectionCounter++;
	}
}
// Closes the output files
void CloseFiles()
{
	fclose(outputBinaryFile);
	fclose(outputTextFile);
}
