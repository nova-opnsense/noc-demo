#include "iptables_struct.h"
#include "utils/string/app_string.h"
#include "utils/mem/mem.h"


cJSON* cvt_iptable_to_json(iptable_t * iptable){
    cJSON * iptable_json = cJSON_CreateObject();
    if(!app_mem_empty(iptable , sizeof(iptable_t))){
        cJSON_AddStringToObject(iptable_json, "id", iptable->id);
        cJSON_AddStringToObject(iptable_json, "name", iptable->name);
        cJSON_AddStringToObject(iptable_json, "port_range", iptable->port_range);
        cJSON_AddStringToObject(iptable_json, "ip_mask", iptable->ip_mask);
        cJSON_AddStringToObject(iptable_json, "protocol", iptable->protocol);
        cJSON_AddStringToObject(iptable_json, "dnat_ip", iptable->dnat_ip);
        cJSON_AddStringToObject(iptable_json, "dnat_port_range", iptable->dnat_port_range);
        cJSON_AddStringToObject(iptable_json, "interface", iptable->interface);
        cJSON_AddBoolToObject(iptable_json, "enable", iptable->enable);
    }
    return iptable_json;
}
iptable_t* cvt_json_to_iptable(cJSON * msg_json){
    log_debug("id");
    iptable_t *iptable = (iptable_t*) calloc(1, sizeof(iptable_t));
    cJSON * id_json  = cJSON_GetObjectItemCaseSensitive(msg_json , "id");
    if(id_json && cJSON_IsString(id_json)){
        iptable->id = app_strdup(cJSON_GetStringValue(id_json));
    }
    log_debug("name");
    cJSON * name_json  = cJSON_GetObjectItemCaseSensitive(msg_json , "name");
    if(name_json && cJSON_IsString(name_json)){
        iptable->name = app_strdup(cJSON_GetStringValue(name_json));
    }

    cJSON * port_range_json  = cJSON_GetObjectItemCaseSensitive(msg_json , "port_range");
    if(port_range_json && cJSON_IsString(port_range_json)){
        iptable->port_range = app_strdup(cJSON_GetStringValue(port_range_json));
    }
    cJSON * ip_mask_json  = cJSON_GetObjectItemCaseSensitive(msg_json , "ip_mask");
    if(ip_mask_json && cJSON_IsString(ip_mask_json)){
        iptable->ip_mask = app_strdup(cJSON_GetStringValue(ip_mask_json));
    }

    cJSON * protocol_json  = cJSON_GetObjectItemCaseSensitive(msg_json , "protocol");
    if(protocol_json && cJSON_IsString(protocol_json)){
        iptable->protocol = app_strdup(cJSON_GetStringValue(protocol_json));
    }

    cJSON * dnat_ip_json  = cJSON_GetObjectItemCaseSensitive(msg_json , "dnat_ip");
    if(dnat_ip_json && cJSON_IsString(dnat_ip_json)){
        iptable->dnat_ip = app_strdup(cJSON_GetStringValue(dnat_ip_json));
    }

    cJSON * dnat_port_range_json  = cJSON_GetObjectItemCaseSensitive(msg_json , "dnat_port_range");
    if(dnat_port_range_json && cJSON_IsString(dnat_port_range_json)){
        iptable->dnat_port_range = app_strdup(cJSON_GetStringValue(dnat_port_range_json));
    }

    cJSON * interface_json  = cJSON_GetObjectItemCaseSensitive(msg_json , "interface");
    if(interface_json && cJSON_IsString(interface_json)){
        iptable->interface = app_strdup(cJSON_GetStringValue(interface_json));
    }

    cJSON * enable_json  = cJSON_GetObjectItemCaseSensitive(msg_json , "enable");
    if(enable_json ){
        if(cJSON_IsTrue(enable_json)){
            iptable->enable = true;
        }
        else{
            iptable->enable = false;
        }
    }
    log_debug("enable");
    return iptable;
}


cJSON* cvt_iptable_list_to_json(iptable_list_t * iptable_list){
    int nb_iptables = iptable_list->nb_iptable;
    cJSON * iptables_list_json = cJSON_CreateArray();
    for (size_t i = 0; i < nb_iptables; i++)
    {
        cJSON * iptable_json  = cvt_iptable_to_json(&iptable_list->iptable_list[i]);
        cJSON_AddItemToArray(iptables_list_json, iptable_json);
    }
    return iptables_list_json;
}

iptable_list_t* cvt_json_to_iptable_list(cJSON * msg_json){
    iptable_list_t *iptable_list = (iptable_list_t*)calloc(1, sizeof(iptable_list_t));
    int nb_iptables = cJSON_GetArraySize(msg_json);
    iptable_t * iptables = (iptable_t*)calloc(nb_iptables,sizeof(iptable_t));
    for (size_t i = 0; i < nb_iptables; i++)
    {
        /* code */
        iptables[i] = * cvt_json_to_iptable(cJSON_GetArrayItem(msg_json,i));
    }
    iptable_list->nb_iptable = nb_iptables;
    iptable_list->iptable_list = iptables;
    return iptable_list;
}


