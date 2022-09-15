#include "mesh_handler.h"
#include "config.h"
#include "utils/queue/queue.h"
#include "utils/timer/timer.h"
#include "utils/file/file.h"
#include "utils/cmd/cmd.h"
#include "pthread.h"
#include "mosquitto.h"
#include "core/mesh/core/emt.h"
#include "core/mesh/topology/wps.h"
#include "api/error_code/error_code.h"
#include "api/message/message_struct.h"
#include "utils/log/log.h"
#include "security-provider/security_provider.h"
#include "config/service_info/service_info.h"
#include "utils/app_mdns/app_mdns.h"

extern size_t polling_wps_timer_id, polling_check_agent_joined_mesh;

void create_mesh(cJSON *msg_json)
{
    // Load Available Mesh Setting
    msg_request_t *msg_request = cvt_json_to_msg_request(msg_json, MSG_MESH_SETTING);
    msg_respond_t *msg_respond = cvt_msg_request_to_msg_respond(msg_request);
    msg_respond->name = cvt_req_name_to_res_name(msg_respond->name);
    error_t *error;
    int work_mode = get_work_mode();
    if (work_mode != CONTROLLER_MODE)
    {
        log_info("Setup to Controller");
        char cmd[200];
        // Get SSID , KEY
        char *ssid = msg_request->data->mesh_setting->wireless->wifi_iface->ssid;
        char *key = msg_request->data->mesh_setting->wireless->wifi_iface->key;

        // Response ERR_SUCCESS message
        msg_respond->respond_data->data->mesh_setting->mesh_id = app_strdup(uuid_gen("mesh"));
        error = error_table_lookup(ERR_SUCCESS);
        msg_respond->respond_data->error = error;
        mqtt_publish_message(msg_respond, MSG_MESH_SETTING, 0);

        msg_respond->respond_data->data->mesh_setting->work_mode = app_strdup("controller");

        // Save controller config
        cJSON *mesh_setting_json = cvt_mesh_setting_to_json(msg_respond->respond_data->data->mesh_setting);
        char *mesh_setting_str = cJSON_Print(mesh_setting_json);
        write_data(mesh_setting_str, strlen(mesh_setting_str), get_config().mesh_setting_file, TRUNCATE);
        cJSON_Delete(mesh_setting_json);
        free(mesh_setting_str);

        // Sleep 300 ms for response message
        usleep(300 * 1000);

        // Allow all iptable traffic for setup mesh
        iptables_allow_all_traffic();

        // setup controller with ssid and key
        run_controller_mode();
    }
    else
    {
        error = error_table_lookup(ERR_MESH_SETUPED_BEFORE);
        msg_respond->respond_data->error = error;
        mqtt_publish_message(msg_respond, MSG_EMPTY, 0);
    }
    cJSON_Delete(msg_json);
    msg_respond_free(msg_respond, MSG_MESH_SETTING);
    free(msg_request);
}

void request_mesh(cJSON *msg_json)
{
    msg_request_t *msg_request = cvt_json_to_msg_request(msg_json, MSG_EMPTY);
    msg_respond_t *msg_respond = cvt_msg_request_to_msg_respond(msg_request);
    msg_respond->name = cvt_req_name_to_res_name(msg_respond->name);
    error_t *error;

    // Get current work mode
    int work_mode = get_work_mode();
    if (work_mode != CONTROLLER_MODE)
    {
        // If not controller -> Mesh Not Found or Dont setup before
        error = error_table_lookup(ERR_SUCCESS);
        msg_respond->respond_data->error = error;
        mqtt_publish_message(msg_respond, MSG_EMPTY, 0);
    }
    else
    {
        // Read Mesh Setting
        char *mesh_setting_str = read_data(get_config().mesh_setting_file);
        cJSON *mesh_setting_json = cJSON_Parse(mesh_setting_str);
        mesh_setting_t *mesh_setting = cvt_json_to_mesh_setting(mesh_setting_json);

        // Reponse to Web
        error = error_table_lookup(ERR_SUCCESS);
        msg_respond->respond_data->error = error;
        msg_respond->respond_data->data->mesh_setting = mesh_setting;
        mqtt_publish_message(msg_respond, MSG_MESH_SETTING, 0);

        // Free memory
        free(mesh_setting_str);
        cJSON_Delete(mesh_setting_json);
    }

    cJSON_Delete(msg_json);
    msg_respond_free(msg_respond, MSG_MESH_SETTING);
    log_debug("Free msg_respond_free done");
    free(msg_request);
    log_debug("Free msg_request done");
    return;
}

