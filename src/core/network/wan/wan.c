#include "wan.h"
#include "stdlib.h"
#include "utils/uci/app_uci.h"
#include "config/service_controller/service_controller.h"
#include "utils/file/file.h"


static error_code_t wan_save_to_setting_file(wan_t * wan);



cJSON* cvt_wan_to_json(wan_t * wan){
    cJSON * wan_json = cJSON_CreateObject();
    cJSON_AddStringToObject(wan_json, "proto", wan->proto);
    cJSON_AddStringToObject(wan_json, "ipaddr", wan->ipaddr);
    return wan_json;
}

wan_t* cvt_json_to_wan(cJSON * msg_json){
    wan_t *wan = (wan_t*) calloc(1, sizeof(wan_t));
    cJSON * proto_json  = cJSON_GetObjectItemCaseSensitive(msg_json , "proto");
    if(proto_json && cJSON_IsString(proto_json)){
        wan->proto = app_strdup(cJSON_GetStringValue(proto_json));
    }

    cJSON * ip_addr_json  = cJSON_GetObjectItemCaseSensitive(msg_json , "ipaddr");
    if(ip_addr_json && cJSON_IsString(ip_addr_json)){
        wan->ipaddr = app_strdup(cJSON_GetStringValue(ip_addr_json));
    }
    return wan;
}

error_code_t wan_set(wan_t* wan){
    error_code_t err;
    app_uci_set("network.wan.proto" , wan->proto);
    app_uci_set("network.wan.ipaddr" , wan->ipaddr);
    // Save config to json file
    err = wan_save_to_setting_file(wan);
    if(err != ERR_SUCCESS){
        return err;
    }
    // Commit network
    app_uci_commit("network");
    return ERR_SUCCESS;
}

error_code_t wan_get(wan_t** wan){   
    return wan_get_from_setting_file(wan);
}

error_code_t wan_get_from_setting_file(wan_t** wan){
    char * wan_str = read_data(get_config().wan_setting_file);
    cJSON* wan_json = cJSON_Parse(wan_str);
    *wan = cvt_json_to_wan(wan_json);
    free(wan_str);
    cJSON_Delete(wan_json);
    return ERR_SUCCESS;
}

static error_code_t wan_save_to_setting_file(wan_t * wan){
    cJSON * wan_json = cvt_wan_to_json(wan);
    char * wan_str = cJSON_Print(wan_json);
    int ret = write_data(wan_str,strlen(wan_str), get_config().wan_setting_file ,TRUNCATE);
    free(wan_str);
    cJSON_Delete(wan_json);
    if(ret == -1){
        return ERR_INTERNAL_SERVER;
    }
    return ERR_SUCCESS;
}

error_code_t wan_reload_from_setting_file(){
    wan_t * wan;
    error_code_t err;
    err = wan_get_from_setting_file(&wan);
    if(err != ERR_SUCCESS){
        return err;
    }
    return wan_set(wan);
}

void wan_free(wan_t * wan){
    if(wan){
        app_free(wan->proto);
        app_free(wan->ipaddr);
        app_free(wan);
    }
}