#include <iostream>
#include <vector>
#include <algorithm>

#include "complex.h"

int main() {

    const size_t qty = 10000;
    Complex::setAllocateCapacity(qty);

    std::vector<Complex*> complexes;

    std::cout << "Fill array of complexes" << std::endl;
    for (size_t i = 0; i < qty; ++i) {
        complexes.push_back(new Complex(i, i));
    }

    std::cout << "Delete random elements" << std::endl;
    std::random_shuffle(complexes.begin(), complexes.end());
    for (size_t i = 0; i != (qty / 2); ++i) {
        delete complexes.back();
        complexes.pop_back();
    }

    std::cout << "Fill array again" << std::endl;
    for (size_t i = 0; i != (qty / 2); ++i) {
        complexes.push_back(new Complex(i, i));
    }

    std::cout << "Try to allocate " << qty + 1 << " element. ";
    try {
        new Complex;
        std::cout << "Something  wrong!!" << std::endl;
    } catch (std::bad_alloc& exc) {
        std::cout << "As expected: " << exc.what() << std::endl;
    }
    
    std::cout << "Delete all elements" << std::endl;
    for (size_t i = 0; i < qty; ++i) {
        delete complexes.back();
        complexes.pop_back();
    }

    return 0;
}
