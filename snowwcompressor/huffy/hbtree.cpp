/*
 * hbtree.cpp
 *
 *  Created on: 18.11.2009
 *      Author: alex
 */
#include <stdio.h>
#include <vector>
#include <string>
#include <list>
#include "../bitfile.h"
#include "../filestructure.h"
#include "../compressor.h"
#include "huffy.h"

HuffmanTree::HuffmanTree() {
    int i;
	myHNode = 0;
    myCurNode = 0;
    for (i = 0; i < 256; ++i) {
    	myArLengths[i] = 0;
    }
}

HuffmanTree::~HuffmanTree() {
    if (myHNode) {
		delete myHNode;
    }

}


void HuffmanTree::BuildTreeByStat(unsigned int *arStat) {

    int i,v;
    Node *curNode=0;
    Node *minNode1, *minNode2;
    int min1, min2;


    std::list<Node*> NodesList;

    //making collection of nodes;
    for (i = 0; i < 256; ++i) {
		v = arStat[i];
		if (v > 0) {
			curNode = new Node(v,i,1);
			NodesList.push_back(curNode);
		}
    }


    while (NodesList.size() > 1) {

		NodesList.sort(nodeCompare);
		minNode1 = NodesList.front();
		NodesList.pop_front();
		minNode2 = NodesList.front();
		NodesList.pop_front();
		min1 = minNode1->GetStat();
		min2 = minNode2->GetStat();
		//making one node from two nodes
		curNode = new Node(min1+min2,'\0',0,minNode1,minNode2);
		NodesList.push_back(curNode);
    }
    myHNode = curNode;

	MakeLengths();
	BuildTreeByLengths(myArLengths);




}

int HuffmanTree::SetLength(int symbol, int value) {

	if (symbol < 0 || symbol >255) {
		return -1; //not correct input data;
	}
	myArLengths[symbol] = value;
	return 0; //success
}

unsigned int HuffmanTree::GetLength(int symbol) const {
	if (symbol < 0 || symbol >255) {
			return 0; //not correct input data;
	}
	return myArLengths[symbol]; //success
}

int HuffmanTree::GetCode(std::vector<unsigned int> & curCode, int symbol) const {
	if (symbol < 0 || symbol > 255) {
		return -1;
	}

	curCode = myArCodes[symbol];
    return 0; //success
}


void HuffmanTree::HuffmanSearchInit() {
	myCurNode = myHNode;
}

int HuffmanTree::HuffmanSearch(std::vector<unsigned int> & curCode) {

	unsigned int bit;
	unsigned char symbol;

	if (myCurNode == 0) {
		HuffmanSearchInit();
	}

	bit = curCode[0];
	if (bit == 0) {
		myCurNode = myCurNode->GetLeft();
	}
	else if (bit == 1) {
		myCurNode = myCurNode->GetRight();
	}

	if (myCurNode == 0) {
		errMsg =  "ошибка при поиске по дереву Хаффмана";
		return -4; //error with search
	}
	if (myCurNode->IsLeaf() == 1) {
		symbol = myCurNode->GetSymbol();
		myCurNode = 0;
		return symbol;
	}
	return -1; // is not list


}


void HuffmanTree::PrintTree(void) {
    if (myHNode != 0) {
	PrintTree(myHNode);
    }
}

void HuffmanTree::PrintTree(Node *curNode) {
    if (curNode!=0) {
	printf("%2d - '%2d',leaf:%d ",curNode->GetStat(),curNode->GetSymbol(), curNode->IsLeaf());
	printf("levyy: ");
	if (curNode->GetLeft()) printf("%2d - '%2d',leaf:%d ",curNode->GetLeft()->GetStat(),curNode->GetLeft()->GetSymbol(), curNode->GetLeft()->IsLeaf());
	else printf("pusto            ");
	printf("pravyy: ");
	if (curNode->GetRight()) printf("%2d - '%2d',leaf:%d ",curNode->GetRight()->GetStat(),curNode->GetRight()->GetSymbol(), curNode->GetRight()->IsLeaf());
	else printf("pusto            ");
	printf("\n");
	PrintTree(curNode->GetLeft());
	PrintTree(curNode->GetRight());
    }
}

