#include "topology.h"
#include "string.h"
#include "utils/log/log.h"
#include "utils/file/file.h"
#include "utils/string/app_string.h"
#include "stdbool.h"

static pattern_t device_pattern_entry[] = {
    {MAC, "QCA IEEE 1905.1 device:", " ", ","},
    {IP, "IPv4 address:", " ", " "},
    {PARENT_MAC, "Upstream Device:", " ", "\n"},
    {CHANNEL_SELECTION_5G, "Radios:", "WLAN", "\n"},
    {CHANNEL_SELECTION_2G, "WLAN", " ", "\n"}};

device_info_t *topology_handler(char *topology_str, int *number_device)
{
    device_info_t *device_list = NULL;
    char *start_pointer, *end_pointer;
    char *str_pointer = NULL;
    char *check_pointer = topology_str;
    size_t nb_field = sizeof(device_pattern_entry) / sizeof(pattern_t);
    int nb_device = 0;
    do
    {
        for (size_t i = 0; i < nb_field; i++)
        {
            check_pointer = app_strstr(check_pointer, device_pattern_entry[i].pattern);
            if (!check_pointer)
            {
                *number_device = nb_device;
                return device_list;
            }
            str_pointer = check_pointer + strlen(device_pattern_entry[i].pattern);
            switch (device_pattern_entry[i].field)
            {
            case MAC:
                log_debug("Topology MAC");
                if (check_pointer)
                {
                    if (nb_device == 0)
                    {
                        device_list = (device_info_t *)calloc(1, sizeof(device_info_t));
                    }
                    else
                    {
                        device_list = (device_info_t *)realloc(device_list, (nb_device + 1) * sizeof(device_info_t));
                        memset(&device_list[nb_device], 0, sizeof(device_info_t));
                    }
                    nb_device++;
                    if (nb_device == 1)
                    {
                        device_list[nb_device - 1].role = app_strdup("controller");
                    }
                    else
                    {
                        device_list[nb_device - 1].role = app_strdup("agent");
                    }
                    start_pointer = app_strstr(str_pointer, device_pattern_entry[i].start) + strlen(device_pattern_entry[i].start);
                    // log_info("3.23");
                    end_pointer = app_strstr(start_pointer, device_pattern_entry[i].end);
                    if (!start_pointer || !end_pointer)
                    {
                        *number_device = nb_device;
                        return device_list;
                    }
                    // log_info("3.3");
                    size_t mac_size = end_pointer - start_pointer;
                    char *mac = (char *)malloc(mac_size + 1);
                    memcpy(mac, start_pointer, mac_size);
                    // log_info("3.4");
                    mac[mac_size] = '\0';
                    device_list[nb_device - 1].mac = upcase_to_lowcase(mac);
                    free(mac);
                    // log_info("role: %s",device_list[nb_device-1].role);
                    // log_info("mac: %s",mac);
                }
                else
                {
                    // log_info("3.5");
                    *number_device = nb_device;
                    return device_list;
                }
                break;
            case IP:
                log_debug("Topology IP");
                if (check_pointer)
                {
                    start_pointer = app_strstr(str_pointer, device_pattern_entry[i].start);
                    if (!start_pointer)
                    {
                        // log_info("Pattern %s not found", device_pattern_entry[i].start);
                        *number_device = nb_device;
                        return device_list;
                    }
                    start_pointer = start_pointer + strlen(device_pattern_entry[i].start);
                    end_pointer = app_strstr(start_pointer, device_pattern_entry[i].end);
                    if (!end_pointer)
                    {
                        // log_info("Pattern %s not found", device_pattern_entry[i].end);
                        *number_device = nb_device;
                        return device_list;
                    }
                    size_t ip_size = end_pointer - start_pointer;
                    char *ip = (char *)malloc(ip_size + 1);
                    memcpy(ip, start_pointer, ip_size);
                    ip[ip_size] = '\0';
                    device_list[nb_device - 1].ip = ip;
                    // log_info("ip: %s",ip);
                }
                else
                {
                    *number_device = nb_device;
                    return device_list;
                }
                break;
            case PARENT_MAC:
                log_debug("Topology PARENT_MAC");
                if (check_pointer)
                {
                    start_pointer = app_strstr(str_pointer, device_pattern_entry[i].start);
                    if (!start_pointer)
                    {
                        // log_info("Pattern %s not found", device_pattern_entry[i].start);
                        *number_device = nb_device;
                        return device_list;
                    }
                    start_pointer = start_pointer + strlen(device_pattern_entry[i].start);
                    end_pointer = app_strstr(start_pointer, device_pattern_entry[i].end);
                    if (!end_pointer)
                    {
                        // log_info("Pattern %s not found", device_pattern_entry[i].end);
                        *number_device = nb_device;
                        return device_list;
                    }
                    size_t parent_mac_size = end_pointer - start_pointer;
                    char *parent_mac = (char *)malloc(parent_mac_size + 1);
                    memcpy(parent_mac, start_pointer, parent_mac_size);
                    parent_mac[parent_mac_size] = '\0';
                    device_list[nb_device - 1].parent_mac = upcase_to_lowcase(parent_mac);
                    free(parent_mac);
                    // log_info("parent_mac: %s",parent_mac);
                }
                else
                {
                    if (nb_device != 1)
                    {
                        *number_device = nb_device;
                        return device_list;
                    }
                }
                break;
            case CHANNEL_SELECTION_5G:
                log_debug("Topology CHANNEL_SELECTION_5G");
                if (check_pointer)
                {
                    start_pointer = app_strstr(str_pointer, device_pattern_entry[i].start);
                    if (!start_pointer)
                    {
                        // log_info("Pattern %s not found", device_pattern_entry[i].start);
                        *number_device = nb_device;
                        return device_list;
                    }
                    start_pointer = start_pointer + strlen(device_pattern_entry[i].start);
                    end_pointer = app_strstr(start_pointer, device_pattern_entry[i].end);
                    if (!end_pointer)
                    {
                        // log_info("Pattern %s not found", device_pattern_entry[i].end);
                        *number_device = nb_device;
                        return device_list;
                    }
                    // log_info("1");
                    size_t channel_selection_size = end_pointer - start_pointer;
                    static bool start_record = false;
                    uint16_t start_index = 0;
                    uint16_t stop_index = 0;
                    // log_info("1");
                    for (size_t data_index = 0; data_index < channel_selection_size; data_index++)
                    {
                        // log_info("1");
                        if (start_record)
                        {
                            if (!(start_pointer[data_index] >= '0' && start_pointer[data_index] <= '9'))
                            {
                                start_record = false;
                                stop_index = data_index;
                                break;
                            }
                        }
                        else if (start_pointer[data_index] >= '0' && start_pointer[data_index] <= '9')
                        {
                            start_record = true;
                            start_index = data_index;
                        }
                        /* code */
                    }
                    // log_info("1");
                    char *channel_selection_5g = (char *)malloc(stop_index - start_index + 1);
                    memcpy(channel_selection_5g, start_pointer + start_index, stop_index - start_index + 1);
                    // log_info("1");
                    channel_selection_5g[stop_index - start_index] = '\0';
                    device_list[nb_device - 1].channel_selection_5g = channel_selection_5g;
                    // log_info("channel_selection_5g: %s",channel_selection_5g);
                }
                else
                {
                    *number_device = nb_device;
                    return device_list;
                }
                break;
            case CHANNEL_SELECTION_2G:
                log_debug("Topology CHANNEL_SELECTION_2G");
                if (check_pointer)
                {
                    start_pointer = app_strstr(str_pointer, device_pattern_entry[i].start);
                    if (!start_pointer)
                    {
                        // log_info("Pattern %s not found", device_pattern_entry[i].start);
                        *number_device = nb_device;
                        return device_list;
                    }
                    start_pointer = start_pointer + strlen(device_pattern_entry[i].start);
                    end_pointer = app_strstr(start_pointer, device_pattern_entry[i].end);
                    if (!end_pointer)
                    {
                        // log_info("Pattern %s not found", device_pattern_entry[i].end);
                        *number_device = nb_device;
                        return device_list;
                    }
                    size_t channel_selection_size = end_pointer - start_pointer;
                    static bool start_record = false;
                    uint16_t start_index = 0;
                    uint16_t stop_index = 0;
                    // log_info("3");
                    for (size_t data_index = 0; data_index < channel_selection_size; data_index++)
                    {
                        if (start_record)
                        {
                            // log_info("4");
                            if (!(start_pointer[data_index] >= '0' && start_pointer[data_index] <= '9'))
                            {
                                start_record = false;
                                stop_index = data_index;
                                break;
                            }
                        }

                        else if (start_pointer[data_index] >= '0' && start_pointer[data_index] <= '9')
                        {
                            start_record = true;
                            start_index = data_index;
                        }
                        /* code */
                    }
                    // log_info("5");
                    char *channel_selection_2g = (char *)malloc(stop_index - start_index + 1);
                    memcpy(channel_selection_2g, start_pointer + start_index, stop_index - start_index + 1);
                    channel_selection_2g[stop_index - start_index] = '\0';
                    device_list[nb_device - 1].channel_selection_2g = channel_selection_2g;
                    // log_info("channel_selection_2g: %s",channel_selection_2g);
                    // log_info("6");
                }
                else
                {
                    *number_device = nb_device;
                    return device_list;
                }
                break;
            default: // case 5G va 2G
                break;
            }
            check_pointer = end_pointer;
        }
    } while (str_pointer != NULL);
    *number_device = nb_device;
    return device_list;
}

