#include "iptables_nova_port.h"
#include "iptables_common.h"
#include "utils/file/file.h"
#include "iptables.h"

error_code_t iptable_nova_service_save_to_setting_file(iptable_nova_service_list_t * iptable_nova_service_list);

error_code_t iptable_nova_service_request_from_setting_file(iptable_nova_service_list_t ** nova_list_out){
    char * iptable_nova_service_list_str = read_data(get_config().iptables_nova_setting_file);
    cJSON * iptable_nova_service_list_json = cJSON_Parse(iptable_nova_service_list_str);
    iptable_nova_service_list_t * iptable_nova_service_list = cvt_json_to_iptable_nova_service_list(iptable_nova_service_list_json);
    *nova_list_out = iptable_nova_service_list;
    // Free mem
    free(iptable_nova_service_list_str);
    cJSON_Delete(iptable_nova_service_list_json);
    return ERR_SUCCESS;
}


error_code_t iptable_nova_service_set(iptable_nova_service_list_t * iptable_nova_service_list){
    error_code_t err;
    err = iptable_nova_service_save_to_setting_file(iptable_nova_service_list);
    if(err != ERR_SUCCESS){
        return err;
    }
    err = iptables_refresh();
    if(err != ERR_SUCCESS){
        return err;
    }
    return ERR_SUCCESS;
}

error_code_t iptable_nova_service_get(iptable_nova_service_list_t ** nova_list_out){
    return iptable_nova_service_request_from_setting_file(nova_list_out);
}


error_code_t iptable_nova_service_save_to_setting_file(iptable_nova_service_list_t * iptable_nova_service_list){
    cJSON * iptable_nova_service_list_json = cvt_iptable_nova_service_list_to_json(iptable_nova_service_list);
    char * iptable_nova_service_list_str = cJSON_Print(iptable_nova_service_list_json);
    int ret = write_data(iptable_nova_service_list_str,strlen(iptable_nova_service_list_str), get_config().iptables_nova_setting_file ,TRUNCATE);
    cJSON_Delete(iptable_nova_service_list_json);
    free(iptable_nova_service_list_str);
    if(ret != -1){
        return ERR_INTERNAL_SERVER;
    }
    return ERR_SUCCESS;
}

error_code_t iptable_nova_service_reload_from_setting_file(){
    error_code_t err;
    iptable_nova_service_list_t * iptable_nova_service_list;
    err = iptable_nova_service_request_from_setting_file(&iptable_nova_service_list);
    if(err != ERR_SUCCESS){
        return err;
    }
    int nb_nova_service_list = iptable_nova_service_list->nb_iptable_nova_services;
    for (size_t i = 0; i < nb_nova_service_list; i++)
    {
        /* code */
        iptable_list_t * nova_service_iptable_list = iptable_nova_service_list->iptable_nova_service_list[i].port_profiles;
        err = iptables_open_port_profiles(nova_service_iptable_list);
        if(err != ERR_SUCCESS){
            iptable_nova_services_list_free(iptable_nova_service_list);
            return err;
        }
    }
    iptable_nova_services_list_free(iptable_nova_service_list);
    return ERR_SUCCESS;
}