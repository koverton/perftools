#include "sol_msg.hpp"
#include "sol_error.hpp"

bool
msg_init(solClient_opaqueMsg_pt* msg_pp)
{
    solClient_returnCode_t rc = SOLCLIENT_OK;
    if ( ( rc = solClient_msg_alloc(msg_pp) ) != SOLCLIENT_OK ) {
        on_error ( rc, "solClient_msg_alloc()" );
        return false;
    }
    return true;
}

/* - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - 
 *                                  SETTERS
 * - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - */
bool
msg_set_dest_queue(solClient_opaqueMsg_pt msg_p, solClient_destination_t* dest, const std::string& queue)
{
    solClient_returnCode_t rc = SOLCLIENT_OK;

    if ( ( rc = solClient_msg_setDeliveryMode ( msg_p,  SOLCLIENT_DELIVERY_MODE_PERSISTENT ) ) != SOLCLIENT_OK ) {
        on_error ( rc, "solClient_msg_setDeliveryMode()" );
        return false;
    }
    dest->dest = queue.c_str();
    dest->destType = SOLCLIENT_QUEUE_DESTINATION;
    if ( ( rc = solClient_msg_setDestination ( msg_p, dest, sizeof(*dest) ) ) != SOLCLIENT_OK ) {
        on_error ( rc, "solClient_msg_setDestination()" );
        return false;
    }
    return true;
}

bool 
msg_set_dest_topic(solClient_opaqueMsg_pt msg_p, solClient_destination_t* dest, const std::string& topic)
{
    solClient_returnCode_t rc = SOLCLIENT_OK;

    if ( ( rc = solClient_msg_setDeliveryMode ( msg_p,  SOLCLIENT_DELIVERY_MODE_DIRECT ) ) != SOLCLIENT_OK ) {
        on_error ( rc, "solClient_msg_setDeliveryMode()" );
        return false;
    }
    dest->dest = topic.c_str();
    dest->destType = SOLCLIENT_TOPIC_DESTINATION;
    if ( ( rc = solClient_msg_setDestination ( msg_p, dest, sizeof(*dest) ) ) != SOLCLIENT_OK ) {
        on_error ( rc, "solClient_msg_setDestination()" );
        return false;
    }
    return true;
}

bool 
msg_set_dest_topic(solClient_opaqueMsg_pt msg_p, const char* topic)
{
    solClient_returnCode_t rc = SOLCLIENT_OK;

    if ( ( rc = solClient_msg_setDeliveryMode ( msg_p,  SOLCLIENT_DELIVERY_MODE_DIRECT ) ) != SOLCLIENT_OK ) {
        on_error ( rc, "solClient_msg_setDeliveryMode()" );
        return false;
    }
    if ( ( rc = solClient_msg_setTopicPtr(msg_p, topic) ) != SOLCLIENT_OK ) {
        on_error ( rc, "solClient_msg_setTopicPtr" );
    }
    return true;
}

bool
msg_set_bin_data(solClient_opaqueMsg_pt msg_p, void* buffer, UINT32 size)
{
    solClient_returnCode_t rc = SOLCLIENT_OK;
    if ( ( rc = solClient_msg_setBinaryAttachmentPtr ( msg_p, buffer, size ) ) != SOLCLIENT_OK ) {
        on_error ( rc, "solClient_msg_setBinaryAttachmentPtr()" );
        return false;
    }
    return true;
}

bool
msg_set_usr_data(solClient_opaqueMsg_pt msg_p, void* buffer, UINT32 size)
{
    if ( size > SOLCLIENT_BUFINFO_MAX_USER_DATA_SIZE ) return false;
    solClient_returnCode_t rc = SOLCLIENT_OK;
    if ( ( rc = solClient_msg_setUserDataPtr ( msg_p, buffer, size ) ) != SOLCLIENT_OK ) {
        on_error ( rc, "solClient_msg_setUserDataPtr()" );
        return false;
    }
    return true;
}


/* - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - 
 *                                  GETTERS
 * - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - */
bool
msg_get_bin_data(solClient_opaqueMsg_pt msg_p, void** buffer, UINT32& size) 
{
    solClient_returnCode_t rc = SOLCLIENT_OK;
    if ( ( rc = solClient_msg_getBinaryAttachmentPtr(msg_p, buffer, &size) ) != SOLCLIENT_OK ) {
        on_error ( rc, "solClient_msg_getBinaryAttachmentPtr()" );
        return false;
    }
    return true;
}

bool
msg_get_usr_data(solClient_opaqueMsg_pt msg_p, void** buffer, UINT32& size) 
{
    solClient_returnCode_t rc = SOLCLIENT_OK;
    if ( ( rc = solClient_msg_getUserDataPtr(msg_p, buffer, &size) ) != SOLCLIENT_OK ) {
        on_error ( rc, "solClient_msg_getUserDataPtr()" );
        return false;
    }
    return true;
}

bool
msg_get_dest(solClient_opaqueMsg_pt msg_p, solClient_destination_t* dest)
{
    return SOLCLIENT_OK == solClient_msg_getDestination( msg_p, dest, sizeof(solClient_destination_t) );
}

bool
dest_is_queue(const solClient_destination_t& dest) 
{
    return (dest.destType == SOLCLIENT_QUEUE_DESTINATION);
}

bool
dest_is_topic(const solClient_destination_t& dest) 
{
    return (dest.destType == SOLCLIENT_TOPIC_DESTINATION);
}

bool
msg_discard_flag(solClient_opaqueMsg_pt msg_p)
{
    return solClient_msg_isDiscardIndication( msg_p );
}

INT64
msg_get_timestamp(solClient_opaqueMsg_pt msg_p)
{
    INT64 tstamp = 0L;
    solClient_msg_getSenderTimestamp(msg_p, &tstamp);
    return tstamp;
}
