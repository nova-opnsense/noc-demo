#include "mesh_setting.h"
#include "config.h"
#include "utils/log/log.h"
#include "utils/uci/app_uci.h"
#include "utils/cmd/cmd.h"
#include "utils/mem/mem.h"
#include "utils/file/file.h"
#include "config/service_info/service_info.h"
#include "core/mesh/topology/device.h"

cJSON *cvt_rssi_threshhold_to_json(rssi_threshhold_t *rssi_threshhold)
{
    if (rssi_threshhold == NULL)
    {
        return NULL;
    }
    cJSON *rssi_threshhold_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(rssi_threshhold_json, "up", rssi_threshhold->up);
    cJSON_AddNumberToObject(rssi_threshhold_json, "down", rssi_threshhold->down);
    return rssi_threshhold_json;
}
cJSON *cvt_antenna_installed_to_json(antenna_installed_t *antenna_installed)
{
    if (antenna_installed == NULL)
    {
        return NULL;
    }
    cJSON *antenna_installed_json = cJSON_CreateObject();
    cJSON_AddBoolToObject(antenna_installed_json, "up", antenna_installed->up);
    cJSON_AddBoolToObject(antenna_installed_json, "down", antenna_installed->down);
    return antenna_installed_json;
}
cJSON *cvt_mesh_setting_to_json(mesh_setting_t *mesh_data)
{
    if (mesh_data == NULL)
    {
        return NULL;
    }
    // log_info("1");
    cJSON *mesh_data_json = cJSON_CreateObject();
    // log_info("1");
    cJSON *rssi_threshhold_json = cvt_rssi_threshhold_to_json(mesh_data->rssi_threshhold);
    // log_info("1");
    cJSON *antenna_installed_json = cvt_antenna_installed_to_json(mesh_data->antenna_installed);
    // log_info("1");
    cJSON *wireless_json = cvt_wireless_to_json(mesh_data->wireless);
    cJSON_AddStringToObject(mesh_data_json, "mesh_id", mesh_data->mesh_id);
    // log_info("1");
    cJSON_AddStringToObject(mesh_data_json, "mesh_name", mesh_data->mesh_name);
    // log_info("1");
    cJSON_AddStringToObject(mesh_data_json, "type", mesh_data->type);
    // log_info("1");
    cJSON_AddItemToObject(mesh_data_json, "rssi_threshhold", rssi_threshhold_json);
    // log_info("1");
    cJSON_AddItemToObject(mesh_data_json, "antenna_installed", antenna_installed_json);
    // log_info("1");
    cJSON_AddStringToObject(mesh_data_json, "backhaul_channel", mesh_data->backhaul_channel);
    // log_info("1");
    cJSON_AddNumberToObject(mesh_data_json, "active_time_after_set", mesh_data->active_time_after_set);
    // log_info("1");
    cJSON_AddStringToObject(mesh_data_json, "work_mode", mesh_data->work_mode);
    // log_info("1");
    cJSON_AddNumberToObject(mesh_data_json, "weight_mesh", mesh_data->weight_mesh);
    // log_info("1");
    cJSON_AddStringToObject(mesh_data_json, "mesh_link_connection", mesh_data->mesh_link_connection);
    // log_info("1");
    cJSON_AddNumberToObject(mesh_data_json, "minimum_mesh_link_snr", mesh_data->minimum_mesh_link_snr);
    // log_info("1");
    cJSON_AddItemToObject(mesh_data_json, "wireless", wireless_json);
    return mesh_data_json;
}

