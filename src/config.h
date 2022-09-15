#pragma once
#include "stdio.h"

typedef struct
{
    char *mosquitto_host;
    int mosquitto_port;
    char *sub_topic_pattern;
    char *pub_topic_pattern;
    char *broadcast_request_name;
    char *mesh_setting_file;
    char *mesh_devices_file;
    char *dhcp_setting_file;
    char *lan_setting_file;
    char *wan_setting_file;
    char *route_setting_file;
    char *iptables_dnat_forwarding_setting_file;
    char *iptables_dnat_dmz_setting_file;
    char *iptables_nova_setting_file;
    char *general_error_table_file;
    char *general_mqtt_error_table_file;
    char *ezm_error_table_file;
    char *setup_agent_file;
    char *setup_controller_file;
    char *setup_wps_file;
    char *service_info_file;
    char *ezm_version;
    char *ezm_name;
    int pair_check_polling_time;
    int pair_timeout;
    int uuid_size;
} config_t;

#define CONFIG_FILE_PATH_DEFAULT "/data/config/noc/noc.cnf"
#define LOG_LEVEL_DEFAULT LOG_ERR
#define MOSQUITTO_HOST_DEFAULT "localhost"
#define MOSQUITTO_PORT_DEFAULT 1883
#define SUB_TOPIC_PATTERN_DEFAULT "hub/event/%s"
#define PUB_TOPIC_PATTERN_DEFAULT "ezm/event/%s"
#define BROADCAST_REQUEST_NAME_DEFAULT "broadcast.ezm.device_list.reported"
#define MESH_SETTING_FILE_DEFAULT "/data/config/ezm/mesh_setting.json"

#define MESH_DEVICES_FILE_DEFAULT "/data/config/ezm/mesh_devices.json"

#define DHCP_SETTING_FILE_DEFAULT "/data/config/ezm/dns_dhcp_setting.json"

#define LAN_SETTING_FILE_DEFAULT "/data/config/ezm/lan_setting.json"

#define WAN_SETTING_FILE_DEFAULT "/data/config/ezm/wan_setting.json"

#define ROUTE_SETTING_FILE_DEFAULT "/data/config/ezm/route_setting.json"

#define IPTABLES_DNAT_FORWARDING_SETTING_FILE_DEFAULT "/data/config/ezm/iptable_dnat_forwarding_setting.json"

#define IPTABLES_DNAT_DMZ_SETTING_FILE_DEFAULT "/data/config/ezm/iptable_dnat_dmz_setting.json"

#define IPTABLES_NOVA_SETTING_FILE_DEFAULT "/data/config/ezm/iptable_nova_setting.json"

#define GENERAL_ERROR_TABLE_FILE_DEFAULT "/data/error_tables/100_general.json"
#define GENERAL_MQTT_ERROR_TABLE_FILE_DEFAULT "/data/error_tables/101_general_mqtt.json"
#define EZM_ERROR_TABLE_FILE_DEFAULT "/data/error_tables/240_ezm.json"
#define SETUP_AGENT_FILE_DEFAULT "/data/script/ezm/setup_agent.sh"
#define SETUP_CONTROLLER_FILE_DEFAULT "/data/script/ezm/setup_controller.sh"
#define SETUP_WPS_FILE_DEFAULT "/data/script/ezm/setup_wps.sh"
#define SERVICE_INFO_FILE_DEFAULT "/data/database/hub/services_info.json"

#define LAN_IP_DEFAULT_FOR_STANDALONE "192.168.124.1"
#define LAN_IP_DEFAULT_FOR_MESH "192.168.123.1"

#ifndef EZM_VERSION
#define EZM_VERSION_DEFAULT "1.0"
#else
#define EZM_VERSION_DEFAULT EZM_VERSION
#endif

#define EZM_NAME_DEFAULT "NovaController"
#define PAIR_CHECK_POLLING_TIME_DEFAULT 5000 // 5 seconds
#define PAIR_TIMEOUT_DEFAULT 60000           // 120 seconds
#define UUID_SIZE_DEFAULT 37

#define AUTHEN_DIR "/data/config/ezm/authen/%s"
#define CAFILE "/data/config/ezm/authen/%s/cafile.pem" //%s is ezm_id
#define SECRET "/data/config/ezm/authen/%s/secret.txt" //%s is ezm_id

#define AVAHI_EZM_SERVICE_FILE "/etc/avahi/services/ezm.service"
#define AVAHI_THING_SERVICE_FILE "/etc/avahi/services/thing.service"
#define WAN_INTERFACE "eth0"
#define MESSAGE_LENGTH_MAX 8192 // 8Kbs

void init_config_default();

void init_config_from_file(char *path_to_file);

config_t get_config();