void update_mesh(cJSON *msg_json)
{
    msg_request_t *msg_request = cvt_json_to_msg_request(msg_json, MSG_MESH_SETTING);
    msg_respond_t *msg_respond = cvt_msg_request_to_msg_respond(msg_request);
    msg_respond->name = cvt_req_name_to_res_name(msg_respond->name);
    error_t *error;
    if (ezm_deamon_is_running() == -1)
    {
        error = error_table_lookup(ERR_EZMESH_DEAMON_CONNECT_FAIL);
        msg_respond->respond_data->error = error;
        msg_respond->name = cvt_req_name_to_res_name(msg_respond->name);
        mqtt_publish_message(msg_respond, MSG_EMPTY, 0);
    }
    else
    {
        // Flag for update mesh-setting
        bool change_wifi_iface_flag = false;

        log_debug("Get work mode");
        int work_mode = get_work_mode();
        if (work_mode != CONTROLLER_MODE)
        {
            error = error_table_lookup(ERR_EZMESH_DEAMON_CONNECT_FAIL);
            msg_respond->respond_data->error = error;
            mqtt_publish_message(msg_respond, MSG_EMPTY, 0);
        }
        else
        {
            // Read mesh-setting file
            char *mesh_setting_str = read_data(get_config().mesh_setting_file);
            cJSON *mesh_setting_json = cJSON_Parse(mesh_setting_str);
            mesh_setting_t *mesh_setting = cvt_json_to_mesh_setting(mesh_setting_json);

            // Check MeshID matching
            log_debug("Check Match Mesh ID");
            if (app_strstr(mesh_setting->mesh_id, msg_request->data->mesh_setting->mesh_id))
            {
                log_info("Match Mesh ID");
                change_wifi_iface_flag = true;

                // Write new Mesh-Setting to file
                cJSON *mesh_setting_request_json = cvt_mesh_setting_to_json(msg_request->data->mesh_setting);
                char *mesh_setting_request_str = cJSON_Print(mesh_setting_request_json);
                int ret = write_data(mesh_setting_request_str, strlen(mesh_setting_request_str), get_config().mesh_setting_file, TRUNCATE);

                // Respond to Web
                error = error_table_lookup(ERR_SUCCESS);
                msg_respond->respond_data->error = error;
                mqtt_publish_message(msg_respond, MSG_MESH_SETTING, 0);

                // Free Memory
                cJSON_Delete(mesh_setting_request_json);
                free(mesh_setting_request_str);
            }
            else
            {
                log_error("Mesh ID not Found");
                error = error_table_lookup(ERR_MESH_NOT_FOUND);
                msg_respond->respond_data->error = error;
                mqtt_publish_message(msg_respond, MSG_EMPTY, 0);
            }

            // Free Memory
            mesh_setting_free(mesh_setting);
            cJSON_Delete(mesh_setting_json);
            free(mesh_setting_str);
        }
        // sleep (300ms)

        log_debug("change_wifi_iface_flag");
        usleep(300 * 1000);
        if (change_wifi_iface_flag)
        {
            change_wifi_iface(msg_respond->respond_data->data->mesh_setting->wireless->wifi_iface);
        }
    }
    // Free Memory
    log_debug("Free Memory");
    cJSON_Delete(msg_json);
    msg_respond_free(msg_respond, MSG_MESH_SETTING);
    free(msg_request);
    return;
}

