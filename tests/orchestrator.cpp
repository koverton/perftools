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

static solClient_opaqueSession_pt sess_p;
static std::vector<std::string> topics;
static int tcount;
static int scount = 0;

bool
proc_msg( solClient_opaqueMsg_pt msg_p )
{
    UINT64 endtime = numClockCycles();
    // MsgID
    int *msgid;
    UINT32 size;
    solClient_returnCode_t rc = SOLCLIENT_FAIL;
    if ( !msg_get_usr_data( msg_p, (void**)&msgid, size ) ) {
        on_error ( rc, "msg_get_usr_data" );
    }
    // std::cout << "MSG " << *msgid << std::endl;
    // Latency calculation
    UINT64 *begtime;
    UINT32 datasz;
    if ( !msg_get_bin_data( msg_p, (void**)&begtime, datasz ) ) {
        on_error ( rc, "msg_get_bin_data" );
    }
    UINT64 latency = (endtime-*begtime);
    stats.add( latency );
    // std::cout << "LATENCY: " << latency << std::endl;
    // Re-send this message out on different topic
    // msg_get_dest(solClient_opaqueMsg_pt msg_p, solClient_destination_t* dest);
    solClient_destination_t dest;
    if ( !msg_get_dest(msg_p, &dest) ) {
        on_error ( rc, "msg_get_dest" );
    }
    // Only route 'data' messages, not control messages
    if ( !strcmp("data", dest.dest) ) {
        scount++;
        const char* topic = topics[ scount % tcount ].c_str();
        //std::cout << "Forwarding to topic " << topic << std::endl;
        if ( !msg_set_dest_topic(msg_p, topic) )
            on_error ( rc, "msg_set_dest_topic" );
        if ( !send( sess_p, msg_p, topic ) )
            on_error ( rc, "send" );
    }
    else {
        // std::cout << "Skipping topic " << dest.dest << std::endl;
    }
    return true;
}

int
main ( int c, char *v[] )
{
    namespace po = boost::program_options;
    po::options_description desc("Orchestrator Options");
    desc.add_options()
        ("help", "Print help messages")
        ("config,c", po::value<std::string>(), "session configuration file")
        ("subscriptions,s", po::value<std::vector<std::string> >(), "list of topic subscriptions (specify multiple -s entries)")
        ("topics,t", po::value<std::vector<std::string> >(), "list of publish topic (specify multiple -t entries)");
    po::variables_map vm; 
    po::store(po::parse_command_line(c, v, desc), vm); // can throw 
    if ( !vm.count("config") || !vm.count("subscriptions") || !vm.count("topics") ) {
        std::cout << desc << std::endl;
        return 1;
    }
    std::string config = vm["config"].as<std::string>();
    std::vector<std::string> subscriptions = vm["subscriptions"].as<std::vector<std::string> >();
    // HACK! Global!
    topics = vm["topics"].as<std::vector<std::string> >();
    tcount = topics.size();

    estimateCpuSpeedInHz();

    sess_p = create_session( config, (void*)&proc_msg );

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
