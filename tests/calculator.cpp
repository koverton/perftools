#include "clock.hpp"
#include "sol_session.hpp"
#include "sol_events.hpp"
#include "sol_msg.hpp"
#include "sol_error.hpp"
#include "perf_stats.hpp"

#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>


static perf_stats stats(1000000, 100);


bool
proc_msg( solClient_opaqueMsg_pt msg_p )
{
    UINT64 endtime = numClockCycles();
    int *msgid = 0;
    UINT32 size = 0;
    solClient_returnCode_t rc = SOLCLIENT_FAIL;
    if ( !msg_get_usr_data( msg_p, (void**)&msgid, size ) ) {
        on_error ( rc, "msg_get_usr_data" );
    }
    // std::cout << "MSG " << *msgid << std::endl;
    UINT64 *begtime;
    UINT32 datasz;
    if ( !msg_get_bin_data( msg_p, (void**)&begtime, datasz ) ) {
        on_error ( rc, "msg_get_bin_data" );
    }
    UINT64 latency = (endtime-*begtime);
    stats.add( latency );
    // std::cout << "LATENCY: " << latency << std::endl;
    // Re-send this message out on different topic
    return true;
}

int
main ( int c, char *v[] )
{
    namespace po = boost::program_options;
    po::options_description desc("Calculator Options");
    desc.add_options()
        ("help", "Print help messages")
        ("config,c", po::value<std::string>(), "session configuration file")
        ("subscriptions,s", po::value<std::vector<std::string> >(), "list of topic subscriptions (specify multiple -s entries)");
    po::variables_map vm; 
    po::store(po::parse_command_line(c, v, desc), vm); // can throw 
    if ( !vm.count("config") || !vm.count("subscriptions") ) {
        std::cout << desc << std::endl;
        return 1;
    }
    std::string config = vm["config"].as<std::string>();
    std::vector<std::string> subscriptions = vm["subscriptions"].as<std::vector<std::string> >();

    estimateCpuSpeedInHz();

    solClient_opaqueSession_pt sess_p = create_session( config, (void*)&proc_msg );

    std::cout << "Subscribing to topics" << std::endl;
    for( std::vector<std::string>::const_iterator i = subscriptions.begin(); i != subscriptions.end(); ++i)
       subscribe( sess_p, *i );

    // std::this_thread::sleep_for( std::chrono::system_clock::duration::max() );

    while( true ) {
	    sleepInSecs( 4 );
              std::cout << "(" << stats.count() << ")" << std::endl
              << "\t50% " << stats.get_pct(0.50) << std::endl
              << "\t95% " << stats.get_pct(0.95) << std::endl
              << "\t99% " << stats.get_pct(0.99) << std::endl << std::endl;
    }

    cleanup( sess_p );

    return 0;
}
