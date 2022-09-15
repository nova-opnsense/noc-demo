#include "cjson/cJSON.h"
#include "service_info.h"
#include "utils/log/log.h"
#include "config.h"
#include "utils/file/file.h"
#include "string.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>
#include <arpa/inet.h>

static cJSON *service_info_json = NULL;
static char *hub_id = NULL;
static char *ezm_id = NULL;

void init_service_info()
{
    char *service_info_char = read_data(get_config().service_info_file);
    if (!service_info_char)
    {
        log_error("Cannot Read file %s", get_config().service_info_file);
        exit(EXIT_FAILURE);
    }
    service_info_json = cJSON_Parse(service_info_char);
    if (!cJSON_IsArray(service_info_json))
    {
        log_error("%s: Wrong Format", get_config().service_info_file);
        exit(EXIT_FAILURE);
    }
    size_t nb_size = cJSON_GetArraySize(service_info_json);
    for (size_t i = 0; i < nb_size; i++)
    {
        /* code */
        cJSON *service = cJSON_GetArrayItem(service_info_json, i);
        // log_info("%s",cJSON_Print(service));
        if (service != NULL)
        {
            cJSON *name_json = cJSON_GetObjectItemCaseSensitive(service, "name");
            if (name_json != NULL && cJSON_IsString(name_json))
            {
                if (!strcmp("hub", cJSON_GetStringValue(name_json)))
                {
                    cJSON *hub_id_json = cJSON_GetObjectItemCaseSensitive(service, "id");
                    if (hub_id_json != NULL && cJSON_IsString(hub_id_json))
                    {
                        hub_id = cJSON_GetStringValue(hub_id_json);
                        log_info("hub_id: %s", hub_id);
                    }
                }
                else if (!strcmp("ezm", cJSON_GetStringValue(name_json)))
                {
                    cJSON *ezm_id_json = cJSON_GetObjectItemCaseSensitive(service, "id");
                    if (ezm_id_json != NULL && cJSON_IsString(ezm_id_json))
                    {
                        ezm_id = cJSON_GetStringValue(ezm_id_json);
                        log_info("ezm_id: %s", ezm_id);
                    }
                }
            }
        }
    }
}
char *get_hub_id()
{
    return hub_id;
}
char *get_ezm_id()
{
    return ezm_id;
}

char *get_interface_ip(char *wan_interface)
{
    int n;
    struct ifreq ifr;

    n = socket(AF_INET, SOCK_DGRAM, 0);
    // Type of address to retrieve - IPv4 IP address
    ifr.ifr_addr.sa_family = AF_INET;
    // Copy the interface name in the ifreq structure
    strncpy(ifr.ifr_name, wan_interface, IFNAMSIZ - 1);
    ioctl(n, SIOCGIFADDR, &ifr);
    close(n);
    // display result
    char *ip_addr = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
    log_debug("IP Address is %s - %s\n", wan_interface, ip_addr);
    return ip_addr;
}

char *get_interface_mac(char *wan_interface)
{
    char mac_path[200];
    sprintf(mac_path, "/sys/class/net/%s/address", wan_interface);
    int mac_line;
    char **mac = read_line(mac_path, &mac_line);
    return mac[0];
}