cJSON* cvt_iptable_nova_service_to_json(iptable_nova_service_t * iptable_nova_service){
    cJSON * iptable_nova_service_json = cJSON_CreateObject();
    cJSON * port_profiles_json = cvt_iptable_list_to_json(iptable_nova_service->port_profiles);
    cJSON_AddStringToObject(iptable_nova_service_json, "name", iptable_nova_service->name);
    cJSON_AddItemToObject(iptable_nova_service_json, "port_profiles", port_profiles_json);
    return iptable_nova_service_json;
}
iptable_nova_service_t* cvt_json_to_iptable_nova_service(cJSON * msg_json){
    iptable_nova_service_t * iptable_nova_service = (iptable_nova_service_t *)calloc(1,sizeof(iptable_nova_service_t));
    cJSON * name_json  = cJSON_GetObjectItemCaseSensitive(msg_json , "name");
    if(name_json && cJSON_IsString(name_json)){
        iptable_nova_service->name = app_strdup(cJSON_GetStringValue(name_json));
    }
    cJSON* port_profiles_json = cJSON_GetObjectItemCaseSensitive(msg_json , "port_profiles");
    if(port_profiles_json && cJSON_IsArray(port_profiles_json)){
        iptable_nova_service->port_profiles = cvt_json_to_iptable_list(port_profiles_json);
    }
    return iptable_nova_service;
}

cJSON* cvt_iptable_nova_service_list_to_json(iptable_nova_service_list_t * iptable_nova_service_list){
    int nb_iptables_nova_service = iptable_nova_service_list->nb_iptable_nova_services;
    cJSON * iptable_nova_service_array = cJSON_CreateArray();
    for (size_t i = 0; i < nb_iptables_nova_service; i++)
    {
        /* code */
        cJSON * iptable_nova_service_json  = cvt_iptable_nova_service_to_json(&iptable_nova_service_list->iptable_nova_service_list[i]);
        cJSON_AddItemToArray(iptable_nova_service_array, iptable_nova_service_json);
    }
    return iptable_nova_service_array;
}
iptable_nova_service_t* cvt_json_to_iptable_nova_service_list(cJSON * msg_json){
    iptable_nova_service_list_t *iptable_nova_service_list = (iptable_nova_service_list_t*)calloc(1, sizeof(iptable_nova_service_list_t));
    int nb_iptables_nova_services = cJSON_GetArraySize(msg_json);
    iptable_nova_service_t * iptables_nova_services = (iptable_nova_service_t*)calloc(nb_iptables_nova_services,sizeof(iptable_nova_service_t));
    for (size_t i = 0; i < nb_iptables_nova_services; i++)
    {
        iptables_nova_services[i] = * cvt_json_to_iptable_nova_service(cJSON_GetArrayItem(msg_json,i));
    }
    iptable_nova_service_list->iptable_nova_service_list = iptables_nova_services;
    iptable_nova_service_list->nb_iptable_nova_services = nb_iptables_nova_services;
    //log_info("1");
    return iptable_nova_service_list;
}


void iptable_free(iptable_t * iptable){
    if(iptable == NULL){
        return;
    }
    log_debug("iptable->id");
    app_free(iptable->id);
    log_debug("iptable->name");
    app_free(iptable->name);
    log_debug("iptable->port_range");
    app_free(iptable->port_range);
    log_debug("iptable->ip_mask");
    app_free(iptable->ip_mask);
    log_debug("iptable->protocol");
    app_free(iptable->protocol);
    log_debug("iptable->dnat_ip");
    app_free(iptable->dnat_ip);
    log_debug("iptable->dnat_port_range");
    app_free(iptable->dnat_port_range);
    log_debug("iptable->interface");
    app_free(iptable->interface);
}


void iptable_list_free(iptable_list_t * iptable_list){
    log_debug("Free iptable_list");
    if(iptable_list == NULL){
        return;
    }
    for (size_t i = 0; i < iptable_list->nb_iptable; i++)
    {
        /* code */
        iptable_free(&iptable_list->iptable_list[i]);
    }
    app_free(iptable_list->iptable_list);
    app_free(iptable_list);
    log_debug("End Free iptable_list");
}


void iptable_nova_services_free(iptable_nova_service_t * iptable_nova_service){
    log_debug("Free iptable_nova_service");
    if(iptable_nova_service == NULL){
        return;
    }
    app_free(iptable_nova_service->name);
    iptable_list_free(iptable_nova_service->port_profiles);
    log_debug("End free iptable_nova_service");
}

void iptable_nova_services_list_free(iptable_nova_service_list_t * iptable_nova_service_list){
    log_debug("Free iptable_nova_service_list");
    if(iptable_nova_service_list == NULL){
        return;
    }
    for (size_t i = 0; i < iptable_nova_service_list->nb_iptable_nova_services; i++)
    {
        /* code */
        iptable_nova_services_free(&iptable_nova_service_list->iptable_nova_service_list[i]);
    }
    app_free(iptable_nova_service_list->iptable_nova_service_list);
    app_free(iptable_nova_service_list);
    log_debug("End iptable_nova_service_list");
}