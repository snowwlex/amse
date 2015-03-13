//============================================================================
// Name        : main.cpp
// Author      : AlexTourkin
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
#include "bitfile.h"
#include "filestructure.h"
#include "compressor.h"
#include "huffy/huffy.h"
#include "lzw/lzw.h"
#include "ppm/ppm.h"




std::string errMsg = "";


int main(int argc, char* argv[]) {

    int curArg,result;
    Compressors *compressor = 0;
	std::vector<std::string>::iterator itVector;
	FileStructure FS;
	BitFile archFile;

	//переменные параметров командной строки
    int paramArchive = 0, paramExtract = 0, paramHelp = 0, paramVerbose = 0;
    int paramList = 0;
    std::vector<std::string> paramInFiles;
    const char *paramOutFile = 0;
    std::string paramCompressor = "";

    //обработка параметров командной строки
    curArg = 1;

    while (curArg < argc) {
		if (argv[curArg][0] == '-') {
			switch(argv[curArg][1]) {

				case 'a':   paramArchive = 1;
							if (curArg+1 < argc && argv[curArg+1][0] != '-') {
								paramCompressor = argv[++curArg];
							}
							break;
				case 'x':
							paramExtract = 1;
							if (curArg+1 < argc && argv[curArg+1][0] != '-') {
								paramCompressor = argv[++curArg];
							}
							break;
				case 'f':
							while ( curArg+1 < argc && argv[curArg+1][0] != '-' ) {
								paramInFiles.push_back( argv[++curArg] );
							}
							break;
				case 'o':
							if (curArg+1 < argc && argv[curArg+1][0] != '-' ) {
								paramOutFile = argv[++curArg];
							}
							break;

				case 'h':   paramHelp = 1;
							break;
				case 'v':	paramVerbose = 1;
							break;
				case 'l':   paramList = 1;
							break;
			}

		}
		++curArg;
    }



	if (paramList) {
		if (paramInFiles.size() > 0) {
			for (itVector = paramInFiles.begin(); itVector != paramInFiles.end(); ++itVector) {
					archFile.Open(itVector->c_str(), "rb");
					printf("Считываем файловую структуру '%s':\n",itVector->c_str());
					//загружаем файловую структуру
					FS.ReadFromFile(archFile);
					//просмотрим получившуюся структуру
					FS.PrintFileTree();
					archFile.Close();
			}
		}
		else {
			ShowHelp(NOT_INPUT_FILES);
			return -1;
		}
	}

	if (paramArchive || paramExtract) {
		if (paramInFiles.size() == 0 ) {
			ShowHelp(NOT_INPUT_FILES);
			return -1;
		}
		if (paramOutFile == 0) {
			ShowHelp(NOT_OUTPUT_FILES);
			return -1;
		}
		if (paramCompressor != "") {

			if (paramCompressor == "huffy") {
				compressor = new HuffyCompressor;
			}
			else if (paramCompressor == "lzw") {
				compressor = new LZWCompressor;
			}
			else if (paramCompressor == "ppm") {
				compressor = new PPMCompressor;
			}
			else {
				ShowHelp(NOT_AVAILABLE_ARCHIVATOR);
				return -1;
			}


			if (paramArchive) {
				result = compressor->Compress(paramInFiles, paramOutFile, paramVerbose);
				if (result < 0) {
					printf("Ошибка: %s\n", errMsg.c_str());
					return -1;
				}
				printf("\nСжатие успешно завершено.\n");
				return 0;
			}
			else if (paramExtract) {
				result = compressor->Decompress(paramInFiles, paramOutFile, paramVerbose);
				if (result < 0) {
					printf("Ошибка: %s\n", errMsg.c_str());
					return -1;
				}
				printf("\nРасжатие успешно завершено\n");
				return 0;
			}

			delete compressor;


		}
		else {
			ShowHelp(NOT_INPUT_ARCHIVATOR);
			return -1;
		}
	}

	if (paramOutFile != 0) {
		ShowHelp(NOT_INPUT_ARCHIVATOR);
		return -1;
	}
	if (paramList == 0) {
		ShowHelp(ALL);
	}

	return 0;
}
