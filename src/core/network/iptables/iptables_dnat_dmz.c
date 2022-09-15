#include "iptables_dnat_dmz.h"
#include "iptables_common.h"
#include "utils/file/file.h"
#include "utils/cmd/cmd.h"
#include "iptables.h"

error_code_t iptables_dnat_dmz_profiles_set(iptable_t * iptable_list);
error_code_t iptable_dnat_dmz_save_to_setting_file(iptable_t *);

error_code_t iptable_dnat_dmz_request_from_setting_file(iptable_t ** iptables_out ){
    char * iptables_str = read_data(get_config().iptables_dnat_dmz_setting_file);
    cJSON * iptables_json = cJSON_Parse(iptables_str);
    iptable_t * iptables = cvt_json_to_iptable(iptables_json);
    *iptables_out = iptables;
    free(iptables_str);
    cJSON_Delete(iptables_json);
    return ERR_SUCCESS;
}

error_code_t iptable_dnat_dmz_update(iptable_t * dnat_dmz_profiles){
    error_code_t err;
    err = iptable_dnat_dmz_save_to_setting_file(dnat_dmz_profiles);
    if(err != ERR_SUCCESS){
        return err;
    };
    err = iptables_refresh();
    if(err != ERR_SUCCESS){
        return err;
    };
    return ERR_SUCCESS;
}
error_code_t iptable_dnat_dmz_request(iptable_t ** iptables_list_out ){
    return iptable_dnat_dmz_request_from_setting_file(iptables_list_out);
}






error_code_t iptable_dnat_dmz_save_to_setting_file(iptable_t * iptables){
    cJSON * iptable_list_json = cvt_iptable_to_json(iptables);
    char * iptable_list_str = cJSON_Print(iptable_list_json);
    int ret = write_data(iptable_list_str,strlen(iptable_list_str), get_config().iptables_dnat_dmz_setting_file ,TRUNCATE);
    cJSON_Delete(iptable_list_json);
    free(iptable_list_str);
    if(ret == -1){
        return ERR_INTERNAL_SERVER;
    }
    return ERR_SUCCESS;
}

error_code_t iptables_dnat_dmz_profiles_set(iptable_t * iptables){
    char cmd [200];
    char tmp[200];
    // Add new profiles
    if(iptables->enable){
        sprintf(cmd , "iptables -t nat -A PREROUTING  -i %s" , WAN_INTERFACE);
        if(iptables->ip_mask && strcmp(iptables->ip_mask,"") != 0){
            //ip_mask not empty
            strcat(cmd , " -s ");
            strcat(cmd , iptables->ip_mask);
        }
        sprintf(tmp , "%s -j DNAT --to-destination %s",cmd ,  iptables->dnat_ip);
        //Exec iptable command
        log_info("%s", tmp);
        execl_non_timeout(tmp);
    }
    return ERR_SUCCESS;
}

error_code_t iptable_dnat_dmz_reload_from_setting_file(){
    error_code_t err;
    iptable_t * iptable_dnat_dmz_list;
    err = iptable_dnat_dmz_request_from_setting_file(&iptable_dnat_dmz_list);
    if(err != ERR_SUCCESS){
        return err;
    };
    err = iptables_dnat_dmz_profiles_set(iptable_dnat_dmz_list);
    iptable_free(iptable_dnat_dmz_list);
    if(err != ERR_SUCCESS){
        return err;
    };
    return ERR_SUCCESS;
}
