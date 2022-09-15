#pragma once
#include <cjson/cJSON.h>
#include "stddef.h"
#include "stdbool.h"
#include "time.h"
#include "api/error_code/error_code.h"
#include "core/mesh/setting/mesh_setting.h"
#include "core/mesh/topology/topology.h"
#include "core/network/dhcp/dhcp.h"
#include "core/network/lan/lan.h"
#include "core/network/wan/wan.h"
#include "core/network/route/route.h"
#include "core/network/iptables/iptables.h"
#include "core/mesh/quality/quality.h"

typedef enum
{
    MSG_EMPTY,
    MSG_MESH_DEVICE_LIST,
    MSG_MESH_SETTING,
    MSG_MESH_QUALITY,
    MSG_NETWORK_LAN,
    MSG_NETWORK_WAN,
    MSG_NETWORK_DNS_DHCP,
    MSG_NETWORK_DHCP4_LEASES,
    MSG_NETWORK_STATIC_ROUTE,
    MSG_IPTABLES_DNAT_PROFILE,
    MSG_IPTABLES_DNAT_DMZ_PROFILE,
    MSG_IPTABLE_NOVA_SERVICE_PORT_PROFILE
} msg_type_t;

typedef union
{
    // mesh
    mesh_setting_t *mesh_setting;
    device_list_t *device_list;
    quality_profile_list_t *quality_profile_list;
    // dns-dhcp
    dns_dhcp_t *dns_dhcp;
    dhcp4_lease_t *dhcp4_lease;
    // lan
    lan_t *lan;
    // wan
    wan_t *wan;
    // static route
    route_list_t *routes_list;
    // iptables - forwarding
    iptable_list_t *iptable_list;
    // iptables - dnat-dmz
    iptable_t *iptable;
    iptable_nova_service_list_t *iptable_nova_service_list;
} data_t;

typedef struct
{
    error_t *error;
    data_t *data;
} respond_data_t;

typedef struct
{
    char *name;
    char *scope;
    char *source;
    char *home_id;
    char *user_id;
    char *id;
    char *auth_token;
    char *parent_id;
    char *client_id;
    data_t *data;
    time_t created_time;
} msg_request_t;

typedef struct
{
    char *name;
    char *scope;
    char *source;
    char *home_id;
    char *user_id;
    char *id;
    char *auth_token;
    char *parent_id;
    char *client_id;
    respond_data_t *respond_data;
    time_t created_time;
} msg_respond_t;

typedef struct
{
    char *mqtt_message_request;
    void (*message_handler)(cJSON *);
} message_handler_mapping_t;

cJSON *cvt_data_to_json(data_t *data, msg_type_t msg_type);
data_t *cvt_json_to_data(cJSON *json, msg_type_t msg_type);

cJSON *cvt_respond_data_to_json(respond_data_t *respond_data, msg_type_t msg_type);
respond_data_t *cvt_json_to_respond_data(cJSON *json, msg_type_t msg_type);

msg_request_t *cvt_json_to_msg_request(cJSON *json, msg_type_t msg_type);
cJSON *cvt_msg_request_to_json(msg_request_t *msg_request, msg_type_t msg_type);

msg_respond_t *cvt_json_to_msg_respond(cJSON *json, msg_type_t msg_type);
cJSON *cvt_msg_respond_to_json(msg_respond_t *msg_respond, msg_type_t msg_type);

msg_respond_t *cvt_msg_request_to_msg_respond(msg_request_t *msg_request);
char *cvt_req_name_to_res_name(char *req_name);

void msg_request_free(msg_request_t *msg_request, msg_type_t type);
void msg_respond_free(msg_respond_t *msg_respond, msg_type_t type);
void respond_data_free(respond_data_t *respond_data, msg_type_t type);
void data_free(data_t *data, msg_type_t type);
