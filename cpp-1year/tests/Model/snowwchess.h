#ifndef SNOWWCHESS_H_
#define SNOWWCHESS_H_

#include <iostream>
#include <map>
#include <string>
#include <vector>

enum GameStatus {
    USUAL, CHECK, MATE, STALEMATE
};

enum PlayerColor {
    WHITE = 0, BLACK = 1, ALL = 2
};

enum PlayerType {
    HUMAN = 0, AI
};

enum RuleType {
    JUMP, SLIDE
};

enum MoveType {
    CAPTURE = 1, MOVE = 2, INPASSING = 4
};

enum MoveEffect {
    LONGMOVE = 1, CASTLE = 2, EXPLOSION = 4
};

enum PlayerCommand {
    NOTHING, TURN, SAVE, EXIT, UNDO
};

enum GameMessage {
    NONE, WRONG_MOVE, GOT_CHECK
};

struct Position {
    int myX;
    int myY;

    Position(int x = 0, int y = 0) : myX(x), myY(y) {
    }

    bool operator!=(const Position & pos) const {
        return myX != pos.myX || myY != pos.myY;
    }

    bool operator==(const Position & pos) const {
        return myX == pos.myX && myY == pos.myY;
    }
};

struct Figure {
    int id;
    Position position;
    bool wasMoved;
    bool captured;

    Figure(int _id = 0) {
        id = 0;
        wasMoved = false;
        captured = false;
    }

    const Figure& setInfo(int _id, Position pos) {
        id = _id;
        position = pos;
        return *this;
    }

    bool operator<(const Figure & figure) const {
        return id < figure.id;
    }
};

struct BoardCell {
    int setId;
    int player;

    BoardCell(int id = 0, int pl = -1) : setId(id), player(pl) {
    }

    bool operator==(int id) {
        return setId == id;
    }

    bool operator!=(int id) {
        return setId != id;
    }

    bool operator>(int id) {
        return setId > id;
    }
};

struct Promoting {
    int horizontal;
    int figure;

    inline Promoting() {
        horizontal = 0;
        figure = 0;
    }

};

struct FigureData {
    std::string name;
    char letter;
    bool explosion;
    int weight;
    Promoting promoting[2];
    std::string picture[2];

    FigureData() {
        name = "";
        letter = 0;
        weight = 0;
        explosion = true;
    }
   
};

struct SimpleMove {
    Position pos1;
    Position pos2;
};
struct Move : public SimpleMove {
    int player;
    int type;
    int effect;
    int figureId;
    int figureCapturedId;

    Move() {
        player = 0;
        type = 0;
        effect = 0;
        figureId = 0;
        figureCapturedId = 0;
    }

    bool operator<(const Move & move) const {
        return type < move.type;
    }
};

struct MoveRule {
    int dx;
    int dy;
    RuleType ruleType;
    int moveType;
    int player;
    int limit;
    int moveEffect;

    MoveRule() {
        dx = 0;
        dy = 0;
        ruleType = SLIDE;
        moveType = 0;
        player = 0;
        limit = 0;
        moveEffect = 0;
    }

    const MoveRule& setData(int _dx, int _dy, RuleType _ruleType, int _moveType= CAPTURE | MOVE, int _player=ALL, int _limit=0, int _moveEffect=0) {
        dx = _dx;
        dy = _dy;
        ruleType = _ruleType;
        moveType = _moveType;
        player = _player;
        limit = _limit;
        moveEffect = _moveEffect;
        return *this;
    }
};

struct CastleRule {
    int dx;
    int dy;
    int player;
    Position kingCell;
    Position rookCellStart, rookCellEnd;

    CastleRule() {
        dx = 0;
        dy = 0;
        player = 0;
    }
};

typedef std::vector<Move> MOVES;
typedef std::vector<Figure> FIGURES;
typedef std::vector<MoveRule> MOVERULES;
typedef std::map<int, MOVERULES > FIGURES_RULES;
typedef std::map<int, FigureData > FIGURES_DATA;
typedef std::map<int, std::string> PLAYERS_DATA;
typedef std::vector<CastleRule> CASTLERULES;

#endif /* SNOWWCHESS_H_ */
