#include "dhcp.h"
#include "utils/uci/app_uci.h"
#include "utils/file/file.h"
#include "utils/split/split.h"
#include "utils/log/log.h"
#include "config.h"
#include "config/service_controller/service_controller.h"
#include "string.h"


static error_code_t dns_set(dns_t *dns);
static error_code_t dhcp4_set(dhcp4_t *dhcp4);

void dns_dhcp_reload_from_setting_file(){
    dns_dhcp_t * dns_dhcp = dns_dhcp_get_from_setting_file();
    dns_dhcp_set(dns_dhcp);
    dns_dhcp_free(dns_dhcp);
}


error_code_t dhcp_lease_get(dhcp_info_t **dhcp_lease_list,  int *n_clients){
    int nb_lines;
    char ** dhcp_lease_data = read_line(DHCP_LEASE_FILE,&nb_lines);
    // If find not found
    if (dhcp_lease_data == NULL){
        *dhcp_lease_list = NULL;
        *n_clients = 0;
        return ERR_SUCCESS;
    }

    dhcp_info_t* dhcp_client_info = (dhcp_info_t*)calloc(1, nb_lines * sizeof(dhcp_info_t));
    int index = 0;
    for (size_t i = 0; i < nb_lines; i++)
    {
        /* code */
        int nb_fields;
        char ** dhcp_clients_data = split(dhcp_lease_data[i]," ",&nb_fields);
        if(dhcp_clients_data && nb_fields >= 4 && strcmp(dhcp_clients_data[0],"duid")){
            dhcp_client_info[index].timestamp  = atoi(dhcp_clients_data[0]);
            dhcp_client_info[index].mac  = app_strdup(dhcp_clients_data[1]);
            dhcp_client_info[index].ip  = app_strdup(dhcp_clients_data[2]);
            dhcp_client_info[index].host_name  = app_strdup(dhcp_clients_data[3]);
            index ++;
        }
        split_free(dhcp_clients_data,nb_fields);
    }
    *n_clients = nb_lines - 1 ;
    *dhcp_lease_list = dhcp_client_info;
    split_free(dhcp_lease_data,nb_lines);
    return ERR_SUCCESS;
}

static error_code_t dns_set(dns_t *dns){
    char cmd[200];
    char * ret_str;
    ret_str = app_uci_delete("dhcp.@dnsmasq[0].address");
    free(ret_str);
    if(dns->nb_host_name > 0){
        //Delete all host
        // Add all host
        for (size_t i = 0; i < dns->nb_host_name; i++)
        {
            /* code */
            sprintf(cmd ,"/%s/%s", dns->custom_hostname[i].domain , dns->custom_hostname[i].ip);
            char * err_str = app_uci_add_list("dhcp.@dnsmasq[0].address", cmd);
            free(err_str);
        }
    }
    // log_info("nb_additional_dns: %d" , dns->nb_additional_dns);
    ret_str = app_uci_delete("dhcp.lan.dhcp_option");
    free(ret_str);
    if(dns->nb_additional_dns){
        // Add additional dns
        memset(cmd, 0 , 200);
        strcat(cmd , "6");
        for (size_t i = 0; i < dns->nb_additional_dns; i++)
        {
            /* code */
            strcat(cmd, ",");
            strcat(cmd, dns->additional_dns[i]);
        }
        char * err_str = app_uci_add_list("dhcp.lan.dhcp_option" ,cmd);
        if(err_str != NULL){
            log_error("%s",err_str);
        }
        free(err_str);
    }
    // log_info("nb_forwarding_dns: %d" , dns->nb_forwarding_dns);

    app_uci_delete("dhcp.@dnsmasq[0].server");
    if(dns->forwarding_dns_enabled){
        if(dns->nb_forwarding_dns){
            for (size_t i = 0; i < dns->nb_forwarding_dns; i++)
            {
                /* code */
                char * err_str = app_uci_add_list("dhcp.@dnsmasq[0].server",dns->forwarding_dns[i]);
                if(err_str != NULL){
                    log_error("%s",err_str);
                };
                free(err_str);
            }
        }
    }
}

