#ifndef _DATE_PRINTER_H_
#define _DATE_PRINTER_H_

class DatePrinter {

public:
	
	virtual int print(int year, int month, int date) const = 0;

};

class AmericanDatePrinter : public DatePrinter {

public:
	
	int print(int year, int month, int date) const;

};

class RussianDatePrinter : public DatePrinter {

public:
	
	int print(int year, int month, int date) const;

};

class RussianBurocratDatePrinter : public DatePrinter {

public:
	
	int print(int year, int month, int date) const;
};

#endif
