/*
 * main.cpp
 *
 *  Created on: 13.04.2010
 *      Author: snowwlex
 */
#include <iostream>
#include <ctime>
#include <unistd.h>
#include <iostream>

#include "clockmodel.h"
#include "digitalclock.h"
#include "analogclock.h"

int main() {

	const int tickTime = 1;

	ClockModel* clock = new ClockModel;
	DigitalClock* digitalView = new DigitalClock;
	AnalogClock* analogView = new AnalogClock;

	digitalView->setModel(clock);
	analogView->setModel(clock);

	clock->addListener(digitalView);
	clock->addListener(analogView);

	std::cout << "2 views at the same time:" << std::endl;
	clock->setTime(time(0));

	for (int i = 0; i < 3; ++i) {
		sleep(tickTime);
		clock->tick(tickTime);
	}

	std::cout << "User change time:" << std::endl;
	digitalView->userChangeTime();

	std::cout << "After removing listener of DigitalView from Clock:" << std::endl;
	clock->removeListener(digitalView);

	for (int i = 0; i < 3; ++i) {
			sleep(tickTime);
			clock->tick(tickTime);
	}


	delete digitalView;
	delete analogView;
	delete clock;





	return 0;
}
