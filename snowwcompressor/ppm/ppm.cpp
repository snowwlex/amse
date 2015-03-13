/*
 * lzw.cpp
 *
 *  Created on: 16.11.2009
 *      Author: alex
 */
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <math.h>
#include <stdio.h>
#include "../bitfile.h"
#include "../filestructure.h"
#include "../compressor.h"

#include "ppm.h"
#include "rangecoder.h"
#include "model.h"

const  int ORDER = 2;

std::string cutString(std::string string, int size) {
	std::string::size_type length, maxsize;
	length = string.length();
	maxsize = (std::string::size_type)size;
	if (maxsize < length ) {
		string = string.substr(length-maxsize);
	}
	return string;
}


int PPMCompressor::CompressFile(BitFile &inputFile, BitFile &outputFile, bool paramVerbose) {
	int result;
	unsigned char curSymbol;
	int curOrder,i;
	std::string context;
	ContextModel *CM;
	Intervals curInterval;
	RangeCoder RC;
	unsigned int mask[256];

	unsigned int cumFreq;

	long int fSize, kSymb;

	FILE *f = tmpfile();
	RC.StartEncode(f);

	CM = new ContextModel[ORDER+1];
	for (i = 0; i <= ORDER; ++i) {
		CM[i].SetOrder(i);
	}

	context = "";


	fSize = inputFile.GetFileSize();
	kSymb = 0;
	if (paramVerbose)  printProcentsBar(kSymb,fSize);


	do {
		inputFile.Read(&curSymbol,sizeof(char));




		if ( !inputFile.FEOF() ) {


			if (paramVerbose) printProcentsBar(++kSymb,fSize);

			curOrder = ORDER;
			for (i = 0; i < 256; ++i) {
				mask[i] = 0;
			}
			do {

				result = CM[curOrder].GetInterval(context, curSymbol, curInterval, &mask[0]);
				if (result != 2) { // если контекст существует
					RC.encode(curInterval.cumFreq,curInterval.curFreq,curInterval.totalFreq);
				}
				--curOrder;
			} while ( result != 0  && curOrder>=0); //пока окончательно не закодировали символ и не зашли за границу порядка

			if (result != 0) { //кодируем по равновероятностной модели
				for (i = 0, cumFreq = 0; i < 256; ++i) {
					if (mask[i] == 0) {
						if (i == (int)curSymbol) {
							curInterval.cumFreq = cumFreq;
							curInterval.curFreq = 1;
						}
						cumFreq += 1;
					}
				}
				curInterval.totalFreq = cumFreq + 1;
				RC.encode(curInterval.cumFreq,curInterval.curFreq,curInterval.totalFreq);
			}

			for (curOrder=0; curOrder <= ORDER; ++curOrder) {
				CM[curOrder].UpdateContextModel(context, curSymbol);
			}

			context.push_back(curSymbol);
			context = cutString(context,ORDER);
		}
	} while ( !inputFile.FEOF() );



	 //записываем символ конца файла. Он кодируется как уход в модели -1 порядка
	curOrder = ORDER;
	for (i = 0; i < 256; ++i) {
		mask[i] = 0;
	}

	do {
		result = CM[curOrder].GetInterval(context, curSymbol, curInterval,&mask[0], true);

		if (result != 2) { // если контекст существует
			RC.encode(curInterval.cumFreq,curInterval.curFreq,curInterval.totalFreq);
		}
		--curOrder;
	} while ( curOrder>=0); //пока окончательно не закодировали символ и не зашли за границу порядка
	//кодируем по равновероятностной модели
	for (i = 0, cumFreq = 0; i < 256; ++i) {
		if (mask[i] == 0) {
			cumFreq += 1;
		}
	}
	curInterval.cumFreq = cumFreq;
	curInterval.curFreq = 1;
	curInterval.totalFreq = cumFreq + 1;
	RC.encode(curInterval.cumFreq,curInterval.curFreq,curInterval.totalFreq);


	RC.FinishEncode();

	fseek(f,0,SEEK_SET);

	while (!feof(f)) {
		 result = fread(&curSymbol, sizeof(curSymbol),1,f);
		 outputFile.Write(&curSymbol,sizeof(curSymbol));
	}
	fclose(f);


	delete[] CM;
	return 0;
}


