
#include "Test.h"

Test::Test() {
    clear();
}


Test::~Test() {
}

void Test::clear() {
    myFailures.clear();
    myQtyTests = 0;
    myQtySuccess = 0;
}


void Test::report() {

    std::cout << "Errors report: " << std::endl;
    int i=1;
    for (FAILURES::const_iterator itFail = myFailures.begin(); itFail != myFailures.end(); ++itFail, ++i) {
        std::cout << i << ". " << *itFail << std::endl;
    }
    std::cout << " -------- SUMMARY: ---------" << std::endl;
    std::cout << "Quantity of tests: " << myQtyTests << std::endl;
    std::cout << "Success tests:     " << myQtySuccess<< std::endl;
    std::cout << "Failed tests:      " << myQtyTests - myQtySuccess << std::endl;

}

void Test::addFailure(const Failure& failure) {
    myFailures.push_back(failure);
}