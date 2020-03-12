#if 0
#include <mutex>
#include "Imutex.h"

// Get current time
void RealTimeClock::gettime(tm *now){
	std::lock_guard<Imutex> lock(guard);
	now->tm_hour = hour;
	now->tm_min = min;
	// do same for seconds, years, months
	// you may not do any kind of processing or printing here
	// all processing must be done outside the critical section (outside this function)
	// Interrupts are enabled/restored at the end by lock_guard destructor
}

RealTimeClock::RealTimeClock(int ticksPerSecond){

}

void RealTimeClock::gettime(tm *now){

}

void RealTimeClock::tick(){
	++sec;
}

#endif
