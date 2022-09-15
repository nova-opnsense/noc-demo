#ifndef __WAN_HANDLER_H
#define __WAN_HANDLER_H

#include <cjson/cJSON.h>

void create_wan(cJSON *msg_json);
void request_wan(cJSON *msg_json);

#endif