#include "sol_events.hpp"

#include "solclient/solClient.h"

void
on_event(solClient_opaqueSession_pt sess_p,
			solClient_session_eventCallbackInfo_pt info_p,
			void *user_p)
{
	switch(info_p->sessionEvent) {
		// connectivity events
		case SOLCLIENT_SESSION_EVENT_UP_NOTICE:
		return;
	case SOLCLIENT_SESSION_EVENT_RECONNECTING_NOTICE:
		return;
		case SOLCLIENT_SESSION_EVENT_RECONNECTED_NOTICE:
		return;
	// publisher events
 	case SOLCLIENT_SESSION_EVENT_ACKNOWLEDGEMENT:
		return;
		case SOLCLIENT_SESSION_EVENT_REJECTED_MSG_ERROR:
		return;
		// Happy days
		case SOLCLIENT_SESSION_EVENT_CAN_SEND:
		case SOLCLIENT_SESSION_EVENT_TE_UNSUBSCRIBE_OK:
		case SOLCLIENT_SESSION_EVENT_PROVISION_OK:
		case SOLCLIENT_SESSION_EVENT_SUBSCRIPTION_OK:
		return;
		// Error events
		case SOLCLIENT_SESSION_EVENT_DOWN_ERROR:
		case SOLCLIENT_SESSION_EVENT_CONNECT_FAILED_ERROR:
		case SOLCLIENT_SESSION_EVENT_SUBSCRIPTION_ERROR:
		case SOLCLIENT_SESSION_EVENT_RX_MSG_TOO_BIG_ERROR:
		case SOLCLIENT_SESSION_EVENT_TE_UNSUBSCRIBE_ERROR:
		case SOLCLIENT_SESSION_EVENT_PROVISION_ERROR:
		return;
	// Unknown events
	default:
		return;
	}
}

void
on_flow_event(solClient_opaqueFlow_pt flow_p, solClient_flow_eventCallbackInfo_pt info_p, void *user_p)
{
	switch(info_p->flowEvent) {
		case SOLCLIENT_FLOW_EVENT_UP_NOTICE:
		case SOLCLIENT_FLOW_EVENT_SESSION_DOWN:
			/* Non error events are logged at the INFO level. */
			solClient_log(SOLCLIENT_LOG_INFO,
			"sol_flow_event_triage(): %s\n",
						  solClient_flow_eventToString(info_p->flowEvent));
			return ;

		case SOLCLIENT_FLOW_EVENT_DOWN_ERROR:
		case SOLCLIENT_FLOW_EVENT_BIND_FAILED_ERROR:
		case SOLCLIENT_FLOW_EVENT_REJECTED_MSG_ERROR:
			{
			/* Error events are output to STDOUT. */
			solClient_errorInfo_pt errorInfo_p = solClient_getLastErrorInfo();
			printf(
			"sol_flow_event_triage(): %s; subCode %s, responseCode %d, reason %s\n",
				   solClient_flow_eventToString(info_p->flowEvent),
				   solClient_subCodeToString(errorInfo_p->subCode),
				   errorInfo_p->responseCode, errorInfo_p->errorStr );
			return ;
			}

		default:
			/* Unrecognized or deprecated events are output to STDOUT. */
			printf("sol_flow_event_triage(): %s.  Unrecognized/deprecated event.\n",
					 solClient_flow_eventToString(info_p->flowEvent));
			return ;
	}
}

