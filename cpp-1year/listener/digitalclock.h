#ifndef DIGITALCLOCK_H_
#define DIGITALCLOCK_H_

#include "listenerpattern.h"
#include "clockmodel.h"

class DigitalClock : public Listener<ClockModel> {

public:
	DigitalClock();
	virtual ~DigitalClock();

public:
	void userChangeTime();
	virtual void update();

};
#endif /* DIGITALCLOCK_H_ */
