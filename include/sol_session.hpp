#ifndef SOL_SESSION_HPP
#define SOL_SESSION_HPP
#include <string>

#include "solclient/solClient.h"

/**
 * Message event processor callback function type
 **/
typedef bool (*rec_proc_ptr)(void* buffer, int len, const char* topic);
typedef bool (*msg_proc_ptr)(solClient_opaqueMsg_pt msg_p);

solClient_opaqueSession_pt
create_session(const std::string& propsfile, void* cb);

bool
send( solClient_opaqueSession_pt sess_p, solClient_opaqueMsg_pt msg_p, const char* topic );

void
subscribe( solClient_opaqueSession_pt sess_p, const std::string& topic_sub);

void
cleanup(solClient_opaqueSession_pt sess_p);


#endif//SOL_SESSION_HPP
