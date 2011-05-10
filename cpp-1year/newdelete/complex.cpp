#include <iostream>
#include <stdexcept>
#include "complex.h"

void* Complex::CURRENT_FREE = 0;
void* Complex::BEGIN_FREE = 0;
size_t Complex::CAPACITY = 1000; //default value
size_t Complex::CURRENT_QTY = 0;

Complex::Complex(int real, int img) : myReal(real), myImg(img) {
}

void* Complex::operator new(size_t) {
    //lazy initialization: allocate memory for all objects
    if (BEGIN_FREE == 0) {
        BEGIN_FREE = new char[ sizeof(Complex) / sizeof (char) * CAPACITY ];

        void* END_FREE = (Complex*) BEGIN_FREE + CAPACITY;
        for (void* cur = BEGIN_FREE; cur != END_FREE; cur = (Complex*) cur + 1) {
            void* next = (Complex*) cur + 1;
            *(void**) cur = next;
        }
        CURRENT_FREE = BEGIN_FREE;
    }

    if (CURRENT_QTY == CAPACITY) {
        throw std::bad_alloc();
    }

    void* ptr = CURRENT_FREE;
    CURRENT_FREE = *(void**) CURRENT_FREE;
    ++CURRENT_QTY;
    return ptr;
}

void Complex::operator delete (void* ptr) {
    *(void**) ptr = CURRENT_FREE;
    CURRENT_FREE = ptr;
    --CURRENT_QTY;

    if (CURRENT_QTY == 0) {
        delete[] (char*) BEGIN_FREE;
        BEGIN_FREE = 0;
    }
}

void Complex::setAllocateCapacity(size_t capacity) {
    if (BEGIN_FREE != 0) {
        throw std::logic_error("Can't set allocate capacity! There are created objects");
    }
    CAPACITY = capacity;
}

Complex& Complex::operator+=(const Complex& complex) {
    myReal += complex.myReal;
    myImg += complex.myImg;
    return *this;
}

Complex& Complex::operator-=(const Complex& complex) {
    myReal -= complex.myReal;
    myImg -= complex.myImg;
    return *this;
}

Complex& Complex::operator*=(const Complex& complex) {
    int tmpReal = myReal;
    myReal = myReal * complex.myReal - myImg * complex.myImg;
    myImg = tmpReal * complex.myImg + myImg * complex.myReal;
    return *this;
}

Complex& Complex::operator/=(const Complex& complex) {
    int denominator = complex.myReal * complex.myReal + complex.myImg * complex.myImg;
    if (denominator == 0) {
        throw ( std::runtime_error("Error! Divide by zero!"));
    }
    int realNumerator = myReal * complex.myReal + myImg * complex.myImg;
    int imgNumerator = myImg * complex.myReal - myReal * complex.myImg;
    if (realNumerator % denominator != 0 || imgNumerator % denominator != 0) {
        throw ( std::runtime_error("Error! Can not divide evenly!"));
    }
    myReal = realNumerator / denominator;
    myImg = imgNumerator / denominator;
    return *this;
}

Complex operator+(const Complex& complex1, const Complex& complex2) {
    Complex complex = complex1;
    return complex += complex2;
}

Complex operator-(const Complex& complex1, const Complex& complex2) {
    Complex complex = complex1;
    return complex -= complex2;
}

Complex operator*(const Complex& complex1, const Complex& complex2) {
    Complex complex = complex1;
    return complex *= complex2;
}

Complex operator/(const Complex& complex1, const Complex& complex2) {
    Complex complex = complex1;
    return complex /= complex2;
}

std::ostream & operator<<(std::ostream& stream, const Complex& complex) {
    if (complex.myReal != 0 || complex.myImg == 0) {
        stream << complex.myReal;
    }
    if (complex.myImg != 0) {
        if (complex.myImg > 0 && complex.myReal != 0) {
            stream << "+";
        }
        if (complex.myImg == -1) {
            stream << "-";
        } else if (complex.myImg != 1) {
            stream << complex.myImg;
        }
        stream << "i";
    }
    return stream;
}
