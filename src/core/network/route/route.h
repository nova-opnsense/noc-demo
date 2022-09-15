#pragma once
#include <cjson/cJSON.h>
#include "config.h"
#include "api/error_code/error_code.h"

typedef struct
{
    char *id;
    char *interface;
    char *ip_mask;
    char *target_ip;
    char *name;
} route_t;

typedef struct
{
    route_t *route_list;
    int nb_routes;
} route_list_t;

cJSON *cvt_route_list_to_json(route_list_t *route_list);
route_list_t *cvt_json_to_route_list(cJSON *msg_json);

cJSON *cvt_route_to_json(route_t *route);
route_t *cvt_json_to_route(cJSON *msg_json);

error_code_t route_create(route_list_t *route_list_in, route_list_t **route_list_out);
error_code_t route_request(route_list_t **route_list_out);
error_code_t route_update(route_list_t *route_list_in, route_list_t **route_list_out);
error_code_t route_delete(route_list_t *route_list_in, route_list_t **route_list_out);

error_code_t route_request_from_setting_file(route_list_t **route_list_out);

void route_free(route_t *route);
void route_list_free(route_list_t *route_list);