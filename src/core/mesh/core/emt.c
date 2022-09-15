#include "emt.h"
#include "curl/curl.h"
#include "utils/log/log.h"
#include "stdio.h"
#include "string.h"

#define msleep(time) usleep(time * 1000)

static char data_recv[1024 * 50];
static char data_send[1024 * 50];
static int send_size;
static int recv_size;

char *emt_debug(debug_action_t debug_action, char *args)
{
    switch (debug_action)
    {
    case DEBUG_HERE:
        /* code */
        break;
    case DEBUG_LEVEL:
        /* code */
        break;
    default:
        break;
    }
    return NULL;
}
char *emt_md(md_action_t md_action, char *args)
{
    switch (md_action)
    {
    case MD_PRINT:
        /* code */
        break;
    case MD_SET:
        /* code */
        break;
    default:
        break;
    }
    return NULL;
}
char *emt_ieee1905(ieee1905_action_t ieee1905_action, char *args)
{
    switch (ieee1905_action)
    {
    case IEEE1905_PRINT:
        /* code */
        break;
    default:
        break;
    }
    return NULL;
}
char *emt_hy_manager(hy_manager_action_t hy_manager_action, char *args)
{
    switch (hy_manager_action)
    {
    case HY_MANAGER_PRIND_HD:
        /* code */
        break;
    case HY_MANAGER_PRIND_HA:
        /* code */
        break;
    case HY_MANAGER_FLUSH_HD:
        /* code */
        break;

    case HY_MANAGER_FLUSH_HA:
        /* code */
        break;

    default:
        break;
    }
    return NULL;
}
char *emt_wlan_manager(wlan_manager_action_t wlan_manager_action, char *args)
{
    switch (wlan_manager_action)
    {
    case WLAN_MANAGER_PRINT:
        /* code */
        break;
    case WLAN_MANAGER_FREQ:
        /* code */
        break;
    default:
        break;
    }
    return NULL;
}
char *emt_mc_manager(mc_manager_action_t mc_manager_action, char *args)
{
    switch (mc_manager_action)
    {
    case MC_MANAGER_PRINT_ENCAP:
        /* code */
        break;
    case MC_MANAGER_PRINT_FLOOD_TABLE:
        /* code */
        break;
    case MC_MANAGER_PRINT_INTERFACE:
        /* code */
        break;
    default:
        break;
    }
    return NULL;
}
// char *emt_multi_ap(mullti)
char *emt_service_priority(service_priority_action_t service_priority_action, char *args)
{
    switch (service_priority_action)
    {
    case SERVICE_PRIORITY_DISPLAY_SERVICE:
        /* code */
        break;
    case SERVICE_PRIORITY_BSP:
        /* code */
        break;
    case SERVICE_PRIORITY_ESP:
        /* code */
        break;
    case SERVICE_PRIORITY_QSP:
        /* code */
        break;
    case SERVICE_PRIORITY_RM_SERVICE_PRIORITY:
        /* code */
        break;

    case SERVICE_PRIORITY_PUSH_ALL_RULE_TO_AGENT:
        /* code */
        break;
    default:
        break;
    }
    return NULL;
}
device_info_t *emt_topology(topology_action_t topology_action, char *args, int *nb_devices)
{
    char *cmd = create_cmd(SECTION_TOPOLOGY, topology_action, args);
    char *response = emt_send_cmd(cmd);
    log_debug("emt reponse length data: %d", strlen(response));
    device_info_t *device_list = topology_handler(response, nb_devices);
    free(response);
    free(cmd);
    return device_list;
}
char *emt_dispatch_service(dispatch_service_action_t dispatch_service_action, char *args)
{
    switch (dispatch_service_action)
    {
    case DISPATCH_SERVICE_PRINT:
        /* code */
        break;
    default:
        break;
    }
    return NULL;
}
char *emt_dbg_service(dbg_service_action_t dbg_service_action, char *args)
{
    switch (dbg_service_action)
    {
    case DBG_SERVICE_LIST_ALL_EVENTS:
        /* code */
        break;

    case DBG_SERVICE_CREATE_EVENT:
        /* code */
        break;
    default:
        break;
    }
    return NULL;
}
char *emt_stadb(stadb_action_t stadb_action, char *args)
{
    switch (stadb_action)
    {
    case STADB_PRINT:
        /* code */
        break;
    case STADB_ENA_DIS:
        /* code */
        break;
    case STADB_RSSI:
        /* code */
        break;
    case STADB_ACT:
        /* code */
        break;
    case STADB_NOSTEER:
        /* code */
        break;
    case STADB_NOSTEER_LIST_RELOAD:
        /* code */
        break;
    case STADB_NOSTEER_LIST_SHOW:
        /* code */
        break;
    case STADB_DIALOG:
        /* code */
        break;
    default:
        break;
    }
    return NULL;
}
char *emt_map_ctrl(mapctrl_action_t mapctrl_action, char *args)
{
    switch (mapctrl_action)
    {
    case MAP_CTRL_PRINT_SUMMARY:
        /* code */
        break;
    case MAP_CTRL_PRINT_NEIGH_REPORT:
        /* code */
        break;
    default:
        break;
    }
    return NULL;
}
char *emt_pc_service(pc_service_action_t pc_service_action, char *args)
{
    switch (pc_service_action)
    {
    case PC_SERVICE_PRINT:
        /* code */
        break;
    case PC_SERVICE_PARAM_ACCESS:
        /* code */
        break;
    case PC_SERVICE_TRIGGER_STAT:
        /* code */
        break;
    case PC_SERVICE_SET_DEBUG:
        /* code */
        break;
    case PC_SERVICE_SET_MEDIUM_ULTILIZATION:
        /* code */
        break;
    case PC_SERVICE_SET_LINK_CAPACITY:
        /* code */
        break;
    case PC_SERVICE_PRINT_MODULE_METRIC:
        /* code */
        break;
    default:
        break;
    }
    return NULL;
}
char *emt_steer_exec(steer_exec_action_t steer_exec_action, char *args)
{
    switch (steer_exec_action)
    {
    case STEER_EXEC_PRINT:
        /* code */
        break;
    case STEER_EXEC_STEER:
        /* code */
        break;
    case STEER_EXEC_ALLOW_ASSOC:
        /* code */
        break;
    case STEER_EXEC_ABORT:
        /* code */
        break;
    case STEER_EXEC_DIALOG:
        /* code */
        break;

    default:
        break;
    }
    return NULL;
}
char *emt_band_mon(band_mon_action_t band_mon_action, char *args)
{
    switch (band_mon_action)
    {
    case BAND_MON_PRINT:
        /* code */
        break;
    case BAND_MON_ENA_DIS_MONITOR:
        /* code */
        break;
    case BAND_MON_UTIL:
        /* code */
        break;
    default:
        break;
    }
    return NULL;
}
char *emt_estimator(estimator_action_t estimator_action, char *args)
{
    switch (estimator_action)
    {
    case ESTIMATOR_PRINT:
        /* code */
        break;
    case ESTIMATOR_RATE:
        /* code */
        break;
    case ESTIMATOR_AIRTIME:
        /* code */
        break;
    case ESTIMATOR_ENA_DIS:
        /* code */
        break;
    case ESTIMATOR_DETECT:
        /* code */
        break;
    case ESTIMATOR_STATS:
        /* code */
        break;
    case ESTIMATOR_DIALOG:
        /* code */
        break;
    default:
        break;
    }
    return NULL;
}
char *emt_steer_alg(steeralg_action_t steeralg_action, char *args)
{
    switch (steeralg_action)
    {
    case STEER_ALG_PRINT_ALG:
        /* code */
        break;
    case STEER_ALG_PRINT_PATH_CAPACITY:
        /* code */
        break;
    default:
        break;
    }
    return NULL;
}

