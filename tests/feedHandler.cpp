#include "clock.hpp"
#include "sol_session.hpp"
#include "sol_events.hpp"
#include "sol_msg.hpp"
#include "sol_error.hpp"
#include "driver.hpp"

#include <boost/program_options.hpp>
#include <iostream>
#include <string>

// The test driver iterates for us, calling send_msg at the right times.
// This means any context crossing invocations needs to be separate and 
// passed into send_msg.
struct context
{
    context(solClient_opaqueSession_pt sess_p, const std::vector<std::string>& topics)
        : sess_p(sess_p), topics(topics), tcount(topics.size()), msgid(0) {}
    solClient_opaqueSession_pt sess_p;
    solClient_opaqueMsg_pt msg_p;
    const std::vector<std::string> topics;
    const int tcount;
    int msgid;
    //char binMsg[1024];
};

void 
send_msg(context* ctx) {
    // Increment the msgid for sending
    ctx->msgid++;
    // Put a msgid in the user-data
    if ( !msg_set_usr_data( ctx->msg_p, &ctx->msgid, sizeof(ctx->msgid) ) )
        cleanup( ctx->sess_p );
    // Choose the next topic in our list of topics
    const char* topic = ctx->topics[ ctx->msgid%ctx->tcount ].c_str();
    // Put a LL timetstamp in the payload (wait until as late as possible)
    UINT64 timestamp = numClockCycles();
    if ( !msg_set_bin_data( ctx->msg_p, &timestamp, sizeof(timestamp) ) )
        cleanup( ctx->sess_p );
    // Send!
    if ( !send( ctx->sess_p, ctx->msg_p, topic ) )
        cleanup( ctx->sess_p );
}

void
publisher_loop(solClient_opaqueSession_pt sess_p, const std::vector<std::string>& topics, int rate, int dur) 
{
    driver<context> test;
    context ctx(sess_p, topics);

    if ( !msg_init( &ctx.msg_p ) )
        cleanup( sess_p );

    UINT64 startTime = getTimeInUs( );

    test.run( rate, dur, &send_msg, &ctx );

    solClient_msg_free ( &ctx.msg_p );

    int total = rate * dur;
    UINT64 elapsedTime = getTimeInUs() - startTime;
    std::cout	<< "Sent " << total
		<< " in "  << elapsedTime << " usec, rate of " 
		<< (long double) total / ( (long double) elapsedTime / (long double) 1000000.0 ) 
		<< " msgs/sec" 
		<< std::endl;
}

bool on_msg( void* buff_p, int buff_sz, const char* topic ) { return true; }

int
main ( int c, char *v[] )
{
    ////
    //// Expected Input Arguments
    namespace po = boost::program_options;
    po::options_description desc("Feedhandler Options");
    desc.add_options()
        ("help,h",   "Print this message")
        ("config,c", po::value<std::string>(), "session configuration file")
        ("topics,t", po::value<std::vector<std::string> >(), "list of publish topics (you can specify multiple -t entries)")
        ("rate,r",   po::value<int>(), "publish rate")
        ("dur,d",    po::value<int>(), "publish duration");
    // Parse the commandline
    po::variables_map vm; 
    po::store(po::parse_command_line(c, v, desc), vm); // can throw 
    if ( !vm.count("config") || !vm.count("topics") || !vm.count("rate") || !vm.count("dur") ) {
        std::cout << desc << std::endl;
        return 1;
    }
    std::string config = vm["config"].as<std::string>();
    std::vector<std::string> topics = vm["topics"].as<std::vector<std::string> >();
    int rate = vm["rate"].as<int>();
    int dur  = vm["dur"].as<int>();

    ////
    //// Run our publishing loop
    estimateCpuSpeedInHz();

    solClient_opaqueSession_pt sess_p = create_session( config, (void*)&on_msg );

    publisher_loop( sess_p, topics, rate, dur );

    cleanup( sess_p );
    //// Done
    //// 

    return 0;
}
