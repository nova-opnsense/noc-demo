#include "route.h"
#include "stdlib.h"
#include "utils/file/file.h"
#include "utils/uuid/uuid.h"
#include "utils/cmd/cmd.h"
#include "utils/log/log.h"

static error_code_t route_save_to_setting_file(route_list_t *route_list);

cJSON *cvt_route_to_json(route_t *route)
{
    cJSON *route_json = cJSON_CreateObject();
    cJSON_AddStringToObject(route_json, "id", route->id);
    cJSON_AddStringToObject(route_json, "interface", route->interface);
    cJSON_AddStringToObject(route_json, "ip_mask", route->ip_mask);
    cJSON_AddStringToObject(route_json, "target_ip", route->target_ip);
    cJSON_AddStringToObject(route_json, "name", route->name);
    return route_json;
}
route_t *cvt_json_to_route(cJSON *msg_json)
{
    route_t *route = (route_t *)calloc(1, sizeof(route_t));
    cJSON *id_json = cJSON_GetObjectItemCaseSensitive(msg_json, "id");
    if (id_json && cJSON_IsString(id_json))
    {
        route->id = app_strdup(cJSON_GetStringValue(id_json));
    }

    cJSON *interface_json = cJSON_GetObjectItemCaseSensitive(msg_json, "interface");
    if (interface_json && cJSON_IsString(interface_json))
    {
        route->interface = app_strdup(cJSON_GetStringValue(interface_json));
    }

    cJSON *ip_mask_json = cJSON_GetObjectItemCaseSensitive(msg_json, "ip_mask");
    if (ip_mask_json && cJSON_IsString(ip_mask_json))
    {
        route->ip_mask = app_strdup(cJSON_GetStringValue(ip_mask_json));
    }
    cJSON *target_ip_json = cJSON_GetObjectItemCaseSensitive(msg_json, "target_ip");
    if (target_ip_json && cJSON_IsString(target_ip_json))
    {
        route->target_ip = app_strdup(cJSON_GetStringValue(target_ip_json));
    }

    cJSON *name_json = cJSON_GetObjectItemCaseSensitive(msg_json, "name");
    if (name_json && cJSON_IsString(name_json))
    {
        route->name = app_strdup(cJSON_GetStringValue(name_json));
    }
    return route;
}

cJSON *cvt_route_list_to_json(route_list_t *route_list)
{
    int nb_routes = route_list->nb_routes;
    cJSON *route_list_json = cJSON_CreateArray();
    for (size_t i = 0; i < nb_routes; i++)
    {
        /* code */
        cJSON *route_item_json = cvt_route_to_json(&route_list->route_list[i]);
        cJSON_AddItemToArray(route_list_json, route_item_json);
    }
    return route_list_json;
}

route_list_t *cvt_json_to_route_list(cJSON *msg_json)
{
    route_list_t *route_list = (route_list_t *)calloc(1, sizeof(route_list_t));
    int nb_routes = cJSON_GetArraySize(msg_json);
    route_t *routes = (route_t *)calloc(nb_routes, sizeof(route_t));
    for (size_t i = 0; i < nb_routes; i++)
    {
        /* code */
        routes[i] = *cvt_json_to_route(cJSON_GetArrayItem(msg_json, i));
    }
    route_list->nb_routes = nb_routes;
    route_list->route_list = routes;
    return route_list;
}

error_code_t route_create(route_list_t *route_list_create, route_list_t **route_list_out)
{
    char cmd[200];
    error_code_t err;
    int nb_route_list_create = route_list_create->nb_routes;
    route_list_t *route_list_setting;
    log_debug("Request Route from file");
    err = route_request_from_setting_file(&route_list_setting);
    if (err != ERR_SUCCESS)
    {
        return err;
    }
    int nb_routes_setting = route_list_setting->nb_routes;
    route_list_setting->route_list = (route_t *)realloc(route_list_setting->route_list, (nb_route_list_create + nb_routes_setting) * sizeof(route_t));
    for (size_t i = 0; i < nb_route_list_create; i++)
    {
        route_t route_create = route_list_create->route_list[i];
        route_create.id = uuid_gen("route");
        sprintf(cmd, "ip route add %s via %s dev %s", route_create.ip_mask, route_create.target_ip, route_create.interface);
        log_debug("%s", cmd);
        execl_non_timeout(cmd);
        route_list_setting->route_list[nb_routes_setting + i] = route_create;
    }
    route_list_setting->nb_routes += nb_route_list_create;
    // Save new route_list
    *route_list_out = route_list_setting;
    return route_save_to_setting_file(route_list_setting);
}

error_code_t route_request(route_list_t **route_list_out)
{
    return route_request_from_setting_file(route_list_out);
}

