#include "iptables.h"

error_code_t iptables_refresh(){
    error_code_t err;
    err = iptables_default_setting();
    if( err != ERR_SUCCESS){
        return err;
    }
    err = iptable_dnat_dmz_reload_from_setting_file();
    if( err != ERR_SUCCESS){
        return err;
    }
    err = iptable_dnat_forwarding_reload_from_setting_file();
    if( err != ERR_SUCCESS){
        return err;
    }
    err = iptable_nova_service_reload_from_setting_file();
    if( err != ERR_SUCCESS){
        return err;
    }
    err = iptables_restrict_basic();
    if( err != ERR_SUCCESS){
        return err;
    }
    return ERR_SUCCESS;
}
