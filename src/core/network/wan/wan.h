#pragma once
#include <cjson/cJSON.h>
#include "config.h"
#include "api/error_code/error_code.h"

typedef struct
{
    char *proto;
    char *ipaddr;
} wan_t;

cJSON *cvt_wan_to_json(wan_t *wan);
wan_t *cvt_json_to_wan(cJSON *msg_json);

error_code_t wan_set(wan_t *wan);
error_code_t wan_get(wan_t **);

error_code_t wan_get_from_setting_file(wan_t **);

error_code_t wan_reload_from_setting_file();

void wan_free(wan_t *);