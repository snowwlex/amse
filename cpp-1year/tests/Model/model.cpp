#include <algorithm>
#include <cassert>

#include "model.h"

Model::Model() {
    myRules = 0;
    initiated = false;
    myLastMoveRecorded = false;
}

Model::~Model() {
}

void Model::setRules(Rules* rules) {
    myRules = rules;
}

void Model::init(bool newGame) {

    assert(myRules != 0);
    assert(initiated == false);

    initiated = true;
    myBoard.init(myRules->getBoardSizeX(), myRules->getBoardSizeY());

    if (newGame == true) {
        mySetFigures[WHITE] = myRules->getInitFigures(WHITE);
        mySetFigures[BLACK] = myRules->getInitFigures(BLACK);
        myCurrentPlayer = myRules->getFirstTurnPlayer();
    }
    for (int player = 0; player < 2; ++player) {
        std::sort(mySetFigures[player].begin(), mySetFigures[player].end(), *myRules);
    }
    for (int player = 0; player < 2; ++player) {
        int i = 0;
        for (FIGURES::const_iterator itFigure = mySetFigures[player].begin(); itFigure != mySetFigures[player].end(); ++itFigure, ++i) {
            if (itFigure->id == myRules->getSpecialFigure(player)) {
                mySpecialFigureSetId[player] = i;
                break;
            }
        }
    }
    for (int player = 0; player < 2; ++player) {
        int setId = 1;
        for (FIGURES::iterator itFigure = mySetFigures[player].begin(); itFigure != mySetFigures[player].end(); ++itFigure, ++setId) {
            if (itFigure->captured == false) {
                myBoard.setBoardCell(itFigure->position.myX, itFigure->position.myY, BoardCell(setId, player));
            }
        }
    }


}

void Model::setFigureOnBoard(int playerId, const Figure& figure) {
    mySetFigures[playerId].push_back(figure);
}

BoardCell Model::getBoardCell(int x, int y) const {
    return myBoard(x, y);
}

int Model::getFigureIdOnBoard(int x, int y) const {
    BoardCell boardCell = myBoard(x, y);
    if (boardCell == 0) {
        return 0;
    }
    if (boardCell == -1) {
        return -1;
    }
    return readFigure(boardCell).id;
}

int Model::getBoardSizeX() const {
    return myRules->getBoardSizeX();
}

int Model::getBoardSizeY() const {
    return myRules->getBoardSizeY();
}

std::string Model::getRulesName() const {
    return myRules->getRulesName();
}

GameStatus Model::checkGameStatus(int player) const {

    MOVES avMoves;
    bool check;
    avMoves = allMoves(player);
    check = isCheck(player);

    if (avMoves.size() == 0 && check == true) {
        return MATE;
    } else if (avMoves.size() == 0) {
        return STALEMATE;
    } else if (check == true) {
        return CHECK;
    }
    return USUAL;
}

bool Model::isCheck(int player) const {
    bool check;
    MOVES avMoves;
    FIGURES::const_iterator itFigure;
    FIGURES::const_iterator itSpecialFigure;
    MOVES::const_iterator itMove;
    int opponent = 1 - player;

    Figure specialFigure = mySetFigures[player].at(mySpecialFigureSetId[player]);

    for (check = false, itFigure = mySetFigures[opponent].begin(); !check && itFigure != mySetFigures[opponent].end(); ++itFigure) {
        if (itFigure->captured == false) {
            avMoves = movesFigure(opponent, *itFigure, CAPTURE, false);
            for (itMove = avMoves.begin(); !check && itMove != avMoves.end(); ++itMove) {
                if (itMove->pos2 == specialFigure.position) {
                    check = true;
                }
            }
        }
    }
    return check;
}

void Model::makeMoveInpassing(const Move& move) {
    int opponent = 1 - move.player;
    mySetFigures[opponent].at(myBoard(myLastMove.pos2).setId - 1).captured = true;
    myBoard.setBoardCell(myLastMove.pos2.myX, myLastMove.pos2.myY, 0);
}

void Model::makeMoveCapture(const Move& move) {
    int opponent = 1 - move.player;
    mySetFigures[opponent].at(myBoard(move.pos2).setId - 1).captured = true;
}

void Model::makeMovePromotion(const Move& move) {
    int promotionToFigure = myRules->getFigureData(move.figureId).promoting[move.player].figure;
    mySetFigures[move.player].at(myBoard(move.pos2).setId - 1).id = promotionToFigure;
}

