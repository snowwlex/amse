/*
 * bitfile.h
 *
 *  Created on: 17.11.2009
 *      Author: alex
 */

#ifndef BITFILE_H_
#define BITFILE_H_

extern std::string errMsg;

class BitFile {

private:

	FILE *myFile;
	char myMode;

	std::string myFilename;
	bool myFEOF;

	long int fileSize;
	bool hasBuffer;
	std::vector<unsigned int> myBuffer;


	unsigned char block;
	unsigned int block_size;

	unsigned char lastBits;
	void lastBit();

    BitFile(const BitFile& file);
    BitFile& operator=(const BitFile& file);


public:

	BitFile();
	~BitFile();

	int Open(const char *filename, const char* mode);
	int OpenTempFile();
	int Write(std::vector<unsigned int> curCode);
	int Write(const void * ptr, size_t size);

	long int GetFileSize();
	std::string GetFilename();
	bool FEOF();

	int Read(void * ptr, size_t size);
	int Read(std::vector<unsigned int>& curCode, unsigned int qBits);

	int Rewind(int whence);

	void Close(void);

};

#endif /* BITFILE_H_ */
