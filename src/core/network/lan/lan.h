#pragma once

#include "stdbool.h"
#include <cjson/cJSON.h>
#include "api/error_code/error_code.h"

typedef struct
{
    bool bridge_ap_mode;
    char *proto;
    char *ipaddr;
    char *netmask;
} lan_t;

cJSON *cvt_lan_to_json(lan_t *);
lan_t *cvt_json_to_lan(cJSON *);

error_code_t lan_set(lan_t *);
error_code_t lan_get(lan_t **);
error_code_t lan_get_from_lan_setting_file(lan_t **);

error_code_t lan_default_setting();
error_code_t lan_reload_from_setting_file();

void lan_free(lan_t *lan);