static error_code_t dhcp4_set(dhcp4_t *dhcp4){
    char * err_str;
    // Set dhcp relay
    if(dhcp4->relay_enabled){
        app_uci_delete("dhcp.@relay[0]");
        err_str = app_uci_add("dhcp","relay");
        free(err_str);
        
        err_str =  app_uci_set("dhcp.@relay[-1].interface" ,"lan");
        if(err_str != NULL){
            log_error("%s",err_str);
        };
        free(err_str);
        err_str = app_uci_set("dhcp.@relay[-1].local_addr" ,dhcp4->relay_local_address);
        if(err_str != NULL){
            log_error("%s",err_str);
        };
        free(err_str);
        err_str = app_uci_set("dhcp.@relay[-1].server_addr" ,dhcp4->relay_server_address);
        if(err_str != NULL){
            log_error("%s",err_str);
        };
        free(err_str);
    }
    // Set static ip
    // Delete all host before
    char * str = NULL;
    do
    {
        str = app_uci_delete("dhcp.@host[-1]");
    } while (str == NULL);
    
    // Set new host
    for (size_t i = 0; i < dhcp4->nb_static_dhcp4_client; i++)
    {   
        /* code */
        err_str = app_uci_add("dhcp", "host");
        free(err_str);
        err_str = app_uci_set("dhcp.@host[-1].name" , dhcp4->static_dhcp4_clients[i].host_name);
        if(err_str != NULL){
            log_error("%s",err_str);
        };
        free(err_str);
        err_str = app_uci_set("dhcp.@host[-1].mac" , dhcp4->static_dhcp4_clients[i].mac);
        if(err_str != NULL){
            log_error("%s",err_str);
        };
        free(err_str);
        err_str = app_uci_set("dhcp.@host[-1].ip" , dhcp4->static_dhcp4_clients[i].ip);
        if(err_str != NULL){
            log_error("%s",err_str);
        };
        free(err_str);
        err_str = app_uci_set("dhcp.@host[-1].dns" , "1");
        if(err_str != NULL){
            log_error("%s",err_str);
        };
    }
    // Set dhcp pool
    // Check IPv4 format
    int nb_splits;
    char ** lanIP = split(dhcp4->start_ip , "." , &nb_splits);
    if(lanIP && nb_splits == 4){
        char * start_ip = lanIP[3];
        // log_info("startip %s", start_ip);
        char limit_ip[200];
        sprintf(limit_ip, "%d" , atoi(lanIP[3]) + dhcp4->pools );
        // log_info("limitip %s", limit_ip);
        err_str = app_uci_set("dhcp.lan.start" , start_ip);
        if(err_str != NULL){
            log_error("%s",err_str);
        };
        free(err_str);
        err_str = app_uci_set("dhcp.lan.limit" , limit_ip);
        if(err_str != NULL){
            log_error("%s",err_str);
        };
        free(err_str);
    }
    split_free(lanIP,nb_splits);
    return ERR_SUCCESS;
}

error_code_t dns_dhcp_set(dns_dhcp_t* dns_dhcp){
    error_code_t err;
    dns_dhcp_save_to_setting_file(dns_dhcp);
    // Setting up dns
    err = dns_set(dns_dhcp->dns);
    if(err != ERR_SUCCESS){
        return err;
    }
    // Setting up dhcp4
    err = dhcp4_set(dns_dhcp->dhcp4);
    if(err != ERR_SUCCESS){
        return err;
    }
    // Commit dhcp
    app_uci_commit("dhcp");
    return ERR_SUCCESS;
}


error_code_t dns_dhcp_get(dns_dhcp_t** dns_dhcp){
    * dns_dhcp = dns_dhcp_get_from_setting_file();
    return ERR_SUCCESS;
}

