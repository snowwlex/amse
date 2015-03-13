/*
 * huffy.h
 *
 *  Created on: 16.11.2009
 *      Author: alex
 */

#ifndef HUFFY_H_
#define HUFFY_H_

class HuffyCompressor : public Compressors {

public:
	int CompressFile(BitFile &inputFile, BitFile &outputFile, bool paramVerbose=0);
	int DecompressFile(BitFile &inputFile, BitFile &outputFile, bool paramVerbose=0);

};


class Node {

private:

    Node *myLeft;
    Node *myRight;

    unsigned char mySymbol;
    int myStat;
    bool myIsLeaf;

    Node(const Node& node);
    Node& operator=(const Node& ar);


public:

    Node(int stat=0, unsigned char symbol='\0',bool is_leaf=0,Node *left=0, Node *right=0);
    ~Node();


	unsigned char GetSymbol(void) const;
	int GetStat(void) const;
	bool IsLeaf(void) const;
	Node* GetLeft(void) const;
	Node* GetRight(void) const;

	int SetLeft(Node* curNode);
	int SetRight(Node* curNode);
	int SetSymbol(unsigned char symbol);
	int SetIsLeaf(bool isLeaf);
	int SetStat(int stat);


};



class HuffmanTree {

private:
    Node *myHNode;
	Node *myCurNode;

    unsigned int myArLengths[256];
    std::vector<unsigned int> myArCodes[256];


    HuffmanTree(HuffmanTree& ht); //copy constructor is denied
    HuffmanTree& operator=(const HuffmanTree& ht); //equal operator is denied


    void MakeCodes(void);
    void MakeCodes(Node *curNode, int deep, std::vector<unsigned int>& tmp_code, int code);

    int MakeNodesByLength(Node *curNode, int curDeep, unsigned char symbol, int deep, bool create);
    void MakeLengths(Node *curNode,int length);
    void MakeLengths(void);

    void PrintTree(Node *hNode);

public:
    HuffmanTree();
    ~HuffmanTree();

    void BuildTreeByStat(unsigned int *arStat);
    void BuildTreeByLengths(unsigned int *arLengths);

 	void HuffmanSearchInit();
    int HuffmanSearch(std::vector<unsigned int> &curCode);

    unsigned int GetLength(int symbol) const;  //int -> char
    int GetCode(std::vector<unsigned int>& curCode, int symbol) const; // int -> char

	int SetLength(int symbol, int value);

    void PrintTree(void);

};




bool nodeCompare(Node *oneNode, Node *secNode);

void MakeStat(int *stat,BitFile &inputFile);
void printInfo(unsigned int *arrayStat, HuffmanTree& HT);
unsigned long CalculateSize(unsigned int *stat, HuffmanTree& HT);

#endif /* HUFFY_H_ */
