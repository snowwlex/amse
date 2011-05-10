
#ifndef _TEST_H
#define	_TEST_H

#include <stdexcept>
#include <iostream>
#include <vector>
#include <string>


#define TEST( logic_expr ) \
{ \
    ++myQtyTests; \
    if (  logic_expr == false) { \
        addFailure( Failure (#logic_expr, __FILE__, __LINE__) ); \
    } else { \
    ++myQtySuccess; \
    } \
}



class Failure {
public:

    Failure(const char* expr, const char* file, int line) :
    myExpr(expr), myFile(file), myLine(line) {
    }
private:

    std::string myExpr;
    std::string myFile;
    int myLine;

    //friends:

    friend std::ostream & operator<<(std::ostream& stream, const Failure& failure) {
        std::cout << failure.myFile << " at line " << failure.myLine << ": " << failure.myExpr;
	return stream;
    }
};

class Test {
public:
    Test();
    virtual ~Test();
public:
    virtual void run() = 0;
    virtual void report();
protected:
    void addFailure(const Failure& failire);
    void clear();
protected:
    typedef std::vector<Failure> FAILURES;
    FAILURES myFailures;
    int myQtyTests;
    int myQtySuccess;

};

#endif	/* _TEST_H */

