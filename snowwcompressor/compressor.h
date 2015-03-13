/*
 * compressor.h
 *
 *  Created on: 16.11.2009
 *      Author: alex
 */

#ifndef COMPRESSOR_H_
#define COMPRESSOR_H_



enum HelpSections { ALL, KEYS, USE_EXPAMLE, AVAILABLE_ARCHIVATORS, NOT_INPUT_FILES, NOT_OUTPUT_FILES, NOT_AVAILABLE_ARCHIVATOR, NOT_INPUT_ARCHIVATOR, NEED_HELP};
void ShowHelp(HelpSections hs);


class Compressors {

protected:
	void printProcentsBar(long int done, long int total);
public:
	virtual int CompressFile(BitFile &inputFile, BitFile &outputFile, bool paramVerbose=0) = 0;
	virtual int DecompressFile(BitFile &inputFile, BitFile &outputFile, bool paramVerbose=0) = 0;

	int Compress(std::vector<std::string> paramInFiles, const char *paramOutFile, bool paramVerbose=0);
	int Decompress(std::vector<std::string> paramInFiles, const char *paramOutDir, bool paramVerbose=0);
};



#endif /* COMPRESSOR_H_ */
