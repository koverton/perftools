#include "clock.hpp"
#include "sol_session.hpp"
#include "sol_events.hpp"
#include "sol_msg.hpp"
#include "sol_flow.hpp"
#include "sol_error.hpp"
#include "driver.hpp"
#include "perf_stats.hpp"
#include "testargs.hpp"

#include "solclient/solClientMsg.h"

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

static perf_stats stats(1000000, 100);

bool
process_msg( void* buff_p, int buff_sz, const char* topic ) 
{
    UINT64 start = 0;
    memcpy( (void*)&start, buff_p, sizeof(start) );
    UINT64 cycles = numClockCycles() - start;
    UINT64 latency = static_cast<UINT64>( timeInMicros(cycles) );
    // std::cout << "cycles = " << cycles << ", latency = " << latency << std::endl;
    stats.add( latency );
    return true;
}

struct context
{
    solClient_destination_t dest;
    solClient_opaqueSession_pt sess_p;
    solClient_opaqueMsg_pt msg_p;
    char binMsg[1024];
};

void 
send_msg(context* ctx) {
    solClient_returnCode_t rc = SOLCLIENT_OK;
    UINT64 timestamp = numClockCycles();
    memcpy ( (void*) ctx->binMsg, &timestamp, sizeof(timestamp) );
    if ( !msg_set_bin_data( ctx->msg_p, ctx->binMsg, sizeof(timestamp) ) ) 
        cleanup( ctx->sess_p );
    if ( ( rc = solClient_session_sendMsg ( ctx->sess_p, ctx->msg_p ) ) != SOLCLIENT_OK ) {
        on_error ( rc, "solClient_session_send" );
    }
}

void
publisher_loop(solClient_opaqueSession_pt sess_p, const test_def& def)
{
    driver<context> test;
    context ctx;
    ctx.sess_p = sess_p;

    if ( !msg_init( &ctx.msg_p ) )
        cleanup( sess_p );

    if ( def.dtype ==QUEUE ) {
	if ( !msg_set_dest_queue( ctx.msg_p, &ctx.dest, def.dest ) )
            cleanup( sess_p );
    } else {
        if ( !msg_set_dest_topic( ctx.msg_p, &ctx.dest, def.dest ) )
            cleanup( sess_p );
    }

   UINT64 startTime = getTimeInUs( );

   test.run( def.rate, def.seconds, &send_msg, &ctx );

    solClient_msg_free ( &ctx.msg_p );

    UINT64 elapsedTime = getTimeInUs() - startTime;
    std::cout	<< "Sent " << (def.rate *  def.seconds)
		<< " in "  << elapsedTime << " usec, rate of " 
		<< ( long double ) (def.rate *  def.seconds) / ( ( long double ) elapsedTime / ( long double ) 1000000.0 ) 
		<< " msgs/sec" 
		<< std::endl;
}

int
main ( int c, char *v[] )
{
    test_def test;
    parse_args( c, v, &test );

    estimateCpuSpeedInHz();

    solClient_opaqueSession_pt sess_p = create_session( test.props, (msg_proc_ptr)&process_msg );

    solClient_opaqueFlow_pt flow_p = 0;
    if ( test.dtype == QUEUE )
        flow_p = create_flow( sess_p, test.dest, process_msg );
    else
        subscribe( sess_p, test.dest );

    publisher_loop( sess_p, test );

    std::cout << "\t50% " << stats.get_pct(0.50) << std::endl;
    std::cout << "\t95% " << stats.get_pct(0.95) << std::endl;
    std::cout << "\t99% " << stats.get_pct(0.99) << std::endl;

    cleanup( sess_p );

    return 0;
}
