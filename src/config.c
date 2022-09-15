#include "config.h"
#include "string.h"
#include "utils/file/file.h"
#include "utils/split/split.h"
#include "utils/log/log.h"
#include "utils/dotenv/dotenv.h"

static config_t config_entry;

void load_config(char *config_file)
{
    env_load(config_file, false);

    config_entry.mosquitto_host = env_get("mosquitto_host", MOSQUITTO_HOST_DEFAULT);
    config_entry.mosquitto_port = env_get_int("mosquitto_port", MOSQUITTO_PORT_DEFAULT);
    config_entry.sub_topic_pattern = env_get("sub_topic_pattern", SUB_TOPIC_PATTERN_DEFAULT);
    config_entry.pub_topic_pattern = env_get("pub_topic_pattern", PUB_TOPIC_PATTERN_DEFAULT);
    config_entry.broadcast_request_name = env_get("broadcast_request_name", BROADCAST_REQUEST_NAME_DEFAULT);
    config_entry.mesh_setting_file = env_get("mesh_setting_file", MESH_SETTING_FILE_DEFAULT);
    config_entry.mesh_devices_file = env_get("mesh_devices_file", MESH_DEVICES_FILE_DEFAULT);
    config_entry.dhcp_setting_file = env_get("dhcp_setting_file", DHCP_SETTING_FILE_DEFAULT);
    config_entry.lan_setting_file = env_get("lan_setting_file", LAN_SETTING_FILE_DEFAULT);
    config_entry.wan_setting_file = env_get("wan_setting_file", WAN_SETTING_FILE_DEFAULT);
    config_entry.route_setting_file = env_get("route_setting_file", ROUTE_SETTING_FILE_DEFAULT);
    config_entry.iptables_dnat_forwarding_setting_file = env_get("iptables_dnat_forwarding_setting_file", IPTABLES_DNAT_FORWARDING_SETTING_FILE_DEFAULT);
    config_entry.iptables_dnat_dmz_setting_file = env_get("iptables_dnat_dmz_setting_file", IPTABLES_DNAT_DMZ_SETTING_FILE_DEFAULT);
    config_entry.iptables_nova_setting_file = env_get("iptables_nova_setting_file", IPTABLES_NOVA_SETTING_FILE_DEFAULT);
    config_entry.general_error_table_file = env_get("general_error_table_file", GENERAL_ERROR_TABLE_FILE_DEFAULT);
    config_entry.general_mqtt_error_table_file = env_get("general_mqtt_error_table_file", GENERAL_MQTT_ERROR_TABLE_FILE_DEFAULT);
    config_entry.ezm_error_table_file = env_get("ezm_error_table_file", EZM_ERROR_TABLE_FILE_DEFAULT);
    config_entry.setup_agent_file = env_get("setup_agent_file", SETUP_AGENT_FILE_DEFAULT);
    config_entry.setup_controller_file = env_get("setup_controller_file", SETUP_CONTROLLER_FILE_DEFAULT);
    config_entry.setup_wps_file = env_get("setup_wps_file", SETUP_WPS_FILE_DEFAULT);
    config_entry.service_info_file = env_get("service_info_file", SERVICE_INFO_FILE_DEFAULT);
    config_entry.ezm_version = env_get("ezm_version", EZM_VERSION_DEFAULT);
    config_entry.ezm_name = env_get("ezm_name", EZM_NAME_DEFAULT);
    config_entry.pair_check_polling_time = env_get_int("pair_check_polling_time", PAIR_CHECK_POLLING_TIME_DEFAULT);
    config_entry.pair_timeout = env_get_int("pair_timeout", PAIR_TIMEOUT_DEFAULT);
    config_entry.uuid_size = env_get_int("uuid_size", UUID_SIZE_DEFAULT);
}

config_t get_config()
{
    return config_entry;
}