rssi_threshhold_t *cvt_json_to_rssi_threshhold(cJSON *json)
{
    rssi_threshhold_t *rssi_threshhold = (rssi_threshhold_t *)calloc(1, sizeof(rssi_threshhold_t));
    cJSON *up_json = cJSON_GetObjectItemCaseSensitive(json, "up");
    if (up_json != NULL && cJSON_IsNumber(up_json))
    {
        rssi_threshhold->up = up_json->valueint;
    }
    cJSON *down_json = cJSON_GetObjectItemCaseSensitive(json, "down");
    if (down_json != NULL && cJSON_IsNumber(down_json))
    {
        rssi_threshhold->down = down_json->valueint;
    }
    return rssi_threshhold;
}
antenna_installed_t *cvt_json_to_antenna_installed(cJSON *json)
{
    antenna_installed_t *antenna_installed = (antenna_installed_t *)calloc(1, sizeof(antenna_installed_t));
    cJSON *up_json = cJSON_GetObjectItemCaseSensitive(json, "up");
    if (up_json != NULL && cJSON_IsBool(up_json))
    {
        antenna_installed->up = cJSON_IsTrue(up_json);
    }
    cJSON *down_json = cJSON_GetObjectItemCaseSensitive(json, "down");
    if (down_json != NULL && cJSON_IsBool(down_json))
    {
        antenna_installed->down = cJSON_IsTrue(down_json);
    }
    return antenna_installed;
}
mesh_setting_t *cvt_json_to_mesh_setting(cJSON *json)
{
    mesh_setting_t *mesh_data = (mesh_setting_t *)calloc(1, sizeof(mesh_setting_t));
    log_debug("Get mesh_id");
    cJSON *mesh_id_json = cJSON_GetObjectItemCaseSensitive(json, "mesh_id");
    if (mesh_id_json != NULL && cJSON_IsString(mesh_id_json))
    {
        mesh_data->mesh_id = app_strdup(cJSON_GetStringValue(mesh_id_json));
    }

    log_debug("Get mesh_name");
    cJSON *mesh_name_json = cJSON_GetObjectItemCaseSensitive(json, "mesh_name");
    if (mesh_name_json != NULL && cJSON_IsString(mesh_name_json))
    {
        mesh_data->mesh_name = app_strdup(cJSON_GetStringValue(mesh_name_json));
    }

    log_debug("Get type");
    cJSON *type_json = cJSON_GetObjectItemCaseSensitive(json, "type");
    if (type_json != NULL && cJSON_IsString(type_json))
    {
        mesh_data->type = app_strdup(cJSON_GetStringValue(type_json));
    }

    log_debug("Get rssi_threshhold");
    cJSON *rssi_threshhold_json = cJSON_GetObjectItemCaseSensitive(json, "rssi_threshhold");
    if (rssi_threshhold_json != NULL && cJSON_IsObject(rssi_threshhold_json))
    {
        mesh_data->rssi_threshhold = cvt_json_to_rssi_threshhold(rssi_threshhold_json);
    }

    log_debug("Get antenna_installed");
    cJSON *antenna_installed_json = cJSON_GetObjectItemCaseSensitive(json, "antenna_installed");
    if (antenna_installed_json != NULL && cJSON_IsObject(antenna_installed_json))
    {
        mesh_data->antenna_installed = cvt_json_to_antenna_installed(antenna_installed_json);
    }

    log_debug("Get backhaul_channel");
    cJSON *backhaul_channel_json = cJSON_GetObjectItemCaseSensitive(json, "backhaul_channel");
    if (backhaul_channel_json != NULL && cJSON_IsString(backhaul_channel_json))
    {
        mesh_data->backhaul_channel = app_strdup(cJSON_GetStringValue(backhaul_channel_json));
    }

    log_debug("Get active_time_after_set");
    cJSON *active_time_after_set_json = cJSON_GetObjectItemCaseSensitive(json, "active_time_after_set");
    if (active_time_after_set_json != NULL && cJSON_IsNumber(active_time_after_set_json))
    {
        mesh_data->active_time_after_set = active_time_after_set_json->valueint;
    }

    log_debug("Get work_mode");
    cJSON *work_mode_json = cJSON_GetObjectItemCaseSensitive(json, "work_mode");
    if (work_mode_json != NULL && cJSON_IsString(work_mode_json))
    {
        mesh_data->work_mode = app_strdup(cJSON_GetStringValue(work_mode_json));
    }

    log_debug("Get weight_mesh");
    cJSON *weight_mesh_json = cJSON_GetObjectItemCaseSensitive(json, "weight_mesh");
    if (weight_mesh_json != NULL && cJSON_IsNumber(weight_mesh_json))
    {
        mesh_data->weight_mesh = weight_mesh_json->valueint;
    }

    log_debug("Get mesh_link_connection");
    cJSON *mesh_link_connection_json = cJSON_GetObjectItemCaseSensitive(json, "mesh_link_connection");
    if (mesh_link_connection_json != NULL && cJSON_IsString(mesh_link_connection_json))
    {
        mesh_data->mesh_link_connection = app_strdup(cJSON_GetStringValue(mesh_link_connection_json));
    }

    log_debug("Get minimum_mesh_link_snr");
    cJSON *minimum_mesh_link_snr_json = cJSON_GetObjectItemCaseSensitive(json, "minimum_mesh_link_snr");
    if (minimum_mesh_link_snr_json != NULL && cJSON_IsNumber(minimum_mesh_link_snr_json))
    {
        mesh_data->minimum_mesh_link_snr = minimum_mesh_link_snr_json->valueint;
    }

    log_debug("Get wireless");
    cJSON *wireless_json = cJSON_GetObjectItemCaseSensitive(json, "wireless");
    if (wireless_json != NULL && cJSON_IsObject(wireless_json))
    {
        mesh_data->wireless = cvt_json_to_wireless(wireless_json);
    }
    return mesh_data;
}

