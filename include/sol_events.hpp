#ifndef SOL_EVENTS_HPP
#define SOL_EVENTS_HPP
#include "solclient/solClient.h"

void
on_event(solClient_opaqueSession_pt sess_p,
			solClient_session_eventCallbackInfo_pt info_p,
			void *user_p);

void
on_flow_event(solClient_opaqueFlow_pt flow_p, 
			solClient_flow_eventCallbackInfo_pt info_p, 
			void *user_p);


#endif//SOL_EVENTS_HPP
