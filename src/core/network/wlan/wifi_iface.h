#include <cjson/cJSON.h>
#include "stddef.h"
#include "stdbool.h"

typedef struct
{
    char *ssid;
    char *key;
    // char *mode;
    // int disabled;
    // char *encryption;
    // int invisible;
} wifi_iface_t;

cJSON *cvt_wifi_iface_to_json(wifi_iface_t *wifi_iface);
wifi_iface_t *cvt_json_to_wifi_iface(cJSON *json);
void change_wifi_iface(wifi_iface_t *wifi_iface);

void wifi_iface_free(wifi_iface_t *wifi_iface);