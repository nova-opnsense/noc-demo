#include "utils/log/log.h"
#include "utils/split/split.h"
#include "utils/mem/mem.h"
#include "message_struct.h"

cJSON *cvt_data_to_json(data_t *data, msg_type_t msg_type)
{
    if (data == NULL)
    {
        return NULL;
    }
    cJSON *data_json;
    switch (msg_type)
    {
    case MSG_MESH_DEVICE_LIST:
        /* code */
        if (data)
        {
            data_json = cvt_device_list_to_json(data->device_list);
        }
        break;
    case MSG_MESH_SETTING:
        ////log_info("1");
        if (data)
        {
            data_json = cvt_mesh_setting_to_json(data->mesh_setting);
        }
        break;
    case MSG_MESH_QUALITY:
        if (data)
        {
            data_json = cvt_quality_profile_list_to_json(data->quality_profile_list);
        }
        break;
    case MSG_NETWORK_DNS_DHCP:
        // log_info("1");
        if (data)
        {
            data_json = cvt_dns_dhcp_to_json(data->dns_dhcp);
        }
        break;
    case MSG_NETWORK_DHCP4_LEASES:
        if (data)
        {
            data_json = cvt_dhcp4_lease_to_json(data->dhcp4_lease);
        }
        break;
    case MSG_NETWORK_LAN:
        // log_info("1");
        if (data)
        {
            data_json = cvt_lan_to_json(data->lan);
        }
        break;
    case MSG_NETWORK_WAN:
        // log_info("1");
        if (data)
        {
            data_json = cvt_wan_to_json(data->wan);
        }
        break;
    case MSG_NETWORK_STATIC_ROUTE:
        // log_info("1");
        if (data)
        {
            data_json = cvt_route_list_to_json(data->routes_list);
        }
        break;
    case MSG_IPTABLES_DNAT_PROFILE:
        // log_info("1");
        if (data)
        {
            data_json = cvt_iptable_list_to_json(data->iptable_list);
        }
        break;
    case MSG_IPTABLES_DNAT_DMZ_PROFILE:
        // log_info("1");
        if (data)
        {
            data_json = cvt_iptable_to_json(data->iptable);
        }
        break;
    case MSG_IPTABLE_NOVA_SERVICE_PORT_PROFILE:
        // log_info("1");
        if (data)
        {
            data_json = cvt_iptable_nova_service_list_to_json(data->iptable_nova_service_list);
        }
        break;
    default:
        if (data)
        {
            data_json = cJSON_CreateObject();
        }
        break;
    }
    return data_json;
}

data_t *cvt_json_to_data(cJSON *json, msg_type_t msg_type)
{
    data_t *data = (data_t *)calloc(1, sizeof(data_t));
    switch (msg_type)
    {
    case MSG_MESH_DEVICE_LIST:
        /* code */
        if (json != NULL && cJSON_IsArray(json))
        {
            data->device_list = cvt_json_to_device_list(json);
        }
        break;
    case MSG_MESH_SETTING:
        if (json != NULL && cJSON_IsObject(json))
        {
            data->mesh_setting = cvt_json_to_mesh_setting(json);
        }
        break;
    case MSG_MESH_QUALITY:
        if (json != NULL && cJSON_IsObject(json))
        {
            data->quality_profile_list = cvt_json_to_quality_profile_list(json);
        }
        break;
    case MSG_NETWORK_DNS_DHCP:
        // log_info("1");
        if (json && cJSON_IsObject(json))
        {
            data->dns_dhcp = cvt_json_to_dns_dhcp(json);
        }
        break;
    case MSG_NETWORK_DHCP4_LEASES:
        // log_info("1");
        if (json && cJSON_IsObject(json))
        {
            data->dhcp4_lease = cvt_json_to_dhcp4_lease(json);
        }
        break;
    case MSG_NETWORK_LAN:
        // log_info("1");
        if (json && cJSON_IsObject(json))
        {
            data->lan = cvt_json_to_lan(json);
        }
        break;
    case MSG_NETWORK_WAN:
        // log_info("1");
        if (json && cJSON_IsObject(json))
        {
            data->wan = cvt_json_to_wan(json);
        }
        break;
    case MSG_NETWORK_STATIC_ROUTE:
        // log_info("1");
        if (json && cJSON_IsArray(json))
        {
            data->routes_list = cvt_json_to_route_list(json);
        }
        break;
    case MSG_IPTABLES_DNAT_PROFILE:
        // log_info("1");
        if (json && cJSON_IsArray(json))
        {
            data->iptable_list = cvt_json_to_iptable_list(json);
        }
        break;
    case MSG_IPTABLES_DNAT_DMZ_PROFILE:
        // log_info("1");
        if (json && cJSON_IsObject(json))
        {
            data->iptable = cvt_json_to_iptable(json);
        }
        break;
    case MSG_IPTABLE_NOVA_SERVICE_PORT_PROFILE:
        // log_info("1");
        if (json && cJSON_IsArray(json))
        {
            // log_info("1");
            data->iptable_nova_service_list = cvt_json_to_iptable_nova_service_list(json);
        }
        break;
    default:
        break;
    }
    return data;
}

