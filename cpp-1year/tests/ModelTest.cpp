/* 
 * File:   ModelTest.cpp
 * Author: snowwlex
 * 
 * Created on 5 Май 2010 г., 23:46
 */

#include <ctime>
#include <cstdlib>

#include "ModelTest.h"

#include "Model/rules.h"
#include "Model/model.h"

ModelTest::ModelTest() {
}

ModelTest::~ModelTest() {
}

void ModelTest::report() {
    std::cout << "ModelTest' reporting:" << std::endl;
    Test::report();
}

void ModelTest::run() {
    Test::clear();
    // normal tests
    usualTest();
    movingTest();
    capturingKingTest();
    //extreme situation test
    wrongRules();

}

void ModelTest::usualTest() {

    Model* model = new Model;
    Rules* rules = new Rules;

    setSomeRules(*rules);


    model->setRules(rules);
    model->init(true);

    TEST(model->allMoves(WHITE).empty() == false);
    TEST(model->checkGameStatus(WHITE) == USUAL);

    TEST(model->getCurrentPlayer() == WHITE);

    TEST(model->movesFromPosition(Position(6, 2)).empty() == true);
    TEST(model->movesFromPosition(Position(2, 7)).empty() == false);

    delete rules;
    delete model;


}

void ModelTest::movingTest() {
    Model* model = new Model;
    Rules* rules = new Rules;

    setSomeRules(*rules);

    model->setRules(rules);
    model->init(true);

    for (int i = 0; i < 1000; ++i) {

        MOVES moves = model->allMoves(model->getCurrentPlayer());
        if (moves.empty() == false) {
            int qtyMoves = moves.size();
            Move move = moves[ rand() % qtyMoves ];
            TEST(model->canMove(move) == true);
            model->makeMove(move);
        } else {
            TEST(model->checkGameStatus(model->getCurrentPlayer()) != USUAL);
        }
        GameStatus status = model->checkGameStatus(model->getCurrentPlayer());
        if ( status  == MATE ||status == STALEMATE) {
            break;
        }
    }

    delete rules;
    delete model;


}

void ModelTest::capturingKingTest() {
    Model* model = new Model;
    Rules* rules = new Rules;
    setSomeRules(*rules);

    model->setRules(rules);
    model->init(true);

    TEST(model->checkGameStatus(BLACK) == CHECK);
    SimpleMove sMove;
    sMove.pos1 = Position(2, 2); //WHITE QUEEN
    sMove.pos2 = Position(2, 7); //BLACK KING
    TEST(model->canMove(sMove) == true);
    TEST(model->makeMove(sMove) == true);

    TEST(model->checkGameStatus(BLACK) == MATE);

    delete rules;
    delete model;
}

void ModelTest::wrongRules() {
    Model* model = new Model;
    Rules* rules = new Rules;

    setWrongRules(*rules);

    model->setRules(rules);
    model->init(true);

    for (int i = 0; i < 1000; ++i) {
        MOVES moves = model->allMoves(model->getCurrentPlayer());
        if (moves.empty() == false) {
            int qtyMoves = moves.size();
            Move move = moves[ rand() % qtyMoves ];
            model->makeMove(move);
        }
    }

    delete rules;
    delete model;

}

