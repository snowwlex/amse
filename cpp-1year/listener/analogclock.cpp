#include <iostream>

#include <string>
#include "analogclock.h"


AnalogClock::AnalogClock() {
}

AnalogClock::~AnalogClock() {
}

void AnalogClock::update() {
	if (myModel == 0) {
		return;
	}

	const int maxHourQty = 30,
			  maxMinQty  = 30,
			  maxSecQty  = 30;

	const int maxHour = 12,
			  maxMin  = 59,
			  maxSec  = 59;

	const char SPACE = ' ';
	int qty,i;
	int hour = myModel->getHour();
	int min = myModel->getMin();
	int sec = myModel->getSec();

	qty = hour > 12 ? hour-12 : hour;
	qty = (double)qty / maxHour * maxHourQty;
	std::cout << "[";
	for (i = 0; i < qty; ++i ) {
		std::cout << "|";
	}
	std::cout << std::string(maxHourQty-i, SPACE) << "]  [";
	qty = (double)min / maxMin * maxMinQty ;
	for (i = 0; i < qty; ++i ) {
			std::cout << "*";
	}
	std::cout << std::string(maxMinQty-i, SPACE) << "]  [";

	qty = (double)sec / maxSec * maxSecQty;
	for (i = 0; i < qty; ++i ) {
			std::cout << "+";
	}
	std::cout << std::string(maxSecQty-i, SPACE) << "]" << std::endl;
}