cJSON *cvt_respond_data_to_json(respond_data_t *respond_data, msg_type_t msg_type)
{
    if (respond_data == NULL)
    {
        return NULL;
    }
    cJSON *respond_data_json = cJSON_CreateObject();
    ////log_info("1");
    cJSON *data_json = cvt_data_to_json(respond_data->data, msg_type);
    ////log_info("1");
    cJSON_AddItemToObject(respond_data_json, "data", data_json);
    ////log_info("1");
    cJSON *error_json = cvt_error_to_json(respond_data->error);
    ////log_info("1");
    cJSON_AddItemToObject(respond_data_json, "error", error_json);
    return respond_data_json;
}

respond_data_t *cvt_json_to_respond_data(cJSON *json, msg_type_t msg_type)
{
    respond_data_t *respond_data = (respond_data_t *)calloc(1, sizeof(respond_data_t));
    cJSON *data_json = cJSON_GetObjectItemCaseSensitive(json, "data");
    if (data_json != NULL && cJSON_IsObject(data_json))
    {
        respond_data->data = cvt_json_to_data(data_json, msg_type);
    }
    cJSON *error_json = cJSON_GetObjectItemCaseSensitive(json, "error");
    if (error_json != NULL && cJSON_IsObject(error_json))
    {
        respond_data->error = cvt_json_to_error(error_json);
    }
    return respond_data;
}

msg_request_t *cvt_json_to_msg_request(cJSON *json, msg_type_t msg_type)
{
    msg_request_t *msg = (msg_request_t *)calloc(1, sizeof(msg_request_t));
    cJSON *name_json = cJSON_GetObjectItemCaseSensitive(json, "name");
    if (name_json != NULL && cJSON_IsString(name_json))
    {
        msg->name = app_strdup(cJSON_GetStringValue(name_json));
    }
    cJSON *scope_json = cJSON_GetObjectItemCaseSensitive(json, "scope");
    if (scope_json != NULL && cJSON_IsString(scope_json))
    {
        msg->scope = app_strdup(cJSON_GetStringValue(scope_json));
    }
    cJSON *source_json = cJSON_GetObjectItemCaseSensitive(json, "source");
    if (source_json != NULL && cJSON_IsString(source_json))
    {
        msg->source = app_strdup(cJSON_GetStringValue(source_json));
    }
    cJSON *home_id_json = cJSON_GetObjectItemCaseSensitive(json, "home_id");
    if (home_id_json != NULL && cJSON_IsString(home_id_json))
    {
        msg->home_id = app_strdup(cJSON_GetStringValue(home_id_json));
    }
    cJSON *user_id_json = cJSON_GetObjectItemCaseSensitive(json, "user_id");
    if (user_id_json != NULL && cJSON_IsString(user_id_json))
    {
        msg->user_id = app_strdup(cJSON_GetStringValue(user_id_json));
    }
    cJSON *id_json = cJSON_GetObjectItemCaseSensitive(json, "id");
    if (id_json != NULL && cJSON_IsString(id_json))
    {
        msg->id = app_strdup(cJSON_GetStringValue(id_json));
    }
    cJSON *auth_token_json = cJSON_GetObjectItemCaseSensitive(json, "auth_token");
    if (auth_token_json != NULL && cJSON_IsString(auth_token_json))
    {
        msg->auth_token = app_strdup(cJSON_GetStringValue(auth_token_json));
    }
    cJSON *parent_id_json = cJSON_GetObjectItemCaseSensitive(json, "parent_id");
    if (parent_id_json != NULL && cJSON_IsString(parent_id_json))
    {
        msg->parent_id = app_strdup(cJSON_GetStringValue(parent_id_json));
    }
    cJSON *client_id_json = cJSON_GetObjectItemCaseSensitive(json, "client_id");
    if (client_id_json != NULL && cJSON_IsString(client_id_json))
    {
        msg->client_id = app_strdup(cJSON_GetStringValue(client_id_json));
    }
    cJSON *msg_data_json = cJSON_GetObjectItemCaseSensitive(json, "data");
    if (msg_data_json)
    {
        msg->data = cvt_json_to_data(msg_data_json, msg_type);
    }
    cJSON *created_time_json = cJSON_GetObjectItemCaseSensitive(json, "created_time");
    if (created_time_json != NULL && cJSON_IsNumber(created_time_json))
    {
        msg->created_time = created_time_json->valueint;
    }
    return msg;
}

