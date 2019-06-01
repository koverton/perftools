#include "sol_flow.hpp"
#include "sol_events.hpp"
#include "sol_msg.hpp"
#include "sol_error.hpp"

#include "solclient/solClientMsg.h"

static msg_proc_ptr my_proc_cb;

solClient_rxMsgCallback_returnCode_t
on_flow_msg ( solClient_opaqueFlow_pt flow_p, solClient_opaqueMsg_pt msg_p, void *user_p )
{
    void* buff_p;
    solClient_destination_t dest;
    solClient_uint32_t buff_sz;
    solClient_returnCode_t rc = SOLCLIENT_OK;

    msg_get_bin_data( msg_p, &buff_p, buff_sz );
    msg_get_dest( msg_p, &dest );

    msg_proc_ptr proc_cb = (msg_proc_ptr) user_p;
    if ( (*proc_cb)( buff_p, buff_sz, dest.dest ) ) {
        /* Acknowledge the message after processing it. */
        solClient_msgId_t msgid;
        if ( solClient_msg_getMsgId ( msg_p, &msgid )  == SOLCLIENT_OK ) {
            solClient_flow_sendAck ( flow_p, msgid );
        }
    }
    return SOLCLIENT_CALLBACK_OK;
}

solClient_opaqueFlow_pt 
create_flow( solClient_opaqueSession_pt sess_p, const std::string& queue_name, msg_proc_ptr proc_cb)
{
	solClient_returnCode_t rc = SOLCLIENT_OK;
	solClient_opaqueFlow_pt flow_p;
	solClient_flow_createFuncInfo_t flow_fninfo = SOLCLIENT_FLOW_CREATEFUNC_INITIALIZER;

	my_proc_cb = proc_cb;

	/*************************************************************************
	 * Create a Flow 
	 *************************************************************************/
	flow_fninfo.rxMsgInfo.callback_p = on_flow_msg;
	flow_fninfo.rxMsgInfo.user_p     = (void*)proc_cb;
	flow_fninfo.eventInfo.callback_p = on_flow_event;

	const char* flowprops[20];
	int index = 0;
	flowprops[index++] = SOLCLIENT_FLOW_PROP_BIND_BLOCKING;
	flowprops[index++] = SOLCLIENT_PROP_ENABLE_VAL;
	flowprops[index++] = SOLCLIENT_FLOW_PROP_ACKMODE;
	flowprops[index++] = SOLCLIENT_FLOW_PROP_ACKMODE_CLIENT;
	flowprops[index++] = SOLCLIENT_FLOW_PROP_WINDOWSIZE;
	flowprops[index++] = "255";
	flowprops[index++] = SOLCLIENT_FLOW_PROP_BIND_ENTITY_ID;
	flowprops[index++] = SOLCLIENT_FLOW_PROP_BIND_ENTITY_QUEUE;
	flowprops[index++] = SOLCLIENT_FLOW_PROP_BIND_NAME;
	flowprops[index++] = queue_name.c_str();
	flowprops[index++] = NULL;

	if ( ( rc = solClient_session_createFlow ( flowprops,
		sess_p,
		&flow_p, &flow_fninfo, sizeof (flow_fninfo) ) ) != SOLCLIENT_OK ) {
		on_error ( rc, "solClient_session_createFlow() did not return SOLCLIENT_OK after session create." );
		cleanup( sess_p );
	}

	return flow_p;
}