cJSON *cvt_dhcp_info_to_json(dhcp_info_t * dhcp_info){
    if(dhcp_info == NULL){
        return NULL;
    }
    cJSON* dhcp_info_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(dhcp_info_json,"timestamp",dhcp_info->timestamp);
    cJSON_AddStringToObject(dhcp_info_json,"mac",dhcp_info->mac);
    cJSON_AddStringToObject(dhcp_info_json,"ip" , dhcp_info->ip);
    cJSON_AddStringToObject(dhcp_info_json,"host_name" , dhcp_info->host_name);
    return dhcp_info_json;
}
dhcp_info_t *cvt_json_to_dhcp_info(cJSON * json){
    dhcp_info_t * dhcp_info = (dhcp_info_t*) calloc(1, sizeof(dhcp_info_t));
    cJSON* timestamp_json = cJSON_GetObjectItemCaseSensitive(json,"timestamp");
    if(timestamp_json != NULL &&  cJSON_IsNumber(timestamp_json)){
        dhcp_info->timestamp =  timestamp_json->valueint;
    }
    cJSON* mac_json = cJSON_GetObjectItemCaseSensitive(json,"mac");
    if(mac_json != NULL &&  cJSON_IsString(mac_json)){
        dhcp_info->mac = app_strdup(cJSON_GetStringValue(mac_json));
    } 
    cJSON* ip_json = cJSON_GetObjectItemCaseSensitive(json,"ip");
    if(ip_json != NULL &&  cJSON_IsString(ip_json)){
        dhcp_info->ip = app_strdup(cJSON_GetStringValue(ip_json));
    } 
    cJSON* host_name_json = cJSON_GetObjectItemCaseSensitive(json,"host_name");
    if(host_name_json != NULL &&  cJSON_IsString(host_name_json)){
        dhcp_info->host_name = app_strdup(cJSON_GetStringValue(host_name_json));
    }
    return dhcp_info;
}

cJSON *cvt_custom_hostname_to_json(custom_hostname_t * custom_hostname){
    if(custom_hostname == NULL){
        return NULL;
    }
    cJSON* custom_hostname_json = cJSON_CreateObject();
    cJSON_AddStringToObject(custom_hostname_json,"name",custom_hostname->name);
    cJSON_AddStringToObject(custom_hostname_json,"domain",custom_hostname->domain);
    cJSON_AddStringToObject(custom_hostname_json,"ip" , custom_hostname->ip);
    return custom_hostname_json;
}
custom_hostname_t *cvt_json_to_custom_hostname(cJSON * json){
    custom_hostname_t * custom_hostname = (custom_hostname_t*) calloc(1, sizeof(custom_hostname_t));
    cJSON* name_json = cJSON_GetObjectItemCaseSensitive(json,"name");
    if(name_json != NULL &&  cJSON_IsString(name_json)){
        custom_hostname->name = app_strdup(cJSON_GetStringValue(name_json));
    } 
    cJSON* domain_json = cJSON_GetObjectItemCaseSensitive(json,"domain");
    if(domain_json != NULL &&  cJSON_IsString(domain_json)){
        custom_hostname->domain = app_strdup(cJSON_GetStringValue(domain_json));
    } 
    cJSON* ip_json = cJSON_GetObjectItemCaseSensitive(json,"ip");
    if(ip_json != NULL &&  cJSON_IsString(ip_json)){
        custom_hostname->ip = app_strdup(cJSON_GetStringValue(ip_json));
    }
    return custom_hostname;
}

