#include <cassert>
#include <stdexcept>
#include "rules.h"

Rules::Rules() {
}

void Rules::setRulesName(std::string rulesName) {
    myRulesName = rulesName;
}

std::string Rules::getRulesName() const {
    return myRulesName;
}

void Rules::setBoardSize(int sizex, int sizey) {
    myBoardSizeX = sizex;
    myBoardSizeY = sizey;
}

void Rules::setFirstTurnPlayer(int firstTurnId) {
    myFirstTurn = firstTurnId;
}

int Rules::getFirstTurnPlayer() const {
    return myFirstTurn;
}

void Rules::setMoveRule(int figureId, const MoveRule& moverule) {
    myMoveRules[figureId].push_back(moverule);
}

void Rules::setInitFigure(int playerId, const Figure& figure) {
    myInitFigures[playerId].push_back(figure);
}

void Rules::setFigureData(int figureId, const FigureData& figureData) {
    myFiguresData[figureId] = figureData;
}

const FIGURES& Rules::getInitFigures(int playerId) const {
    return myInitFigures[playerId];
}

const FigureData& Rules::getFigureData(int figureId) const {
    FIGURES_DATA::const_iterator it = myFiguresData.find(figureId);
    if (it == myFiguresData.end()) {
        throw std::logic_error(":Rules: getFigureData(): No fugure data for this figureId!");
    }
    return it->second;
}

const FIGURES_DATA& Rules::getAllFiguresData() const {
    return myFiguresData;
}

const MOVERULES& Rules::getMoveRules(int figureId) const {
    FIGURES_RULES::const_iterator it = myMoveRules.find(figureId);
    if (it == myMoveRules.end()) {
        throw std::logic_error(":Rules: getMoveRules(): No move rules for this figureId!");
    }
    return it->second;
}

std::string Rules::getPlayerData(int playerId) const {
    PLAYERS_DATA::const_iterator it = myPlayersData.find(playerId);
    if (it == myPlayersData.end()) {
        throw std::logic_error(":Rules: getPlayerData(): No such player!");
    }
    return it->second;
}

int Rules::getSpecialFigure(int player) const {
    return mySpecialFigureId[player];
}

void Rules::setPlayerData(int playerId, std::string name) {
    myPlayersData[playerId] = name;
}

void Rules::setSpecialFigure(int player, int figureId) {
    mySpecialFigureId[player] = figureId;
}

int Rules::getBoardSizeX() const {
    return myBoardSizeX;
}

int Rules::getBoardSizeY() const {
    return myBoardSizeY;
}

const CastleRule& Rules::getCastleRule(int x, int y, int player) const {
    for (CASTLERULES::const_iterator it = myCastlesRules.begin(); it != myCastlesRules.end(); ++it) {
        if (Position(x, y) == it->kingCell && player == it->player) {
            return *it;
        }
    }

    assert(false);
    return *(myCastlesRules.begin());
}

void Rules::setCastleRule(const CastleRule& castlerule) {
    myCastlesRules.push_back(castlerule);
}