void delete_mesh(cJSON *msg_json)
{
    // Load available Mesh Setting
    msg_request_t *msg_request = cvt_json_to_msg_request(msg_json, MSG_EMPTY);
    msg_respond_t *msg_respond = cvt_msg_request_to_msg_respond(msg_request);
    msg_respond->name = cvt_req_name_to_res_name(msg_respond->name);
    error_t *error;
    if (ezm_deamon_is_running() == -1)
    {
        error = error_table_lookup(ERR_EZMESH_DEAMON_CONNECT_FAIL);
        msg_respond->respond_data->error = error;
        msg_respond->name = cvt_req_name_to_res_name(msg_respond->name);
        mqtt_publish_message(msg_respond, MSG_EMPTY, 0);
    }
    else
    {
        log_debug("Get work mode");
        int work_mode = get_work_mode();
        if (work_mode != CONTROLLER_MODE)
        {
            error = error_table_lookup(ERR_MESH_NOT_FOUND);
            msg_respond->respond_data->error = error;
            mqtt_publish_message(msg_respond, MSG_EMPTY, 0);
        }
        else
        {
            // Read a current mesh setting
            char *mesh_setting_str_current = read_data(get_config().mesh_setting_file);
            cJSON *mesh_setting_json_current = cJSON_Parse(mesh_setting_str_current);
            mesh_setting_t *mesh_setting = cvt_json_to_mesh_setting(mesh_setting_json_current);

            // Move Work Mode to Standalone
            mesh_setting->work_mode = "standalone";

            // Save Mesh Setting to file
            cJSON *mesh_setting_json_new = cvt_mesh_setting_to_json(mesh_setting);
            char *mesh_setting_str_new = cJSON_Print(mesh_setting_json_new);
            write_data(mesh_setting_str_new, strlen(mesh_setting_str_new), get_config().mesh_setting_file, TRUNCATE);

            // Reponse Success to Web
            error = error_table_lookup(ERR_SUCCESS);
            msg_respond->respond_data->error = error;
            mqtt_publish_message(msg_respond, MSG_EMPTY, 0);

            // Run Standalone Mode
            run_standalone_mode();

            // Free Memory
            log_debug("Free Memory");
            cJSON_Delete(mesh_setting_json_current);
            cJSON_Delete(mesh_setting_json_new);
            free(mesh_setting_str_current);
            free(mesh_setting_str_new);
            mesh_setting_free(mesh_setting);
        }
    }
    cJSON_Delete(msg_json);
    msg_respond_free(msg_respond, MSG_MESH_SETTING);
    free(msg_request);
    return;
}

void request_mesh_devices(cJSON *msg_json)
{
    msg_request_t *msg_request = cvt_json_to_msg_request(msg_json, MSG_EMPTY);
    msg_respond_t *msg_respond = cvt_msg_request_to_msg_respond(msg_request);
    msg_respond->name = cvt_req_name_to_res_name(msg_respond->name);
    error_t *error;
    // Check EZMesh Deamon
    if (ezm_deamon_is_running() == -1)
    {
        // error = error_table_lookup(ERR_EZMESH_DEAMON_CONNECT_FAIL);
        error = error_table_lookup(ERR_SUCCESS);
        msg_respond->respond_data->error = error;
        mqtt_publish_message(msg_respond, MSG_EMPTY, 0);
        msg_respond_free(msg_respond, MSG_EMPTY);
    }
    else
    {
        // Get emt device
        int nb_devices;
        device_info_t *device_list_info = emt_topology(TOPOLOGY_S2, NULL, &nb_devices);
        device_list_t *device_list = (device_list_t *)calloc(1, sizeof(device_list_t));
        device_list->device_list = device_list_info;
        device_list->nb_device = nb_devices;

        // Cvt devicelist to json
        cJSON *device_list_json = cvt_device_list_to_json(device_list);
        char *device_list_str = cJSON_Print(device_list_json);
        cJSON_Minify(device_list_str);
        log_debug("emt devices: %s", device_list_str);
        cJSON_Delete(device_list_json);
        free(device_list_str);
        log_debug("debug 1");
        // Update emt device to device DB
        update_device_list_to_file_from_emt_device(device_list);
        log_debug("debug 1");
        device_list_free(device_list);
        log_debug("debug 3");
        // Respond to Web
        error = error_table_lookup(ERR_SUCCESS);
        msg_respond->respond_data->error = error;
        msg_respond->respond_data->data->device_list = request_device_list_from_file_by_status(DEVICE_ON);
        mqtt_publish_message(msg_respond, MSG_MESH_DEVICE_LIST, 0);
        msg_respond_free(msg_respond, MSG_MESH_DEVICE_LIST);
    }
    // Free mem
    cJSON_Delete(msg_json);
    free(msg_request);
    return;
}

