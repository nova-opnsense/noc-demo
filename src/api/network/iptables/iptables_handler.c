#include "iptables_handler.h"
#include "api/message/message_struct.h"
#include "config.h"
#include "utils/file/file.h"
#include "utils/log/log.h"
#include "core/network/iptables/iptables.h"

void create_dnat_port_forwarding_profile(cJSON *msg_json)
{
    msg_request_t *msg_request = cvt_json_to_msg_request(msg_json, MSG_IPTABLES_DNAT_PROFILE);
    msg_respond_t *msg_respond = cvt_msg_request_to_msg_respond(msg_request);
    msg_respond->name = cvt_req_name_to_res_name(msg_respond->name);
    error_t *error;
    iptable_list_t *iptable_list = msg_request->data->iptable_list;
    if (iptable_list)
    {
        // Setting up iptables_list
        iptable_list_t *iptable_list_setting_out;
        error_code_t err = iptables_dnat_forwarding_create(iptable_list, &iptable_list_setting_out);

        // Reponse to Web
        error = error_table_lookup(err);
        msg_respond->respond_data->error = error;
        msg_respond->respond_data->data->iptable_list = iptable_list_setting_out;
        mqtt_publish_message(msg_respond, MSG_IPTABLES_DNAT_PROFILE, 0);
    }

    cJSON_Delete(msg_json);
    msg_respond_free(msg_respond, MSG_IPTABLES_DNAT_PROFILE);
    free(msg_request);
    return;
}

// DNAT
void request_dnat_port_forwarding_profiles(cJSON *msg_json)
{
    // Get dns_dhcp setting and respond
    msg_request_t *msg_request = cvt_json_to_msg_request(msg_json, MSG_EMPTY);
    msg_respond_t *msg_respond = cvt_msg_request_to_msg_respond(msg_request);
    msg_respond->name = cvt_req_name_to_res_name(msg_respond->name);
    error_t *error;

    // Get iptable_setting file
    iptable_list_t *iptable_list;
    error_code_t err = iptables_dnat_forwarding_request(&iptable_list);

    // Reponse to Web
    error = error_table_lookup(err);
    msg_respond->respond_data->error = error;
    msg_respond->respond_data->data->iptable_list = iptable_list;
    mqtt_publish_message(msg_respond, MSG_IPTABLES_DNAT_PROFILE, 0);

    // Free mem
    cJSON_Delete(msg_json);
    msg_respond_free(msg_respond, MSG_IPTABLES_DNAT_PROFILE);
    free(msg_request);
    return;
}
void update_dnat_port_forwarding_profiles(cJSON *msg_json)
{
    msg_request_t *msg_request = cvt_json_to_msg_request(msg_json, MSG_IPTABLES_DNAT_PROFILE);
    msg_respond_t *msg_respond = cvt_msg_request_to_msg_respond(msg_request);
    msg_respond->name = cvt_req_name_to_res_name(msg_respond->name);
    error_t *error;

    // Get iptable list
    iptable_list_t *iptable_list = msg_request->data->iptable_list;
    if (iptable_list)
    {
        // Check iptable id and update iptable
        iptable_list_t *iptable_list_setting_out;
        error_code_t err = iptables_dnat_forwarding_update(iptable_list, &iptable_list_setting_out);

        // Make a copy from msg_request
        // Create error
        error = error_table_lookup(err);
        msg_respond->respond_data->error = error;
        msg_respond->respond_data->data->iptable_list = iptable_list_setting_out;
        // Publish respond
        mqtt_publish_message(msg_respond, MSG_IPTABLES_DNAT_PROFILE, 0);
    }

    cJSON_Delete(msg_json);
    msg_respond_free(msg_respond, MSG_IPTABLES_DNAT_PROFILE);
    free(msg_request);
    return;
}

void delete_dnat_port_forwarding_profile(cJSON *msg_json)
{
    msg_request_t *msg_request = cvt_json_to_msg_request(msg_json, MSG_IPTABLES_DNAT_PROFILE);
    msg_respond_t *msg_respond = cvt_msg_request_to_msg_respond(msg_request);
    msg_respond->name = cvt_req_name_to_res_name(msg_respond->name);
    error_t *error;
    // Get iptable list
    iptable_list_t *iptable_list = msg_request->data->iptable_list;
    if (iptable_list)
    {
        iptable_list_t *iptable_list_setting_out;
        error_code_t err = iptables_dnat_forwarding_delete(iptable_list, &iptable_list_setting_out);

        // Respond to Web
        error = error_table_lookup(err);
        msg_respond->respond_data->error = error;
        msg_respond->respond_data->data->iptable_list = iptable_list_setting_out;
        // Publish respond
        mqtt_publish_message(msg_respond, MSG_IPTABLES_DNAT_PROFILE, 0);
    }

    cJSON_Delete(msg_json);
    msg_respond_free(msg_respond, MSG_IPTABLES_DNAT_PROFILE);
    free(msg_request);
    return;
}

