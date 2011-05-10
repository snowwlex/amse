
#ifndef ANALOGCLOCK_H_
#define ANALOGCLOCK_H_

#include "listenerpattern.h"
#include "clockmodel.h"

class AnalogClock : public Listener<ClockModel> {

public:
	AnalogClock();
	virtual ~AnalogClock();

public:
	virtual void update();
};


#endif /* ANALOGCLOCK_H_ */