cJSON *cvt_dns_to_json(dns_t* dns){
    if(dns == NULL){
        return NULL;
    }
    cJSON* dns_json = cJSON_CreateObject();
    cJSON* custom_hostname_list_json =  cJSON_CreateArray();
    for (size_t i = 0; i < dns->nb_host_name; i++)
    {
        /* code */
        cJSON *custom_hostname_json = cvt_custom_hostname_to_json(&(dns->custom_hostname[i]));
        cJSON_AddItemToArray(custom_hostname_list_json,custom_hostname_json);
    }
    cJSON_AddItemToObject(dns_json , "custom_hostname" , custom_hostname_list_json);
    cJSON* additional_dns_json =  cJSON_CreateStringArray(dns->additional_dns , dns->nb_additional_dns);
    cJSON_AddItemToObject(dns_json , "additional_dns" , additional_dns_json);
    cJSON* forwarding_dns_json =  cJSON_CreateStringArray(dns->forwarding_dns , dns->nb_forwarding_dns);
    cJSON_AddItemToObject(dns_json , "forwarding_dns" , forwarding_dns_json);
    cJSON_AddBoolToObject(dns_json , "forwarding_dns_enabled" , dns->forwarding_dns_enabled);
    return dns_json;
}
dns_t *cvt_json_to_dns(cJSON * json){
    dns_t * dns = (dns_t*) calloc(1, sizeof(dns_t));
    cJSON* custom_hostname_json = cJSON_GetObjectItemCaseSensitive(json,"custom_hostname");
    if(custom_hostname_json != NULL){
        dns->nb_host_name = cJSON_GetArraySize(custom_hostname_json);
        if(dns->nb_host_name > 0){
            dns->custom_hostname = (custom_hostname_t*)calloc(dns->nb_host_name,sizeof(custom_hostname_t));
            for (size_t i = 0; i < dns->nb_host_name; i++)
            {
                dns->custom_hostname[i] = * cvt_json_to_custom_hostname(cJSON_GetArrayItem(custom_hostname_json,i));
            }
        }
        else{
            return NULL;
        }
    } 
    cJSON* additional_dns_json = cJSON_GetObjectItemCaseSensitive(json,"additional_dns");
    if(additional_dns_json != NULL){
        dns->nb_additional_dns = cJSON_GetArraySize(additional_dns_json);
        if(dns->nb_additional_dns > 0){
            dns->additional_dns = (char **)calloc( dns->nb_additional_dns ,sizeof(char*));
            for (size_t i = 0; i < dns->nb_additional_dns; i++)
            {
                cJSON *additional_json = cJSON_GetArrayItem(additional_dns_json,i);
                if(additional_json){
                    dns->additional_dns[i] = app_strdup(cJSON_GetStringValue(additional_json));
                }
            }
        }
    }
    cJSON* forwawrding_dns_list_json = cJSON_GetObjectItemCaseSensitive(json,"forwarding_dns");
    if(forwawrding_dns_list_json != NULL){
        dns->nb_forwarding_dns = cJSON_GetArraySize(forwawrding_dns_list_json) ;
        if( dns->nb_forwarding_dns > 0){
            dns->forwarding_dns = (char **)calloc( dns->nb_forwarding_dns ,sizeof(char*));
            for (size_t i = 0; i < dns->nb_forwarding_dns; i++)
            {
                cJSON *forwawrding_dns_json = cJSON_GetArrayItem(forwawrding_dns_list_json,i);
                if(forwawrding_dns_json){
                    dns->forwarding_dns[i] = app_strdup(cJSON_GetStringValue(forwawrding_dns_json));
                }
            }
        }
    }
    cJSON* forwarding_dns_enabled_json = cJSON_GetObjectItemCaseSensitive(json,"forwarding_dns_enabled");
    if(forwarding_dns_enabled_json && cJSON_IsNumber(forwarding_dns_enabled_json) ){
        dns->forwarding_dns = forwarding_dns_enabled_json->valueint;
    }
    return dns;
}

cJSON *cvt_dhcp4_to_json(dhcp4_t* dhcp4){
    if(dhcp4 == NULL){
        return NULL;
    }
    cJSON* dhcp4_json = cJSON_CreateObject();

    cJSON_AddStringToObject(dhcp4_json,"start_ip", dhcp4->start_ip);
    cJSON_AddNumberToObject(dhcp4_json,"pools", dhcp4->pools);
    cJSON_AddBoolToObject(dhcp4_json,"relay_enabled", dhcp4->relay_enabled);
    cJSON_AddStringToObject(dhcp4_json,"relay_server_address", dhcp4->relay_server_address);
    cJSON_AddStringToObject(dhcp4_json,"relay_local_address", dhcp4->relay_local_address);

    cJSON* static_dhcp4_client_list_json =  cJSON_CreateArray();
    for (size_t i = 0; i < dhcp4->nb_static_dhcp4_client; i++)
    {
        /* code */
        cJSON *static_dhcp4_client_json = cvt_dhcp_info_to_json(&(dhcp4->static_dhcp4_clients[i]));
        cJSON_AddItemToArray(static_dhcp4_client_list_json,static_dhcp4_client_json);
    }
    cJSON_AddItemToObject(dhcp4_json , "static_dhcp4_clients" , static_dhcp4_client_list_json);
    return dhcp4_json;
}

