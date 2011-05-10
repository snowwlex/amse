#include "ModelTest.h"

int main() {

    ModelTest t;
    t.run();
    t.report();
    
    std::cout << "(comment: so many tests, because there's cycle: 1000 move generating and making)" << std::endl;
    return 0;
}

