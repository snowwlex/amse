/*
 * compressor.cpp
 *
 *  Created on: 16.11.2009
 *      Author: alex
 */

#include <stdio.h>
#include <string>
#include <vector>
#include "bitfile.h"
#include "filestructure.h"
#include "compressor.h"
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

void ShowHelp(HelpSections hs) {

	switch (hs) {
	case ALL:
		printf("Помощь: \n");
		ShowHelp(KEYS);
		ShowHelp(USE_EXPAMLE);
		ShowHelp(AVAILABLE_ARCHIVATORS);
		break;
	case KEYS:
		printf("----------------------------------------------------------\n");
		printf("Описания ключей \n");
		printf("  -f имена_файлов     устанавливает имена файлов, которые нужно сжать \n");
		printf("                      или имя архива, который нужно расжать  \n");
		printf("  -o имя_файла        устанавливает имя выходного файла (в случае сжатия\n");
		printf("                      или имя папки в которую нужно распаковать (в случае расжатия)\n");
		printf("  -a имя_архиватора   устанавливает режим сжатия и назначает архиватор\n");
		printf("  -x имя_архиватора   устанавливает режим распаковки и назначает архиватор\n");
		printf("  -h                  выводит помощь\n");
		printf("  -v                  режим подробного вывода информации\n");
		printf("  -l                  показывает содержимое архива, имя файла которого задается с '-f'\n");
		break;
	case USE_EXPAMLE:
		printf("---------------------------------------------------------\n");
		printf("ПРИМЕР ИСПОЛЬЗОВАНИЯ:\n");
		printf("  - сжимаем файл inf.txt с помощью архиватора Хаффмана, выходной файл inf.huf\n");
		printf("       compressX -a huffy -f inf.txt -o inf.huf\n");
		printf("  - сжимаем папку docs и файл docslist.txt с помощью архиватора LZW, в выходной файл docs.lzw \n");
		printf("       compressX -a lzw -f docs docslist.txt  -o docs.lzw\n");\
		printf("  - просмотрим содержимое архива docs.lzw\n");
		printf("       compressX -l -f docs.lzw\n");
		printf("  - распакуем docs.lzw, в папку myDocs, архиватор lzw:\n");
		printf("       compressX -x lzw -f docs.lzw -o myDocs \n");
		break;
	case AVAILABLE_ARCHIVATORS:
		printf("---------------------------------------------------------\n");
		printf("ПОДДЕРЖИВАЕМЫЕ АРХИВАТОРЫ:\n");
		printf("  'huffy' - архиватор по статическому алгоритму Хаффмана\n");
		printf("  'lzw'   - LZW архиватор\n");
		printf("  'ppm'   - контекстно-моделирующий архиватор\n");
		printf("---------------------------------------------------------\n");
		break;
	case NOT_INPUT_FILES:
		printf("Ошибка: не введены входные файлы.\n");
		ShowHelp(USE_EXPAMLE);
		ShowHelp(NEED_HELP);
		break;
	case NOT_OUTPUT_FILES:
		printf("ОШИБКА: не введен выходной файл (для сжатия) или папка (для распаковки).\n");
		ShowHelp(USE_EXPAMLE);
		ShowHelp(NEED_HELP);
		break;
	case NOT_AVAILABLE_ARCHIVATOR:
		printf("ОШИБКА: Архиватор, введенный вами,  не поддерживается. \n");
		ShowHelp(AVAILABLE_ARCHIVATORS);
		ShowHelp(NEED_HELP);
		break;
	case NOT_INPUT_ARCHIVATOR:
		printf("ОШИБКА: Вы не ввели имя архиватора\n");
		ShowHelp(AVAILABLE_ARCHIVATORS);
		ShowHelp(KEYS);
		ShowHelp(NEED_HELP);
		break;
	case NEED_HELP:
		printf("Наберите 'compressX -h' для помощи.\n");
		break;
	}

}
void Compressors::printProcentsBar(long int done, long int total) {

	int i;
	const char BAR = '*',
			   EMPTY = ' ',
		       PROCENT = '%';
	const int barSize = 30;

	float progress;
	int fillBar;

	progress = (float)done/total;

	fillBar = (int)(progress*barSize);
	progress *= 100;
	if (progress>100) {
		progress = 100;
	}
	printf("[");
	for (i = 0; i < fillBar; ++i) {
		printf("%c",BAR);
	}
	for (i = fillBar; i < barSize; ++i) {
		printf("%c",EMPTY);
	}
	printf("] %.2f %c\r",progress,PROCENT);

}

