#include "perf_stats.hpp"

#include <iostream>

void test2() {
	UINT64 scores[] = { 43, 54, 56, 61, 62, 66, 68, 69, 69, 70, 71, 72, 77, 78, 79, 85, 87, 88, 89, 93, 95, 96, 98, 99, 99 };
	perf_stats s( 50, 2 );
	for(int i = 0; i < 25; i++) {
		s.add( scores[i] );
	}
	if (s.get_pct(0.2) != 64) std::cout << "Failed! 20% should be 64 but was " << s.get_pct(0.2) << std::endl;
	if (s.get_pct(0.9) != 98) std::cout << "Failed! 90% should be 98 but was " << s.get_pct(0.9) << std::endl;
	std::cout << "Done!" << std::endl;
}

void test1() {
        perf_stats s(40, 2);
        for( int i = 1; i <= 40; i++) {
                UINT64 n = i;
                std::cout << n << ",";
                s.add( n );
        }
        std::cout << std::endl << "50% : " << s.get_pct(.5) << std::endl;
	std::cout << std::flush;
        std::cout << std::endl << "90% : " << s.get_pct(.9) << std::endl;
	std::cout << std::flush;

        s.debug();
}


int main(int c, char** a)
{

	test2();

        return 0;
}
