#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "complex.h"

template class Complex<int>;

int main(int argc, char* argv[]) {

	int one = 1;
	int two = 2;
	int three =3;

	Complex<int> a(one, two);
	Complex<int> b(two,three);
	Complex<int> c = two*three;

	c.print();
	c = a+b;
	c.print();
	c = a*b;
	c.print();
	


	return 0;	
}
