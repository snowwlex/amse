#ifndef CLOCK_H_
#define CLOCK_H_

#include "listenerpattern.h"

class ClockModel : public Subject<ClockModel> {

public:
	ClockModel();
	~ClockModel();

public:
	void tick(int seconds);

public:
	int getHour();
	int getMin();
	int getSec();

public:
	void setTime(int hour, int min, int sec);
	void setTime(const time_t& timer);

private:
	int myHour;
	int myMin;
	int mySec;


};

#endif /* MODEL_H_ */