cJSON *cvt_msg_request_to_json(msg_request_t *msg_request, msg_type_t msg_type)
{
    if (msg_request == NULL)
    {
        return NULL;
    }
    cJSON *message_json = cJSON_CreateObject();
    ////log_info("1");
    cJSON *data_json = cvt_data_to_json(msg_request->data, msg_type);
    ////log_info("1");
    cJSON_AddStringToObject(message_json, "name", msg_request->name);
    ////log_info("1");
    cJSON_AddStringToObject(message_json, "scope", msg_request->scope);
    ////log_info("1");
    cJSON_AddStringToObject(message_json, "source", msg_request->source);
    ////log_info("1");
    cJSON_AddStringToObject(message_json, "home_id", msg_request->home_id);
    ////log_info("1");
    cJSON_AddStringToObject(message_json, "user_id", msg_request->user_id);
    ////log_info("1");
    cJSON_AddStringToObject(message_json, "id", msg_request->id);
    ////log_info("1");
    cJSON_AddStringToObject(message_json, "auth_token", msg_request->auth_token);
    ////log_info("1");
    cJSON_AddStringToObject(message_json, "parent_id", msg_request->parent_id);
    ////log_info("1");
    cJSON_AddStringToObject(message_json, "client_id", msg_request->client_id);
    ////log_info("1");
    cJSON_AddItemToObject(message_json, "data", data_json);
    ////log_info("1");
    cJSON_AddNumberToObject(message_json, "created_time", msg_request->created_time);
    ////log_info("1");
    return message_json;
}

msg_respond_t *cvt_json_to_msg_respond(cJSON *json, msg_type_t msg_type)
{
    msg_respond_t *msg = (msg_respond_t *)calloc(1, sizeof(msg_respond_t));
    cJSON *name_json = cJSON_GetObjectItemCaseSensitive(json, "name");
    if (name_json != NULL && cJSON_IsString(name_json))
    {
        msg->name = app_strdup(cJSON_GetStringValue(name_json));
    }
    cJSON *scope_json = cJSON_GetObjectItemCaseSensitive(json, "scope");
    if (scope_json != NULL && cJSON_IsString(scope_json))
    {
        msg->scope = app_strdup(cJSON_GetStringValue(scope_json));
    }
    cJSON *source_json = cJSON_GetObjectItemCaseSensitive(json, "source");
    if (source_json != NULL && cJSON_IsString(source_json))
    {
        msg->source = app_strdup(cJSON_GetStringValue(source_json));
    }
    cJSON *home_id_json = cJSON_GetObjectItemCaseSensitive(json, "home_id");
    if (home_id_json != NULL && cJSON_IsString(home_id_json))
    {
        msg->home_id = app_strdup(cJSON_GetStringValue(home_id_json));
    }
    cJSON *user_id_json = cJSON_GetObjectItemCaseSensitive(json, "user_id");
    if (user_id_json != NULL && cJSON_IsString(user_id_json))
    {
        msg->user_id = app_strdup(cJSON_GetStringValue(user_id_json));
    }
    cJSON *id_json = cJSON_GetObjectItemCaseSensitive(json, "id");
    if (id_json != NULL && cJSON_IsString(id_json))
    {
        msg->id = app_strdup(cJSON_GetStringValue(id_json));
    }
    cJSON *auth_token_json = cJSON_GetObjectItemCaseSensitive(json, "auth_token");
    if (auth_token_json != NULL && cJSON_IsString(auth_token_json))
    {
        msg->auth_token = app_strdup(cJSON_GetStringValue(auth_token_json));
    }
    cJSON *parent_id_json = cJSON_GetObjectItemCaseSensitive(json, "parent_id");
    if (parent_id_json != NULL && cJSON_IsString(parent_id_json))
    {
        msg->parent_id = app_strdup(cJSON_GetStringValue(parent_id_json));
    }
    cJSON *client_id_json = cJSON_GetObjectItemCaseSensitive(json, "client_id");
    if (client_id_json != NULL && cJSON_IsString(client_id_json))
    {
        msg->client_id = app_strdup(cJSON_GetStringValue(client_id_json));
    }
    cJSON *msg_data_json = cJSON_GetObjectItemCaseSensitive(json, "data");
    if (msg_data_json != NULL && cJSON_IsObject(msg_data_json))
    {
        msg->respond_data = cvt_json_to_respond_data(msg_data_json, msg_type);
    }
    cJSON *created_time_json = cJSON_GetObjectItemCaseSensitive(json, "created_time");
    if (created_time_json != NULL && cJSON_IsNumber(created_time_json))
    {
        msg->created_time = created_time_json->valueint;
    }
    return msg;
}

