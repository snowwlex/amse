/*
 * ppm.h
 *
 *  Created on: 16.11.2009
 *      Author: alex
 */

#ifndef PPM_H_
#define PPM_H_

extern const  int ORDER;

std::string cutString(std::string string, int size);

struct Intervals {
	unsigned int cumFreq;
	unsigned int curFreq;
	unsigned int totalFreq;
};


class PPMCompressor : public Compressors {

private:

public:
	int CompressFile(BitFile &inputFile, BitFile &outputFile,bool paramVerbose=0);
	int DecompressFile(BitFile &inputFile, BitFile &outputFile,bool paramVerbose=0);

};



#endif /* PPM_H_ */