error_code_t route_update(route_list_t *route_list_update, route_list_t **route_list_out)
{
    char cmd[200];
    bool found_id = false;
    error_code_t err;
    int nb_routes_update = route_list_update->nb_routes;
    route_list_t *route_list_setting;
    // log_info("1");
    err = route_request_from_setting_file(&route_list_setting);
    if (err != ERR_SUCCESS)
    {
        return err;
    }
    // log_info("1");
    int nb_routes_setting = route_list_setting->nb_routes;
    for (size_t i = 0; i < nb_routes_update; i++)
    {
        route_t route_update = route_list_update->route_list[i];
        for (size_t j = 0; j < nb_routes_setting; j++)
        {
            route_t route_index = route_list_setting->route_list[j];
            // Check match ID
            if (app_strstr(route_update.id, route_index.id))
            {
                sprintf(cmd, "ip route del %s via %s dev %s", route_index.ip_mask, route_index.target_ip, route_index.interface);
                execl_timeout(2000, cmd);
                // add new route
                sprintf(cmd, "ip route add %s via %s dev %s", route_update.ip_mask, route_update.target_ip, route_update.interface);
                execl_non_timeout(cmd);
                // replace new route in route_list
                route_list_setting->route_list[j] = route_update;
                found_id = true;
                break;
            }
        }
    }
    // Save new route_list
    *route_list_out = route_list_setting;
    if (!found_id)
    {
        return ERR_ID_MISMATCHED;
    }
    err = route_save_to_setting_file(route_list_setting);
    return err;
}
error_code_t route_delete(route_list_t *route_list_delete, route_list_t **route_list_out)
{
    char cmd[200];
    error_code_t err;
    bool found_id = false;
    // log_info("1");
    int nb_routes_delete = route_list_delete->nb_routes;
    // log_info("1");
    route_list_t *route_list_setting;
    err = route_request_from_setting_file(&route_list_setting);
    if (err != ERR_SUCCESS)
    {
        return err;
    }
    int nb_routes_setting = route_list_setting->nb_routes;
    // log_info("1");
    for (size_t i = 0; i < nb_routes_delete; i++)
    {
        // log_info("1");
        route_t route_delete = route_list_delete->route_list[i];
        for (size_t j = 0; j < nb_routes_setting; j++)
        {
            route_t route_index = route_list_setting->route_list[j];
            // Check match ID
            if (route_index.id && route_delete.id && app_strstr(route_delete.id, route_index.id))
            {
                sprintf(cmd, "ip route del %s via %s dev %s", route_delete.ip_mask, route_delete.target_ip, route_delete.interface);
                execl_non_timeout(cmd);
                // remove route item from route_list
                memmove(&route_list_setting->route_list[j], &route_list_setting->route_list[j + 1], (nb_routes_setting - j - 1) * sizeof(route_t));
                route_list_setting->nb_routes = nb_routes_setting - 1;
                found_id = true;
                break;
            }
        }
    }
    // Save new route_list
    *route_list_out = route_list_setting;
    err = route_save_to_setting_file(route_list_setting);
    if (err != ERR_SUCCESS)
    {
        return err;
    }
    if (!found_id)
    {
        return ERR_ID_MISMATCHED;
    }
    return ERR_SUCCESS;
}

error_code_t route_request_from_setting_file(route_list_t **route_list_out)
{
    char *route_list_str = read_data(get_config().route_setting_file);
    cJSON *route_list_json = cJSON_Parse(route_list_str);
    *route_list_out = cvt_json_to_route_list(route_list_json);
    free(route_list_str);
    cJSON_Delete(route_list_json);
    return ERR_SUCCESS;
}

static error_code_t route_save_to_setting_file(route_list_t *route_list)
{
    cJSON *route_list_json = cvt_route_list_to_json(route_list);
    char *route_list_str = cJSON_Print(route_list_json);
    int ret = write_data(route_list_str, strlen(route_list_str), get_config().route_setting_file, TRUNCATE);
    cJSON_Delete(route_list_json);
    free(route_list_str);
    if (ret == -1)
    {
        return ERR_INTERNAL_SERVER;
    }
    return ERR_SUCCESS;
}

void route_free(route_t *route)
{
    if (route)
    {
        app_free(route->id);
        app_free(route->interface);
        app_free(route->ip_mask);
        app_free(route->target_ip);
        app_free(route->name);
    }
}

void route_list_free(route_list_t *route_list)
{
    for (size_t i = 0; i < route_list->nb_routes; i++)
    {
        /* code */
        route_free(&route_list->route_list[i]);
    }
    app_free(route_list->route_list);
    app_free(route_list);
}