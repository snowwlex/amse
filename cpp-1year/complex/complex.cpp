#include <iostream>
#include <stdexcept>
#include "complex.h"


Complex::Complex(int real, int img): myReal(real), myImg(img) { }
Complex& Complex::operator+= (const Complex& complex) {
	myReal += complex.myReal;
	myImg  += complex.myImg;
	return *this;
}
Complex& Complex::operator-= (const Complex& complex) {
	myReal -= complex.myReal;
	myImg  -= complex.myImg;
	return *this;
}
Complex& Complex::operator*= (const Complex& complex) {
	int tmpReal = myReal;
	myReal = myReal*complex.myReal - myImg*complex.myImg;
	myImg  = tmpReal*complex.myImg + myImg*complex.myReal;
	return *this;
}
Complex& Complex::operator/= (const Complex& complex) {
	int denominator = complex.myReal*complex.myReal + complex.myImg* complex.myImg;
	if (denominator == 0) {
		throw ( std::runtime_error("Error! Divide by zero!") );
	}
	int realNumerator = myReal*complex.myReal + myImg*complex.myImg;
	int imgNumerator  = myImg*complex.myReal  - myReal*complex.myImg;
	if (realNumerator % denominator != 0 || imgNumerator % denominator != 0) {
		throw ( std::runtime_error("Error! Can not divide evenly!") );
	}
	myReal = realNumerator / denominator;
	myImg  = imgNumerator  / denominator;
	return *this;
}
Complex operator+(const Complex& complex1, const Complex& complex2) {
	Complex complex = complex1;
	return complex += complex2;
}
Complex operator- (const Complex& complex1, const Complex& complex2) {
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
std::ostream& operator<< (std::ostream& stream, const Complex& complex) {
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
