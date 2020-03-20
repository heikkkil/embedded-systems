/*
 * RealTimeClock.h
 *
 *  Created on: 14.2.2020
 */

#ifndef REALTIMECLOCK_H_
#define REALTIMECLOCK_H_

class RealTimeClock {
public:
	// You must also provide a copnstructor variant with systick rate and current time
	RealTimeClock(int ticksPerSecond);
	RealTimeClock(tm curr);
	void gettime(tm *now);
	void tick();

private:
	volatile int hour;
	volatile int min;
	volatile int sec;
};

#endif /* REALTIMECLOCK_H_ */
