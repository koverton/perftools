#include "sol_session.hpp"
#include "sol_error.hpp"
#include "sol_msg.hpp"

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

bool
process_msg( void* buff_p, int buff_sz, const char* topic ) 
{
    std::cout << "Msg Callback..." << std::endl;
    return true;
}

int
main ( int c, char *v[] )
{
    const char* TOPIC = "a/b/c/d/e";
    const char* DATA = "HELLO WORLD!";
    std::cout << "Creating session..." << std::endl;
    solClient_opaqueSession_pt sess_p = create_session( "configs/test1.properties", (void*)&process_msg );
    solClient_opaqueMsg_pt msg_p, msg2_p;
    std::cout << "Msg Init..." << std::endl;
    msg_init( &msg_p );
    msg_init( &msg2_p );
    
    solClient_msg_setTopicPtr( msg_p, TOPIC );
    solClient_msg_setBinaryAttachmentPtr( msg_p, (void*)DATA, 1+strlen(DATA) );
    std::cout << "GetUserProps..." << std::endl;
    solClient_opaqueContainer_pt map_p;
    solClient_msg_createUserPropertyMap( msg_p, &map_p, 1024 );
    std::cout << "Setting User Props on map..." << std::endl;
    solClient_container_addString( map_p, "ken", "name" );
    solClient_container_addString( map_p, "red", "name" );
    solClient_container_addDouble( map_p, 3.1415926, "pi" );
    solClient_container_addBoolean( map_p, true, "fuzzy" );

    solClient_bufInfo_t smfbuf;
    solClient_opaqueDatablock_pt data_p;
    // solClient_returnCode_t rc = solClient_msg_getSmfPtr( msg_p, (solClient_uint8_t**)&(smfbuf.buf_p), &(smfbuf.bufSize) );
    solClient_returnCode_t rc = solClient_msg_encodeToSMF( msg_p, &smfbuf, &data_p );
    if ( rc != SOLCLIENT_OK ) on_error( rc, "solClient_msg_encodeToSMF" );
    rc = solClient_msg_decodeFromSmf( &smfbuf, &msg2_p );
    if ( rc != SOLCLIENT_OK ) on_error( rc, "solClient_msg_decodeFromSmf" );
    std::cout << "Dumping original message: " << std::endl;
    solClient_msg_dump( msg_p, NULL, 0 );
    std::cout << "Dumping duplicate message: " << std::endl;
    solClient_msg_dump( msg2_p, NULL, 0 );
    
    solClient_msg_free ( &msg_p );

    return 0;
}