int get_work_mode()
{
    char *mesh_setting_str = read_data(get_config().mesh_setting_file);
    cJSON *mesh_setting_json = cJSON_Parse(mesh_setting_str);
    mesh_setting_t *mesh_setting = cvt_json_to_mesh_setting(mesh_setting_json);

    // Free memory
    cJSON_Delete(mesh_setting_json);
    free(mesh_setting_str);
    if (!strcmp(mesh_setting->work_mode, "standalone"))
    {
        mesh_setting_free(mesh_setting);
        return STANDALONE_MODE;
    }
    else if (!strcmp(mesh_setting->work_mode, "agent"))
    {
        mesh_setting_free(mesh_setting);
        return AGENT_MODE;
    }
    else if (!strcmp(mesh_setting->work_mode, "controller"))
    {
        mesh_setting_free(mesh_setting);
        return CONTROLLER_MODE;
    }

    return -1;
}

char *get_work_mode_str()
{
    char *mesh_setting_str = read_data(get_config().mesh_setting_file);
    cJSON *mesh_setting_json = cJSON_Parse(mesh_setting_str);
    mesh_setting_t *mesh_setting = cvt_json_to_mesh_setting(mesh_setting_json);
    return mesh_setting->work_mode;
}

int run_standalone_mode()
{
    log_info("Standalone Mode");
    if (request_mqtt_bridge_client())
    {
        log_info("Delete Mosquitto Bridge Client");
        delete_mqtt_bridge_client();
    }
    if (request_mqtt_bridge_server())
    {
        log_info("Delete Mosquitto Bridge Server");
        // Enable Authen Service, Thing Service
        delete_mqtt_bridge_server();
        restart_mqtt_service();
    }

    // Stop Ezmesh Service
    log_info("Stop Ezmesh Deamon");
    stop_service("ezmesh");
    stop_service("hyd");
    disable_service("ezmesh");
    disable_service("hyd");
    app_uci_set("repacd.repacd.Ezmesh", "0");
    app_uci_set("repacd.repacd.Enable", "0");
    app_uci_set("repacd.MAPConfig.Enable", "0");
    app_uci_commit("repacd");

    // Config Network
    network_init();

    // Config LAN
    app_uci_set("network.lan", "interface");
    app_uci_set("network.lan.force_link", "'1'");
    app_uci_set("network.lan.type", "'bridge'");
    app_uci_set("network.lan.ipaddr", LAN_IP_DEFAULT_FOR_STANDALONE);
    app_uci_set("network.lan.netmask", "'255.255.255.0'");
    app_uci_set("network.lan.ip6assign", "'60'");
    app_uci_set("network.lan.multicast_querier", "'0'");
    app_uci_set("network.lan.igmp_snooping", "'0'");
    app_uci_set("network.lan.ieee1905managed", "'1'");
    app_uci_set("network.lan.proto", "'static'");
    app_uci_set("network.lan.ifname", "'eth1 eth2 eth3 eth4'");

    // Config WAN
    app_uci_set("network.wan", "interface");
    app_uci_set("network.wan.proto", "'dhcp'");
    app_uci_set("network.wan.ifname", "'eth0'");

    // //Config br-wwan
    // app_uci_set("network.wwan" , "interface");
    // app_uci_set("network.wwan.ifname" , "'eth0'");
    // app_uci_set("network.wwan.proto" , "'dhcp'");
    // app_uci_set("network.wwan.type" , "'bridge'");
    // app_uci_set("network.wwan6" , "interface");
    // app_uci_set("network.wwan6.ifname" , "'eth0'");
    // app_uci_set("network.wwan6.proto" , "'dhcpv6'");
    // app_uci_commit("network");

    // Config DHCP
    app_uci_set("dhcp.lan.ignore", "0");
    app_uci_commit("dhcp");

    // Restart Service
    log_info("Restart DNS-DHCP");
    restart_service("dnsmasq");
    log_info("Restart Network");
    restart_service("network");

    // Sleep 60s for Network Restart done
    sleep(180);

    log_info("Enable Authen Service , Thing Service");
    // Enable Authen Service, Thing Service
    enable_service("authen_service");
    enable_service("thing_service");
    restart_service("authen_service");
    restart_service("thing_service");
    // Create Ezm Service Avahi
    mdns_create_publisher();
}