void ModelTest::setSomeRules(Rules& rules) { //set some figures and some rules for them

    rules.setRulesName("Test Rules 1");
    rules.setFirstTurnPlayer(WHITE);
    rules.setSpecialFigure(WHITE, 1);
    rules.setSpecialFigure(BLACK, 1);
    rules.setBoardSize(8, 8);

    rules.setInitFigure(WHITE, Figure().setInfo(1, Position(0, 0))); //KING
    rules.setInitFigure(WHITE, Figure().setInfo(2, Position(2, 2)));

    rules.setInitFigure(BLACK, Figure().setInfo(1, Position(2, 7))); //KING
    rules.setInitFigure(BLACK, Figure().setInfo(2, Position(6, 5)));
    rules.setInitFigure(BLACK, Figure().setInfo(2, Position(6, 7)));
    rules.setInitFigure(BLACK, Figure().setInfo(2, Position(5, 4)));
    rules.setInitFigure(BLACK, Figure().setInfo(2, Position(7, 1)));

    //    updateMoveRules(rules):
    // parameters for setData:
    //(int _dx, int _dy, 
    //RuleType _ruleType: JUMP, SLIDE,
    //int moveType:  CAPTURE, MOVE, INPASSING ,
    //int _player : WHITE, BLACK, ALL ,
    //int _limit, int _moveEffect

    FigureData fd;
    fd.letter = 'K';
    fd.name = "King";
    fd.weight = 100;
    rules.setFigureData(1, fd);
    fd.letter = 'O';
    fd.name = "Queen";
    fd.weight = 10;
    rules.setFigureData(2, fd);

    rules.setPlayerData(WHITE, "WHITE");
    rules.setPlayerData(BLACK, "BLACK");

    //KING
    rules.setMoveRule(1, MoveRule().setData(1, 0, JUMP));
    rules.setMoveRule(1, MoveRule().setData(-1, 0, JUMP));
    rules.setMoveRule(1, MoveRule().setData(0, 1, JUMP));
    rules.setMoveRule(1, MoveRule().setData(0, -1, JUMP));
    rules.setMoveRule(1, MoveRule().setData(1, 1, JUMP));
    rules.setMoveRule(1, MoveRule().setData(-1, -1, JUMP));
    rules.setMoveRule(1, MoveRule().setData(1, -1, JUMP));
    rules.setMoveRule(1, MoveRule().setData(-1, 1, JUMP));

    //QUEEN
    rules.setMoveRule(2, MoveRule().setData(1, 1, SLIDE));
    rules.setMoveRule(2, MoveRule().setData(1, -1, SLIDE));
    rules.setMoveRule(2, MoveRule().setData(-1, 1, SLIDE));
    rules.setMoveRule(2, MoveRule().setData(-1, -1, SLIDE));
    rules.setMoveRule(2, MoveRule().setData(1, 0, SLIDE));
    rules.setMoveRule(2, MoveRule().setData(-1, 0, SLIDE));
    rules.setMoveRule(2, MoveRule().setData(0, 1, SLIDE));
    rules.setMoveRule(2, MoveRule().setData(0, -1, SLIDE));


}

void ModelTest::setWrongRules(Rules& rules) { //set wrong rules

    rules.setRulesName("Test Rules 2");
    rules.setFirstTurnPlayer(WHITE);
    rules.setSpecialFigure(WHITE, 1);
    rules.setSpecialFigure(BLACK, 2); // QUEEN is special figure for BLACK
    rules.setBoardSize(8, 8);

    rules.setInitFigure(WHITE, Figure().setInfo(1, Position(0, 0))); //KING
    rules.setInitFigure(WHITE, Figure().setInfo(2, Position(2, 2)));

    rules.setInitFigure(BLACK, Figure().setInfo(1, Position(2, 7))); //KING
    rules.setInitFigure(BLACK, Figure().setInfo(2, Position(6, 5)));
    rules.setInitFigure(BLACK, Figure().setInfo(2, Position(2, 7)));
    rules.setInitFigure(BLACK, Figure().setInfo(2, Position(5, 4)));
    rules.setInitFigure(BLACK, Figure().setInfo(2, Position(7, 1)));

    //    updateMoveRules(rules):
    // parameters for setData:
    //(int _dx, int _dy,
    //RuleType _ruleType: JUMP, SLIDE,
    //int moveType:  CAPTURE, MOVE, INPASSING ,
    //int _player : WHITE, BLACK, ALL ,
    //int _limit, int _moveEffect

    FigureData fd;
    fd.letter = 'K';
    fd.name = "King";
    fd.weight = 100;
    rules.setFigureData(1, fd);
    fd.letter = 'O';
    fd.name = "Queen";
    fd.weight = 10;
    rules.setFigureData(2, fd);

    rules.setPlayerData(WHITE, "WHITE");
    rules.setPlayerData(BLACK, "BLACK");


    // move rules are  incorrect:


    //KING
    rules.setMoveRule(1, MoveRule().setData(-8, 8, JUMP));
    rules.setMoveRule(1, MoveRule().setData(13, 12, JUMP));
    rules.setMoveRule(1, MoveRule().setData(1, 0, SLIDE));

    //QUEEN
    rules.setMoveRule(2, MoveRule().setData(16, 3, SLIDE));
    rules.setMoveRule(2, MoveRule().setData(13, 25, JUMP));
    rules.setMoveRule(2, MoveRule().setData(8, -8, SLIDE));




}