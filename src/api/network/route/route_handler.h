#pragma once

#include <cjson/cJSON.h>

// STATIC ROUTE
void request_static_routes(cJSON *msg_json);
void create_static_route(cJSON *msg_json);
void delete_static_route(cJSON *msg_json);
void update_static_routes(cJSON *msg_json);
