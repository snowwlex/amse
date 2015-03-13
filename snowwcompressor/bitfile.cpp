/*
 * bitfile.cpp
 *
 *  Created on: 17.11.2009
 *      Author: alex
 */
#include <stdio.h>
#include <vector>
#include <string>
#include "bitfile.h"

BitFile::BitFile() : block_size(8*sizeof(block)) {
    myMode = 'n';
    myFile = 0;
    myFilename = "";
    hasBuffer = false;
    myFEOF = true;
}
BitFile::~BitFile() {

	if (myFile) {
		Close();
	}

}
long int BitFile::GetFileSize() {
	if (myFile == 0 || myMode == 'w') {
		return -1;
	}
	return fileSize;

}
int BitFile::Open(const char* filename, const char* mode) {


	if (myFile) {
		Close();
	}

	myFile = fopen(filename, mode);
	if (myFile) {
		myFilename = filename;
		myMode = mode[0];
		hasBuffer = false;
		if (myMode == 'r') {
			fseek(myFile,0,SEEK_END);
			fileSize = ftell(myFile);
			fseek(myFile,0,SEEK_SET);
		}
		myFEOF = false;
		return 0; //success
	}
	errMsg = "Не получилось открыть файл '";
	errMsg += filename;
	errMsg +="'";
	return -1; //fail
}
int BitFile::OpenTempFile() {
	if (myFile != 0) {
		Close();
	}

	myFile = tmpfile();
	if (myFile) {
		myFilename = "временный файл";
		myFEOF = false;
		myMode = 't';
		fileSize = 0;
		hasBuffer = false;
		return 0; //success
	}
	errMsg = "Не получилось создать временный файл";
	return -1; //fail
}
void BitFile::Close(void) {

	int res;

	if (myFile != 0) {

		//check if we don't write all Buffer's bits
		if (myMode == 'w') {
			if (hasBuffer) {
				lastBit();
			}
		}

		res = fclose(myFile);
		if (res == 0) {
			myFile = 0;
			myFilename = "";
			myFEOF = true;
		}

	}
}
void BitFile::lastBit() {
	lastBits = 0;
	unsigned int i;
	if (myBuffer.size() > 0) { //if buffer is not empty, filling with 0 and counting quantity of nulls

		while (myBuffer.size() < block_size) {
			myBuffer.push_back(0);
			lastBits++;
		}

		for (i = 0, block = 0; i < block_size; i++) {
			block|=myBuffer[i] << (block_size-i-1);
		}

		fwrite(&block,sizeof(block),1,myFile);
		myBuffer.clear();
	}
	//adding info about quantity of last nulls
	Write(&lastBits, sizeof(lastBits));
	hasBuffer = false;

}
int BitFile::Read(void * ptr, size_t size) {

	size_t res;
	if (myFile && (myMode == 'r' || myMode == 't') ) {

		res = fread(ptr, size, 1, myFile);
		if (feof(myFile)) {
			myFEOF = true;
			return 0;
		}
		else if (res != 1 ) {
			errMsg = "Не получается прочитать из файла '";
			errMsg +=myFilename + "'";
			return -1;
		}
		else {
			return 0;
		}
	}
	errMsg = "Файл '";
	errMsg += myFilename;
	errMsg +="' не открыт, или открыт не для чтения";
	return -1;

}
int BitFile::Read(std::vector<unsigned int>& curCode, unsigned int qBits) {


	unsigned int i, curBit,mask, limit;
	bool unexp_end;
	size_t res;
	long file_pos;

	if (myFile && (myMode == 'r' || myMode == 't') ) {
		unexp_end = 0;

		// if Buffer don't enough:
		while ( (myBuffer.size() < qBits) && !unexp_end ) {

			res = fread(&block,sizeof(block),1,myFile);

			if (!feof(myFile)) {


				// last Byte problem:
				limit = block_size;
				file_pos = ftell(myFile);
				if (file_pos == (fileSize - 1) ) {
					res = fread(&lastBits,sizeof(lastBits),1,myFile);
					if (lastBits<=8) {
						limit -= (unsigned int) lastBits;
					}

				}


				for (i = 0; i < limit; ++i) {
					mask = 1 << (block_size-i-1);
					curBit = (block&mask) >> (block_size-i-1);
					myBuffer.push_back(curBit);
				}
			}
			else	{

				unexp_end = true;
			}
		}

		// reading from Buffer
		if (!unexp_end) {
			curCode.clear();
			for (i = 0; i < qBits; ++i) {
				curCode.push_back(myBuffer[i]);
			}
			myBuffer.erase(myBuffer.begin(),myBuffer.begin()+qBits);
			return 0; //success
		}

		curCode = myBuffer;
		myFEOF = true;
		return 1; // file eof
	}
	errMsg = "Файл '";
	errMsg += myFilename;
	errMsg +="' не открыт, или открыт не для чтения";
	return -2;
}
int BitFile::Write(const void * ptr, size_t size) {
	if (myFile && (myMode == 'w' || myMode == 't') ) {

		fwrite(ptr, size, 1, myFile);
		return 0;
	}
	errMsg = "Файл '";
	errMsg += myFilename;
	errMsg +="' не открыт, или открыт не для записи";
	return -1;

}
int BitFile::Write(std::vector<unsigned int> curCode) {
	unsigned int i;
	std::vector<unsigned int>::iterator itCode;
	if (myFile && (myMode == 'w' || myMode == 't') ) {
		hasBuffer = true;
		for (itCode = curCode.begin(); itCode != curCode.end(); ++itCode) {
			myBuffer.push_back(*itCode);
		}

		while (myBuffer.size() >= block_size) { //writing content of buffer in file
			for (i = 0, block = 0; i < block_size; i++) {
				block|= (myBuffer[i] << (block_size-i-1) );
			}
			myBuffer.erase(myBuffer.begin(),myBuffer.begin()+block_size);

			fwrite(&block,sizeof(block),1,myFile);

		}
		return 0; //success
	}
	errMsg = "Файл '";
	errMsg += myFilename;
	errMsg +="' не открыт, или открыт не для чтения";
	return -1;
}
int BitFile::Rewind(int whence) {
	if (myFile == 0) {
		errMsg = "Файл не открыт";
		return -1;
	}
	if (myMode == 't' && whence == SEEK_SET) {
		if (hasBuffer) {
			lastBit();
		}
		fileSize = ftell(myFile);

	}
	fseek(myFile,0,whence);
	myFEOF = false;
	return 0;

}

std::string BitFile::GetFilename() {
	return myFilename;
}
bool BitFile::FEOF() {
	return myFEOF;
}

