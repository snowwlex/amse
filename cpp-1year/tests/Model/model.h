#ifndef MODEL_H_
#define MODEL_H_

#include <cassert>

#include "snowwchess.h"
#include "rules.h"

class Model {
public:
    Model();
    ~Model();

public: //initialization
    void init(bool newGame);
    void setRules(Rules* rules);


public: //moves methods
    bool makeMove(const SimpleMove& simpleMove);
    bool canMove(const SimpleMove& simpleMove) const;
    MOVES movesFromPosition(const Position& pos1) const;
    MOVES allMoves(int player, int movetype = (MOVE | CAPTURE)) const;

public: //game status
    bool isCheck(int player) const;
    GameStatus checkGameStatus(int player) const;

public: //getters
    BoardCell getBoardCell(int x, int y) const;
    int getFigureIdOnBoard(int x, int y) const;
    int getBoardSizeX() const;
    int getBoardSizeY() const;
    std::string getRulesName() const;
    const FIGURES& getSetFigures(int player) const;
    const FigureData& getFigureData(int figureId) const;
    const FIGURES_DATA& getAllFiguresData() const;
    std::string getPlayerData(int playerId) const;
    int getFirstTurnPlayer() const;
    int getCurrentPlayer() const;
    bool getLastMove(Move& move) const;

public: //setters
    void setFigureOnBoard(int playerId, const Figure& figure);
    void setCurrentPlayer(int playerId);

private: //inner methods
    void innerMakeMove(const Move& move);
    MOVES movesFigure(int player, const Figure& figure, int movetype, bool needCheck = true) const;
    inline Figure& accessFigure(const BoardCell& boardCell);
    inline const Figure& readFigure(const BoardCell& boardCell) const;
    int getDirection(int dir) const;

private: //generator moves' checks
    bool checkIsFree(MoveRule moveRule, Position startPos) const;
    bool checkPosition(MoveRule moveRule, const Figure& figure, Move& move, int movetype, bool needCheck) const;

    bool checkMove(MoveRule moveRule, const Figure& figure, Move& move) const;
    bool checkCapture(MoveRule moveRule, const Figure& figure, Move& move) const;
    bool checkInpassing(MoveRule moveRule, const Figure& figure, Move& move) const;
    bool checkIfCheck(MoveRule moveRule, const Figure& figure, Move& move) const;

    bool checkLongMoveEffect(MoveRule moveRule, const Figure& figure, Move& move) const;
    bool checkCastleEffect(MoveRule moveRule, const Figure& figure, Move& move) const;
    bool checkExplosionEffect(MoveRule moveRule, const Figure& figure, Move& move) const;

private: //makeMoves' methods
    void makeMovePromotion(const Move& move);

    void makeMoveInpassing(const Move& move);
    void makeMoveCapture(const Move& move);

    void makeMoveEffectExplosion(const Move& move);
    void makeMoveEffectLongMove(const Move& move);
    void makeMoveEffectCastle(const Move& move);

private:

    class Board {
    public:
        Board();
        ~Board();
        Board(const Board& board);
        Board & operator=(const Board& board);

    public:
        void init(int sizex, int sizey);
        void setBoardCell(int x, int y, const BoardCell& boardCell);

    public: //sort operator()
        BoardCell operator() (int x, int y) const;
        BoardCell operator() (const Position& pos) const;

    private:
        inline int getCoordinates(int x, int y) const;

    private:
        BoardCell* myBoardArray;
        int mySizeX, mySizeY;

    };

private: //main private fields
    Board myBoard;
    Rules* myRules;

private:
    bool myLastMoveRecorded;
    Move myLastMove;
    int mySpecialFigureSetId[2];
    FIGURES mySetFigures[2];

    int myCurrentPlayer;
    bool initiated;

};
#endif /* MODEL_H_ */