void Model::makeMoveEffectLongMove(const Move&) {
    return;
}

void Model::makeMoveEffectCastle(const Move& move) {
    CastleRule castleRule = myRules->getCastleRule(move.pos2.myX, move.pos2.myY, move.player);


    BoardCell boardCell = myBoard(castleRule.rookCellStart);
    if (boardCell.setId <= 0 || boardCell.player != move.player) {
        return; //there is no rook on the place
    }

    // hack make move for castling:
    Move rookMove;
    rookMove.pos1 = castleRule.rookCellStart;
    rookMove.pos2 = castleRule.rookCellEnd;

    accessFigure(boardCell).position = move.pos2;
    accessFigure(boardCell).wasMoved = true;

    myBoard.setBoardCell(move.pos2.myX, move.pos2.myY, myBoard(move.pos1.myX, move.pos1.myY));
    myBoard.setBoardCell(move.pos1.myX, move.pos1.myY, 0);
}

void Model::makeMoveEffectExplosion(const Move& move) {
    Position curPos;
    int figureId;
    BoardCell boardCell;
    for (int i = -1; i < 2; ++i) {
        for (int j = -1; j < 2; ++j) {
            curPos.myX = move.pos2.myX + i;
            curPos.myY = move.pos2.myY + j;
            if ((boardCell = myBoard(curPos)).setId != 0) {
                figureId = mySetFigures[boardCell.player].at(boardCell.setId - 1).id;
                if (curPos == move.pos2 || getFigureData(figureId).explosion == true) {
                    mySetFigures[boardCell.player].at(boardCell.setId - 1).captured = true;
                    myBoard.setBoardCell(curPos.myX, curPos.myY, 0);
                }
            }
        }
    }

}

bool Model::canMove(const SimpleMove& simpleMove) const {

    assert(initiated == true);
    bool accepted;
    MOVES avMoves;
    MOVES::const_iterator itMove;

    avMoves = movesFromPosition(simpleMove.pos1);
    for (accepted = false, itMove = avMoves.begin(); !accepted && itMove != avMoves.end(); ++itMove) {
        if (itMove->player == myCurrentPlayer && itMove->pos1 == simpleMove.pos1 && itMove->pos2 == simpleMove.pos2) {
            accepted = true;
        }
    }
    return accepted;
}

int Model::getFirstTurnPlayer() const {
    return myRules->getFirstTurnPlayer();
}

int Model::getCurrentPlayer() const {
    return myCurrentPlayer;
}

void Model::setCurrentPlayer(int playerId) {
    myCurrentPlayer = playerId;
}

bool Model::getLastMove(Move& move) const {
    if (myLastMoveRecorded == false) return false;
    move = myLastMove;
    return true;
}

bool Model::makeMove(const SimpleMove& simpleMove) {


    assert(initiated == true);

    Move makingMove;

    MOVES avMoves = movesFromPosition(simpleMove.pos1);
    bool accepted;
    MOVES::iterator itMove;
    for (accepted = false, itMove = avMoves.begin(); !accepted && itMove != avMoves.end(); ++itMove) {
        if (itMove->player == myCurrentPlayer && itMove->pos1 == simpleMove.pos1 && itMove->pos2 == simpleMove.pos2) {
            accepted = true;
            makingMove = *itMove;
        }
    }

    if (accepted == true) {
        innerMakeMove(makingMove);
    }

    return accepted;


}

void Model::innerMakeMove(const Move& move) {

    BoardCell boardCell;
    boardCell = myBoard(move.pos1);

 //   assert(move.player == myCurrentPlayer);

    if (move.type == INPASSING) {
        makeMoveInpassing(move);
    } else if (move.type == CAPTURE) {
        makeMoveCapture(move);
    }

    accessFigure(boardCell).position = move.pos2;
    accessFigure(boardCell).wasMoved = true;

    myBoard.setBoardCell(move.pos2.myX, move.pos2.myY, myBoard(move.pos1.myX, move.pos1.myY));
    myBoard.setBoardCell(move.pos1.myX, move.pos1.myY, 0);

    if (myRules->getFigureData(accessFigure(boardCell).id).promoting[move.player].figure != 0 &&
            myRules->getFigureData(accessFigure(boardCell).id).promoting[move.player].horizontal == move.pos2.myY) {
        makeMovePromotion(move);
    }


    if (move.effect == LONGMOVE) {
        makeMoveEffectLongMove(move);
    }
    if (move.effect == CASTLE) {
        makeMoveEffectCastle(move);
    }
    if (move.effect == EXPLOSION) {
        makeMoveEffectExplosion(move);
    }

    myLastMoveRecorded = true;
    myLastMove = move;

    myCurrentPlayer = 1 - myCurrentPlayer;
}

