#include "dhcp_handler.h"
#include "core/network/dhcp/dhcp.h"
#include "stdbool.h"
#include "api/message/message_struct.h"
#include "api/mqtt/mqtt_handler.h"
#include "utils/log/log.h"
#include "utils/file/file.h"
#include "config/service_controller/service_controller.h"

void create_dns_dhcp(cJSON *msg_json)
{
    msg_request_t *msg_request = cvt_json_to_msg_request(msg_json, MSG_NETWORK_DNS_DHCP);
    msg_respond_t *msg_respond = cvt_msg_request_to_msg_respond(msg_request);
    msg_respond->name = cvt_req_name_to_res_name(msg_respond->name);
    error_t *error;

    // Get dns-dhcp
    dns_dhcp_t *dns_dhcp = msg_request->data->dns_dhcp;
    error_code_t err_code = dns_dhcp_set(dns_dhcp);
    // Make a copy from msg_request
    error = error_table_lookup(err_code);
    msg_respond->respond_data->error = error;
    mqtt_publish_message(msg_respond, MSG_NETWORK_DNS_DHCP, 0);
    // Wait 1 second and Restart dns_dhcp service
    sleep(1);
    restart_service("dnsmasq");

    cJSON_Delete(msg_json);
    msg_respond_free(msg_respond, MSG_NETWORK_DNS_DHCP);
    free(msg_request);
    return;
}
void request_dns_dhcp(cJSON *msg_json)
{
    // Get dns_dhcp setting and respond
    msg_request_t *msg_request = cvt_json_to_msg_request(msg_json, MSG_EMPTY);
    msg_respond_t *msg_respond = cvt_msg_request_to_msg_respond(msg_request);
    msg_respond->name = cvt_req_name_to_res_name(msg_respond->name);
    error_t *error;
    // Get dns_dhcp data
    dns_dhcp_t *dns_dhcp = dns_dhcp_get_from_setting_file();

    // Create error code
    error = error_table_lookup(ERR_SUCCESS);
    msg_respond->respond_data->error = error;
    msg_respond->respond_data->data->dns_dhcp = dns_dhcp;

    mqtt_publish_message(msg_respond, MSG_NETWORK_DNS_DHCP, 0);

    cJSON_Delete(msg_json);
    msg_respond_free(msg_respond, MSG_NETWORK_DNS_DHCP);
    free(msg_request);
    return;
}

void request_dhcp4_leases(cJSON *msg_json)
{
    msg_request_t *msg_request = cvt_json_to_msg_request(msg_json, MSG_EMPTY);
    // Create respond msg from msg_request
    msg_respond_t *msg_respond = cvt_msg_request_to_msg_respond(msg_request);
    msg_respond->name = cvt_req_name_to_res_name(msg_respond->name);
    error_t *error;
    // Get dns_dhcp data
    dhcp4_lease_t *dhcp4_lease = (dhcp4_lease_t *)calloc(1, sizeof(dhcp4_lease_t));
    int nb_dhcp_lease;
    dhcp_info_t *dhcp_list;
    error_code_t err_code = dhcp_lease_get(&dhcp_list, &nb_dhcp_lease);
    if (err_code == ERR_SUCCESS)
    {
        dhcp4_lease->list_device = dhcp_list;
        dhcp4_lease->nb_device = nb_dhcp_lease;
    }

    error = error_table_lookup(err_code);
    msg_respond->respond_data->data->dhcp4_lease = dhcp4_lease;
    msg_respond->respond_data->error = error;
    mqtt_publish_message(msg_respond, MSG_NETWORK_DHCP4_LEASES, 0);

    // Free mem
    cJSON_Delete(msg_json);
    msg_respond_free(msg_respond, MSG_NETWORK_DHCP4_LEASES);
    free(msg_request);
    return;
}