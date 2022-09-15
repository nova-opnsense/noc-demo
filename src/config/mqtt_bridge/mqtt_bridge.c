#include "mqtt_bridge.h"
#include "utils/file/file.h"
#include "utils/log/log.h"
#include "utils/cmd/cmd.h"
#include "utils/split/split.h"
#include "config/service_controller/service_controller.h"

static char *get_listener_ip_default_port();
static char *get_bridge_client_connection_ip();

mqtt_bridge_ret_t create_mqtt_bridge_server()
{
    if (check_file_exist(DEFAULT_MOSQUITTO_FILE))
    {
        char *ip = get_listener_ip_default_port();
        if (ip)
        {
            // log_info("1");
            return MOSQUITTO_SERVER_BRIDGE_EXISTED;
        }
        else
        {
            // log_info("1");
            //  create password file
            char create_password_cmd[200];
            sprintf(create_password_cmd, "touch  %s ", DEFAULT_MOSQUITTO_PASSWORD_FILE);
            cmd_ret_t ret = execl_timeout(120000, create_password_cmd);
            free(ret.str);
            sprintf(create_password_cmd, "mosquitto_passwd -b %s  %s  %s", DEFAULT_MOSQUITTO_PASSWORD_FILE, DEFAULT_MOSQUITTO_USERNAME, DEFAULT_MOSQUITTO_PASSWD);
            ret = execl_timeout(2000, create_password_cmd);
            free(ret.str);

            char mqtt_bridge_conf[200];
            sprintf(mqtt_bridge_conf, "\nlistener %s %s\n"
                                      "allow_anonymous false\n"
                                      "password_file %s\n ",
                    DEFAULT_MOSQUITTO_PORT_CONTROLLER, DEFAULT_MOSQUITTO_IP_CONTROLLER, DEFAULT_MOSQUITTO_PASSWORD_FILE);
            write_data(mqtt_bridge_conf, strlen(mqtt_bridge_conf), DEFAULT_MOSQUITTO_FILE, APPEND);
            return MOSQUITTO_SUCCESS;
        }
    }
    else
    {
        log_error("Cannot read %s file", DEFAULT_MOSQUITTO_FILE);
    }
    return MOSQUITTO_FILE_NOT_FOUND;
}

// Create Bridge to MQTT Broker IP
mqtt_bridge_ret_t create_mqtt_bridge_client()
{
    if (!get_bridge_client_connection_ip())
    {
        // log_info("1");
        char mosquitto_client_br_conf[1024];
        sprintf(mosquitto_client_br_conf, "\nconnection mqtt-bridge \n"
                                          "address %s:%s \n"
                                          "topic ezm/# both 1 \n"
                                          "topic hub/# both 1 \n"
                                          "topic zib/# both 1 \n"
                                          "topic zwa/# both 1 \n"
                                          "try_private false \n"
                                          "remote_username %s  \n"
                                          "remote_password %s  \n",
                DEFAULT_MOSQUITTO_IP_CONTROLLER, DEFAULT_MOSQUITTO_PORT_CONTROLLER, DEFAULT_MOSQUITTO_USERNAME, DEFAULT_MOSQUITTO_PASSWD);
        log_info("%s", mosquitto_client_br_conf);
        write_data(mosquitto_client_br_conf, strlen(mosquitto_client_br_conf), DEFAULT_MOSQUITTO_FILE, APPEND);
        // log_info("1");
        return MOSQUITTO_SUCCESS;
    }
    else
    {
        // log_info("1");
        return MOSQUITTO_CLIENT_BRIDGE_EXISTED;
    }
}

// If dont have bridge before so return ip NULL and port 0 and
char *request_mqtt_bridge_client()
{
    return get_bridge_client_connection_ip();
}

// If dont have bridge before so return ip NULL and port 0 and
char *request_mqtt_bridge_server()
{
    return get_listener_ip_default_port();
}

// Return 0 if bridge is up and
mqtt_bridge_ret_t update_mqtt_bridge_client()
{
    // Delete previout bridge client config
    delete_mqtt_bridge_client();
    // Create new bridge client config
    return create_mqtt_bridge_client();
}

mqtt_bridge_ret_t update_mqtt_bridge_server()
{
    delete_mqtt_bridge_server();
    // Create new bridge client config
    return create_mqtt_bridge_server();
}

