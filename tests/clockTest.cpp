#include "clock.hpp"

#include <iostream>

#define BILLION 1000000000.0

int main(int argc, char** argv) {
	UINT64 start = numClockCycles();
	UINT64 end = numClockCycles();
	UINT64 clocks = end - start;
	UINT64 hz = estimateCpuSpeedInHz();

	std::cout << "Hello clock speed is " 
		<< hz << " Hz" 
		<< std::endl;

	std::cout << std::endl << "Two concurrent calls cost " <<  timeInNanos(clocks) << " nanos" << std::endl;

	while( true ) {
		std::cout << "Java time: " << getTimeInMs() << std::endl;
		sleepInSecs( 1 );
	}

	return 0;
}