Figure& Model::accessFigure(const BoardCell& boardCell) {
    return mySetFigures[boardCell.player].at(boardCell.setId - 1);
}

const Figure& Model::readFigure(const BoardCell& boardCell) const {
    return mySetFigures[boardCell.player].at(boardCell.setId - 1);
}

MOVES Model::movesFigure(int player, const Figure& figure, int movetype, bool needCheck) const {


    bool accepted;
    int curLimit;
    MOVES avMoves;
    Move move;
    MOVERULES::iterator itRule;
    Position curPos;
    MOVERULES curRules = myRules->getMoveRules(figure.id);

    for (itRule = curRules.begin(); itRule != curRules.end(); ++itRule) {
        if ((movetype == CAPTURE && itRule->moveType == MOVE) || (movetype == MOVE && itRule->moveType == CAPTURE)) {
            // пропускаем правило
        } else if (itRule->player == ALL || itRule->player == player) {
            if (itRule->ruleType == JUMP) {
                accepted = false;
                move.pos1 = figure.position;
                move.pos2.myX = move.pos1.myX + itRule->dx;
                move.pos2.myY = move.pos1.myY + itRule->dy;
                move.player = player;
                accepted = checkPosition(*itRule, figure, move, movetype, needCheck);
                if (accepted == true) {
                    avMoves.push_back(move);
                }
            } else if (itRule->ruleType == SLIDE) {
                curLimit = 0;
                move.pos1 = figure.position;
                move.pos2 = move.pos1;
                bool isFree;
                do {
                    isFree = checkIsFree(*itRule, move.pos2);
                    if (isFree == true) {
                        move.pos2.myX += itRule->dx;
                        move.pos2.myY += itRule->dy;
                        move.player = player;
                        accepted = checkPosition(*itRule, figure, move, movetype, needCheck);
                        if (accepted == true) {
                            avMoves.push_back(move);
                        }
                        ++curLimit;
                    }
                } while (isFree == true && myBoard(move.pos2.myX, move.pos2.myY) == 0 && (itRule->limit == 0 || curLimit < itRule->limit));
            }
        }
    }
    return avMoves;
}

bool Model::checkIsFree(MoveRule moveRule, Position curPos) const {
    bool isFree = true;
    Position endPos;
    int dx, dy;
    endPos = curPos;
    endPos.myX += moveRule.dx;
    endPos.myY += moveRule.dy;
    dx = getDirection(moveRule.dx);
    dy = getDirection(moveRule.dy);
    do {
        curPos.myX += dx;
        curPos.myY += dy;
        if (curPos != endPos && myBoard(curPos.myX, curPos.myY) != 0) {
            isFree = false;
        }
    } while (curPos != endPos && isFree == true);
    return isFree;
}

bool Model::checkPosition(MoveRule moveRule, const Figure& figure, Move& move, int movetype, bool needCheck) const {

    if (myBoard(move.pos2.myX, move.pos2.myY) == -1) {
        return false;
    }
    Position curPos = move.pos2;
    bool accepted = false;

    move.figureId = figure.id;

    if ((movetype & MOVE) && myBoard(move.pos2.myX, move.pos2.myY) == 0 && (moveRule.moveType & MOVE)) {
        accepted = checkMove(moveRule, figure, move);
    } else if ((movetype & CAPTURE) && myBoard(curPos.myX, curPos.myY) > 0 && (moveRule.moveType & CAPTURE)) {
        accepted = checkCapture(moveRule, figure, move);
    } else if ((movetype & CAPTURE) && myBoard(curPos.myX, curPos.myY) == 0 && (moveRule.moveType == INPASSING)) {
        accepted = checkInpassing(moveRule, figure, move);
    }
    if (accepted == true && needCheck == true) {
        accepted = !checkIfCheck(moveRule, figure, move);
    }

    return accepted;
}

bool Model::checkExplosionEffect(MoveRule, const Figure&, Move& move) const {
    move.effect = EXPLOSION;
    return true;
}

bool Model::checkLongMoveEffect(MoveRule, const Figure& figure, Move& move) const {
    if (figure.wasMoved == true) {
        return false;
    }
    move.effect = LONGMOVE;
    return true;
}