device_list_t *request_device_list_from_file()
{
    char *device_list_str = read_data(get_config().mesh_devices_file);
    cJSON *device_list_json = cJSON_Parse(device_list_str);
    device_list_t *device_list = cvt_json_to_device_list(device_list_json);
    // Free dynamic memory
    cJSON_Delete(device_list_json);
    free(device_list_str);
    return device_list;
}

device_list_t *request_device_list_from_file_by_status(int status)
{
    char *device_list_str = read_data(get_config().mesh_devices_file);
    cJSON *device_list_json = cJSON_Parse(device_list_str);
    device_list_t *device_list = cvt_json_to_device_list(device_list_json);
    device_list_t *device_list_status = (device_list_t *)calloc(1, sizeof(device_list_t));
    for (size_t i = 0; i < device_list->nb_device; i++)
    {
        if (device_list->device_list[i].status == status)
        {
            device_list_status->nb_device++;
            device_list_status->device_list = (device_info_t *)realloc(device_list_status->device_list, device_list_status->nb_device * sizeof(device_info_t));
            device_info_copy(&device_list_status->device_list[device_list_status->nb_device - 1], &device_list->device_list[i]);
        }
    }
    cJSON_Delete(device_list_json);
    free(device_list_str);
    device_list_free(device_list);
    return device_list_status;
}