cJSON *cvt_msg_respond_to_json(msg_respond_t *msg_respond, msg_type_t msg_type)
{
    if (msg_respond == NULL)
    {
        return NULL;
    }
    cJSON *message_json = cJSON_CreateObject();
    ////log_info("1");
    cJSON *data_json = cvt_respond_data_to_json(msg_respond->respond_data, msg_type);
    ////log_info("1");
    cJSON_AddStringToObject(message_json, "name", msg_respond->name);
    ////log_info("1");
    cJSON_AddStringToObject(message_json, "scope", msg_respond->scope);
    ////log_info("1");
    cJSON_AddStringToObject(message_json, "source", msg_respond->source);
    ////log_info("1");
    cJSON_AddStringToObject(message_json, "home_id", msg_respond->home_id);
    ////log_info("1");
    cJSON_AddStringToObject(message_json, "user_id", msg_respond->user_id);
    ////log_info("1");
    cJSON_AddStringToObject(message_json, "id", msg_respond->id);
    ////log_info("1");
    cJSON_AddStringToObject(message_json, "auth_token", msg_respond->auth_token);
    ////log_info("1");
    cJSON_AddStringToObject(message_json, "parent_id", msg_respond->parent_id);
    ////log_info("1");
    cJSON_AddStringToObject(message_json, "client_id", msg_respond->client_id);
    ////log_info("1");
    cJSON_AddItemToObject(message_json, "data", data_json);
    ////log_info("1");
    cJSON_AddNumberToObject(message_json, "created_time", msg_respond->created_time);
    ////log_info("1");
    return message_json;
}

msg_respond_t *cvt_msg_request_to_msg_respond(msg_request_t *msg_request)
{
    msg_respond_t *msg_respond = (msg_respond_t *)calloc(1, sizeof(msg_respond_t));
    msg_respond->name = msg_request->name;
    msg_respond->scope = msg_request->scope;
    msg_respond->source = msg_request->source;
    msg_respond->home_id = msg_request->home_id;
    msg_respond->user_id = msg_request->user_id;
    msg_respond->id = msg_request->id;
    msg_respond->auth_token = msg_request->auth_token;
    msg_respond->parent_id = msg_request->parent_id;
    msg_respond->client_id = msg_request->client_id;
    msg_respond->respond_data = (respond_data_t *)calloc(1, sizeof(respond_data_t));
    msg_respond->respond_data->data = msg_request->data;
    msg_respond->created_time = msg_request->created_time;
}

