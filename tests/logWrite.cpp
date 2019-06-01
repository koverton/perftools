#include "event_log_writer.hpp"

#include <iostream>
#include <fstream>

int main() {
    int size = 100;
    event_log_writer l( "data/text", 100 );
    while( true ) {
        char topic[256], payload[256];
        std::cout << "Enter topic: ";
        std::cin.getline( topic, 256 );
        std::cout << "Enter message: ";
        std::cin.getline( payload, 256 );
        record f( topic, (void*)payload, strlen(payload)+1 );
        int length = l.write( f );
        if ( !length || length != f.totalsz() ) 
            std::cout << "FAILED -- no space" << std::endl;
    }
}

