#include "lan.h"
#include "config.h"
#include "utils/uci/app_uci.h"
#include "config/service_controller/service_controller.h"
#include "utils/file/file.h"
#include "api/error_code/error_code.h"


static error_code_t lan_save_to_setting_file(lan_t * lan);

cJSON *cvt_lan_to_json(lan_t *lan){
    cJSON * lan_json = cJSON_CreateObject();
    cJSON_AddBoolToObject(lan_json,"bridge_ap_mode" , lan->bridge_ap_mode);
    cJSON_AddStringToObject(lan_json, "proto", lan->proto);
    cJSON_AddStringToObject(lan_json, "ipaddr", lan->ipaddr);
    cJSON_AddStringToObject(lan_json, "netmask", lan->netmask);
    return lan_json;
}


lan_t * cvt_json_to_lan(cJSON *json){
    lan_t *lan = (lan_t*) calloc(1, sizeof(lan_t));
    cJSON * bridge_ap_mode_json  = cJSON_GetObjectItemCaseSensitive(json , "bridge_ap_mode");
    if(bridge_ap_mode_json){
        if(cJSON_IsTrue(bridge_ap_mode_json)){
            lan->bridge_ap_mode = true;
        }
        else{
            lan->bridge_ap_mode = false;
        }
    }
    cJSON * proto_json  = cJSON_GetObjectItemCaseSensitive(json , "proto");
    if(proto_json && cJSON_IsString(proto_json)){
        lan->proto = app_strdup(cJSON_GetStringValue(proto_json));
    }

    cJSON * ip_addr_json  = cJSON_GetObjectItemCaseSensitive(json , "ipaddr");
    if(ip_addr_json && cJSON_IsString(ip_addr_json)){
        lan->ipaddr = app_strdup(cJSON_GetStringValue(ip_addr_json));
    }

    cJSON * netmask_json  = cJSON_GetObjectItemCaseSensitive(json , "netmask");
    if(netmask_json && cJSON_IsString(netmask_json)){
        lan->netmask = app_strdup(cJSON_GetStringValue(netmask_json));
    }
    return lan;
}

error_code_t lan_set(lan_t* lan){
    error_code_t err;
    // Delete previous config of LAN
    // Set lan_setting to deamon
    if(lan->bridge_ap_mode){
        if(app_strstr(lan->proto , "dhcp")){
            app_uci_set("network.lan.proto" , "dhcp");
        }
        else{
            app_uci_set("network.lan.ipaddr" , lan->ipaddr);
            app_uci_set("network.lan.netmask" , lan->netmask);
            app_uci_set("network.lan.proto" , lan->proto);
        }
    }
    else{
        app_uci_set("network.lan.ipaddr" , lan->ipaddr);
        app_uci_set("network.lan.netmask" , lan->netmask);
        app_uci_set("network.lan.proto" , lan->proto);
    }
    //Save to setting file 
    err = lan_save_to_setting_file(lan);
    if(err != ERR_SUCCESS){
        return err;
    }
    // Commit network
    app_uci_commit("network");
    return ERR_SUCCESS;
}


error_code_t lan_get(lan_t** lan){
    // Set lan_setting to deamon
    return lan_get_from_lan_setting_file(lan);
}


error_code_t lan_get_from_lan_setting_file(lan_t** lan){
    char * lan_str = read_data(get_config().lan_setting_file);
    cJSON* lan_json = cJSON_Parse(lan_str);
    *lan = cvt_json_to_lan(lan_json);
    free(lan_str);
    cJSON_Delete(lan_json);
    return ERR_SUCCESS;
}

static error_code_t lan_save_to_setting_file(lan_t * lan){
    cJSON * lan_json = cvt_lan_to_json(lan);
    char * lan_str = cJSON_Print(lan_json);
    int ret = write_data(lan_str,strlen(lan_str), get_config().lan_setting_file ,TRUNCATE);
    free(lan_str);
    cJSON_Delete(lan_json);
    if(ret == -1){
        return ERR_INTERNAL_SERVER;
    }
    return ERR_SUCCESS;
}

error_code_t lan_reload_from_setting_file(){
    lan_t * lan;
    error_code_t err;
    err = lan_get_from_lan_setting_file(&lan);
    if(err != ERR_SUCCESS){
        return err;
    }
    return lan_set(lan);
}


void lan_free(lan_t * lan){
    if(lan){
        app_free(lan->proto);
        app_free(lan->ipaddr);
        app_free(lan->netmask);
        app_free(lan);
    }
}