#include "utils/cmd/cmd.h"



void start_service (char * service_name){
    char cmd[200];
    sprintf(cmd, "/etc/init.d/%s start" , service_name);
    cmd_ret_t ret = execl_timeout(120000,cmd );
    free(ret.str);
}
void stop_service (char * service_name){
    char cmd[200];
    sprintf(cmd, "/etc/init.d/%s stop" , service_name);
    cmd_ret_t ret = execl_timeout(120000,cmd );
    free(ret.str);
}
void restart_service(char * service_name){
    char cmd[200];
    sprintf(cmd, "/etc/init.d/%s restart" , service_name);
    cmd_ret_t ret = execl_timeout(120000,cmd );
    free(ret.str);
}
void disable_service(char * service_name){
    char cmd[200];
    sprintf(cmd, "/etc/init.d/%s disable" , service_name);
    cmd_ret_t ret = execl_timeout(120000,cmd );
    free(ret.str);
}
void enable_service(char * service_name){
    char cmd[200];
    sprintf(cmd, "/etc/init.d/%s enable" , service_name);
    cmd_ret_t ret = execl_timeout(120000,cmd );
    free(ret.str);
}
