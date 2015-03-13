/*
 * filestructure.h
 *
 *  Created on: 28.11.2009
 *      Author: alex
 */

#ifndef FILESTRUCTURE_H_
#define FILESTRUCTURE_H_


std::string split(std::string one, std::string two);
int getBit (__mode_t &mode, int bit);

class FileStructure {

public:
	struct recordFile {
		unsigned int parent;
		std::string absPath;
		std::string filename;
		__mode_t filemode;
		off_t filesize;
	};
	 std::vector<recordFile> FileStruct;
private:
	int MakeFileTreeR(std::string absPath, std::string relPath, std::string filename, unsigned int parent);
public:
	int MakeFileTree(std::vector<std::string> &paramInFiles);
	void WriteToFile(BitFile &file);
	int ReadFromFile(BitFile &file);
	int PrintFileTree();
	unsigned int GetFilePath(std::string &filepath, unsigned int id);
};

#endif /* FILESTRUCTURE_H_ */
