#include "device.h"
#include "utils/log/log.h"
#include "config/service_info/service_info.h"
#include "core/mesh/setting/mesh_setting.h"
#include "config.h"

cJSON *cvt_device_info_to_json(device_info_t *device)
{
    cJSON *device_json = cJSON_CreateObject();
    log_debug("hub_id");
    cJSON_AddStringToObject(device_json, "hub_id", device->hub_id);
    log_debug("ezm_id");
    cJSON_AddStringToObject(device_json, "ezm_id", device->ezm_id);
    log_debug("ezm_version");
    cJSON_AddStringToObject(device_json, "ezm_version", device->ezm_version);
    log_debug("ezm_name");
    cJSON_AddStringToObject(device_json, "ezm_name", device->ezm_name);
    log_debug("role");
    cJSON_AddStringToObject(device_json, "role", device->role);
    log_debug("mac");
    cJSON_AddStringToObject(device_json, "mac", device->mac);
    log_debug("ip");
    cJSON_AddStringToObject(device_json, "ip", device->ip);
    log_debug("parent_mac");
    cJSON_AddStringToObject(device_json, "parent_mac", device->parent_mac);
    cJSON *channel_selection = cJSON_CreateObject();
    log_debug("5g");
    cJSON_AddStringToObject(channel_selection, "5g", device->channel_selection_5g);
    log_debug("2g");
    cJSON_AddStringToObject(channel_selection, "2g", device->channel_selection_2g);
    log_debug("channel_selection");
    cJSON_AddItemToObject(device_json, "channel_selection", channel_selection);
    log_debug("status");
    cJSON_AddNumberToObject(device_json, "status", device->status);
    return device_json;
}

device_info_t *cvt_json_to_device_info(cJSON *json)
{
    device_info_t *device = (device_info_t *)calloc(1, sizeof(device_info_t));
    cJSON *hub_id_json = cJSON_GetObjectItemCaseSensitive(json, "hub_id");
    if (hub_id_json != NULL && cJSON_IsString(hub_id_json))
    {
        device->hub_id = app_strdup(cJSON_GetStringValue(hub_id_json));
    }
    cJSON *ezm_id_json = cJSON_GetObjectItemCaseSensitive(json, "ezm_id");
    if (ezm_id_json != NULL && cJSON_IsString(ezm_id_json))
    {
        device->ezm_id = app_strdup(cJSON_GetStringValue(ezm_id_json));
    }
    cJSON *ezm_version_json = cJSON_GetObjectItemCaseSensitive(json, "ezm_version");
    if (ezm_version_json != NULL && cJSON_IsString(ezm_version_json))
    {
        device->ezm_version = app_strdup(cJSON_GetStringValue(ezm_version_json));
    }
    cJSON *ezm_name_json = cJSON_GetObjectItemCaseSensitive(json, "ezm_name");
    if (ezm_name_json != NULL && cJSON_IsString(ezm_name_json))
    {
        device->ezm_name = app_strdup(cJSON_GetStringValue(ezm_name_json));
    }
    cJSON *role_json = cJSON_GetObjectItemCaseSensitive(json, "role");
    if (role_json != NULL && cJSON_IsString(role_json))
    {
        device->role = app_strdup(cJSON_GetStringValue(role_json));
    }
    cJSON *mac_json = cJSON_GetObjectItemCaseSensitive(json, "mac");
    if (mac_json != NULL && cJSON_IsString(mac_json))
    {
        device->mac = app_strdup(cJSON_GetStringValue(mac_json));
    }
    cJSON *ip_json = cJSON_GetObjectItemCaseSensitive(json, "ip");
    if (ip_json != NULL && cJSON_IsString(ip_json))
    {
        device->ip = app_strdup(cJSON_GetStringValue(ip_json));
    }
    cJSON *parent_mac_json = cJSON_GetObjectItemCaseSensitive(json, "parent_mac");
    if (parent_mac_json != NULL && cJSON_IsString(parent_mac_json))
    {
        device->parent_mac = app_strdup(cJSON_GetStringValue(parent_mac_json));
    }
    cJSON *channel_selection_json = cJSON_GetObjectItemCaseSensitive(json, "channel_selection");
    if (channel_selection_json != NULL && cJSON_IsObject(channel_selection_json))
    {
        cJSON *channel_selection_5g_json = cJSON_GetObjectItemCaseSensitive(channel_selection_json, "5g");
        if (channel_selection_5g_json != NULL && cJSON_IsString(channel_selection_5g_json))
        {
            device->channel_selection_5g = app_strdup(cJSON_GetStringValue(channel_selection_5g_json));
        }
        cJSON *channel_selection_2g_json = cJSON_GetObjectItemCaseSensitive(channel_selection_json, "2g");
        if (channel_selection_2g_json != NULL && cJSON_IsString(channel_selection_2g_json))
        {
            device->channel_selection_2g = app_strdup(cJSON_GetStringValue(channel_selection_2g_json));
        }
    }
    cJSON *status_json = cJSON_GetObjectItemCaseSensitive(json, "status");
    if (status_json != NULL && cJSON_IsNumber(status_json))
    {
        device->status = status_json->valueint;
    }
    return device;
}