void set_device_list_to_file(device_list_t *device_list_set)
{
    device_list_t *current_device_list = request_device_list_from_file();
    int nb_current_device_list = current_device_list->nb_device;
    int nb_device_list_set = device_list_set->nb_device;
    bool flag_found;
    for (size_t i = 0; i < nb_device_list_set; i++)
    {
        /* code */
        flag_found = false;
        for (size_t j = 0; j < nb_current_device_list; j++)
        {
            /* code */
            // Check Match ezm_id
            if (app_strstr(device_list_set->device_list[i].mac, current_device_list->device_list[j].mac) ||
                app_strstr(device_list_set->device_list[i].ezm_id, current_device_list->device_list[j].ezm_id))
            {
                // Just Update Ezm_name
                current_device_list->device_list[j] = device_list_set->device_list[i];
                flag_found = true;
                break;
            }
        }
        if (!flag_found)
        {
            nb_current_device_list++;
            current_device_list->device_list = realloc(current_device_list->device_list, nb_current_device_list * sizeof(device_info_t));
            current_device_list->device_list[nb_current_device_list - 1] = device_list_set->device_list[i];
        }
    }
    current_device_list->nb_device = nb_current_device_list;
    cJSON *current_device_list_json = cvt_device_list_to_json(current_device_list);
    char *current_device_list_str = cJSON_Print(current_device_list_json);
    write_data(current_device_list_str, strlen(current_device_list_str), get_config().mesh_devices_file, TRUNCATE);
}

