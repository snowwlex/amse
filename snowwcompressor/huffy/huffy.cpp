/*
 * huffy.cpp
 *
 *  Created on: 16.11.2009
 *      Author: alex
 */
#include "stdio.h"
#include <vector>
#include <string>
#include "../bitfile.h"
#include "../filestructure.h"
#include "../compressor.h"
#include "huffy.h"

void MakeStat(unsigned int *statArray,BitFile &inputFile) {


    unsigned char ch;
	int res;
    unsigned int i;

     //initialize statArray with zeros
    for (i = 0; i < 256; ++i) {
    	statArray[i] = 0;
    }

    do {
    	res = inputFile.Read(&ch,sizeof(unsigned char));
		if (inputFile.FEOF() == false) {
			statArray[ch]+=1;
		}
    } while (inputFile.FEOF() == false );

}

int HuffyCompressor::CompressFile(BitFile &inputFile, BitFile &outputFile, bool paramVerbose) {
	int res, i;

	long int fSize, kSymb;

	unsigned int arrayLengths[256], arrayStat[256];
	HuffmanTree HT;
	std::vector<unsigned int> curBitCode;
	unsigned char curSymbol;
	unsigned long filesize;
	MakeStat(&arrayStat[0],inputFile);

	inputFile.Rewind(SEEK_SET);

	//making archive:
	//	building huffman tree
	//	while reading is available:
	//  read input file by 1 symbol
	//	write to output file by Code

	HT.BuildTreeByStat(&arrayStat[0]);

	//printInfo(arrayStat,HT);

	filesize = CalculateSize(arrayStat,HT);
	if (paramVerbose) printf("Размер выходного файла: %ld байт\n", filesize);
	//preparing array of length
	for (i = 0; i < 256; ++i) {
		arrayLengths[i] = HT.GetLength(i);
	}
	res = outputFile.Write(&arrayLengths[0],sizeof(unsigned int)*256);
	fSize = inputFile.GetFileSize();
	kSymb = 0;
	if (paramVerbose) printProcentsBar(kSymb,fSize);
	do {
		inputFile.Read(&curSymbol,sizeof(unsigned char));
		if (inputFile.FEOF() == false) {
			HT.GetCode(curBitCode,(int)curSymbol);
			outputFile.Write(curBitCode);
			if (paramVerbose) printProcentsBar(++kSymb,fSize);
		}
	} while (inputFile.FEOF() == false);

	if (paramVerbose) printProcentsBar(fSize,fSize);
	return 0;

}

int HuffyCompressor::DecompressFile(BitFile &inputFile, BitFile &outputFile, bool paramVerbose) {
	int res;

	long int fSize, kCode;



	unsigned int arrayLengths[256];
	HuffmanTree HT;

	unsigned char curSymbol;
	int extr;
	std::vector<unsigned int> curBitCode;

	//making extracting:
	//	building huffman tree by arrays of lengths
	//  reading input file by 1 bit
	//	use HuffmanTree for recognizing the symbol
	//	writing to output file the symbol


	//loading array of lengths;

	inputFile.Read(&arrayLengths[0],sizeof(unsigned int)*256);

	fSize = ( inputFile.GetFileSize() - sizeof(unsigned int)*256 )*8;
	kCode = 0;

	if (paramVerbose) printProcentsBar(kCode,fSize);



	//building tree
	HT.BuildTreeByLengths(arrayLengths);

	//reading input file by 1 bit
	res=0;
	extr = -1;
	HT.HuffmanSearchInit();
	do {
		res = inputFile.Read(curBitCode,1);

		if (paramVerbose)  printProcentsBar(++kCode,fSize);


		if (res == 0) {
			extr = HT.HuffmanSearch(curBitCode);
			if (extr >= 0) {
				curSymbol = (unsigned char)extr;
				outputFile.Write(&curSymbol,sizeof(unsigned char));
				HT.HuffmanSearchInit();
			}
			else if (extr<-1){
				return -1;
			}

		}
	} while (res == 0 );

	if (paramVerbose) printProcentsBar(fSize,fSize);


	return 0;
}

void printInfo(unsigned int *arrayStat, HuffmanTree& HT) {
	unsigned int  i,j;
	std::vector<unsigned int> curBitCode;
	for (i = 0; i < 256; ++i) {
		if (arrayStat[i]) {
			printf("[%c] (%d) = %d\n",i,i,arrayStat[i]);
		}
	}
	printf("\n массив длин");

	for (i = 0; i < 256; ++i) {
		if (HT.GetLength(i)) {
			   printf("[%c] (%d) = %d\n",i,i,HT.GetLength(i));
		}
	}

	printf("\n кодировки");

	for (i = 0; i < 256; ++i) {
		if (arrayStat[i]) {
			printf("[%c] (%d) = ",i,i);
			HT.GetCode(curBitCode,i);
			for (j = 0; j < curBitCode.size(); ++j) {
				printf("%d",curBitCode[j]);
			}
			printf("\n");
		}
	}

}

unsigned long CalculateSize(unsigned int *stat, HuffmanTree & HT) {
	int i;
	unsigned long res;
	std::vector <unsigned int> curCode;
	for (i = 0, res = 0; i < 256; ++i) {
		if (stat[i] > 0) {
			HT.GetCode(curCode, i);
			res += stat[i]*curCode.size();
		}

	}

	//bits to bytes
	if (res%8 > 0) {
		res += (8 - res%8);
	}
	res = res / 8;

	//adding length's array and last bit info
	res += sizeof(int)*256 + 1;
	return res;
}