cJSON *cvt_device_list_to_json(device_list_t *device_list)
{
    cJSON *device_list_json = cJSON_CreateArray();
    for (size_t i = 0; i < device_list->nb_device; i++)
    {
        cJSON *device_json = cvt_device_info_to_json(&device_list->device_list[i]);
        cJSON_AddItemToArray(device_list_json, device_json);
    }
    return device_list_json;
}

device_list_t *cvt_json_to_device_list(cJSON *json)
{
    device_list_t *device_list = (device_list_t *)calloc(1, sizeof(device_list_t));
    int nb_devices = cJSON_GetArraySize(json);
    device_info_t *device_info_list = (device_info_t *)calloc(1, sizeof(device_info_t) * nb_devices);
    for (size_t i = 0; i < nb_devices; i++)
    {
        cJSON *device_temp = cJSON_GetArrayItem(json, i);
        device_info_list[i] = *cvt_json_to_device_info(device_temp);
    }
    device_list->device_list = device_info_list;
    device_list->nb_device = nb_devices;
    return device_list;
}

device_info_t *get_current_device_info()
{
    device_info_t *device = (device_info_t *)calloc(1, sizeof(device_info_t));
    log_debug("get_hub_id ");
    device->hub_id = app_strdup(get_hub_id());
    log_debug("get_ezm_id ");
    device->ezm_id = app_strdup(get_ezm_id());
    log_debug("EZM_VERSION_DEFAULT ");
    device->ezm_version = EZM_VERSION_DEFAULT;
    log_debug("get_interface_mac ");
    device->ezm_name = app_strdup(get_interface_mac(WAN_INTERFACE));
    log_debug("get_work_mode_str ");
    device->role = app_strdup(get_work_mode_str());
    log_debug("get_interface_mac ");
    device->mac = app_strdup(get_interface_mac(WAN_INTERFACE));
    log_debug("get_interface_ip ");
    device->ip = app_strdup(get_interface_ip(WAN_INTERFACE));
    log_debug("get_hub_id ");
    return device;
}

void device_info_free(device_info_t *device_info)
{
    if (device_info == NULL)
    {
        return;
    }
    log_debug("Free hub_id");
    app_free(device_info->hub_id);
    log_debug("Free ezm_id");
    app_free(device_info->ezm_id);
    log_debug("Free ezm_version");
    app_free(device_info->ezm_version);
    log_debug("Free ezm_name");
    app_free(device_info->ezm_name);
    log_debug("Free role");
    app_free(device_info->role);
    log_debug("Free mac");
    app_free(device_info->mac);
    log_debug("Free ip");
    app_free(device_info->ip);
    log_debug("Free parent_mac");
    app_free(device_info->parent_mac);
    log_debug("Free channel_selection_5g");
    app_free(device_info->channel_selection_5g);
    log_debug("Free channel_selection_2g");
    app_free(device_info->channel_selection_2g);
    log_debug("Free device_info");
}

void device_list_free(device_list_t *device_list)
{
    if (device_list == NULL)
    {
        return;
    }
    log_debug("Begin free devive list");
    for (size_t i = 0; i < device_list->nb_device; i++)
    {
        device_info_free(&device_list->device_list[i]);
    }
    app_free(device_list->device_list);
    app_free(device_list);
    log_debug("End free devive list");
}