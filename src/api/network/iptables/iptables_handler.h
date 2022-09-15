#ifndef __IPTABLES_HANDLER_H__
#define __IPTABLES_HANDLER_H__

#include <cjson/cJSON.h>

// DNAT
void request_dnat_port_forwarding_profiles(cJSON *msg_json);
void update_dnat_port_forwarding_profiles(cJSON *msg_json);
void create_dnat_port_forwarding_profile(cJSON *msg_json);
void delete_dnat_port_forwarding_profile(cJSON *msg_json);

// DMZ
void request_dnat_dmz(cJSON *msg_json);
void create_dnat_dmz(cJSON *msg_json);

// NOVA Service
void request_nova_services_port_profiles(cJSON *msg_json);
void create_nova_services_port_profiles(cJSON *msg_json);

#endif