int run_agent_mode()
{
    // Run agent mode
    log_info("Run an agent mode");
    if (request_mqtt_bridge_server())
    {
        log_info("Delete Mosquitto Bridge Mode Server");
        delete_mqtt_bridge_server();
    }
    if (!request_mqtt_bridge_client())
    {
        log_info("Disable Authen Service , Thing Service");

        log_info("Create Mosquitto Bridge Mode Client");
        create_mqtt_bridge_client();
        log_info("Restart Mosquitto Service");
        restart_mqtt_service();
    }
    // disable thing service and authen service
    stop_service("authen_service");
    stop_service("thing_service");
    disable_service("authen_service");
    disable_service("thing_service");

    // remove thing_service avahi client
    mdns_remove_publisher(AVAHI_THING_SERVICE_FILE);

    // remove ezm_service avahi client
    mdns_remove_publisher(AVAHI_EZM_SERVICE_FILE);

    log_info("Init Network Config");
    network_init();
    log_info("Init Agent Config");
    // Symplink MAP Tools
    execl_non_timeout("/data/script/ezm/symplink.sh");

    // Config LAN
    app_uci_set("network.lan", "interface");
    app_uci_set("network.lan.force_link", "'1'");
    app_uci_set("network.lan.type", "'bridge'");
    app_uci_set("network.lan.ipaddr", LAN_IP_DEFAULT_FOR_MESH);
    app_uci_set("network.lan.netmask", "'255.255.255.0'");
    app_uci_set("network.lan.ip6assign", "'60'");
    app_uci_set("network.lan.multicast_querier", "'0'");
    app_uci_set("network.lan.igmp_snooping", "'0'");
    app_uci_set("network.lan.ieee1905managed", "'1'");
    app_uci_set("network.lan.proto", "'dhcp'");
    app_uci_set("network.lan.ifname", "'eth1 eth2 eth3 eth4'");

    // Delete WAN
    app_uci_delete("network.wan");
    app_uci_commit("network");

    // Disable LAN DHCP
    app_uci_set("dhcp.lan.ignore", "1");
    app_uci_commit("dhcp");

    // Restart Service
    restart_service("dnsmasq");
    restart_service("network");

    // Config Wireless LAN
    app_uci_set("wireless.wifi0.repacd_map_bsta_preference", "1");
    app_uci_set("wireless.wifi1.repacd_map_bsta_preference", "2");
    app_uci_set("wireless.qcawifi", "qcawifi");
    app_uci_set("wireless.qcawifi.samessid_disable", "1");
    app_uci_commit("wireless");

    // Config WSPLCD
    app_uci_set("wsplcd.config.MapMaxBss", "6");
    app_uci_commit("wsplcd");

    // Enable Ezmesh or Hyd
    enable_service("ezmesh");
    enable_service("hyd");

    // Config repacd
    app_uci_set("repacd.repacd.Ezmesh", "1");
    app_uci_set("repacd.repacd.Enable", "1");
    app_uci_set("repacd.repacd.ConfigREMode", "son");
    app_uci_set("repacd.MAPConfig.FirstConfigRequired", "1");
    app_uci_set("repacd.MAPConfig.Enable", "1");
    app_uci_set("repacd.MAPConfig.MapVersionEnabled", "3");
    app_uci_set("repacd.MAPConfig.OnboardingType", "'none'");
    app_uci_set("repacd.MAPConfig.FronthaulAuthType", "1");
    app_uci_commit("repacd");
    restart_service("repacd");

    // // Sleep 5
    // sleep(5);
    // // Reconfig br-wwan
    // log_info("Config br-wwan");
    // app_uci_set("network.wwan" , "interface");
    // app_uci_set("network.wwan.ifname" , "'eth0'");
    // app_uci_set("network.wwan.proto" , "'dhcp'");
    // app_uci_set("network.wwan.type" , "'bridge'");
    // app_uci_set("network.wwan6" , "interface");
    // app_uci_set("network.wwan6.ifname" , "'eth0'");
    // app_uci_set("network.wwan6.proto" , "'dhcpv6'");
    // app_uci_commit("network");
    // log_info("Restart Network");
    // restart_service("network");

    log_info("Checking emt connection");
    // Block to check when agent setup done
    while (ezm_deamon_is_running() == -1)
    {
        sleep(1);
    }
    log_info("Agent Up");
}