int Compressors::Compress(std::vector<std::string> paramInFiles, const char *paramOutFile, bool paramVerbose) {

	char tmpChar;
	int result;

	std::vector<FileStructure::recordFile>::iterator it;

    BitFile archFile, tmpFile;
    BitFile inputFile, outputFile;

    long int tmpFileSize;
    long int inputFileSize;
    float procent;
    const char PROCENT = '%';

	FileStructure FS;

	std::string fullPath, filepath;

	unsigned int curId;
	long int filesize;

   	//создаем структуру файлов
	FS.MakeFileTree(paramInFiles);

	//запишем структуру в файл
	result = archFile.Open(paramOutFile, "wb");

	if (result < 0) {
		errMsg = "Архивный файл '";
		errMsg += paramOutFile;
		errMsg += "' нельзя создать";
		return -1;
	}
	FS.WriteToFile(archFile);


	//архивируем файлы, записываем в выходной файл

	for (it = FS.FileStruct.begin(), curId = 1, result = 0; it != FS.FileStruct.end() && result >= 0; ++it, ++curId) {
		if ( (it->filemode & S_IFMT) == S_IFREG ) { // если файл, то архивируем

			fullPath = it->absPath;
			FS.GetFilePath(filepath, curId);
			fullPath = split(fullPath,  filepath);

			result = inputFile.Open(fullPath.c_str(),  "rb");
			if (result == -1) {
				return -1;
			}
			result = tmpFile.OpenTempFile();
			if (result < 0) {
				return -1;
			}
			printf("Сжатие файла '%s' в процессе: \n",fullPath.c_str());
			result = CompressFile(inputFile,tmpFile,paramVerbose);

			if (result < 0) {
				return -1;
			}
			printf("\n");


			tmpFile.Rewind(SEEK_SET);
			//выводим процент сжатия
			tmpFileSize = tmpFile.GetFileSize();
			inputFileSize = inputFile.GetFileSize();
			procent = (float)tmpFileSize / inputFileSize;
			procent *= 100;
			printf("Размер сжатого файла %d%c от исходного\n", (unsigned int)procent, PROCENT);


			//копируем из временного файла содержимое
			filesize = tmpFile.GetFileSize();

			//записываем данные о файле (ID и размер)
			archFile.Write(&curId,sizeof(curId));
			archFile.Write(&filesize,sizeof(filesize));


			do  {
				result = tmpFile.Read(&tmpChar,sizeof(char));
				if (tmpFile.FEOF() == false) {
					result = archFile.Write(&tmpChar,sizeof(char));
				}
			} while (tmpFile.FEOF() == false);

			tmpFile.Close();
			inputFile.Close();
		}
	}


	archFile.Close();
	return 0;
}


int Compressors::Decompress(std::vector<std::string> paramInFiles, const char *paramOutDir, bool paramVerbose) {

	//char tmpChar;
	int result;
	std::vector<std::string>::iterator itVector;
	std::vector<FileStructure::recordFile>::iterator it;

	FileStructure::recordFile tmpRecordFile;
	BitFile archFile, tmpFile;
	BitFile inputFile, outputFile;
	FileStructure FS;

	std::string fullPath, relPath, absPath;

	char tmpChar;
	long int filesize, curLoad;
	unsigned int curId;

	absPath = paramOutDir;

	result = 0;
	for (itVector = paramInFiles.begin(); itVector != paramInFiles.end(); ++itVector) {
		archFile.Open(itVector->c_str(), "rb");

		if (result < 0) {
			return -1;
		}
		//загружаем файловую структуру
		result = FS.ReadFromFile(archFile);

		if (result < 0) {
			return -1;
		}

		//воссоздаем структуру папок
		for (it = FS.FileStruct.begin(), curId = 1, result = 0; it != FS.FileStruct.end() && result >= 0; ++it, ++curId) {
			if ( (it->filemode & S_IFMT) == S_IFDIR ) { 		//если директория
				FS.GetFilePath(relPath, curId);
				fullPath = split(absPath, relPath);
				result = mkdir(fullPath.c_str(), it->filemode); // то создаем ее
			}
		}
		//разархивация файлов:
		while (archFile.FEOF() == false) {
			//считываем ID и размер
			result = archFile.Read(&curId, sizeof(curId));
			result = archFile.Read(&filesize, sizeof(filesize));
			if (archFile.FEOF() == false && curId <= FS.FileStruct.size() ) {
				tmpRecordFile = FS.FileStruct[curId-1];

				if ( (tmpRecordFile.filemode & S_IFMT) == S_IFREG ) { //если обычный файл

					//то создаем TMP-файл
					tmpFile.OpenTempFile();
					for (curLoad = 0; curLoad < filesize && archFile.FEOF() == false ; ++curLoad) {
						result = archFile.Read(&tmpChar,sizeof(char));
						if (archFile.FEOF() == false) {
							result = tmpFile.Write(&tmpChar,sizeof(char));
						}
					}

					if (curLoad < filesize) {
						errMsg = "неожиданный конец архива '";
						errMsg +=archFile.GetFilename();
						errMsg +="'";
						return -1;
					}

					// разархивируем temp файл
					FS.GetFilePath(relPath, curId);
					fullPath = split(absPath, relPath);
					result = tmpFile.Rewind(SEEK_SET);
					if (result == -1) {
						return -1;
					}
					result = outputFile.Open(fullPath.c_str(),"wb");
					if (result == -1) {
						return -1;
					}

					printf("Распаковка файла '%s' в процессе:\n",fullPath.c_str());
					result = DecompressFile(tmpFile,outputFile,paramVerbose);

					outputFile.Close();
					if (result < 0) {
						printf("Ошибка при распаковке %s : %s\n",fullPath.c_str(), errMsg.c_str());
					}
					printf("\n");
					//устанавливаем права файлу
					result = chmod(fullPath.c_str(), tmpRecordFile.filemode);

					printf("Файл успешно распакован\n");
					tmpFile.Close();
				}
			}



		}

		archFile.Close();



	}


	return 0;

}
