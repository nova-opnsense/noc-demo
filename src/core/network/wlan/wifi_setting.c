#include "wifi_setting.h"

cJSON *cvt_wifi_setting_to_json(wifi_setting_t* wifi_setting)
{
    if(wifi_setting == NULL){
        return NULL;
    }
    cJSON *wifi_setting_json = cJSON_CreateObject();
    cJSON_AddStringToObject(wifi_setting_json, "country", wifi_setting->country);
    cJSON_AddNumberToObject(wifi_setting_json, "channel", wifi_setting->channel);
    cJSON_AddNumberToObject(wifi_setting_json, "txpower", wifi_setting->txpower);
    return wifi_setting_json;
}

wifi_setting_t* cvt_json_to_wifi_setting(cJSON *json){
    wifi_setting_t *wifi_setting = (wifi_setting_t *)calloc(1,sizeof(wifi_setting_t));
    cJSON* country_json = cJSON_GetObjectItemCaseSensitive(json,"country");
    if(country_json != NULL && cJSON_IsString(country_json)){
        wifi_setting->country = cJSON_GetStringValue(country_json);
    }  
    cJSON* channel_json = cJSON_GetObjectItemCaseSensitive(json,"channel");
    if(channel_json != NULL && cJSON_IsNumber(channel_json)){
        wifi_setting->channel = channel_json->valueint;
    }  
    cJSON* txpower_json = cJSON_GetObjectItemCaseSensitive(json,"txpower");
    if(txpower_json != NULL && cJSON_IsNumber(txpower_json)){
        wifi_setting->txpower = txpower_json->valueint;
    }
    return wifi_setting;
}