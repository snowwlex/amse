#include <cstdio>
#include "dateprinter.h"

int AmericanDatePrinter::print(int year, int month, int date) const { 
	if (year < 0 || date < 1 || date > 31 || month < 1 || month > 12) {
		return 1;	
	}
	const char *months[] = {"January", "February", "March", "April", 
					        "May", "June", "July", "August", 
					        "September", "October", "November", "December"};  
	printf("%s %d %d", months[month-1], date, year);
	return 0;
}

int RussianDatePrinter::print(int year, int month, int date) const { 
	if (year < 0 || date < 0 || date > 31 || month < 1 || month > 12) {
		return 1;	
	}
	const char *months[] = {"января", "февраля",  "марта",  "апреля", 
					        "мая",  "июня", "июля", "августа", 
					        "сентября", "октября", "ноября", "декабря"};
	printf("%d %s %d", date, months[month-1], year);
	return 0;
}

int RussianBurocratDatePrinter::print(int year, int month, int date) const { 
	if (year < 0 || date < 0 || date > 31 || month < 1 || month > 12) {
		return 1;	
	}
	if (date < 10) {
		printf("0");	
	}
	printf("%d.",date);
	if (month < 10) {
		printf("0");	
	}
	printf("%d.%d",month, year);
	return 0;
}




