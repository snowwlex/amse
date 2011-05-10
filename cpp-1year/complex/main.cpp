#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include "complex.h"

int main(int argc, char* argv[]) {

	Complex a(8, 6),
			b(-4, 8),
			c(-4, -2),
			d(0, 5),
			e(0, -10),
			f(4),
			g,
			h(0, 1),
			i(0, -1),
			j(-10, -1);

	std::cout << "nice output:" << std::endl;
	std::cout << a << std::endl
			  << b << std::endl
			  << c << std::endl
			  << d << std::endl
			  << e << std::endl
			  << f << std::endl
			  << g << std::endl
			  << h << std::endl
			  << i << std::endl
			  << j << std::endl;
	
	std::cout << std::endl;

	std::cout << "operator '/' : " << std::endl;
	std::cout << b << " / " << f << " = " << (b/f) << std::endl
			  << g << " / " << a << " = " << (g/a) << std::endl
			  << e << " / " << c << " = " << (e/c) << std::endl
			  << d << " / " << h << " = " << (d/h) << std::endl;

	std::cout << std::endl;

	std::cout << "divide by zero test:" << std::endl;
	try {
		std::cout << a << " / " << g << " = ";
		std::cout << (a/g) << std::endl;
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	std::cout << std::endl;

	std::cout << "divide not evenly test:" << std::endl;
	try {
		std::cout << b << " / " << d << " = ";
		std::cout << (b/d) << std::endl;
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	return 0;	
}
