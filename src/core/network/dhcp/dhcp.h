#ifndef __DHCP_H__
#define __DHCP_H__

#include "stdint.h"
#include "stdbool.h"
#include <cjson/cJSON.h>
#include "api/error_code/error_code.h"

#define DHCP_LEASE_FILE "/tmp/dhcp.leases"

typedef struct
{
    int timestamp;
    char *mac;
    char *ip;
    char *host_name;
} dhcp_info_t;

typedef struct
{
    char *name;
    char *domain;
    char *ip;
} custom_hostname_t;

typedef struct
{
    custom_hostname_t *custom_hostname;
    int nb_host_name;
    char **additional_dns;
    int nb_additional_dns;
    char **forwarding_dns;
    int nb_forwarding_dns;
    bool forwarding_dns_enabled;
} dns_t;

typedef struct
{
    char *start_ip;
    int pools;
    bool relay_enabled;
    char *relay_server_address;
    char *relay_local_address;
    dhcp_info_t *static_dhcp4_clients;
    int nb_static_dhcp4_client;
} dhcp4_t;

typedef struct
{
    dns_t *dns;
    dhcp4_t *dhcp4;
} dns_dhcp_t;

typedef struct
{
    dhcp_info_t *list_device;
    int nb_device;
} dhcp4_lease_t;

error_code_t dhcp_lease_get(dhcp_info_t **dhcp_lease_list, int *n_clients);
error_code_t dns_dhcp_get(dns_dhcp_t **dns_dhcp);
error_code_t dns_dhcp_set(dns_dhcp_t *dns_dhcp);

cJSON *cvt_dhcp_info_to_json(dhcp_info_t *dhcp_info);
dhcp_info_t *cvt_json_to_dhcp_info(cJSON *json);

cJSON *cvt_custom_hostname_to_json(custom_hostname_t *custom_hostname);
custom_hostname_t *cvt_json_to_custom_hostname(cJSON *json);

cJSON *cvt_dns_to_json(dns_t *dns);
dns_t *cvt_json_to_dns(cJSON *json);

cJSON *cvt_dhcp4_to_json(dhcp4_t *dhcp4);
dhcp4_t *cvt_json_to_dhcp4(cJSON *json);

cJSON *cvt_dns_dhcp_to_json(dns_dhcp_t *dns_dhcp);
dns_dhcp_t *cvt_json_to_dns_dhcp(cJSON *json);

cJSON *cvt_dhcp4_lease_to_json(dhcp4_lease_t *dhcp4_lease);
dhcp4_lease_t *cvt_json_to_dhcp4_lease(cJSON *json);

dns_dhcp_t *dns_dhcp_get_from_uci();
dns_dhcp_t *dns_dhcp_get_from_setting_file();
void dns_dhcp_reload_from_setting_file();

void dhcp_info_free(dhcp_info_t *);
void custom_hostname_free(custom_hostname_t *);
void dns_free(dns_t *);
void dhcp4_free(dhcp4_t *);
void dns_dhcp_free(dns_dhcp_t *);
void dhcp4_lease_free(dhcp4_lease_t *);

#endif