void request_pair_mesh_devices(cJSON *msg_json)
{
    msg_request_t *msg_request = cvt_json_to_msg_request(msg_json, MSG_MESH_DEVICE_LIST);
    msg_respond_t *msg_respond = cvt_msg_request_to_msg_respond(msg_request);
    msg_respond->name = cvt_req_name_to_res_name(msg_respond->name);
    error_t *error;
    if (ezm_deamon_is_running() == -1)
    {
        error = error_table_lookup(ERR_EZMESH_DEAMON_CONNECT_FAIL);
        msg_respond->respond_data->error = error;
        mqtt_publish_message(msg_respond, MSG_EMPTY, 0);

        // Free mem
        msg_respond_free(msg_respond, MSG_MESH_DEVICE_LIST);
        free(msg_request);
        return;
    }
    else
    {
        // Get device list before;
        log_debug("Get emt device list");
        int nb_device_before;
        device_info_t *device_list_before = emt_topology(TOPOLOGY_S2, NULL, &nb_device_before);

        // Get device for pair
        log_debug("Get pair device list");
        cJSON *device_list_json = cvt_device_list_to_json(msg_request->data->device_list);
        char *device_list_str = cJSON_Print(device_list_json);
        cJSON_Minify(device_list_str);
        log_debug("%s", device_list_str);

        int nb_device_for_pair = msg_request->data->device_list->nb_device;
        device_info_t *device_list_for_pair = msg_request->data->device_list->device_list;

        for (size_t i = 0; i < nb_device_for_pair; i++)
        {
            /* code */
            // Set status of Device to Pairing
            device_list_for_pair[i].status = DEVICE_PAIRING;
            // Only pair standalone device
            if (!strcmp(device_list_for_pair[i].role, "standalone"))
            {
                msg_request->name = "ezm.ezm.mesh.device.agent.activate.requested";
                log_debug("Send Pair Request to IP:%s , ezm_id:%s", device_list_for_pair[i].ip, device_list_for_pair[i].ezm_id);
                int ret = mqtt_publish_message_to_external(device_list_for_pair[i].ip, device_list_for_pair[i].ezm_id, msg_request, MSG_EMPTY, 0);
                if (ret == -1)
                {
                    log_error("mqtt_publish_message_to_external: %s:%s Failed", device_list_for_pair[i].ip, device_list_for_pair[i].ezm_id);
                    error = error_table_lookup(ERR_INTERNAL_SERVER);
                    msg_respond->respond_data->error = error;
                    mqtt_publish_message(msg_respond, MSG_EMPTY, 0);
                    // Free mem
                    cJSON_Delete(msg_json);
                    msg_respond_free(msg_respond, MSG_MESH_DEVICE_LIST);
                    free(msg_request);
                    return;
                }
            }
            else
            {
                log_error("Cannot pair controller device: %s:%s", device_list_for_pair[i].ip, device_list_for_pair[i].mac);
                error = error_table_lookup(ERR_UNKNOWN);
                msg_respond->respond_data->error = error;
                mqtt_publish_message(msg_respond, MSG_EMPTY, 0);
                // Free mem
                cJSON_Delete(msg_json);
                msg_respond_free(msg_respond, MSG_MESH_DEVICE_LIST);
                free(msg_request);
                return;
            }
        }

        // Save List Device for pairing to DB
        set_device_list_to_file(msg_request->data->device_list);

        int user_data[2] = {0};
        user_data[0] = nb_device_before;
        user_data[1] = 0;
        bool timeout = false;
        size_t pair_polling = start_timer(get_config().pair_check_polling_time, add_device_check, TIMER_PERIODIC, user_data);
        size_t pair_check_timeout = start_timer(get_config().pair_timeout, timeout_check, TIMER_SINGLE_SHOT, &timeout);
        while (user_data[1] == 0 && timeout == false)
        {
            usleep(10 * 1000);
        }
        stop_timer(pair_check_timeout);
        stop_timer(pair_polling);
        if (timeout == true)
        {
            // Cannot found new device
            error = error_table_lookup(ERR_TIMEOUT);
            msg_respond->respond_data->error = error;
            mqtt_publish_message(msg_respond, MSG_EMPTY, 0);
            // Free mem
            msg_respond_free(msg_respond, MSG_MESH_DEVICE_LIST);
            free(msg_request);
        }
        else
        {
            error = error_table_lookup(ERR_SUCCESS);
            msg_respond->respond_data->error = error;
            mqtt_publish_message(msg_respond, MSG_MESH_DEVICE_LIST, 0);
            // Free mem
            msg_respond_free(msg_respond, MSG_MESH_DEVICE_LIST);
            free(msg_request);
        }
    }
    cJSON_Delete(msg_json);
    return;
}

