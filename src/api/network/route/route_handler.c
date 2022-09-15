#include "route_handler.h"
#include "core/network/route/route.h"
#include "api/message/message_struct.h"
#include "config.h"
#include "utils/file/file.h"
#include "utils/log/log.h"

void create_static_route(cJSON *msg_json)
{
    msg_request_t *msg_request = cvt_json_to_msg_request(msg_json, MSG_NETWORK_STATIC_ROUTE);
    msg_respond_t *msg_respond = cvt_msg_request_to_msg_respond(msg_request);
    msg_respond->name = cvt_req_name_to_res_name(msg_respond->name);
    error_t *error;
    route_list_t *route_list = msg_request->data->routes_list;
    if (route_list)
    {
        // Setting up route_list
        route_list_t *route_list_remained;
        error_code_t err_code = route_create(route_list, &route_list_remained);
        log_debug("1");
        // Make a copy from msg_request
        error = error_table_lookup(err_code);
        log_debug("2");
        msg_respond->respond_data->error = error;
        log_debug("3");
        msg_respond->respond_data->data->routes_list = route_list_remained;
        cJSON *json = cvt_route_list_to_json(route_list_remained);
        char *str = cJSON_Print(json);
        cJSON_Minify(str);
        log_debug(str);
        log_debug("4");
        // Publish respond
        mqtt_publish_message(msg_respond, MSG_NETWORK_STATIC_ROUTE, 0);
        log_debug("5");
    }

    cJSON_Delete(msg_json);
    msg_respond_free(msg_respond, MSG_NETWORK_STATIC_ROUTE);
    log_debug("6");
    free(msg_request);
    return;
}

void request_static_routes(cJSON *msg_json)
{
    // Get dns_dhcp setting and respond
    msg_request_t *msg_request = cvt_json_to_msg_request(msg_json, MSG_EMPTY);
    msg_respond_t *msg_respond = cvt_msg_request_to_msg_respond(msg_request);
    msg_respond->name = cvt_req_name_to_res_name(msg_respond->name);
    error_t *error;
    // Get route_setting file
    route_list_t *route_list;
    error_code_t err_code = route_request(&route_list);

    // Setting up route
    error = error_table_lookup(err_code);
    msg_respond->respond_data->error = error;
    msg_respond->respond_data->data->routes_list = route_list;
    // Publish respond
    mqtt_publish_message(msg_respond, MSG_NETWORK_STATIC_ROUTE, 0);

    cJSON_Delete(msg_json);
    msg_respond_free(msg_respond, MSG_NETWORK_STATIC_ROUTE);
    free(msg_request);
    return;
}

void delete_static_route(cJSON *msg_json)
{
    msg_request_t *msg_request = cvt_json_to_msg_request(msg_json, MSG_NETWORK_STATIC_ROUTE);
    msg_respond_t *msg_respond = cvt_msg_request_to_msg_respond(msg_request);
    msg_respond->name = cvt_req_name_to_res_name(msg_respond->name);
    error_t *error;
    route_list_t *route_list = msg_request->data->routes_list;
    if (route_list)
    {
        // Check route_id and delte route
        route_list_t *route_list_remained;
        error_code_t err_code = route_delete(route_list, &route_list_remained);
        // Make a copy from msg_request
        // Create error
        error = error_table_lookup(err_code);
        msg_respond->respond_data->error = error;
        msg_respond->respond_data->data->routes_list = route_list_remained;
        // Publish respond
        mqtt_publish_message(msg_respond, MSG_NETWORK_STATIC_ROUTE, 0);
    }

    cJSON_Delete(msg_json);
    route_list_free(route_list);
    msg_respond_free(msg_respond, MSG_NETWORK_STATIC_ROUTE);
    free(msg_request);
    return;
}
void update_static_routes(cJSON *msg_json)
{
    msg_request_t *msg_request = cvt_json_to_msg_request(msg_json, MSG_NETWORK_STATIC_ROUTE);
    msg_respond_t *msg_respond = cvt_msg_request_to_msg_respond(msg_request);
    msg_respond->name = cvt_req_name_to_res_name(msg_respond->name);
    error_t *error;
    route_list_t *route_list = msg_request->data->routes_list;
    // log_info("1");
    if (route_list)
    {
        // Setting up route_list
        route_list_t *route_list_remained;
        error_code_t err = route_update(route_list, &route_list_remained);

        // Make a copy from msg_request
        // Create error
        error = error_table_lookup(err);
        msg_respond->respond_data->error = error;
        msg_respond->respond_data->data->routes_list = route_list_remained;
        // Publish respond
        mqtt_publish_message(msg_respond, MSG_NETWORK_STATIC_ROUTE, 0);
    }

    cJSON_Delete(msg_json);
    route_list_free(route_list);
    msg_respond_free(msg_respond, MSG_NETWORK_STATIC_ROUTE);
    free(msg_request);
    return;
}