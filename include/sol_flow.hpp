#ifndef SOL_FLOW_HPP
#define SOL_FLOW_HPP
#include "sol_session.hpp"

solClient_opaqueFlow_pt 
create_flow( solClient_opaqueSession_pt sess_p, const std::string& queue_name, msg_proc_ptr proc_cb);

#endif// SOL_FLOW_HPP
