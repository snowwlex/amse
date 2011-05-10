#ifndef _COMPLEX_H
#define _COMPLEX_H

class Complex {

	public:
		Complex(int real=0, int img=0);

	public: //arithmetic operators
		Complex& operator+= (const Complex& complex);
		Complex& operator-= (const Complex& complex);
		Complex& operator*= (const Complex& complex);
		Complex& operator/= (const Complex& complex);

	private:
		int myReal;
		int myImg;

	//friends:

	//arithmetic operators
	friend Complex operator+(const Complex& complex1, const Complex& complex2);
	friend Complex operator- (const Complex& complex);
	friend Complex operator*(const Complex& complex1, const Complex& complex2);
	friend Complex operator/(const Complex& complex1, const Complex& complex2);
	//output in stream
	friend std::ostream& operator<< (std::ostream& stream, const Complex& complex);
};

#endif
