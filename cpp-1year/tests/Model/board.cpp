#include <cstring> //for memcpy() function
#include "board.h"

Model::Board::Board(): myBoardArray(0)  { }

void Model::Board::init(int sizex, int sizey) {
	mySizeX = sizex;
	mySizeY = sizey;
	if (myBoardArray == 0) {
		myBoardArray = new BoardCell[ mySizeY * mySizeX ];
	}
}
Model::Board::~Board() {
	if (myBoardArray != 0) {
		delete[] myBoardArray;
	}
}

Model::Board::Board(const Board& board) {
	mySizeX=board.mySizeX;
	mySizeY=board.mySizeY;
	myBoardArray = new BoardCell[ mySizeX * mySizeY ];
	std::memcpy(myBoardArray, board.myBoardArray, sizeof(BoardCell)*mySizeX*mySizeY );
}

Model::Board& Model::Board::operator=(const Board& board) {
	if (myBoardArray != 0) {
		delete[] myBoardArray;
	}
	mySizeX=board.mySizeX;
	mySizeY=board.mySizeY;
	myBoardArray = new BoardCell[ mySizeX * mySizeY ];
	std::memcpy(myBoardArray, board.myBoardArray, sizeof(BoardCell)*mySizeX*mySizeY );
	return *this;
}


BoardCell Model::Board::operator() (int x,int y) const {
	if ( x<0 || x >= mySizeX ||
	     y<0 || y >= mySizeY ) {
		return BoardCell(-1);
	}
	return myBoardArray[getCoordinates(x,y)];
}

BoardCell Model::Board::operator() (const Position& pos) const {
	if ( pos.myX<0 || pos.myX >= mySizeX ||
	     pos.myY<0 || pos.myY >= mySizeY ) {
		return BoardCell(-1);
	}
	return myBoardArray[getCoordinates(pos.myX,pos.myY)];
}

inline int Model::Board::getCoordinates(int x,int y) const {
	return x*mySizeY+y;
}

void Model::Board::setBoardCell(int x,int y,const BoardCell& boardCell) {
	myBoardArray[getCoordinates(x,y)] = boardCell;
}