int run_controller_mode()
{
    /* code */
    log_info("Controller Mode");
    if (request_mqtt_bridge_client())
    {
        log_info("Delete Mosquitto Bridge Client");
        delete_mqtt_bridge_client();
    }
    if (!request_mqtt_bridge_server())
    {
        log_info("Create Mosquitto Bridge Server");
        create_mqtt_bridge_server();
        log_info("Restart Mosquitto Service");
        restart_mqtt_service();
    }
    log_info("Enable Authen Service , Thing Service");
    // Enable Authen Service, Thing Service
    enable_service("authen_service");
    enable_service("thing_service");
    restart_service("authen_service");
    restart_service("thing_service");

    // disable thing service and authen service
    stop_service("authen_service");
    stop_service("thing_service");
    disable_service("authen_service");
    disable_service("thing_service");

    // remove thing_service avahi client
    mdns_remove_publisher(AVAHI_THING_SERVICE_FILE);

    // remove ezm_service avahi client
    mdns_remove_publisher(AVAHI_EZM_SERVICE_FILE);

    log_info("Init Network Config");
    network_init();
    log_info("Init Controller Config");
    char *mesh_setting_str = read_data(get_config().mesh_setting_file);
    mesh_setting_t *mesh_setting = cvt_json_to_mesh_setting(cJSON_Parse(mesh_setting_str));

    // Symplink MAP Tools
    execl_non_timeout("/data/script/ezm/symplink.sh");

    // Enable LAN DHCP
    log_info("Config DHCP");
    app_uci_set("dhcp.lan.ignore", "0");
    app_uci_commit("dhcp");

    // Set LAN Static IP
    log_info("Config LAN");
    app_uci_set("network.lan.proto", "static");
    app_uci_set("network.lan.ipaddr", LAN_IP_DEFAULT_FOR_MESH);

    // Set WAN DHCP and Interface
    log_info("Config WAN");
    app_uci_set("network.wan", "interface");
    app_uci_set("network.wan.ifname", "eth0");
    app_uci_set("network.wan.proto", "dhcp");
    app_uci_commit("network");

    // Set Wireless LAN
    log_info("Config Wireless");
    app_uci_set("wireless.wifi0.repacd_create_ctrl_fbss", "1");
    app_uci_set("wireless.wifi0.repacd_create_ctrl_bbss", "1");
    app_uci_set("wireless.wifi1.repacd_create_ctrl_fbss", "1");
    app_uci_set("wireless.wifi1.repacd_create_ctrl_bbss", "1");
    app_uci_set("wireless.qcawifi", "qcawifi");
    app_uci_set("wireless.qcawifi.samessid_disable", "1");
    app_uci_commit("wireless");

    // Config repacd
    char ssid[128];
    char key[128];
    log_info("Config repacd");
    app_uci_set("repacd.repacd.Ezmesh", "1");
    app_uci_set("repacd.repacd.Enable", "1");
    app_uci_set("repacd.repacd.ConfigREMode", "son");
    app_uci_set("repacd.MAPConfig.Enable", "1");
    app_uci_set("repacd.MAPConfig.FirstConfigRequired", "1");
    app_uci_set("repacd.MAPConfig.BSSInstantiationTemplate", "'scheme-a.conf'");
    app_uci_set("repacd.MAPConfig.MapVersionEnabled", "3");
    sprintf(ssid, "'%s'", mesh_setting->wireless->wifi_iface->ssid);
    sprintf(key, "'%s'", mesh_setting->wireless->wifi_iface->key);
    app_uci_set("repacd.MAPConfig.FronthaulSSID", ssid);
    app_uci_set("repacd.MAPConfig.FronthaulKey", key);
    app_uci_set("repacd.MAPConfig.BackhaulSSID", "'EzMeshBackhaul'");
    app_uci_set("repacd.MAPConfig.BackhaulKey", "'QCOM#1WiFi'");
    app_uci_set("repacd.MAPConfig.FronthaulAuthType", "1");
    app_uci_commit("repacd");

    // Config ezlbd
    log_info("Config ezlbd");
    app_uci_set("ezlbd.Estimator_Adv.ActDetectMinInterval", "'10'");
    app_uci_set("ezlbd.Estimator_Adv.ActDetectMinPktPerSec", "'5'");
    app_uci_set("ezlbd.APSteer.LowRSSIAPSteerThreshold_SIG", "'35'");
    app_uci_set("ezlbd.APSteer.APSteerMaxRetryCount", "'20'");
    app_uci_set("ezlbd.StaDB.MarkAdvClientAsDualBand", "1");
    app_uci_set("ezlbd.SteerExec_Adv.StartInBTMActiveState", "1");
    app_uci_set("ezlbd.config.InactDetectionFromTx", "'1'");
    app_uci_commit("ezlbd");

    // Config Ezmesh
    log_info("Config ezmesh");
    app_uci_set("ezmesh.MultiAP.EnableChannelSelection", "1");
    app_uci_set("ezmesh.MultiAP.BkScanIntervalMin", "0");
    app_uci_set("ezmesh.MultiAP.ChannelSelectionDelaySec", "0");
    app_uci_set("ezmesh.MultiAP.ChannelSelectionOnGlobalPref", "0");
    app_uci_set("ezmesh.MultiAP.EnableTopologyOpt", "1");
    app_uci_commit("ezmesh");

    // Enable Ezmesh or Hyd
    enable_service("ezmesh");
    enable_service("hyd");

    // Start Ezmesh Service
    log_info("Restart Repacd");
    restart_service("repacd");
    // execl_timeout(2*60*1000, "/data/script/ezm/restart_controller_config.sh");

    // // Sleep 5
    // sleep(5);
    // // Reconfig br-wwan
    // log_info("Config br-wwan");
    // app_uci_set("network.wwan" , "interface");
    // app_uci_set("network.wwan.ifname" , "'eth0'");
    // app_uci_set("network.wwan.proto" , "'dhcp'");
    // app_uci_set("network.wwan.type" , "'bridge'");
    // app_uci_set("network.wwan6" , "interface");
    // app_uci_set("network.wwan6.ifname" , "'eth0'");
    // app_uci_set("network.wwan6.proto" , "'dhcpv6'");
    // app_uci_commit("network");
    // log_info("Restart Network");
    // restart_service("network");

    // Sleep 30
    while (ezm_deamon_is_running() == -1)
    {
        sleep(1);
    }
    log_info("Controller Up");
    // Create Ezm Service Avahi
    sleep(5);
    // log_info("Create Avahi Publisher");
    // mdns_create_publisher();

    // when controller mode up -> save controller device to db
    log_info("Save Controller Device to Device List");
    device_info_t *controller_device = get_current_device_info();
    controller_device->status = DEVICE_ON;
    device_list_t device_list;
    device_list.device_list = controller_device;
    device_list.nb_device = 1;
    set_device_list_to_file(&device_list);
}

void mesh_setting_free(mesh_setting_t *mesh_setting)
{
    if (mesh_setting == NULL)
    {
        return;
    }
    log_debug("Free mesh_id");
    app_free(mesh_setting->mesh_id);
    log_debug("Free mesh_name");
    app_free(mesh_setting->mesh_name);
    log_debug("Free rssi_threshhold");
    app_free(mesh_setting->rssi_threshhold);
    log_debug("Free antenna_installed");
    app_free(mesh_setting->antenna_installed);
    log_debug("Free backhaul_channel");
    app_free(mesh_setting->backhaul_channel);
    log_debug("Free work_mode");
    app_free(mesh_setting->work_mode);
    log_debug("Free mesh_link_connection");
    app_free(mesh_setting->mesh_link_connection);
    log_debug("Free wireless");
    wireless_free(mesh_setting->wireless);
    log_debug("Free mesh_setting");
    app_free(mesh_setting);
}