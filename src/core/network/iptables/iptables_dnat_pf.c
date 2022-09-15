#include "iptables_dnat_pf.h"
#include "iptables_common.h"
#include "config.h"
#include "utils/file/file.h"
#include "utils/uuid/uuid.h"
#include "iptables.h"


error_code_t iptables_dnat_forwarding_save_to_setting_file(iptable_list_t *);

error_code_t iptables_dnat_forwarding_create(iptable_list_t * dnat_forwarding_port_porfiles ,  iptable_list_t ** iptables_list_out ){
    char cmd [200];
    error_code_t err;
    int port_profiles = dnat_forwarding_port_porfiles->nb_iptable;
    iptable_list_t * dnat_forwarding_port_porfiles_setting;
    err = iptables_dnat_forwarding_request(&dnat_forwarding_port_porfiles_setting); 
    if(err != ERR_SUCCESS){
        return err;
    } 
    // Realloc for new dnat forwarding
    dnat_forwarding_port_porfiles_setting->iptable_list = (iptable_t*)realloc(dnat_forwarding_port_porfiles_setting->iptable_list , (dnat_forwarding_port_porfiles_setting->nb_iptable + port_profiles)*sizeof(iptable_t));
    dnat_forwarding_port_porfiles_setting->nb_iptable += port_profiles;
    for ( size_t i = 0; i < port_profiles; i++)
    {
        iptable_t iptables = dnat_forwarding_port_porfiles->iptable_list[i];
        // Add to dnat_port_forwarding_setting
        iptables.id = uuid_gen("iptable");
        log_info("nb_previous %d , nb_iptables_list %d" ,dnat_forwarding_port_porfiles_setting->nb_iptable , port_profiles );
        dnat_forwarding_port_porfiles_setting->iptable_list[dnat_forwarding_port_porfiles_setting->nb_iptable - port_profiles + i] = iptables;
    }
    err = iptables_dnat_forwarding_save_to_setting_file(dnat_forwarding_port_porfiles_setting);
    *iptables_list_out = dnat_forwarding_port_porfiles_setting;
    if(err != ERR_SUCCESS){
        return err;
    } 
    err = iptables_refresh();
    if(err != ERR_SUCCESS){
        return err;
    } 
    return ERR_SUCCESS;
}


error_code_t iptables_dnat_forwarding_request(iptable_list_t ** iptable_list_out){
    return iptable_dnat_forwarding_request_from_setting_file(iptable_list_out);
}

error_code_t iptables_dnat_forwarding_update(iptable_list_t * dnat_forwarding_port_porfiles , iptable_list_t ** iptable_list_out ){
    char cmd [200];
    bool found_id = false;
    error_code_t err;
    int port_profiles = dnat_forwarding_port_porfiles->nb_iptable;
    iptable_list_t * dnat_forwarding_port_porfiles_setting;
    err = iptables_dnat_forwarding_request(&dnat_forwarding_port_porfiles_setting);
    for ( size_t i = 0; i < port_profiles; i++)
    {
        /* code */
        for (size_t j = 0; j < dnat_forwarding_port_porfiles_setting->nb_iptable; j++)
        {
            /* code */
            if(app_strstr(dnat_forwarding_port_porfiles->iptable_list[i].id,dnat_forwarding_port_porfiles_setting->iptable_list[j].id )){
                dnat_forwarding_port_porfiles_setting->iptable_list[j] = dnat_forwarding_port_porfiles->iptable_list[i];
                found_id = true;
            }
        }
    }
    err = iptables_dnat_forwarding_save_to_setting_file(dnat_forwarding_port_porfiles_setting);
    *iptable_list_out = dnat_forwarding_port_porfiles_setting;
    if(err != ERR_SUCCESS){
        return err;
    }
    err = iptables_refresh();
    if(err != ERR_SUCCESS){
        return err;
    }
    if(!found_id){
        return ERR_ID_MISMATCHED;
    }
    return ERR_SUCCESS;
}

error_code_t iptables_dnat_forwarding_delete(iptable_list_t * dnat_forwarding_port_porfiles , iptable_list_t ** iptable_list_out){
    char cmd [200];
    int port_profiles = dnat_forwarding_port_porfiles->nb_iptable;
    error_code_t err;
    bool found_id = false;
    iptable_list_t * dnat_forwarding_port_porfiles_setting;
    err = iptables_dnat_forwarding_request(&dnat_forwarding_port_porfiles_setting);
    for ( size_t i = 0; i < port_profiles; i++)
    {
        /* code */
        for (size_t j = 0; j < dnat_forwarding_port_porfiles_setting->nb_iptable; j++)
        {
            /* code */
            if(app_strstr(dnat_forwarding_port_porfiles->iptable_list[i].id,dnat_forwarding_port_porfiles_setting->iptable_list[j].id )){
                // Remove dnat_profile from dnat_profile setting
                memmove(&dnat_forwarding_port_porfiles_setting->iptable_list[j],
                        &dnat_forwarding_port_porfiles_setting->iptable_list[j+1],
                        (dnat_forwarding_port_porfiles_setting->nb_iptable - j - 1 ) * sizeof(iptable_t));
                dnat_forwarding_port_porfiles_setting->nb_iptable --;
                found_id = true;
                break;
            }
        }
    }
    *iptable_list_out = dnat_forwarding_port_porfiles_setting;
    err = iptables_dnat_forwarding_save_to_setting_file(dnat_forwarding_port_porfiles_setting);
    if(err != ERR_SUCCESS){
        return err;
    }
    err = iptables_refresh();
    if(err != ERR_SUCCESS){
        return err;
    }
    if(!found_id){
        return ERR_ID_MISMATCHED;
    }
    return ERR_SUCCESS;
}

error_code_t iptables_dnat_forwarding_save_to_setting_file(iptable_list_t * iptable_list){
    cJSON * iptable_list_json = cvt_iptable_list_to_json(iptable_list);
    char * iptable_list_str = cJSON_Print(iptable_list_json);
    int ret = write_data(iptable_list_str,strlen(iptable_list_str), get_config().iptables_dnat_forwarding_setting_file ,TRUNCATE);
    if(ret != NULL){
        return ERR_INTERNAL_SERVER;
    }
    return ERR_SUCCESS;
}

error_code_t iptable_dnat_forwarding_reload_from_setting_file(){
    iptable_list_t * iptable_dnat_forwarding_list;
    error_code_t err;
    err = iptable_dnat_forwarding_request_from_setting_file(&iptable_dnat_forwarding_list);
    if(err != ERR_SUCCESS){
        return err;
    }
    err = iptables_open_port_profiles(iptable_dnat_forwarding_list);
    if(err != ERR_SUCCESS){
        return err;
    }
    return err;
}


error_code_t iptable_dnat_forwarding_request_from_setting_file(iptable_list_t ** iptable_list_out){
    char* iptables_list_str = read_data(get_config().iptables_dnat_forwarding_setting_file);
    cJSON * iptables_list_json = cJSON_Parse(iptables_list_str);
    iptable_list_t * iptable_list = cvt_json_to_iptable_list(iptables_list_json);
    *iptable_list_out = iptable_list;
    free(iptables_list_str);
    cJSON_Delete(iptables_list_json);
    return ERR_SUCCESS;
}