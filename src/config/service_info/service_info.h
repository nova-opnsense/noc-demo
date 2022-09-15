#ifndef __SERVICE_INFO_H__
#define __SERVICE_INFO_H__

char *get_hub_id();
char *get_ezm_id();
char *get_interface_ip(char *wan_interface);
char *get_interface_mac(char *wan_interface);
void init_service_info();

#endif