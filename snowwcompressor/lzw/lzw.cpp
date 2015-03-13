/*
 * lzw.cpp
 *
 *  Created on: 16.11.2009
 *      Author: alex
 */
#include <stdio.h>
#include <map>
#include <vector>
#include <string>
#include <math.h>
#include "../bitfile.h"
#include "../filestructure.h"
#include "../compressor.h"
#include "lzw.h"



int LZWCompressor::CompressFile(BitFile &inputFile, BitFile &outputFile, bool paramVerbose) {
	int res;

	long int fSize, kSymb;

	const unsigned int EOF_CODE = 0;
	unsigned int i, qBits,threshold, nextCode;
	std::map<std::string, unsigned int> Dictionary;
	std::map<std::string, unsigned int>::iterator itDictionary;
	std::vector<unsigned int> curBitCode;
	std::string curString, tmpString;
	char curSymbol;



	curString = "";
	tmpString = "";
	qBits = 9;
	threshold = 512;
	nextCode = 1;
	// initialize Dictionary
	for (i = 0; i < 256; ++i) {
		tmpString = i;
		Dictionary.insert( std::pair<std::string,unsigned int>(tmpString,nextCode++) );
	}



	fSize = inputFile.GetFileSize();
	kSymb = 0;
	if (paramVerbose) printProcentsBar(kSymb,fSize);


	do {
		res = inputFile.Read(&curSymbol,sizeof(char));

		if (paramVerbose) printProcentsBar(++kSymb, fSize);

		if (inputFile.FEOF() == false  ) {
		//LZW ALGORITHM:
			tmpString = curString + curSymbol;

			itDictionary = Dictionary.find(tmpString);
			if ( itDictionary != Dictionary.end() ) { //if found
				curString += curSymbol;
			}
			else { //if not found
				curBitCode = IntToCode(Dictionary[curString], qBits);
				outputFile.Write(curBitCode);
				Dictionary.insert( std::pair<std::string,unsigned int>( tmpString, nextCode++ ) );
				curString = curSymbol;
			    if (nextCode-1 >= threshold) {
					qBits++;
					threshold *= 2;
				}
			}
		}
	}
	while (inputFile.FEOF() == false ); //while input stream is not empty

	if (paramVerbose) printProcentsBar(fSize, fSize);

	curBitCode = IntToCode(Dictionary[curString], qBits);
	outputFile.Write(curBitCode);
	outputFile.Write(IntToCode(EOF_CODE,qBits));



	return 0;
}


int LZWCompressor::DecompressFile(BitFile &inputFile, BitFile &outputFile,bool paramVerbose) {

	int res;

	long int fSize, kCode;

	const unsigned int EOF_CODE = 0;
	unsigned int i, qBits,threshold, oldCode,curCode;
	std::vector<std::string> Dictionary;

	std::vector<unsigned int> curBitCode;
	std::string curString, tmpString, oldString;

	// initialize Dictionary

	Dictionary.push_back(""); // EOF Symbol

	for (i = 0; i < 256; ++i) {
		tmpString = i;
		Dictionary.push_back(tmpString);

	}

	qBits = 9;
	threshold = 512;

	/* LZW Algorithm
	 */

	oldCode = 0;


	fSize = inputFile.GetFileSize()*8;
	kCode = 0;
	if (paramVerbose) printProcentsBar(kCode,fSize);

	do {
		res = inputFile.Read(curBitCode,qBits);
		curCode = CodeToInt(curBitCode);

		if (paramVerbose) printProcentsBar(++kCode,fSize);



		if (inputFile.FEOF() == false && curCode > EOF_CODE) {

			if ( curCode < Dictionary.size() )  {//code is in dictionary
				//translate current code:
				curString = Dictionary[curCode];
				//writing string with current code
				outputFile.Write(curString.c_str(),sizeof(char)*curString.length());
				//putting in Dictionary old string with first element of current string
				if (oldCode != 0) {
					Dictionary.push_back(Dictionary[oldCode] + curString[0]);
				}
				oldCode=curCode;

			} else { //code is not in dictionary
				//translate old code
				//string = oldstring + 1st symbol
				if (oldCode >= Dictionary.size() || oldCode == 0) {
					return -2; //error with decompressing;
				}
				curString = Dictionary[oldCode]+Dictionary[oldCode][0];
				outputFile.Write(curString.c_str(),sizeof(char)*curString.length());


				//adding string to dictionary
				Dictionary.push_back(curString);
				oldCode = curCode;
			}

			if (Dictionary.size()  >= threshold ) {
				qBits++;
				threshold *= 2;
			}
		}
	}
	while (inputFile.FEOF() == false && curCode > EOF_CODE); //while input stream is not empty

	if (paramVerbose) printProcentsBar(fSize,fSize);


	return 0;
}


std::vector<unsigned int> LZWCompressor::IntToCode(unsigned int integer, unsigned int qBits) {

	std::vector<unsigned int> curCode;
	unsigned int mnoj,curBit, count;
	count = 0;
	while(integer != 0 && count < qBits) {
		mnoj = (int)floor(integer/2);
		curBit = integer-mnoj*2;
		curCode.insert(curCode.begin(),curBit);
		count++;
		integer = mnoj;
	}
	while (curCode.size() < qBits) {
		curCode.insert(curCode.begin(),0);
	}
	return curCode;

}

unsigned int LZWCompressor::CodeToInt(std::vector<unsigned int>& curCode) {
	unsigned int size,res,i;
	size = curCode.size();
	for (i = 0, res = 0; i < size; ++i) {
		res += ( curCode[i] << (size-i-1) );
	}
	return res;

}

void LZWCompressor::printDictionary(std::vector<std::string> & Dictionary) {
	unsigned int i;
	printf("Словарь:\n");
	for ( i = 257; i < Dictionary.size(); ++i) {
		printf("%s = %d\n", Dictionary[i].c_str(), i);
	}
	printf("Total: %d\n",i-257);
}

void LZWCompressor::printDictionary(std::map <std::string, unsigned int> & Dictionary) {
	std::map <std::string,unsigned int>::iterator itDictionary;
	unsigned int i;
	printf("Словарь:\n");
	for ( itDictionary = Dictionary.begin(), i = 0; itDictionary != Dictionary.end(); ++itDictionary) {
		if (itDictionary->second > 256) {
			printf("%s = %d\n", itDictionary->first.c_str(), itDictionary->second);
			++i;
		}
	}
	printf("Total: %d\n",i);
}
