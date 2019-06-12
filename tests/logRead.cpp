#include "event_log_reader.hpp"

int main(int c, char* a[]) {
    event_log_reader log( a[1] );
    log.dump_index();
    for( int i = 0; i < log.count(); i++) {
        solClient_opaqueMsg_pt m = log.read_next_msg();
	solClient_msg_dump( m, NULL, 0 );
    }
}