dhcp4_t *cvt_json_to_dhcp4(cJSON * json){
    //log_info("1");
    dhcp4_t * dhcp4 = (dhcp4_t*) calloc(1, sizeof(dhcp4_t));
    cJSON* start_ip_json = cJSON_GetObjectItemCaseSensitive(json,"start_ip");
    if(start_ip_json && cJSON_IsString(start_ip_json)){
        dhcp4->start_ip = app_strdup(cJSON_GetStringValue(start_ip_json));
    }
    //log_info("1");
    cJSON* pools_json = cJSON_GetObjectItemCaseSensitive(json,"pools");
    if(pools_json && cJSON_IsNumber(pools_json)){
        dhcp4->pools =  pools_json->valueint;
    }
    //log_info("1");
    cJSON* relay_enabled_json = cJSON_GetObjectItemCaseSensitive(json,"relay_enabled");
    if(relay_enabled_json && cJSON_IsNumber(relay_enabled_json)){
        dhcp4->relay_enabled =  relay_enabled_json->valueint;
    }
    //log_info("1");
    cJSON* relay_server_address_json = cJSON_GetObjectItemCaseSensitive(json,"relay_server_address");
    if(relay_server_address_json && cJSON_IsString(relay_server_address_json)){
        dhcp4->relay_server_address = app_strdup(cJSON_GetStringValue(relay_server_address_json));
    }
    //log_info("1");
    cJSON* relay_local_address_json = cJSON_GetObjectItemCaseSensitive(json,"relay_local_address");
    if(relay_local_address_json && cJSON_IsString(relay_local_address_json)){
        dhcp4->relay_local_address = app_strdup(cJSON_GetStringValue(relay_local_address_json));
    }
    //log_info("1");
    cJSON* static_dhcp4_client_list_json = cJSON_GetObjectItemCaseSensitive(json,"static_dhcp4_clients");
    cJSON *static_dhcp4_client_json;
    if(static_dhcp4_client_list_json != NULL){
        dhcp4->nb_static_dhcp4_client = cJSON_GetArraySize(static_dhcp4_client_list_json);
        if( dhcp4->nb_static_dhcp4_client > 0){
            dhcp4->static_dhcp4_clients = (dhcp_info_t*)calloc( dhcp4->nb_static_dhcp4_client ,sizeof(dhcp_info_t));
            for (size_t i = 0; i < dhcp4->nb_static_dhcp4_client; i++)
            {
                static_dhcp4_client_json = cJSON_GetArrayItem(static_dhcp4_client_list_json,i);
                if(static_dhcp4_client_json){
                    dhcp_info_t * dhcp_info =  cvt_json_to_dhcp_info(static_dhcp4_client_json);
                    dhcp4->static_dhcp4_clients[i] = *dhcp_info;
                }
            }
        }
    }
    return dhcp4;
}

cJSON *cvt_dns_dhcp_to_json(dns_dhcp_t* dns_dhcp){
    cJSON * dns_dhcp_json = cJSON_CreateObject();
    cJSON * dns_json = cvt_dns_to_json(dns_dhcp->dns);
    cJSON * dhcp4_json = cvt_dhcp4_to_json(dns_dhcp->dhcp4);
    cJSON_AddItemToObject(dns_dhcp_json, "dns" , dns_json);
    cJSON_AddItemToObject(dns_dhcp_json, "dhcp4", dhcp4_json);
    return dns_dhcp_json;
}
dns_dhcp_t *cvt_json_to_dns_dhcp(cJSON * json){
    dns_dhcp_t * dns_dhcp = (dns_dhcp_t *) calloc( 1, sizeof(dns_dhcp_t));
    cJSON * dns_json = cJSON_GetObjectItemCaseSensitive(json , "dns");
    if(dns_json){
        dns_dhcp->dns = cvt_json_to_dns(dns_json);
    }
    cJSON * dhcp4_json = cJSON_GetObjectItemCaseSensitive(json , "dhcp4");
    if(dhcp4_json){
        dns_dhcp->dhcp4 = cvt_json_to_dhcp4(dhcp4_json);
    }
    return dns_dhcp;
}

cJSON *cvt_dhcp4_lease_to_json(dhcp4_lease_t* dhcp4_lease){
    cJSON* dhcp4_lease_array = cJSON_CreateArray();
    for (size_t i = 0; i < dhcp4_lease->nb_device; i++)
    {
        /* code */
        cJSON *dhcp_lease_device = cvt_dhcp_info_to_json(&dhcp4_lease->list_device[i]);
        cJSON_AddItemToArray(dhcp4_lease_array , dhcp_lease_device);
    }
    return dhcp4_lease_array;
}
dhcp4_lease_t *cvt_json_to_dhcp4_lease(cJSON * json){
    dhcp4_lease_t* dhcp4_lease = (dhcp4_lease_t* )calloc(1, sizeof(dhcp4_lease_t));
    int nb_device = cJSON_GetArraySize(json);
    dhcp4_lease->nb_device = nb_device;
    dhcp4_lease->list_device = (dhcp_info_t*)calloc(sizeof(dhcp_info_t), nb_device);
    for (size_t i = 0; i < nb_device; i++)
    {
        /* code */
        dhcp4_lease->list_device[i] = *cvt_json_to_dhcp_info(cJSON_GetArrayItem(json,i));
    }
    return dhcp4_lease;
}



