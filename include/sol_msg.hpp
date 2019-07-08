#ifndef SOL_MSG_HPP
#define SOL_MSG_HPP
#include "data_types.hpp"
#include "solclient/solClientMsg.h"
#include <string>

bool
msg_init(solClient_opaqueMsg_pt* msg_pp);

/* - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - 
 *                                  SETTERS
 * - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - */

bool
msg_set_dest_queue(solClient_opaqueMsg_pt msg_p, solClient_destination_t* dest, const std::string& queue);

bool 
msg_set_dest_topic(solClient_opaqueMsg_pt msg_p, solClient_destination_t* dest, const std::string& topic);

bool 
msg_set_dest_topic(solClient_opaqueMsg_pt msg_p, const char* topic);

bool
msg_set_bin_data(solClient_opaqueMsg_pt msg_p, void* buffer, UINT32 size);

bool
msg_set_usr_data(solClient_opaqueMsg_pt msg_p, void* buffer, UINT32 size);



/* - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - 
 *                                  GETTERS
 * - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - + - */

bool
msg_get_bin_data(solClient_opaqueMsg_pt msg_p, void** buffer, UINT32& size);

bool
msg_get_usr_data(solClient_opaqueMsg_pt msg_p, void** buffer, UINT32& size);

bool
msg_get_dest(solClient_opaqueMsg_pt msg_p, solClient_destination_t* dest);

bool
dest_is_queue(const solClient_destination_t& dest);

bool
dest_is_topic(const solClient_destination_t& dest);

bool
msg_discard_flag(solClient_opaqueMsg_pt msg_p);

INT64
msg_get_timestamp(solClient_opaqueMsg_pt msg_p);

#endif// SOL_MSG_HPP
