#ifndef __LAN_HANDLER_H__
#define __LAN_HANDLER_H__
#include <cjson/cJSON.h>

void create_lan(cJSON *msg_json);

void request_lan(cJSON *msg_json);

#endif