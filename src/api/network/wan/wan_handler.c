#include "wan_handler.h"
#include "api/message/message_struct.h"
#include "config.h"
#include "utils/file/file.h"
#include "utils/log/log.h"
#include "core/network/wan/wan.h"
#include "api/error_code/error_code.h"

void create_wan(cJSON *msg_json)
{
    msg_request_t *msg_request = cvt_json_to_msg_request(msg_json, MSG_NETWORK_WAN);
    msg_respond_t *msg_respond = cvt_msg_request_to_msg_respond(msg_request);
    msg_respond->name = cvt_req_name_to_res_name(msg_respond->name);
    error_t *error;
    wan_t *wan = msg_request->data->wan;
    // log_info("1");
    if (wan)
    {
        // Setting up wan
        error_code_t err = wan_set(wan);

        // Create error
        error = error_table_lookup(err);
        msg_respond->respond_data->error = error;
        // Publish respond
        mqtt_publish_message(msg_respond, MSG_NETWORK_WAN, 0);
        // Wait 2 second and Restart network service
        sleep(2);
        restart_service("network");
    }

    cJSON_Delete(msg_json);
    msg_respond_free(msg_respond, MSG_NETWORK_WAN);
    free(msg_request);
    return;
}
void request_wan(cJSON *msg_json)
{
    // Get dns_dhcp setting and respond
    msg_request_t *msg_request = cvt_json_to_msg_request(msg_json, MSG_EMPTY);
    msg_respond_t *msg_respond = cvt_msg_request_to_msg_respond(msg_request);
    msg_respond->name = cvt_req_name_to_res_name(msg_respond->name);
    error_t *error;
    // Get lan_setting file
    wan_t *wan;
    error_code_t err = wan_get(&wan);

    // Response to Web
    error = error_table_lookup(err);
    msg_respond->respond_data->error = error;
    msg_respond->respond_data->data->wan = wan;
    mqtt_publish_message(msg_respond, MSG_NETWORK_WAN, 0);

    cJSON_Delete(msg_json);
    msg_respond_free(msg_respond, MSG_NETWORK_WAN);
    free(msg_request);
    return;
}