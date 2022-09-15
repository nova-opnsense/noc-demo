#include "lan_handler.h"
#include "api/message/message_struct.h"
#include "config.h"
#include "utils/file/file.h"
#include "utils/log/log.h"
#include "api/error_code/error_code.h"

void create_lan(cJSON *msg_json)
{
    msg_request_t *msg_request = cvt_json_to_msg_request(msg_json, MSG_NETWORK_LAN);
    msg_respond_t *msg_respond = cvt_msg_request_to_msg_respond(msg_request);
    msg_respond->name = cvt_req_name_to_res_name(msg_respond->name);
    error_t *error;

    lan_t *lan = msg_request->data->lan;
    if (lan)
    {
        error_code_t err = lan_set(lan);
        // Setting up lan

        // Make a copy from msg_request
        // Create error
        error = error_table_lookup(err);
        msg_respond->respond_data->error = error;
        // Publish respond
        mqtt_publish_message(msg_respond, MSG_NETWORK_LAN, 0);
        // Wait 1 second and Restart network service
        sleep(1);
        restart_service("network");
    }
    cJSON_Delete(msg_json);
    msg_respond_free(msg_respond, MSG_NETWORK_LAN);
    free(msg_request);
    return;
}
void request_lan(cJSON *msg_json)
{
    // Get dns_dhcp setting and respond
    msg_request_t *msg_request = cvt_json_to_msg_request(msg_json, MSG_EMPTY);
    msg_respond_t *msg_respond = cvt_msg_request_to_msg_respond(msg_request);
    msg_respond->name = cvt_req_name_to_res_name(msg_respond->name);
    error_t *error;
    // Get lan_setting file
    lan_t *lan;
    error_code_t err = lan_get(&lan);

    // Reponse to Web
    msg_respond->respond_data->data->lan = lan;
    // Create error
    error = error_table_lookup(err);
    msg_respond->respond_data->error = error;
    // Publish respond
    mqtt_publish_message(msg_respond, MSG_NETWORK_LAN, 0);

    // Free mem
    cJSON_Delete(msg_json);
    msg_respond_free(msg_respond, MSG_NETWORK_LAN);
    free(msg_request);
    return;
}