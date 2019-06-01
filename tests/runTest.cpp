#include "driver.hpp"

#include <iostream>

void one_cycle(const char* ctx) {
    std::cout << ctx << std::endl;
}

int main() {
	driver<const char> d;
	d.run( 1, 10, &one_cycle, "hi there" );
}
