#include "wps.h"
#include "core/mesh/core/emt.h"
#include "config.h"
#include "stdbool.h"

void add_device_check(void * user_data){
    log_info("add_device_check");
    int *data = (int*)user_data;
    int prev_nb_device = data[0];
    int *flag = &data[1];
    int nb_device;
    device_info_t *device_list = emt_topology(TOPOLOGY_S2,NULL,&nb_device);
    if(nb_device > prev_nb_device){
        *flag = 1;
    }
    else{
        *flag = 0;
    }
    return;
}

void remove_device_check(void * user_data){
    log_info("remove_device_check");
    int *data = (int*)user_data;
    int prev_nb_device = data[0];
    int *flag = &data[1];
    int nb_device;
    device_info_t *device_list = emt_topology(TOPOLOGY_S2,NULL,&nb_device);
    if(nb_device < prev_nb_device){
        *flag = 1;
    }
    else{
        *flag = 0;
    }
    return;
}

void timeout_check(void * user_data){
    log_info("timeout");
    bool *timeout_flag = (bool*)user_data;
    (*timeout_flag)  = true;
    return;
}


void polling_wps(void * user_data){
    log_info("polling wps");
    execl_non_timeout(get_config().setup_wps_file);
    return;
}