dns_dhcp_t * dns_dhcp_get_from_uci(){
    return NULL;
}
dns_dhcp_t * dns_dhcp_get_from_setting_file(){
    char * dns_dhcp_str = read_data(get_config().dhcp_setting_file);
    cJSON* dns_dhcp_json = cJSON_Parse(dns_dhcp_str);
    dns_dhcp_t *dns_dhcp = cvt_json_to_dns_dhcp(dns_dhcp_json);

    cJSON_Delete(dns_dhcp_json);
    free(dns_dhcp_str);
    return dns_dhcp;
}

void dns_dhcp_save_to_setting_file(dns_dhcp_t * dns_dhcp){
    cJSON * dns_dhcp_json = cvt_dns_dhcp_to_json(dns_dhcp);
    char * dns_dhcp_str = cJSON_Print(dns_dhcp_json);
    int ret = write_data(dns_dhcp_str,strlen(dns_dhcp_str), get_config().dhcp_setting_file , TRUNCATE);
    //Free mem
    cJSON_Delete(dns_dhcp_json);
    free(dns_dhcp_str);
}



void dhcp_info_free(dhcp_info_t* dhcp_info){
    log_debug("Begin dhcp_info_free");
    if(dhcp_info){
        app_free(dhcp_info->mac);
        app_free(dhcp_info->ip);
        app_free(dhcp_info->host_name);
    }
    log_debug("End dhcp_info_free");
}
void custom_hostname_free(custom_hostname_t* custom_hostname){
    if(custom_hostname){
        app_free(custom_hostname->name);
        app_free(custom_hostname->domain);
        app_free(custom_hostname->ip);
    }
}
void dns_free(dns_t* dns){
    log_debug("Begin dns_free");
    if(dns){
        for (size_t i = 0; i < dns->nb_host_name; i++)
        {
            custom_hostname_free(&dns->custom_hostname[i]);
        }
        app_free(dns->custom_hostname);
        for (size_t i = 0; i < dns->nb_additional_dns; i++)
        {
            app_free(dns->additional_dns[i]);
        }
        app_free(dns->additional_dns);
        for (size_t i = 0; i < dns->nb_forwarding_dns; i++)
        {
            app_free(dns->forwarding_dns[i]);
        }
        app_free(dns->nb_forwarding_dns);
        app_free(dns);
    }
    log_debug("End dns_free");
}
void dhcp4_free(dhcp4_t* dhcp4){
    log_debug("Begin dhcp4_free");
    if(dhcp4){
        app_free(dhcp4->start_ip);
        app_free(dhcp4->relay_server_address);
        app_free(dhcp4->relay_local_address);
        for (size_t i = 0; i < dhcp4->nb_static_dhcp4_client; i++)
        {
            dhcp_info_free(&dhcp4->static_dhcp4_clients[i]);
        }
        log_debug("%p",dhcp4->static_dhcp4_clients);
        log_debug("Free dhcp4->static_dhcp4_clients");
        app_free(dhcp4->static_dhcp4_clients);
        log_debug("Free dhcp4");
        app_free(dhcp4);
    }   
    log_debug("End dhcp4_free");
}
void dns_dhcp_free(dns_dhcp_t* dns_dhcp){
    log_debug("Begin dns_dhcp_free");
    if(dns_dhcp){
        dns_free(dns_dhcp->dns);
        dhcp4_free(dns_dhcp->dhcp4);
        app_free(dns_dhcp);
    }
    log_debug("End dns_dhcp_free");
}
void dhcp4_lease_free(dhcp4_lease_t* dhcp4_lease){
    log_debug("Begin dhcp4_lease_free");
    if(dhcp4_lease){
        for (size_t i = 0; i < dhcp4_lease->nb_device; i++)
        {
            /* code */
            dhcp_info_free(&dhcp4_lease->list_device[i]);
        }
        app_free(dhcp4_lease->list_device);
        app_free(dhcp4_lease);
    }
    log_debug("End dhcp4_lease_free");
}
