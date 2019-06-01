#include "event_log_reader.hpp"

int main(int c, char* a[]) {
    event_log_reader log( a[1] );
    log.dump_index();
    for( int i = 0; i < log.count(); i++) {
        record l = log.read_next();
	l.show();
    }
}
