#include "event_log_reader.hpp"

int main(int c, char* a[]) {
    for ( int i = 1; i < c; ++i) {
        std::cout << " === " << a[i] << " === " << std::endl;
        event_log_reader log( a[i] );
        log.dump_index();
    }
}

