#include <cstdio>
#include "dateprinter.h"

void print(DatePrinter *printer, int year, int month, int date) {
	int result;
	result = printer->print(year,month,date);
	if (result == 1) {
		printf("Ошибка ввода данных");	
	}
	printf("\n");
}

int main(void) {

	DatePrinter *printer;

	printer = new AmericanDatePrinter();
	print(printer,2009,11,27);
	delete printer;

	printer = new RussianDatePrinter();
	print(printer,2009,12,14);
	delete printer;

	printer = new RussianBurocratDatePrinter();
	print(printer,2010,1,1);
	delete printer;

	return 0;
}
