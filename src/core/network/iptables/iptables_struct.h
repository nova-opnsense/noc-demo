#pragma once
#include "stdbool.h"
#include <cjson/cJSON.h>
#include "stdlib.h"
#include "config.h"
#include "utils/log/log.h"

typedef struct
{
    char *id;
    char *name;
    char *port_range;
    char *ip_mask;
    char *protocol;
    char *dnat_ip;
    char *dnat_port_range;
    char *interface;
    bool enable;
} iptable_t;

typedef struct
{
    iptable_t *iptable_list;
    int nb_iptable;
} iptable_list_t;

typedef struct
{
    char *name;
    iptable_list_t *port_profiles;
} iptable_nova_service_t;

typedef struct
{
    iptable_nova_service_t *iptable_nova_service_list;
    int nb_iptable_nova_services;
} iptable_nova_service_list_t;

cJSON *cvt_iptable_list_to_json(iptable_list_t *iptable_list);
iptable_list_t *cvt_json_to_iptable_list(cJSON *msg_json);

cJSON *cvt_iptable_to_json(iptable_t *iptable);
iptable_t *cvt_json_to_iptable(cJSON *msg_json);

cJSON *cvt_iptable_nova_service_to_json(iptable_nova_service_t *iptable_nova_service);
iptable_nova_service_t *cvt_json_to_iptable_nova_service(cJSON *msg_json);

cJSON *cvt_iptable_nova_service_list_to_json(iptable_nova_service_list_t *iptable_nova_service_list);
iptable_nova_service_t *cvt_json_to_iptable_nova_service_list(cJSON *msg_json);

void iptable_free(iptable_t *iptable);
void iptable_list_free(iptable_list_t *iptable_list);
void iptable_nova_services_free(iptable_nova_service_t *iptable_nova_service);
void iptable_nova_services_list_free(iptable_nova_service_list_t *iptable_nova_service_list);
