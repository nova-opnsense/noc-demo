#ifndef __DHCP_HANDLER_H__
#define __DHCP_HANDLER_H__
#include <cjson/cJSON.h>

void create_dns_dhcp(cJSON *msg_json);
void request_dns_dhcp(cJSON *msg_json);
void request_dhcp4_leases(cJSON *msg_json);

#endif