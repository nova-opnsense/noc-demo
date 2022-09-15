#include <cjson/cJSON.h>
#include "stddef.h"
#include "stdbool.h"

typedef struct
{
    char *country;
    int channel;
    int txpower;
} wifi_setting_t;

cJSON *cvt_wifi_setting_to_json(wifi_setting_t *wifi_setting);
wifi_setting_t *cvt_json_to_wifi_setting(cJSON *json);