int PPMCompressor::DecompressFile(BitFile &inputFile, BitFile &outputFile,bool paramVerbose) {

	int result;
	unsigned int i;
	long int fSize;
	int curOrder, curDecodeOrder;
	unsigned int cum_freq=0, totFreq, CumFreqUnder;
	unsigned int mask[256];
	bool EndOfFile, found;
	std::string context;
	RangeCoder RC;
	ContextModel *CM;
	Intervals curInterval;
	unsigned char curSymbol;
	FILE *f;


	f = tmpfile();

	while (inputFile.FEOF() == false) {
			 result = inputFile.Read(&curSymbol, sizeof(curSymbol));
			 result = fwrite(&curSymbol, sizeof(curSymbol),1, f);
	}

	fseek(f,0,SEEK_SET);

	//инициализация
	RC.StartDecode(f);

	CM = new ContextModel[ORDER+1];
	for (curOrder = 0; curOrder <= ORDER; ++curOrder) {
		CM[curOrder].SetOrder(curOrder);
	}

	context = "";
	curDecodeOrder = -1;
	totFreq = 257;
	EndOfFile = false;

	for (i = 0; i < 256; ++i) {
		mask[i] = 0;
	}

	fSize = inputFile.GetFileSize();
	if (paramVerbose)  printProcentsBar(0,fSize);


	 do {

		cum_freq = RC.get_freq(totFreq);

		if (curDecodeOrder == -1) {

			for (i = 0, found = false, CumFreqUnder = 0; i < 256; ++i) {

				if (mask[i] == 0) {
					if  ( found == false && (CumFreqUnder + 1) > cum_freq ) {
						curSymbol = (unsigned char)i;
						curInterval.cumFreq = CumFreqUnder;
						curInterval.curFreq = 1;
						found = true;
					}
				CumFreqUnder += 1;
				mask[i] = 1;
				}
			}

			curInterval.totalFreq = CumFreqUnder + 1;

			if (found == false) {
				EndOfFile = true;
				curInterval.curFreq = 1;
				curInterval.cumFreq = CumFreqUnder;
			}



			RC.decode_update(curInterval.cumFreq,curInterval.curFreq, curInterval.totalFreq);

			if (found == true) {
				outputFile.Write(&curSymbol, sizeof(curSymbol));

				for (curOrder=0; curOrder <= ORDER; ++curOrder) {
					CM[curOrder].UpdateContextModel(context, curSymbol);
				}

				if (paramVerbose) printProcentsBar(ftell(f),fSize);


				context.push_back(curSymbol);
				context = cutString(context,ORDER);

				for (i = 0; i < 256; ++i) {
					mask[i] = 0;
				}

				//находим значение максимального порядка, по которому будем декодировать
				curDecodeOrder = ORDER;
				for (curOrder = curDecodeOrder, totFreq = 0; curOrder >= 0 && totFreq == 0; --curOrder) {
					totFreq = CM[curOrder].GetTotalFreq( cutString(context, curOrder));
					curDecodeOrder = curOrder;
				}
				if (totFreq == 0) {
					totFreq = 257;
					curDecodeOrder = -1;
				}
			}

		}
		else {
			result = CM[curDecodeOrder].GetSymbol( cutString(context, curDecodeOrder) , cum_freq,curSymbol,curInterval, &mask[0]);

			RC.decode_update(curInterval.cumFreq,curInterval.curFreq, curInterval.totalFreq);

			if (result == 0) {
				outputFile.Write(&curSymbol, sizeof(curSymbol));

				for (curOrder=0; curOrder <= ORDER; ++curOrder) {
					CM[curOrder].UpdateContextModel(context, curSymbol);
				}

				if (paramVerbose)  printProcentsBar(ftell(f),fSize);

				context.push_back(curSymbol);
				context = cutString(context,ORDER);

				for (i = 0; i < 256; ++i) {
					mask[i] = 0;
				}

				//находим значение максимального порядка, по которому можно декодировать
				curDecodeOrder = ORDER;
				for (curOrder = curDecodeOrder, totFreq = 0; curOrder >= 0 && totFreq == 0; --curOrder) {
					totFreq = CM[curOrder].GetTotalFreq( cutString(context, curOrder));
					curDecodeOrder = curOrder;
				}
				if (totFreq == 0) {
					totFreq = 257;
					curDecodeOrder = -1;
				}

			}
			else {
				--curDecodeOrder;

				if (curDecodeOrder>=0) {
					totFreq = CM[curDecodeOrder].GetTotalFreq( cutString(context, curDecodeOrder) , &mask[0]);
				} else {
					for (i = 0, CumFreqUnder = 0; i<256; ++i) {
						if (mask[i] == 0) {
							CumFreqUnder += 1;
						}
					}
					totFreq = CumFreqUnder + 1;
				}


			}

		}
	}while (!feof(f) && !EndOfFile);


	 if (paramVerbose)  printProcentsBar(fSize,fSize);

	RC.FinishDecode();
	fclose(f);

	delete[] CM;
	return 0;
}


