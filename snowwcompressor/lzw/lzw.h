/*
 * lzw.h
 *
 *  Created on: 16.11.2009
 *      Author: alex
 */

#ifndef LZW_H_
#define LZW_H_

class LZWCompressor : public Compressors {

private:
	unsigned int CodeToInt(std::vector<unsigned int>& curCode);
	std::vector<unsigned int> IntToCode(unsigned int integer, unsigned int qBits);

	void printDictionary(std::vector<std::string> & Dictionary);
	void printDictionary(std::map <std::string, unsigned int> & Dictionary);

public:
	int CompressFile(BitFile &inputFile, BitFile &outputFile,bool paramVerbose=0);
	int DecompressFile(BitFile &inputFile, BitFile &outputFile,bool paramVerbose=0);

};



#endif /* LZW_H_ */
