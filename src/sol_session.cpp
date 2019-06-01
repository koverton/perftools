#include "sol_session.hpp"
#include "sol_msg.hpp"
#include "sol_props.hpp"
#include "sol_events.hpp"
#include "sol_error.hpp"

#include "solclient/solClientMsg.h"

#include <cstdlib>

static msg_proc_ptr my_proc_cb;

solClient_rxMsgCallback_returnCode_t
on_msg ( solClient_opaqueSession_pt sess_p, solClient_opaqueMsg_pt msg_p, void *user_p )
{
    void* buff_p;
    solClient_uint32_t buff_sz;
    solClient_destination_t dest;
    solClient_returnCode_t rc = SOLCLIENT_OK;

    msg_get_bin_data( msg_p, &buff_p, buff_sz );
    msg_get_dest( msg_p, &dest );

    msg_proc_ptr proc_cb = (msg_proc_ptr) user_p;
    (*proc_cb)( buff_p, buff_sz, dest.dest );
    return SOLCLIENT_CALLBACK_OK;
}

solClient_opaqueSession_pt
create_session(const std::string& propsfile, msg_proc_ptr proc_cb)
{
    solClient_returnCode_t rc = SOLCLIENT_OK;

    my_proc_cb = proc_cb;

    solClient_opaqueContext_pt ctx_p;
    solClient_context_createFuncInfo_t ctx_info = SOLCLIENT_CONTEXT_CREATEFUNC_INITIALIZER;

    solClient_opaqueSession_pt sess_p = 0;
    solClient_session_createFuncInfo_t sess_info = SOLCLIENT_SESSION_CREATEFUNC_INITIALIZER;

    if ( ( rc = solClient_initialize ( SOLCLIENT_LOG_DEFAULT_FILTER, NULL ) ) != SOLCLIENT_OK ) {
        on_error ( rc, "solClient_initialize()" );
        cleanup( 0 );
    }

    solClient_log_setFilterLevel ( SOLCLIENT_LOG_CATEGORY_ALL, SOLCLIENT_LOG_ERROR );

    if ( ( rc = solClient_context_create ( SOLCLIENT_CONTEXT_PROPS_DEFAULT_WITH_CREATE_THREAD,
                                           &ctx_p, &ctx_info, sizeof ( ctx_info ) ) ) != SOLCLIENT_OK ) {
        on_error ( rc, "solClient_context_create()" );
        cleanup( 0 );
    }

    sess_info.rxMsgInfo.callback_p = on_msg;
    sess_info.rxMsgInfo.user_p     = (void*)proc_cb;
    sess_info.eventInfo.callback_p = on_event;

    /* Create a Session for sending and receiving messages. */
    const char** sessprops = read_props( propsfile );

    solClient_log ( SOLCLIENT_LOG_INFO, "creating solClient session" );
    if ( ( rc = solClient_session_create ( sessprops,
                                           ctx_p, &sess_p, 
                                           &sess_info, sizeof (sess_info) ) )
         != SOLCLIENT_OK ) {
        on_error ( rc, "solClient_session_create()" );
        cleanup( sess_p );
    }

    solClient_log ( SOLCLIENT_LOG_INFO, "connecting solClient session" );
    if ( ( rc = solClient_session_connect ( sess_p ) ) != SOLCLIENT_OK ) {
        on_error ( rc, "solClient_session_connect()" );
        cleanup( sess_p );
    }

    return sess_p;
}

void 
subscribe( solClient_opaqueSession_pt sess_p, const std::string& topic_sub)
{
    solClient_returnCode_t rc = SOLCLIENT_OK;

    if ( (rc = solClient_session_topicSubscribeExt ( sess_p,
                                          SOLCLIENT_SUBSCRIBE_FLAGS_WAITFORCONFIRM,
                                          topic_sub.c_str() ) )
         != SOLCLIENT_OK ) {
        on_error( rc, "solClient_session_topicSubscribeExt" );
        cleanup( sess_p );
    }
}

void
cleanup(solClient_opaqueSession_pt sess_p)
{
    solClient_returnCode_t rc = SOLCLIENT_OK;
    if (sess_p != 0) {
        if ( ( rc = solClient_session_disconnect ( sess_p ) ) != SOLCLIENT_OK ) {
            on_error ( rc, "solClient_session_disconnect()" );
        }
    }
    if ( ( rc = solClient_cleanup (  ) ) != SOLCLIENT_OK ) {
        on_error ( rc, "solClient_cleanup()" );
    }
    exit( 0 );
}