void request_devices_available_for_pairing(cJSON *msg_json)
{
    msg_request_t *msg_request = cvt_json_to_msg_request(msg_json, MSG_EMPTY);
    msg_respond_t *msg_respond = cvt_msg_request_to_msg_respond(msg_request);
    msg_respond->name = cvt_req_name_to_res_name(msg_respond->name);
    error_t *error;
    if (ezm_deamon_is_running() == -1)
    {
        error = error_table_lookup(ERR_SUCCESS);
        msg_respond->respond_data->error = error;
        mqtt_publish_message(msg_respond, MSG_EMPTY, 0);
        msg_respond_free(msg_respond, MSG_EMPTY);
    }
    else
    {
        // Get List IP using Avahi Client Browser
        iptables_allow_all_traffic();

        device_list_t *device_list = mdns_query_device_list("_ezm._tcp.local.", 1);
        log_debug("%p", device_list);
        log_debug("%p", device_list->device_list);
        log_debug("%d", device_list->nb_device);

        iptables_refresh();

        // Respond to Web
        error = error_table_lookup(ERR_SUCCESS);
        msg_respond->respond_data->error = error;
        msg_respond->respond_data->data->device_list = device_list;
        cJSON *device_json = cvt_device_list_to_json(device_list);
        char *device_str = cJSON_Print(device_json);
        cJSON_Minify(device_str);
        log_debug("%s", device_str);
        log_debug("1");
        mqtt_publish_message(msg_respond, MSG_MESH_DEVICE_LIST, 0);
        log_debug("1");
        msg_respond_free(msg_respond, MSG_MESH_DEVICE_LIST);
        log_debug("1");
    }
    // Free mem
    cJSON_Delete(msg_json);
    free(msg_request);
    return;
}

void update_mesh_devices(cJSON *msg_json)
{
    msg_request_t *msg_request = cvt_json_to_msg_request(msg_json, MSG_MESH_DEVICE_LIST);
    msg_respond_t *msg_respond = cvt_msg_request_to_msg_respond(msg_request);
    msg_respond->name = cvt_req_name_to_res_name(msg_respond->name);
    error_t *error;
    if (ezm_deamon_is_running() == -1)
    {
        error = error_table_lookup(ERR_EZMESH_DEAMON_CONNECT_FAIL);
        msg_respond->respond_data->error = error;
        mqtt_publish_message(msg_respond, MSG_EMPTY, 0);
    }
    else
    {
        // Get Device List for Update
        log_debug("Get Device List for Update:");
        device_list_t *device_list = msg_request->data->device_list;
        log_debug("Update device list to DB");
        bool ret = update_device_list_to_file_from_web(device_list);

        // Response to Web
        if (ret)
        {
            error = error_table_lookup(ERR_SUCCESS);
        }
        else
        {
            error = error_table_lookup(ERR_DEVICE_NOT_FOUND);
        }
        msg_respond->respond_data->error = error;
        msg_respond->respond_data->data->device_list = device_list;
        mqtt_publish_message(msg_respond, MSG_MESH_DEVICE_LIST, 0);
    }
    cJSON_Delete(msg_json);
    msg_respond_free(msg_respond, MSG_MESH_DEVICE_LIST);
    free(msg_request);
    return;
}