char *emt_send_cmd(char *cmd)
{
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, "telnet://localhost:7777");

        curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            log_error("curl_easy_perform() failed: %s\n",
                      curl_easy_strerror(res));
        else
        {
            // log_info("curl_easy_perform() successed\n");
        }

        strcpy(data_send, "\n");
        res = curl_easy_send(curl, data_send, strlen(data_send), &send_size);
        /* Check for errors */
        if (res != CURLE_OK)
            log_error("curl_easy_send() failed: %s\n",
                      curl_easy_strerror(res));
        else
        {
            // log_info("curl_easy_send() successed with size %d: \n%s\n", send_size, data_send);
        }
        memset(data_send, 0, sizeof(data_send));
        msleep(10);
        /* always cleanup */
        res = curl_easy_recv(curl, data_recv, sizeof(data_recv), &recv_size);
        /* Check for errors */
        if (res != CURLE_OK)
            log_error("curl_easy_recv() failed: %s\n",
                      curl_easy_strerror(res));
        else
        {
            // log_info("curl_easy_recv() successed with size %d: \n%s\n", recv_size, data_recv);
        }
        memset(data_recv, 0, sizeof(data_recv));

        strcpy(data_send, cmd);
        res = curl_easy_send(curl, data_send, strlen(data_send), &send_size);
        /* Check for errors */
        if (res != CURLE_OK)
            log_error("curl_easy_send() failed: %s\n",
                      curl_easy_strerror(res));
        else
        {
            // log_info("curl_easy_send() successed with size %d: \n%s\n", send_size, data_send);
        }
        msleep(1000);
        res = curl_easy_recv(curl, data_recv, sizeof(data_recv), &recv_size);
        /* Check for errors */
        if (res != CURLE_OK)
            log_error("curl_easy_recv() failed: %s\n",
                      curl_easy_strerror(res));
        else
        {
            // log_info("curl_easy_recv() successed with size %d\n", recv_size);
        }
        curl_easy_cleanup(curl);
    }
    char *response = (char *)calloc(recv_size + 1, sizeof(char));
    memcpy(response, data_recv, recv_size);
    response[recv_size] = "\0";
    memset(data_recv, 0, sizeof(data_recv));
    memset(data_send, 0, sizeof(data_send));

    // log_info("1");
    return response;
}

int ezm_deamon_is_running()
{
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, "telnet://localhost:7777");
        curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        // log_info("CURL Code: %d", res);
        if (res != CURLE_OK)
        {
            log_error("Cannot connect to ezmesh deamon");
            return -1;
        }
        else
        {
            log_info("Connected to ezmesh deamon");
        }
    }
    return 0;
}