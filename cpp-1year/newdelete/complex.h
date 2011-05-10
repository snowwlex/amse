#ifndef _COMPLEX_H
#define _COMPLEX_H

class Complex {

    public:

        Complex(int real = 0, int img = 0);

    public: //arithmetic operators

        Complex & operator+=(const Complex& complex);
        Complex & operator-=(const Complex& complex);
        Complex & operator*=(const Complex& complex);
        Complex & operator/=(const Complex& complex);

    public: //memory allocation operators

        void* operator new(size_t size);
        void operator delete (void* ptr);

    public: //allocation managment

        /** it can be used:
         * 1) Before creating any objects
         * 2) After deleting all objects
         * If it's not set, default value (1000) is used.  */
        static void setAllocateCapacity(size_t capacity);

    private: //allocation fields

        static void* BEGIN_FREE;
        static void* CURRENT_FREE;
        static size_t CAPACITY;
        static size_t CURRENT_QTY;

    private:
        int myReal;
        int myImg;

    //friends:

    //arithmetic operators
    friend Complex operator+(const Complex& complex1, const Complex& complex2);
    friend Complex operator-(const Complex& complex);
    friend Complex operator*(const Complex& complex1, const Complex& complex2);
    friend Complex operator/(const Complex& complex1, const Complex& complex2);
    //output in stream
    friend std::ostream & operator<<(std::ostream& stream, const Complex& complex);
};

#endif
