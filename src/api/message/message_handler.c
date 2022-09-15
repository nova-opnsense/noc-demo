#include "stdlib.h"
#include "stdio.h"
#include <string.h>
#include "message_handler.h"
#include "message_struct.h"
#include "utils/queue/queue.h"
#include "utils/timer/timer.h"
#include "utils/file/file.h"
#include "utils/cmd/cmd.h"
#include "utils/log/log.h"
#include "pthread.h"
#include "mosquitto.h"
#include "core/mesh/topology/wps.h"
#include "api/mesh/mesh_handler.h"
#include "api/network/dhcp/dhcp_handler.h"
#include "api/network/iptables/iptables_handler.h"
#include "api/network/lan/lan_handler.h"
#include "api/network/wan/wan_handler.h"
#include "api/network/route/route_handler.h"
#include "api/network/iptables/iptables_handler.h"

static pthread_mutex_t message_mtx_lock;
extern Queue_t message_queue;

static pthread_mutex_t thread_mtx_lock;
extern Queue_t thread_queue;

static message_handler_mapping_t message_handler_entry[] = {
    // HUB -> EZM
    /* Mesh Settting API */
    {"mesh.created", create_mesh},
    {"mesh.requested", request_mesh},
    {"mesh.updated", update_mesh},
    {"mesh.deleted", delete_mesh},

    /* Mesh Device API */
    {"mesh.devices.requested", request_mesh_devices},
    {"mesh.devices.pair.requested", request_pair_mesh_devices},
    {"mesh.devices.available_for_pairing.requested", request_devices_available_for_pairing},
    {"mesh.devices.updated", update_mesh_devices},
    {"mesh.devices.deleted", delete_mesh_devices},

    // EZM -> EZM
    /* Active and Inactive Agent Config */
    {"mesh.device.agent.activate.requested", request_activate_agent_device},
    {"mesh.device.agent.inactivate.requested", request_inactivate_agent_device},

    /* Mesh Quality */
    {"mesh.quality.requested", request_mesh_quality},

    // Network API
    {"network.lan.requested", request_lan},
    {"network.lan.created", create_lan},
    {"network.wan.requested", request_wan},
    {"network.wan.created", create_wan},
    {"network.dns_dhcp.requested", request_dns_dhcp},
    {"network.dns_dhcp.created", create_dns_dhcp},
    {"network.dhcp4.leases.requested", request_dhcp4_leases},
    {"network.static.routes.requested", request_static_routes},
    {"network.static.routes.created", create_static_route},
    {"network.static.routes.deleted", delete_static_route},
    {"network.static.routes.updated", update_static_routes},
    // iptables api
    {"iptables.dnat.port_forwarding.profiles.requested", request_dnat_port_forwarding_profiles},
    {"iptables.dnat.port_forwarding.profiles.updated", update_dnat_port_forwarding_profiles},
    {"iptables.dnat.port_forwarding.profiles.created", create_dnat_port_forwarding_profile},
    {"iptables.dnat.port_forwarding.profiles.deleted", delete_dnat_port_forwarding_profile},
    {"iptables.dnat.dmz.profiles.requested", request_dnat_dmz},
    {"iptables.dnat.dmz.profiles.created", create_dnat_dmz},
    {"iptables.nova-services.port-profiles.requested", request_nova_services_port_profiles},
    {"iptables.nova-services.port-profiles.created", create_nova_services_port_profiles},
};

void mqtt_on_message_cb_handler(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
    cJSON *msg_json = cJSON_Parse(msg->payload);
    // Get Request name and check request valid
    char *request_name = NULL;
    cJSON *request_name_json = cJSON_GetObjectItemCaseSensitive(msg_json, "name");
    if (request_name_json && cJSON_IsString(request_name_json))
    {
        request_name = cJSON_GetStringValue(request_name_json);
    }
    else
    {
        log_error("Cannot get request name");
        return;
    }
    if (msg_json != NULL)
    {
        size_t nb_handler = sizeof(message_handler_entry) / sizeof(message_handler_mapping_t);
        for (size_t i = 0; i < nb_handler; i++)
        {
            if (app_strstr(request_name, message_handler_entry[i].mqtt_message_request))
            {
                log_info("%s request", message_handler_entry[i].mqtt_message_request);
                pthread_t new_thread;
                pthread_create(&new_thread, NULL, message_handler_entry[i].message_handler, msg_json);
                pthread_mutex_lock(&thread_mtx_lock);
                q_push(&thread_queue, &new_thread);
                pthread_mutex_unlock(&thread_mtx_lock);
                break;
            }
            if (i == nb_handler - 1)
            {
                error_t *error = error_table_lookup(ERR_REQUEST_INVALID);
                char *respond_name = cvt_req_name_to_res_name(request_name);
                if (request_name)
                {
                    msg_request_t *msg_request = cvt_json_to_msg_request(msg_json, MSG_EMPTY);
                    msg_respond_t *msg_respond = cvt_msg_request_to_msg_respond(msg_request);
                    msg_respond->respond_data->error = error;
                    mqtt_publish_message(msg_respond, NULL, MSG_EMPTY, 0);
                }
            }
        }
    }
    else
    {
        log_error("Message Format Invalid");
    }
    mosquitto_message_free(msg);
}

void thread_garbage(void *user_data)
{
    if (!q_isEmpty(&thread_queue))
    {
        pthread_t new_thread;
        pthread_mutex_lock(&thread_mtx_lock);
        q_pop(&thread_queue, &new_thread);
        if (pthread_tryjoin_np(new_thread, NULL) != 0)
        {
            q_push(&thread_queue, &new_thread);
        }
        pthread_mutex_unlock(&thread_mtx_lock);
    }
}
