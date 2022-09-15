#include "wireless.h"
#include "utils/log/log.h"

cJSON *cvt_wireless_to_json(wireless_t *wireless)
{
    if (wireless == NULL)
    {
        return NULL;
    }
    // log_info("1");
    cJSON *wireless_json = cJSON_CreateObject();
    // log_info("1");
    cJSON *wifi_iface_json = cvt_wifi_iface_to_json(wireless->wifi_iface);
    cJSON_AddItemToObject(wireless_json, "wifi_iface", wifi_iface_json);
    return wireless_json;
}

wireless_t *cvt_json_to_wireless(cJSON *json)
{
    log_debug("cvt_json_to_wireless");
    wireless_t *wireless = (wireless_t *)calloc(1, sizeof(wireless_t));
    cJSON *wifi_iface_json = cJSON_GetObjectItemCaseSensitive(json, "wifi_iface");
    if (wifi_iface_json != NULL && cJSON_IsObject(wifi_iface_json))
    {
        log_debug("cvt_json_to_wireless");
        wireless->wifi_iface = cvt_json_to_wifi_iface(wifi_iface_json);
    }
    return wireless;
}

void wireless_free(wireless_t *wireless)
{
    if (wireless == NULL)
    {
        return;
    }
    wifi_iface_free(wireless->wifi_iface);
    app_free(wireless);
}