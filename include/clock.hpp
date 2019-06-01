#ifndef CLOCK_HPP
#define CLOCK_HPP
#include "data_types.hpp"

void sleepInSecs( int secToSleep);

void sleepInUs( int usToSleep);

UINT64 getTimeInUs(void);
UINT64 getTimeInMs(void);

volatile UINT64 numClockCycles(void);

UINT64 estimateCpuSpeedInHz(    void);

double timeInNanos(UINT64 cycles);
double timeInMicros(UINT64 cycles);
double timeInMillis(UINT64 cycles);


#endif //CLOCK_HPP
