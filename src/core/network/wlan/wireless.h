#include <cjson/cJSON.h>
#include "stddef.h"
#include "stdbool.h"
#include "wifi_iface.h"
#include "wifi_setting.h"

typedef struct
{
    wifi_iface_t *wifi_iface;
} wireless_t;

void wireless_free(wireless_t *wireless);

cJSON *cvt_wireless_to_json(wireless_t *wireless);
wireless_t *cvt_json_to_wireless(cJSON *json);
void exec_wifi_iface(wifi_iface_t *wifi_iface);
