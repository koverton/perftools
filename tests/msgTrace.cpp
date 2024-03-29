#include "clock.hpp"
#include "sol_session.hpp"
#include "sol_events.hpp"
#include "sol_msg.hpp"
#include "sol_error.hpp"
#include "event_log_writer.hpp"

#include "solclient/solClientMsg.h"

#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>

#define MAX_TRIES 3

static event_log_writer* writer;
static std::string fnamebase;
static int logsize;

std::string new_logname() {
	static int logcount = 1;
	std::stringstream ss;
	ss << fnamebase << logcount++;
	return ss.str();
}
event_log_writer* NEW_WRITER() {
	return new event_log_writer( new_logname(), logsize );
}


bool
proc_rec( void* buff_p, int buff_sz, const char* topic ) 
{
	record f( topic, (void*)buff_p, buff_sz+1 );
	// f.show();
	int length = 0, tries = 0;
	while( length != f.totalsz() && tries++ < MAX_TRIES )  {
		length = writer->write( f );
		if ( !length || length != f.totalsz() ) {
			delete writer;
			writer = NEW_WRITER();
		}
	}
	return true;
}

bool
proc_msg( solClient_opaqueMsg_pt msg_p )
{
	if ( solClient_msg_isDiscardIndication( msg_p ) ) {
		std::cout << "Discards!" << std::endl;
	}
    solClient_bufInfo_t smfbuf;
    solClient_opaqueDatablock_pt data_p;
    solClient_returnCode_t rc = solClient_msg_encodeToSMF( msg_p, &smfbuf, &data_p );
    int length = 0, tries = 0;
    while( length != smfbuf.bufSize && tries++ < MAX_TRIES )  {
        length = writer->write( reinterpret_cast<const char*>(smfbuf.buf_p), smfbuf.bufSize );
        if ( !length ) {
            delete writer;
            writer = NEW_WRITER();
        }
    }
    return true;
}


int
main ( int c, char *v[] )
{
	if ( c < 5 ) {
		std::cout << "\tUSAGE: " << v[0] << " <conn.properties> <tracelogname> <logsize> <topic1> <topic2> ..." << std::endl;
		return 0;
	}
	estimateCpuSpeedInHz();

	fnamebase = v[2];
	logsize  = atoi(v[3]);
	std::cout << "Creating writer on ["<<fnamebase<<"]:["<<logsize<<"]" << std::endl;
	writer = NEW_WRITER( );
	solClient_opaqueSession_pt sess_p = create_session( v[1], (void*) &proc_rec );

	for( int i = 4; i < c; ++i )
		subscribe( sess_p, v[i] );

	while( true )
		sleepInSecs( 1 );

	cleanup( sess_p );

	return 0;
}