bool delete_device_list_from_file(device_list_t *device_list_delete)
{
    device_list_t *current_device_list = request_device_list_from_file();
    int nb_current_device_list = current_device_list->nb_device;
    int nb_device_list_delete = device_list_delete->nb_device;
    bool found_any_device = false;
    for (size_t i = 0; i < nb_device_list_delete; i++)
    {
        for (size_t j = 0; j < nb_current_device_list; j++)
        {
            // Check Match mac
            if (app_strstr(device_list_delete->device_list[i].mac, current_device_list->device_list[j].mac))
            {
                // Just delete device
                current_device_list->device_list[j].status = DEVICE_DELETED;
                found_any_device = true;
                break;
            }
        }
    }
    current_device_list->nb_device = nb_current_device_list;
    cJSON *current_device_list_json = cvt_device_list_to_json(current_device_list);
    char *current_device_list_str = cJSON_Print(current_device_list_json);
    write_data(current_device_list_str, strlen(current_device_list_str), get_config().mesh_devices_file, TRUNCATE);
    return found_any_device;
}

void update_device_list_to_file_from_emt_device(device_list_t *device_list_set)
{
    device_list_t *current_device_list = request_device_list_from_file();
    int nb_current_device_list = current_device_list->nb_device;
    int nb_device_list_set = device_list_set->nb_device;
    bool flag_found;
    log_debug("1");

    // Update Emt Device to Current Device
    for (size_t i = 0; i < nb_device_list_set; i++)
    {
        /* code */
        if (!app_strstr(device_list_set->device_list[i].role, "controller"))
        {
            log_debug("update agent");
            flag_found = false;
            for (size_t j = 0; j < nb_current_device_list; j++)
            {
                // Check Match mac
                log_debug("%s", current_device_list->device_list[j].hub_id);
                // Just Update Device Role Differen Controller because mismatch ezm_id and mac
                if (app_strstr(device_list_set->device_list[i].mac, current_device_list->device_list[j].mac))
                {
                    // Just Update parent mac , channel selection,
                    current_device_list->device_list[j].role = app_strdup(device_list_set->device_list[i].role);
                    current_device_list->device_list[j].mac = app_strdup(device_list_set->device_list[i].mac);
                    current_device_list->device_list[j].ip = app_strdup(device_list_set->device_list[i].ip);
                    current_device_list->device_list[j].parent_mac = app_strdup(device_list_set->device_list[i].parent_mac);
                    current_device_list->device_list[j].channel_selection_5g = app_strdup(device_list_set->device_list[i].channel_selection_5g);
                    current_device_list->device_list[j].channel_selection_2g = app_strdup(device_list_set->device_list[i].channel_selection_2g);
                    if (current_device_list->device_list[j].status != DEVICE_DELETED)
                    {
                        current_device_list->device_list[j].status = DEVICE_ON;
                    }
                    flag_found = true;
                }
            }
            if (!flag_found)
            {
                log_debug("5");
                nb_current_device_list++;
                current_device_list->device_list = realloc(current_device_list->device_list, nb_current_device_list * sizeof(device_info_t));
                log_debug("Before copy");
                device_info_copy(&current_device_list->device_list[nb_current_device_list - 1], &device_list_set->device_list[i]);
                log_debug("After copy");
                current_device_list->device_list[nb_current_device_list - 1].status = DEVICE_ON;
            }
        }
        else
        {
            log_debug("update controller");
            // If role = controller -> update ip, mac, channel_selection, parent_mac of previous controller
            for (size_t j = 0; j < nb_current_device_list; j++)
            {
                if (app_strstr(current_device_list->device_list[j].role, "controller"))
                {
                    // Just Update parent mac , channel selection,
                    current_device_list->device_list[j].mac = app_strdup(device_list_set->device_list[i].mac);
                    current_device_list->device_list[j].ip = app_strdup(device_list_set->device_list[i].ip);
                    current_device_list->device_list[j].parent_mac = app_strdup(device_list_set->device_list[i].parent_mac);
                    current_device_list->device_list[j].channel_selection_5g = app_strdup(device_list_set->device_list[i].channel_selection_5g);
                    current_device_list->device_list[j].channel_selection_2g = app_strdup(device_list_set->device_list[i].channel_selection_2g);
                    current_device_list->device_list[j].status = DEVICE_ON;
                }
            }
        }
    }

    // Turn off all device in current device withou in emt device
    for (size_t i = 0; i < nb_current_device_list; i++)
    {
        /* code */
        // Only Update agent, Ignore Controller because It always ON
        if (!app_strstr(current_device_list->device_list[i].role, "controller"))
        {
            flag_found = false;
            for (size_t j = 0; j < nb_device_list_set; j++)
            {
                /* code */
                // Check current device have in emt device
                if (app_strstr(current_device_list->device_list[i].mac, device_list_set->device_list[j].mac))
                {
                    flag_found = true;
                    break;
                }
            }
            if (!flag_found)
            {
                current_device_list->device_list[i].status = DEVICE_OFF;
            }
        }
    }

    current_device_list->nb_device = nb_current_device_list;
    log_debug("Before cvt_device_list_to_json");
    cJSON *current_device_list_json = cvt_device_list_to_json(current_device_list);
    log_debug("After cvt_device_list_to_json");
    char *current_device_list_str = cJSON_Print(current_device_list_json);
    log_debug("Before write_data");
    write_data(current_device_list_str, strlen(current_device_list_str), get_config().mesh_devices_file, TRUNCATE);
    log_debug("After write_data");

    // Free all dynamic memory
    cJSON_Delete(current_device_list_json);
    free(current_device_list_str);
    log_debug("Before device_list_free");
    device_list_free(current_device_list);
    log_debug("After device_list_free");
}