void delete_mesh_devices(cJSON *msg_json)
{
    msg_request_t *msg_request = cvt_json_to_msg_request(msg_json, MSG_MESH_DEVICE_LIST);
    msg_respond_t *msg_respond = cvt_msg_request_to_msg_respond(msg_request);
    msg_respond->name = cvt_req_name_to_res_name(msg_respond->name);
    error_t *error;
    if (ezm_deamon_is_running() == -1)
    {
        error = error_table_lookup(ERR_EZMESH_DEAMON_CONNECT_FAIL);
        msg_respond->respond_data->error = error;
        mqtt_publish_message(msg_respond, MSG_EMPTY, 0);
    }
    else
    {
        // Get device list from emt;
        device_list_t *device_list = (device_list_t *)calloc(1, sizeof(device_list_t));
        int nb_device_emt;
        device_info_t *device_list_emt = emt_topology(TOPOLOGY_S2, NULL, &nb_device_emt);
        device_list->device_list = device_list_emt;
        device_list->nb_device = nb_device_emt;
        // Update emt device to db device
        update_device_list_to_file_from_emt_device(device_list);

        // Get device list is ON before delete device
        device_list_t *device_list_before = request_device_list_from_file_by_status(DEVICE_ON);

        // Get deleted device from request
        device_list_t *device_list_deleted = msg_request->data->device_list;

        // Delete deleted device from db, if device not found -> respond to web
        bool ret = delete_device_list_from_file(device_list_deleted);
        if (!ret)
        {
            error = error_table_lookup(ERR_DEVICE_NOT_FOUND);
            msg_respond->respond_data->error = error;
            mqtt_publish_message(msg_respond, MSG_EMPTY, 0);
        }
        else
        {
            // Send Request Inactivate to Agent by ezm_id
            for (size_t i = 0; i < device_list_deleted->nb_device; i++)
            {
                msg_request->name = "ezm.ezm.mesh.device.agent.inactivate.requested";
                mqtt_publish_message_to_mqtt_bridge(device_list_deleted->device_list[i].ezm_id, msg_request, MSG_EMPTY, 0);
            }

            int user_data[2] = {0};
            user_data[0] = device_list_before->nb_device;
            user_data[1] = 0;
            bool timeout = false;
            size_t remove_polling = start_timer(get_config().pair_check_polling_time, remove_device_check, TIMER_PERIODIC, user_data);
            size_t remove_check_timeout = start_timer(get_config().pair_timeout, timeout_check, TIMER_SINGLE_SHOT, &timeout);
            while (user_data[1] == 0 && timeout == false)
            {
                usleep(10 * 1000);
            }
            stop_timer(remove_check_timeout);
            stop_timer(remove_polling);
            if (timeout == true)
            {
                // Cannot found new device
                error = error_table_lookup(ERR_TIMEOUT);
                msg_respond->respond_data->error = error;
                mqtt_publish_message(msg_respond, MSG_EMPTY, 0);
            }
            else
            {
                error = error_table_lookup(ERR_SUCCESS);
                msg_respond->respond_data->error = error;
                mqtt_publish_message(msg_respond, MSG_MESH_DEVICE_LIST, 0);

                // Broadcast all device list
                error = error_table_lookup(ERR_SUCCESS);
                msg_respond->respond_data->error = error;
                msg_respond->name = get_config().broadcast_request_name;
                mqtt_publish_message(msg_respond, MSG_MESH_DEVICE_LIST, 0);
            }
        }
        device_list_free(device_list);
        device_list_free(device_list_before);
    }
    cJSON_Delete(msg_json);
    msg_respond_free(msg_respond, MSG_MESH_DEVICE_LIST);
    free(msg_request);
    return;
}

