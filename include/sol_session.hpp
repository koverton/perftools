#ifndef SOL_SESSION_HPP
#define SOL_SESSION_HPP
#include <string>

#include "solclient/solClient.h"

/**
 * Message event processor callback function type
 **/
typedef bool (*msg_proc_ptr)(void* buffer, int len, const char* topic);

solClient_opaqueSession_pt
create_session(const std::string& propsfile, msg_proc_ptr proc_cb);

void
subscribe( solClient_opaqueSession_pt sess_p, const std::string& topic_sub);

void
cleanup(solClient_opaqueSession_pt sess_p);


#endif//SOL_SESSION_HPP