void HuffmanTree::MakeLengths(void) {
    MakeLengths(myHNode,0);
}
void HuffmanTree::MakeLengths(Node *curNode, int length) {

    if (curNode) {
	if (curNode->IsLeaf()) {
	   myArLengths[curNode->GetSymbol()] = length;
	}
	else {
	    MakeLengths(curNode->GetLeft(), length+1);
	    MakeLengths(curNode->GetRight(),length+1);
	}
    }

}



void HuffmanTree::BuildTreeByLengths(unsigned int* arLengths) {
    int i,v;
    int res;


	if (arLengths != myArLengths) {
		for (i = 0; i < 256; ++i)
			myArLengths[i] = arLengths[i];
	}

	if (myHNode) {
		delete myHNode;
	}

    myHNode = new Node(0,'\0',0);

    for (i = 0; i < 256; ++i) {
	v = myArLengths[i];
	if (v>0) {
	    res = MakeNodesByLength(myHNode,0,i,v,1);
	    if (res == 1) {
	    	printf("Ошибка построения дерева Хаффмана\n");
	    }
	}
    }

	MakeCodes();


}

int HuffmanTree::MakeNodesByLength(Node *curNode, int curDeep, unsigned char symbol, int deep, bool create) { //implementing of rule: make left, if can't - then right

    int res;

    if (curNode->IsLeaf() == 1) {
	return 1;
    }

    if (curDeep == deep) {
		if (create == 1) {
			curNode->SetSymbol(symbol);
			curNode->SetIsLeaf(1);
			return 0; //succes input
		}
		else {
			return 1; //we can't place in not new nodes
		}
		}


		if (curNode->GetLeft() == 0) {
			curNode->SetLeft( new Node(0,'\0') );
			create = 1;
		}
		else {
			create=0;
		}

		res = MakeNodesByLength(curNode->GetLeft(),curDeep+1,symbol,deep,create);

		if (res != 0) {

		if (curNode->GetRight() == 0) {
		   curNode->SetRight( new Node(0,'\0') );
		   create = 1;
		}
		else {
			create = 0;
		}

		res = MakeNodesByLength(curNode->GetRight(),curDeep+1,symbol,deep,create);
		if (res != 0) {
			return 1;
		}

    }

    return 0;
}

void HuffmanTree::MakeCodes(void) {

    std::vector<unsigned int> tmp_code;
    MakeCodes(myHNode, 0, tmp_code,0);

}


void HuffmanTree::MakeCodes(Node *curNode, int deep, std::vector<unsigned int>& tmp_code, int code) {

    if (curNode) {

		if (curNode->IsLeaf() == 0) {
			if (deep>0) {
			tmp_code.push_back(code);
			}
			MakeCodes(curNode->GetLeft(),deep+1,tmp_code,0);
			MakeCodes(curNode->GetRight(),deep+1,tmp_code,1);
		}
		else {

			 if (deep>0) {
			tmp_code.push_back(code);
			}

			myArCodes[curNode->GetSymbol()] = tmp_code;
		}

		tmp_code.pop_back();

    }




}

/********************************************************************
    Node Class Methods
********************************************************************/

Node::Node(int stat, unsigned char symbol,bool is_leaf,Node *left, Node *right) {

    myStat=stat;
    mySymbol=symbol;
    myIsLeaf=is_leaf;
    myLeft=left;
    myRight=right;

}

Node::~Node() {
    if (myLeft) delete myLeft;
    if (myRight) delete myRight;
}

unsigned char Node::GetSymbol(void) const {
	return mySymbol;
}
int Node::GetStat(void) const {
	return myStat;
}

bool Node::IsLeaf(void) const {
	return myIsLeaf;
}

Node* Node::GetLeft(void) const {
	return myLeft;
}

Node* Node::GetRight(void) const {
	return myRight;
}

int Node::SetLeft(Node* curNode) {
	myLeft = curNode;
	return 0;
}

int Node::SetRight(Node* curNode) {
	myRight = curNode;
	return 0;
}

int Node::SetSymbol(unsigned char symbol) {
	mySymbol = symbol;
	return 0;
}

int Node::SetIsLeaf(bool isLeaf) {
	myIsLeaf = isLeaf;
	return 0;
}


int Node::SetStat(int stat) {
	myStat = stat;
	return 0;
}


bool nodeCompare(Node * const oneNode, Node * const secNode) {
	return oneNode->GetStat() < secNode->GetStat();
}
