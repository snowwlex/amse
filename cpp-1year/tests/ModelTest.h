/* 
 * File:   ModelTest.h
 * Author: snowwlex
 *
 * Created on 5 Май 2010 г., 23:46
 */

#ifndef _MODELTEST_H
#define	_MODELTEST_H


#include "Test.h"
#include "Model/model.h"
#include "Model/rules.h"

class ModelTest : public Test {
public:
    ModelTest();
    virtual ~ModelTest();
    virtual void run();
    virtual void report();

private: //normal tests
    void usualTest(); //  reaction at normal conditions
    void movingTest(); // generation of 1000 moves and making it; without mate-checking
    void capturingKingTest(); // capture king; should be MATE situation after this

private: //extreme test
    void wrongRules(); // like movingTest, but with unvalid rules

private: // rules setters
    void setSomeRules(Rules& rules); //set some figures and some rules for them; rules are normal
    void setWrongRules(Rules& rules);

};


#endif	/* _MODELTEST_H */