bool update_device_list_to_file_from_web(device_list_t *device_list_set)
{
    device_list_t *current_device_list = request_device_list_from_file();
    int nb_current_device_list = current_device_list->nb_device;
    int nb_device_list_set = device_list_set->nb_device;
    bool found_any_device = false;
    bool flag_found;
    for (size_t i = 0; i < nb_device_list_set; i++)
    {
        /* code */
        flag_found = false;
        for (size_t j = 0; j < nb_current_device_list; j++)
        {
            /* code */
            // Check Match mac
            if (app_strstr(device_list_set->device_list[i].mac, current_device_list->device_list[j].mac))
            {
                // Just Update ezm_name
                current_device_list->device_list[j].ezm_name = app_strdup(device_list_set->device_list[i].ezm_name);
                flag_found = true;
                found_any_device = true;
                break;
            }
        }
    }
    current_device_list->nb_device = nb_current_device_list;
    cJSON *current_device_list_json = cvt_device_list_to_json(current_device_list);
    char *current_device_list_str = cJSON_Print(current_device_list_json);
    write_data(current_device_list_str, strlen(current_device_list_str), get_config().mesh_devices_file, TRUNCATE);
    cJSON_Delete(current_device_list_json);
    free(current_device_list_str);
    device_list_free(current_device_list);
    return found_any_device;
}

device_info_t *get_device_from_file_by_mac(char *mac)
{
    device_list_t *current_device_list = request_device_list_from_file();
    int nb_current_device_list = current_device_list->nb_device;
    for (size_t j = 0; j < nb_current_device_list; j++)
    {
        /* code */
        // Check Match mac
        if (app_strstr(mac, current_device_list->device_list[j].mac))
        {
            // Just Update Ezm_name
            return &current_device_list->device_list[j];
        }
    }
    return NULL;
}

void device_info_copy(device_info_t *device_dest, device_info_t *device_src)
{
    device_dest->hub_id = app_strdup(device_src->hub_id);
    device_dest->ezm_id = app_strdup(device_src->ezm_id);
    device_dest->ezm_version = app_strdup(device_src->ezm_version);
    device_dest->ezm_name = app_strdup(device_src->ezm_name);
    device_dest->role = app_strdup(device_src->role);
    device_dest->mac = app_strdup(device_src->mac);
    device_dest->ip = app_strdup(device_src->ip);
    device_dest->parent_mac = app_strdup(device_src->parent_mac);
    device_dest->channel_selection_5g = app_strdup(device_src->channel_selection_5g);
    device_dest->channel_selection_2g = app_strdup(device_src->channel_selection_2g);
    device_dest->status = device_src->status;
}