// DMZ
void request_dnat_dmz(cJSON *msg_json)
{
    msg_request_t *msg_request = cvt_json_to_msg_request(msg_json, MSG_EMPTY);
    msg_respond_t *msg_respond = cvt_msg_request_to_msg_respond(msg_request);
    msg_respond->name = cvt_req_name_to_res_name(msg_respond->name);
    error_t *error;
    // Get route_setting file
    iptable_t *iptable;
    error_code_t err = iptable_dnat_dmz_request_from_setting_file(&iptable);
    log_debug("1");
    // Create error
    error = error_table_lookup(err);
    log_debug("1");
    msg_respond->respond_data->error = error;
    msg_respond->respond_data->data->iptable = iptable;
    log_debug("1");
    // Publish respond
    mqtt_publish_message(msg_respond, MSG_IPTABLES_DNAT_DMZ_PROFILE, 0);
    // Free mem
    log_debug("1");
    cJSON_Delete(msg_json);
    msg_respond_free(msg_respond, MSG_IPTABLES_DNAT_DMZ_PROFILE);
    free(msg_request);
    return;
}

void create_dnat_dmz(cJSON *msg_json)
{
    msg_request_t *msg_request = cvt_json_to_msg_request(msg_json, MSG_IPTABLES_DNAT_DMZ_PROFILE);
    msg_respond_t *msg_respond = cvt_msg_request_to_msg_respond(msg_request);
    msg_respond->name = cvt_req_name_to_res_name(msg_respond->name);
    error_t *error;
    iptable_t *iptable = msg_request->data->iptable;
    if (iptable)
    {
        // Update iptables_list
        error_code_t err = iptable_dnat_dmz_update(iptable);
        error = error_table_lookup(err);
        msg_respond->respond_data->error = error;
        // Publish respond
        mqtt_publish_message(msg_respond, MSG_IPTABLES_DNAT_DMZ_PROFILE, 0);
    }

    cJSON_Delete(msg_json);
    msg_respond_free(msg_respond, MSG_IPTABLES_DNAT_DMZ_PROFILE);
    free(msg_request);
    return;
}

void create_nova_services_port_profiles(cJSON *msg_json)
{
    msg_request_t *msg_request = cvt_json_to_msg_request(msg_json, MSG_IPTABLE_NOVA_SERVICE_PORT_PROFILE);
    msg_respond_t *msg_respond = cvt_msg_request_to_msg_respond(msg_request);
    msg_respond->name = cvt_req_name_to_res_name(msg_respond->name);
    error_t *error;
    // log_info("1");
    iptable_nova_service_list_t *iptable_nova_service_list = msg_request->data->iptable_nova_service_list;
    // log_info("1");
    if (iptable_nova_service_list)
    {
        // Save route_list file
        // Setting up iptables_list
        error_code_t err = iptable_nova_service_set(iptable_nova_service_list);
        // Make a copy from msg_request
        // Create error
        error = error_table_lookup(err);
        msg_respond->respond_data->error = error;
        // Publish respond
        mqtt_publish_message(msg_respond, MSG_IPTABLE_NOVA_SERVICE_PORT_PROFILE, 0);
    }

    cJSON_Delete(msg_json);
    msg_respond_free(msg_respond, MSG_IPTABLE_NOVA_SERVICE_PORT_PROFILE);
    free(msg_request);
    return;
}

// NOVA Service
void request_nova_services_port_profiles(cJSON *msg_json)
{
    msg_request_t *msg_request = cvt_json_to_msg_request(msg_json, MSG_EMPTY);
    msg_respond_t *msg_respond = cvt_msg_request_to_msg_respond(msg_request);
    msg_respond->name = cvt_req_name_to_res_name(msg_respond->name);
    error_t *error;

    // Get Nova Service Iptable
    iptable_nova_service_list_t *iptable_nova_service_list;
    error_code_t err = iptable_nova_service_request_from_setting_file(&iptable_nova_service_list);

    // Create error
    error = error_table_lookup(err);
    msg_respond->respond_data->error = error;
    msg_respond->respond_data->data->iptable_nova_service_list = iptable_nova_service_list;
    // Publish respond
    mqtt_publish_message(msg_respond, MSG_IPTABLE_NOVA_SERVICE_PORT_PROFILE, 0);

    cJSON_Delete(msg_json);
    msg_respond_free(msg_respond, MSG_IPTABLE_NOVA_SERVICE_PORT_PROFILE);
    free(msg_request);
    return;
}