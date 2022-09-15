#include "wifi_iface.h"
#include "utils/log/log.h"
#include "utils/cmd/cmd.h"
#include "utils/timer/timer.h"
#include "utils/mem/mem.h"
// #define CONFIG_WAN_DELAY    1000
// void repacd_restart();
// void config_wan(size_t time_id, void *user_data);

cJSON *cvt_wifi_iface_to_json(wifi_iface_t *wifi_iface)
{
    if (wifi_iface == NULL)
    {
        return NULL;
    }
    cJSON *wifi_iface_json = cJSON_CreateObject();
    cJSON_AddStringToObject(wifi_iface_json, "ssid", wifi_iface->ssid);
    cJSON_AddStringToObject(wifi_iface_json, "key", wifi_iface->key);
    return wifi_iface_json;
}

wifi_iface_t *cvt_json_to_wifi_iface(cJSON *json)
{
    log_debug("cvt_json_to_wifi_iface");
    wifi_iface_t *wifi_iface = (wifi_iface_t *)calloc(1, sizeof(wifi_iface_t));
    cJSON *ssid_json = cJSON_GetObjectItemCaseSensitive(json, "ssid");
    if (ssid_json != NULL && cJSON_IsString(ssid_json))
    {
        log_debug("ssid_json");
        wifi_iface->ssid = app_strdup(cJSON_GetStringValue(ssid_json));
    }
    cJSON *key_json = cJSON_GetObjectItemCaseSensitive(json, "key");
    if (key_json != NULL && cJSON_IsString(key_json))
    {
        log_debug("key_json");
        wifi_iface->key = app_strdup(cJSON_GetStringValue(key_json));
    }
    // cJSON* mode_json = cJSON_GetObjectItemCaseSensitive(json,"mode");
    // if(mode_json != NULL &&  cJSON_IsString(mode_json)){
    //     wifi_iface->mode = cJSON_GetStringValue(mode_json);
    // }
    // cJSON* disabled_json = cJSON_GetObjectItemCaseSensitive(json,"disabled");
    // if(disabled_json != NULL &&  cJSON_IsNumber(disabled_json)){
    //     wifi_iface->disabled = disabled_json->valueint;
    // }
    // cJSON* encryption_json = cJSON_GetObjectItemCaseSensitive(json,"encryption");
    // if(encryption_json != NULL &&  cJSON_IsString(encryption_json)){
    //     wifi_iface->encryption = cJSON_GetStringValue(encryption_json);
    // }

    // cJSON* invisible_json = cJSON_GetObjectItemCaseSensitive(json,"invisible");
    // if(invisible_json != NULL &&  cJSON_IsNumber(invisible_json)){
    //     wifi_iface->invisible = invisible_json->valueint;
    // }
    return wifi_iface;
}

void change_wifi_iface(wifi_iface_t *wifi_iface)
{
    cmd_ret_t ret;
    char cmd[200];
    if (wifi_iface->ssid == NULL || wifi_iface->key == NULL)
    {
        log_error("ssid or password is null");
        return;
    }
    log_info("Start Change Wifi Iface with ssid: %s , passwd: %s", wifi_iface->ssid, wifi_iface->key);
    sprintf(cmd, "uci set repacd.MAPConfig.FronthaulSSID='%s'", wifi_iface->ssid);
    ret = execl_timeout(1000, cmd);
    free(ret.str);
    memset(cmd, 0, 200);
    sprintf(cmd, "uci set repacd.MAPConfig.FronthaulKey='%s'", wifi_iface->key);
    ret = execl_timeout(1000, cmd);
    free(ret.str);
    memset(cmd, 0, 200);
    sprintf(cmd, "uci set repacd.MAPConfig.FirstConfigRequired='1'");
    ret = execl_timeout(1000, cmd);
    free(ret.str);
    memset(cmd, 0, 200);
    sprintf(cmd, "uci commit repacd");
    ret = execl_timeout(1000, cmd);
    free(ret.str);
    memset(cmd, 0, 200);
    sprintf(cmd, "/etc/init.d/repacd restart");
    ret = execl_timeout(1000, cmd);
    free(ret.str);
    memset(cmd, 0, 200);
}

// void repacd_restart(){
//     cmd_ret_t ret;
//     char cmd[200];
//     sprintf(cmd, "/etc/init.d/repacd restart");
//     ret = execl_timeout(1000,cmd);
//     memset(cmd,0,200);
//     start_timer(CONFIG_WAN_DELAY,config_wan,TIMER_SINGLE_SHOT,NULL);
// }

// void config_wan(size_t time_id, void *user_data){
//     cmd_ret_t ret;
//     char cmd[200];
//     ret = execl_timeout(1000,cmd);
//     memset(cmd,0,200);
//     sprintf(cmd, "uci set network.wan=interface");
//     log_info("%s",cmd);
//     ret = execl_timeout(1000,cmd);
//     memset(cmd,0,200);
//     sprintf(cmd, "uci set network.wan.proto='dhcp'");
//     log_info("%s",cmd);
//     ret = execl_timeout(1000,cmd);
//     memset(cmd,0,200);
//     sprintf(cmd, "uci set network.wan.ifname='eth0'");
//     log_info("%s",cmd);
//     ret = execl_timeout(1000,cmd);
//     memset(cmd,0,200);
//     sprintf(cmd, "uci set network.wan.type='bridge'");
//     log_info("%s",cmd);
//     ret = execl_timeout(1000,cmd);
//     memset(cmd,0,200);
//     sprintf(cmd, "uci set network.lan.ifname='eth1 eth2 eth3 eth4'");
//     log_info("%s",cmd);
//     ret = execl_timeout(1000,cmd);
//     memset(cmd,0,200);
//     sprintf(cmd, "uci commit network");
//     log_info("%s",cmd);
//     ret = execl_timeout(1000,cmd);
//     memset(cmd,0,200);
//     sprintf(cmd, "/etc/init.d/network restart");
//     log_info("%s",cmd);
//     ret = execl_timeout(1000,cmd);
// }

void wifi_iface_free(wifi_iface_t *wifi_iface)
{
    app_free(wifi_iface->ssid);
    app_free(wifi_iface->key);
    app_free(wifi_iface);
}