bool Model::checkCastleEffect(MoveRule, const Figure& figure, Move& move) const {
    // ISSUE: add a checking for non-checked cells

    return false; //temporaly; because bug with castling

    if (figure.wasMoved == true) {
        return false;
    }
    CastleRule castleRule = myRules->getCastleRule(move.pos2.myX, move.pos2.myY, move.player);

    BoardCell boardCell = myBoard(castleRule.rookCellStart);
    if (boardCell.setId <= 0 || boardCell.player != move.player) {
        return false;
    }
    Figure rook = readFigure(boardCell);
    if (rook.wasMoved == true || rook.captured == true) {
        return false;
    }
    move.effect = CASTLE;
    return true;
}

bool Model::checkCapture(MoveRule moveRule, const Figure& figure, Move& move) const {
    bool accepted;
    BoardCell boardCell;
    boardCell = myBoard(move.pos2);
    if (boardCell.setId <= 0 || boardCell.player == move.player) {
        return false;
    }
    move.type = CAPTURE;
    move.effect = 0;
    move.figureCapturedId = readFigure(boardCell).id;
    if (moveRule.moveEffect == EXPLOSION) {
        accepted = checkExplosionEffect(moveRule, figure, move);
    } else {
        accepted = true;
    }
    return accepted;
}

bool Model::checkIfCheck(MoveRule, const Figure&, Move& move) const {
    Model m = *this;
    m.innerMakeMove(move);
    return m.isCheck(move.player);
}

bool Model::checkMove(MoveRule moveRule, const Figure& figure, Move& move) const {
    bool accepted;
    move.type = MOVE;
    move.effect = 0;
    move.figureCapturedId = 0;
    if (moveRule.moveEffect == LONGMOVE) {
        accepted = checkLongMoveEffect(moveRule, figure, move);
    } else if (moveRule.moveEffect == CASTLE) {
        accepted = checkCastleEffect(moveRule, figure, move);
    } else {
        accepted = true;
    }
    return accepted;
}

bool Model::checkInpassing(MoveRule moveRule, const Figure& figure, Move& move) const {
    bool accepted;
    Position passantCell;
    if (myLastMoveRecorded != true || !(myLastMove.type & INPASSING)) {
        return false;
    }

    if (myLastMove.player == WHITE) {
        passantCell.myX = myLastMove.pos2.myX;
        passantCell.myY = myLastMove.pos2.myY + 1;
    } else {
        passantCell.myX = myLastMove.pos2.myX;
        passantCell.myY = myLastMove.pos2.myY - 1;
    }
    if (passantCell != move.pos2) {
        return false;
    }

    move.type = INPASSING;
    move.effect = 0;
    move.figureCapturedId = readFigure(myBoard(move.pos2)).id;
    if (moveRule.moveEffect == EXPLOSION) {
        accepted = checkExplosionEffect(moveRule, figure, move);
    } else {
        accepted = true;
    }
    return accepted;
}

MOVES Model::movesFromPosition(const Position& pos1) const {

    assert(initiated == true);
    MOVES avMoves;
    BoardCell boardCell;

    boardCell = myBoard(pos1);

    if (boardCell.setId <= 0) {
        return avMoves;
    }

    //qDebug() << ":Model:" << "movesFromPosition() before moesFigure";

    avMoves = movesFigure(boardCell.player, readFigure(boardCell), CAPTURE | MOVE);
    return avMoves;
}

MOVES Model::allMoves(int player, int movetype) const {

    assert(initiated == true);

    MOVES avMoves, allMoves;
    FIGURES::const_iterator itFigure;
    MOVES::const_iterator itMove;

    for (itFigure = mySetFigures[player].begin(); itFigure != mySetFigures[player].end(); ++itFigure) {
        if (itFigure->captured == false) {
            avMoves = movesFigure(player, *itFigure, movetype);
            for (itMove = avMoves.begin(); itMove != avMoves.end(); ++itMove) {
                allMoves.push_back(*itMove);
            }
        }
    }
    return allMoves;
}

int Model::getDirection(int dir) const {
    if (dir > 0) return 1;
    if (dir < 0) return -1;
    return 0;
}

const FIGURES& Model::getSetFigures(int player) const {
    return mySetFigures[player];
}

const FigureData& Model::getFigureData(int figureId) const {
    return myRules->getFigureData(figureId);
}

const FIGURES_DATA& Model::getAllFiguresData() const {
    return myRules->getAllFiguresData();
}

std::string Model::getPlayerData(int playerId) const {
    return myRules->getPlayerData(playerId);
}