mqtt_bridge_ret_t delete_mqtt_bridge_client()
{
    int n_line;
    char **mosquitto_conf = read_line(DEFAULT_MOSQUITTO_FILE, &n_line);
    for (size_t i = 0; i < n_line; i++)
    {
        /* code */
        if (mosquitto_conf[i][0] != '#' && app_strstr(mosquitto_conf[i], "connection"))
        {
            // Get IP
            char cmd[200];
            sprintf(cmd, "rm %s", DEFAULT_MOSQUITTO_FILE);
            cmd_ret_t ret = execl_timeout(120000, cmd);
            free(ret.str);
            FILE *fp = fopen(DEFAULT_MOSQUITTO_FILE, "a+");
            for (size_t j = 0; j < i; j++)
            {
                /* code */
                fwrite(mosquitto_conf[j], 1, strlen(mosquitto_conf[j]), fp);
                fwrite("\n", 1, 1, fp);
            }
            fclose(fp);
            return MOSQUITTO_SUCCESS;
        }
    }
    return MOSQUITTO_CLIENT_BRIDGE_NOT_FOUND;
}

mqtt_bridge_ret_t delete_mqtt_bridge_server()
{
    int n_line;
    char **mosquitto_conf = read_line(DEFAULT_MOSQUITTO_FILE, &n_line);
    char tmp[200];
    sprintf(tmp, "%s %s", "listener", DEFAULT_MOSQUITTO_PORT_CONTROLLER);
    for (size_t i = 0; i < n_line; i++)
    {
        /* code */
        if (mosquitto_conf[i][0] != '#' && app_strstr(mosquitto_conf[i], tmp))
        {
            // Get IP
            char cmd[200];
            sprintf(cmd, "rm %s", DEFAULT_MOSQUITTO_FILE);
            cmd_ret_t ret = execl_timeout(120000, cmd);
            free(ret.str);
            FILE *fp = fopen(DEFAULT_MOSQUITTO_FILE, "a+");
            for (size_t j = 0; j < i; j++)
            {
                /* code */
                fwrite(mosquitto_conf[j], 1, strlen(mosquitto_conf[j]), fp);
                fwrite("\n", 1, 1, fp);
            }
            fclose(fp);
            return MOSQUITTO_SUCCESS;
        }
    }
    return MOSQUITTO_CLIENT_BRIDGE_NOT_FOUND;
}

void restart_mqtt_service()
{
    restart_service("mosquitto");
}

// Get ip of port 1884
static char *get_listener_ip_default_port()
{
    int n_line;
    char **mosquitto_conf = read_line(DEFAULT_MOSQUITTO_FILE, &n_line);
    char tmp[200];
    sprintf(tmp, "%s %s", "listener", DEFAULT_MOSQUITTO_PORT_CONTROLLER);
    for (size_t i = 0; i < n_line; i++)
    {
        /* code */
        if (mosquitto_conf[i][0] != '#' && app_strstr(mosquitto_conf[i], tmp))
        {
            // Get IP
            // log_info("1");
            int n_split_ele;
            char **listener_info = split(mosquitto_conf[i], " ", &n_split_ele);
            char *listener = app_strdup(listener_info[n_split_ele - 1]);
            log_info("%s", listener);
            return listener;
        }
    }
    return NULL;
}

// Get ip of port 1884
static char *get_bridge_client_connection_ip()
{
    int n_line;
    char **mosquitto_conf = read_line(DEFAULT_MOSQUITTO_FILE, &n_line);
    for (size_t i = 0; i < n_line; i++)
    {
        /* code */
        if (mosquitto_conf[i][0] != '#' && app_strstr(mosquitto_conf[i], "address"))
        {
            // Get IP
            // log_info("1");
            int n_splits;
            char **broker_info = split(mosquitto_conf[i], " ", &n_splits);
            if (broker_info && n_splits >= 1)
            {
                // log_info("1");
                int nb_splits_ip_port;
                char **ip_port_info = split(broker_info[1], ":", &nb_splits_ip_port);
                char *ip_port = app_strdup(ip_port_info[0]);
                log_info("%s", ip_port);
                return ip_port;
            }
        }
    }
    return NULL;
}