void request_activate_agent_device(cJSON *msg_json)
{
    msg_request_t *msg_request = cvt_json_to_msg_request(msg_json, MSG_EMPTY);
    // Read a current mesh setting
    char *mesh_setting_str_current = read_data(get_config().mesh_setting_file);
    cJSON *mesh_setting_json_current = cJSON_Parse(mesh_setting_str_current);
    mesh_setting_t *mesh_setting = cvt_json_to_mesh_setting(mesh_setting_json_current);
    // Move Work Mode to Agent
    mesh_setting->work_mode = "agent";
    // Save Mesh Setting to file
    cJSON *mesh_setting_json_new = cvt_mesh_setting_to_json(mesh_setting);
    char *mesh_setting_str_new = cJSON_Print(mesh_setting_json_new);
    write_data(mesh_setting_str_new, strlen(mesh_setting_str_new), get_config().mesh_setting_file, TRUNCATE);
    // Run RE Mode
    run_agent_mode();

    // Free mem
    cJSON_Delete(msg_json);
    cJSON_Delete(mesh_setting_json_current);
    free(mesh_setting_str_current);
    cJSON_Delete(mesh_setting_json_new);
    free(mesh_setting_str_new);
    mesh_setting_free(mesh_setting);
    return;
}

void request_inactivate_agent_device(cJSON *msg_json)
{
    msg_request_t *msg_request = cvt_json_to_msg_request(msg_json, MSG_EMPTY);
    // Read a current mesh setting
    char *mesh_setting_str_current = read_data(get_config().mesh_setting_file);
    cJSON *mesh_setting_json_current = cJSON_Parse(mesh_setting_str_current);
    mesh_setting_t *mesh_setting = cvt_json_to_mesh_setting(mesh_setting_json_current);
    // Move Work Mode to Standalone
    mesh_setting->work_mode = "standalone";
    cJSON *mesh_setting_json_new = cvt_mesh_setting_to_json(mesh_setting);
    char *mesh_setting_str_new = cJSON_Print(mesh_setting_json_new);
    write_data(mesh_setting_str_new, strlen(mesh_setting_str_new), get_config().mesh_setting_file, TRUNCATE);
    // Save Mesh Setting to file
    run_standalone_mode();

    // Free mem
    cJSON_Delete(msg_json);
    cJSON_Delete(mesh_setting_json_current);
    free(mesh_setting_str_current);
    cJSON_Delete(mesh_setting_json_new);
    free(mesh_setting_str_new);
    mesh_setting_free(mesh_setting);
    return;
}

void request_mesh_quality(cJSON *msg_json)
{
    msg_request_t *msg_request = cvt_json_to_msg_request(msg_json, MSG_EMPTY);
    // Get List IP using Avahi Client Browser
    quality_profile_list_t *quality_profile_list = get_mesh_quality();
    error_t *error = error_table_lookup(ERR_SUCCESS);
    msg_respond_t *msg_respond = cvt_msg_request_to_msg_respond(msg_request);
    if (msg_respond->respond_data->data == NULL)
    {
        msg_respond->respond_data->data = (data_t *)calloc(1, sizeof(data_t));
    }
    msg_respond->respond_data->error = error;
    msg_respond->respond_data->data->quality_profile_list = quality_profile_list;
    msg_respond->name = cvt_req_name_to_res_name(msg_respond->name);
    mqtt_publish_message(msg_respond, MSG_MESH_QUALITY, 0);

    cJSON_Delete(msg_json);
    msg_respond_free(msg_respond, MSG_MESH_QUALITY);
    free(msg_request);
    return;
}