char *cvt_req_name_to_res_name(char *req_name)
{
    int size_of_pattern;
    char *res_name = (char *)calloc(1, 200);
    // log_info("%s",req_name);
    char **name_split = split(req_name, ".", &size_of_pattern);
    size_t res_name_size;
    if (name_split != NULL)
    {
        if (size_of_pattern)
        {
            strcat(res_name, name_split[1]);
            strcat(res_name, ".");
            strcat(res_name, name_split[0]);
            for (size_t i = 2; i < size_of_pattern - 1; i++)
            {
                /* code */
                strcat(res_name, ".");
                strcat(res_name, name_split[i]);
            }
            strcat(res_name, ".");
            if (app_strstr(name_split[size_of_pattern - 1], "requested"))
            {

                strcat(res_name, "responded");
            }
            else
            {
                strcat(res_name, name_split[size_of_pattern - 1]);
            }
            log_info("%s", res_name);
        }
        else
        {
            log_error("Request name is invalid");
        }
    }
    else
    {
        log_error("Cannot split request name");
    }
    free(req_name);
    return res_name;
}

void msg_request_free(msg_request_t *msg_request, msg_type_t type)
{
    free(msg_request->name);
    free(msg_request->scope);
    free(msg_request->source);
    free(msg_request->home_id);
    free(msg_request->user_id);
    free(msg_request->id);
    free(msg_request->auth_token);
    free(msg_request->parent_id);
    free(msg_request->client_id);
    data_free(msg_request->data, type);
    free(msg_request);
}

void msg_respond_free(msg_respond_t *msg_respond, msg_type_t type)
{
    if (msg_respond == NULL)
    {
        return;
    }
    log_debug("Free name");
    app_free(msg_respond->name);
    log_debug("Free scope");
    app_free(msg_respond->scope);
    log_debug("Free source");
    app_free(msg_respond->source);
    log_debug("Free home_id");
    app_free(msg_respond->home_id);
    log_debug("Free user_id");
    app_free(msg_respond->user_id);
    log_debug("Free id");
    app_free(msg_respond->id);
    log_debug("Free auth_token");
    app_free(msg_respond->auth_token);
    log_debug("Free parent_id");
    app_free(msg_respond->parent_id);
    log_debug("Free client_id");
    app_free(msg_respond->client_id);
    log_debug("Free respond_data");
    respond_data_free(msg_respond->respond_data, type);
    log_debug("Free msg_respond");
    app_free(msg_respond);
    log_debug("Free msg_respond done");
}

void respond_data_free(respond_data_t *respond_data, msg_type_t type)
{
    if (respond_data == NULL)
    {
        return;
    }
    log_debug("Free data");
    data_free(respond_data->data, type);
    log_debug("Free error");
    error_free(respond_data->error);
    log_debug("Free respond_data");
    app_free(respond_data);
}

void data_free(data_t *data, msg_type_t type)
{
    if (data == NULL)
    {
        return;
    }
    switch (type)
    {
    case MSG_MESH_DEVICE_LIST:
        if (data)
        {
            device_list_free(data->device_list);
        }
        break;
    case MSG_MESH_SETTING:
        if (data)
        {
            mesh_setting_free(data->mesh_setting);
        }
        break;
    case MSG_MESH_QUALITY:
        if (data)
        {
            quality_profile_list_free(data->quality_profile_list);
        }
        break;
    case MSG_NETWORK_DNS_DHCP:
        if (data)
        {
            dns_dhcp_free(data->dns_dhcp);
        }
        break;
    case MSG_NETWORK_DHCP4_LEASES:
        if (data)
        {
            dhcp4_lease_free(data->dhcp4_lease);
        }
        break;
    case MSG_NETWORK_LAN:
        if (data)
        {
            lan_free(data->lan);
        }
        break;
    case MSG_NETWORK_WAN:
        if (data)
        {
            wan_free(data->wan);
        }
        break;
    case MSG_NETWORK_STATIC_ROUTE:
        if (data)
        {
            route_list_free(data->routes_list);
        }
        break;
    case MSG_IPTABLES_DNAT_PROFILE:
        if (data)
        {
            iptable_list_free(data->iptable_list);
        }
        break;
    case MSG_IPTABLE_NOVA_SERVICE_PORT_PROFILE:
        if (data)
        {
            iptable_nova_services_list_free(data->iptable_nova_service_list);
        }
        break;
    default:
        break;
    }
    free(data);
}