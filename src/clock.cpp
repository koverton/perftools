#include "clock.hpp"

#include "solclient/solClient.h"

#include <cerrno>
#include <unistd.h>

#define BILLION 1000000000.0


void sleepInSecs( int secToSleep)
{
    sleep(secToSleep);
}

void sleepInUs( int usToSleep)
{
    struct timespec timeVal;
    struct timespec timeRem;
    timeVal.tv_sec = usToSleep / 1000000;
    timeVal.tv_nsec = (usToSleep % 1000000) * 1000;
again:
    if (nanosleep(&timeVal, &timeRem) < 0) {
        if (errno == EINTR) {
            /* Nanosleep was interrupted */
            timeVal = timeRem;
            goto again;
        } else {
            /* We will not actually pass back the error, but at least
             *                log that something bad happened */
            solClient_log(SOLCLIENT_LOG_ERROR, "Could not nanosleep, error = %d", errno);
        }
    }
}

UINT64 getTimeInUs(void)
{
    struct timespec tv;

    clock_gettime(CLOCK_REALTIME, &tv);
    return ((UINT64)tv.tv_sec  * (UINT64)1000000) +
           ((UINT64)tv.tv_nsec / (UINT64)1000);
}

UINT64 getTimeInMs(void)
{
    struct timespec tv;

    clock_gettime(CLOCK_REALTIME, &tv);
    return ((UINT64)tv.tv_sec  * (UINT64)1000) +
           ((UINT64)tv.tv_nsec / (UINT64)1000000);
}

volatile UINT64 numClockCycles(void)
{
        UINT32 hi, lo;
        __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
        return ( (UINT64)lo)|( ((UINT64)hi)<<32 );
}

static double nanosPerClock;

UINT64 estimateCpuSpeedInHz(    void)
{
    UINT64 startCycles;
    UINT64 endCycles;
    UINT64 cycleDiff;
    UINT64 startTime;
    UINT64 endTime;
    UINT64 timeDiff;
    UINT64 cpuHz;
    startTime = getTimeInUs();
    startCycles = numClockCycles();
    sleepInSecs(1);
    endTime = getTimeInUs();
    endCycles = numClockCycles();
    timeDiff = endTime - startTime;
    cycleDiff = endCycles - startCycles;
    cpuHz = (cycleDiff * 1000000) / timeDiff;
    nanosPerClock = BILLION/(double)cpuHz;
    return cpuHz;
}

double timeInNanos(UINT64 cycles) {
	return cycles / nanosPerClock;
}

double timeInMicros(UINT64 cycles) {
	return timeInNanos(cycles) / 1000;
}

double timeInMillis(UINT64 cycles) {
	return timeInNanos(cycles) / 1000000;
}

