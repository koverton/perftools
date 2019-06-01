#include "sol_error.hpp"

void
on_error ( solClient_returnCode_t rc, const char *errorStr )
{
    solClient_errorInfo_pt errorInfo = solClient_getLastErrorInfo (  );
    solClient_log ( SOLCLIENT_LOG_ERROR,
                    "%s - ReturnCode=\"%s\", SubCode=\"%s\", ResponseCode=%d, Info=\"%s\"",
                    errorStr, solClient_returnCodeToString ( rc ),
                    solClient_subCodeToString ( errorInfo->subCode ), errorInfo->responseCode, errorInfo->errorStr );
    solClient_resetLastErrorInfo (  );
}

