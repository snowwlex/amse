#include "ctime"

#include "clockmodel.h"

ClockModel::ClockModel(): myHour(0), myMin(0) , mySec(0) { }

ClockModel::~ClockModel() {
}

int ClockModel::getSec() {
	return mySec;
}

int ClockModel::getMin() {
	return myMin;
}

int ClockModel::getHour() {
	return myHour;
}

void ClockModel::tick(int seconds) {
	mySec += seconds;
	while (mySec > 59) {
		myMin += 1;
		mySec -= 60;
	}
	while (myMin > 59) {
		myHour += 1;
		myMin -= 60;
	}
	while (myHour > 23) {
		myHour -= 24;
	}

	notify();
}



void ClockModel::setTime(const time_t& timer) {
	tm* time;
	time = localtime(&timer);
	myHour = time->tm_hour;
	myMin  = time->tm_min;
	mySec  = time->tm_sec;

	notify();
}

void ClockModel::setTime(int hour, int min, int sec) {
	myHour = hour;
	myMin = min;
	mySec = sec;

	notify();
}

