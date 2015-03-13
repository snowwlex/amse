/*
 * filestructure.cpp
 *
 *  Created on: 28.11.2009
 *      Author: alex
 */
#include "stdio.h"
#include <string>
#include <vector>
#include "bitfile.h"
#include "filestructure.h"
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

std::string split(std::string one, std::string two) {
	const char SLASH = '/';
	int length, length2;
	length = one.length();
	length2 = two.length();
	if (length > 0 && one[length-1] != SLASH && length2 > 0 && two[0] != SLASH) {
			one += SLASH;
	}
	one += two;
	return one;
}

int FileStructure::MakeFileTreeR(std::string absPath, std::string relPath, std::string filename, unsigned int parent) {

	DIR *curDirectory;
	struct dirent *folderContent;
	struct stat fileInfo;

	recordFile tmpRecordFile;

	std::string newRelPath, newFilename;

	int result;
	int type;
	unsigned int dir_id;

	std::string fullPath;

	//проверка на . и ..
	if (filename == ".." || filename[0] == '.') {
		return -2;
	}

	//формируем полный путь к файлу
	fullPath = absPath;
	fullPath = split(fullPath, relPath);
	fullPath = split(fullPath, filename);


	//считываем информацию об обрабатываемых данных:
	result = stat( fullPath.c_str(), &fileInfo );
	if (result != 0) {
		return -1; //ОШИБКА доступа к файлу
	}
	type = fileInfo.st_mode & S_IFMT;

	if (type == S_IFREG) { // если обычный файл;
		//формируем запись о файле
		tmpRecordFile.parent = parent;
		tmpRecordFile.filemode = fileInfo.st_mode;
		tmpRecordFile.absPath = absPath;
		tmpRecordFile.filename = filename;
		tmpRecordFile.filesize = fileInfo.st_size;
		FileStruct.push_back(tmpRecordFile);
	}
	else if (type == S_IFDIR ) { //если  директория

		//формируем запись о директории
		dir_id = FileStruct.size()+1;
		tmpRecordFile.parent = parent;
		tmpRecordFile.filemode = fileInfo.st_mode;
		tmpRecordFile.absPath = absPath;
		tmpRecordFile.filename = filename;
		tmpRecordFile.filesize = fileInfo.st_size;
		FileStruct.push_back(tmpRecordFile);

		curDirectory = opendir( fullPath.c_str() );
		do {
			folderContent = readdir (curDirectory);
			if (folderContent != 0) {
				/*
				 * структура dirent
				   unsigned char d_type; - тип файла - если файл то значение =0x8
				   char d_name[256];	 - имя файла
				*/
				newRelPath = split(relPath, filename);
				newFilename = folderContent->d_name;
				MakeFileTreeR(absPath, newRelPath, newFilename , dir_id);
			}
		} while (folderContent != 0);
		closedir(curDirectory);
	}
	return 0;
}
int FileStructure::MakeFileTree(std::vector<std::string> &paramInFiles) {
	std::vector<std::string>::iterator itVector;
	std::string fullPath;
	std::string absPath, relPath, filename;
	size_t found;
	FileStruct.clear();
	for (itVector = paramInFiles.begin(); itVector != paramInFiles.end(); ++itVector) {
		fullPath = *itVector;
		found = fullPath.find_last_of('/');
		if (found == std::string::npos) {
			absPath = "";
			relPath = "";
			filename = fullPath;
		}
		else {
			absPath = fullPath.substr(0,found);
			relPath = "";
			filename =  fullPath.substr(found+1);
		}


		MakeFileTreeR(absPath, relPath, filename,0);
	}
	return 0;
}
int FileStructure::PrintFileTree() {
	std::vector<recordFile>::iterator it;
	std::string filepath;
	unsigned int curId, deep, i;
	__mode_t curMode;
	const char TAB = ' ';
	const unsigned int TAB_SIZE = 3;
	std::string tmpString;
	for (it = FileStruct.begin(), curId = 1; it != FileStruct.end(); ++it, ++curId) {


		curMode = it->filemode;
		if ( getBit(curMode, 17) == 1 ) printf("d"); else printf(" ");
		for (i = 23; i < sizeof(__mode_t)*8-2; i+=3) {
			if ( getBit(curMode, i) == 1)	printf("r"); else	printf("-");
			if ( getBit(curMode, i+1) == 1) printf("w");	else   printf("-");
			if ( getBit(curMode, i+2) == 1) printf("x"); else   printf("-");
		}
		printf("   ");
		if ( (curMode & S_IFMT ) ==  S_IFREG) printf( "%11ld", (long)it->filesize);
		if ( (curMode & S_IFMT ) ==  S_IFDIR) printf("[directory]");

		deep = GetFilePath(filepath, curId);
		deep = (deep-1)*TAB_SIZE;
		for (i = 0, tmpString.clear(); i < deep; ++i) {
			tmpString+=TAB;
		}
		printf(" %s%s ",tmpString.c_str(),it->filename.c_str());
		printf("\n");
	}
	return 0;
}
void FileStructure::WriteToFile(BitFile &file) {
	std::vector<recordFile>::iterator it;
	size_t len;
	unsigned int size;

	size = (int)FileStruct.size();
	file.Write(&size, sizeof(size));
	for (it = FileStruct.begin(); it != FileStruct.end(); ++it) {
			len = it->filename.length();
			file.Write(&it->parent,sizeof(it->parent));
			file.Write(&len,sizeof(len));
			file.Write(it->filename.c_str(),len);
			file.Write(&it->filesize,sizeof(it->filesize));
			file.Write(&it->filemode,sizeof(it->filemode));

	}
}
int FileStructure::ReadFromFile(BitFile &file) {
	std::vector<recordFile>::iterator it;
	size_t len;
	int result;
	recordFile tmpRecordFile;
	const unsigned int MAX_FILENAME_SIZE = 65535;
	char filename[MAX_FILENAME_SIZE];
	unsigned int size,i;

	FileStruct.clear();
	file.Read(&size, sizeof(size));

	for (i = 0, result = 0; i < size && file.FEOF() != true; ++i) {
			file.Read(&tmpRecordFile.parent, sizeof(tmpRecordFile.parent));
			file.Read(&len,sizeof(len));
			file.Read(filename,len);
			filename[len] = '\0';
			tmpRecordFile.filename = filename;
			file.Read(&tmpRecordFile.filesize,sizeof(tmpRecordFile.filesize));
			file.Read(&tmpRecordFile.filemode,sizeof(tmpRecordFile.filemode));
			FileStruct.push_back(tmpRecordFile);
	}
	if (i!=size) {
		errMsg = "Из архива '";
		errMsg += file.GetFilename();
		errMsg +="' не удается прочитать данные о структуре файлов";
		return -1;
	}
	return 0;
}
unsigned int FileStructure::GetFilePath(std::string &filepath, unsigned int curId) {
	recordFile tmpRecordFile;
	unsigned int deep = 0;
	filepath.clear();
	while (curId != 0) {
		tmpRecordFile = FileStruct[curId-1];
		filepath = split(tmpRecordFile.filename, filepath);
		++deep;
		curId = tmpRecordFile.parent;
	}
	return deep;
}

int getBit (__mode_t &mode, int bit) {
	int curBit;
	curBit= ( mode >> (sizeof(mode)*8-bit-1) ) & 1;
	return curBit;
}
