#include <iostream>

#include "digitalclock.h"


DigitalClock::DigitalClock() {
}

DigitalClock::~DigitalClock() {
}

void DigitalClock::update() {
	if (myModel == 0) {
		return;
	}

	int hour = myModel->getHour();
	int min = myModel->getMin();
	int sec = myModel->getSec();

	if (hour < 10) {
		std::cout << '0';
	}
	std::cout << hour << ":";
	if (min < 10) {
		std::cout << '0';
	}
	std::cout << min << ":";
	if (sec < 10) {
		std::cout << '0';
	}
	std::cout << sec << std::endl;
}


void DigitalClock::userChangeTime() {
	if (myModel == 0) {
		return;
	}

	std::cout << "Enter new time in format [hh mm ss]: ";
	int h,m,s;
	std::cin >> h >> m >> s;
	if (h < 0 || h > 23 ||
		m < 0 || m > 59 ||
		s < 0 || s > 59
	   )
	{
		std::cout << "Invalid time!" << std::endl;
		return;
	}

	myModel->setTime(h,m,s);
}
