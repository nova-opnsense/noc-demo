#include "mqtt_handler.h"
#include "utils/log/log.h"
#include "utils/cmd/cmd.h"
#include "utils/file/file.h"
#include "config/service_info/service_info.h"
#include "stdlib.h"
#include "pthread.h"

static char SUB_TOPIC[100];
static char PUB_TOPIC[100];
static struct mosquitto *mosq_handler;

extern void mqtt_on_message_cb_handler(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg);

static void mqtt_on_connect_cb_handler(struct mosquitto *mosq, void *obj, int reason_code)
{
    if (reason_code != 0)
    {
        /* If the connection fails for any reason, we don't want to keep on
         * retrying in this example, so disconnect. Without this, the client
         * will attempt to reconnect. */
        log_error("on_connect: %s\n", mosquitto_connack_string(reason_code));
        mosquitto_disconnect(mosq);
    }
    else
    {
        log_info("on_connect: %s\n", mosquitto_connack_string(reason_code));
        int rc = mosquitto_subscribe(mosq, NULL, SUB_TOPIC, 1);
        if (rc != MOSQ_ERR_SUCCESS)
        {
            log_error("Error subscribing: %s\n", mosquitto_strerror(rc));
            /* We might as well disconnect if we were unable to subscribe */
            mosquitto_disconnect(mosq);
        }
        else
        {
            log_info("Subcribed to : %s\n", SUB_TOPIC);
        }
    }
}
static void mqtt_on_disconnect_cb_handler(struct mosquitto *mosq, void *obj, int reason_code)
{
    if (reason_code != 0)
    {
        /* If the connection fails for any reason, we don't want to keep on
         * retrying in this example, so disconnect. Without this, the client
         * will attempt to reconnect. */
        log_error("on_disconnect: %s\n", mosquitto_connack_string(reason_code));
    }
    // exit(1);
}
static void mqtt_on_publish_cb_handler(struct mosquitto *mosq, void *obj, int mesId)
{
    log_info("Message with mid %d has been published.\n", mesId);
}

static void mqtt_on_subcribe_cb_handler(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos)
{
    int i;
    bool have_subscription = false;

    /* In this example we only subscribe to a single topic at once, but a
     * SUBSCRIBE can contain many topics at once, so this is one way to check
     * them all. */
    for (i = 0; i < qos_count; i++)
    {
        log_info("on_subscribe: %d:granted qos = %d\n", i, granted_qos[i]);
        if (granted_qos[i] <= 2)
        {
            have_subscription = true;
        }
    }
    if (have_subscription == false)
    {
        /* The broker rejected all of our subscriptions, we know we only sent
         * the one SUBSCRIBE, so there is no point remaining connected. */
        log_error("Error: All subscriptions rejected.\n");
        mosquitto_disconnect(mosq);
    }
}

int mqtt_connect(const char *host, int port, int keep_alive)
{
    sprintf(SUB_TOPIC, get_config().sub_topic_pattern, get_ezm_id());
    sprintf(PUB_TOPIC, get_config().pub_topic_pattern, get_ezm_id());
    mosq_handler = mosquitto_new(NULL, true, NULL);
    if (mosq_handler == NULL)
    {
        log_error("Error: Out of memory.\n");
        return -1;
    }
    mosquitto_connect_callback_set(mosq_handler, mqtt_on_connect_cb_handler);
    mosquitto_disconnect_callback_set(mosq_handler, mqtt_on_disconnect_cb_handler);
    mosquitto_publish_callback_set(mosq_handler, mqtt_on_publish_cb_handler);
    mosquitto_subscribe_callback_set(mosq_handler, mqtt_on_subcribe_cb_handler);
    mosquitto_message_callback_set(mosq_handler, mqtt_on_message_cb_handler);

    int rc = mosquitto_connect(mosq_handler, host, port, keep_alive);
    if (rc != MOSQ_ERR_SUCCESS)
    {
        mosquitto_destroy(mosq_handler);
        log_error("Error: Connect to %s:%d %s\n", host, port, mosquitto_strerror(rc));
        return -1;
    }
    return 0;
}
void mqtt_loop()
{
    int rc = mosquitto_loop_start(mosq_handler);
    if (rc != MOSQ_ERR_SUCCESS)
    {
        log_error("Error: %s\n", mosquitto_strerror(rc));
    }
}

void mqtt_publish_message(msg_respond_t *msg_respond, msg_type_t msg_type, int retain)
{
    cJSON *msg_respond_json = cvt_msg_respond_to_json(msg_respond, msg_type);
    char *msg_str = cJSON_Print(msg_respond_json);
    cJSON_Minify(msg_str);
    log_info("%s", msg_str);
    int ret = mosquitto_publish(mosq_handler, NULL, PUB_TOPIC, strlen(msg_str), msg_str, 1, retain);
    if (ret != MOSQ_ERR_SUCCESS)
    {
        log_error("Error publishing: %s\n", mosquitto_strerror(ret));
    }
    cJSON_Delete(msg_respond_json);
    free(msg_str);
}

int mqtt_publish_message_to_mqtt_bridge(char *ezm_id, msg_request_t *msg_request, msg_type_t msg_type, int retain)
{
    cJSON *msg_request_json = cvt_msg_request_to_json(msg_request, msg_type);
    char *msg_str = cJSON_Print(msg_request_json);
    cJSON_Minify(msg_str);
    log_info("%s", msg_str);
    // Gen topic
    char topic[100];
    sprintf(topic, get_config().sub_topic_pattern, ezm_id);
    // log_debug("1");
    int ret = mosquitto_publish(mosq_handler, NULL, topic, strlen(msg_str), msg_str, 1, retain);
    // log_debug("1");
    if (ret != MOSQ_ERR_SUCCESS)
    {
        log_error("Error publishing: %s\n", mosquitto_strerror(ret));
    }
    return 0;
}

int mqtt_publish_message_to_external(char *host, char *ezm_id, msg_request_t *msg_request, msg_type_t msg_type, int retain)
{
    cJSON *msg_request_json = cvt_msg_request_to_json(msg_request, msg_type);
    char *msg_str = cJSON_Print(msg_request_json);
    cJSON_Minify(msg_str);
    log_info("%s", msg_str);
    // Create CAfile name with ezm_id
    char ca_file_name[1024], authen_dir[1024], secret_file_name[1024], cmd[4096];
    sprintf(authen_dir, AUTHEN_DIR, ezm_id);
    sprintf(ca_file_name, CAFILE, ezm_id);
    sprintf(secret_file_name, SECRET, ezm_id);
    // Get Cafile , ID, SecretKey from External Authen Service
    // Fixed http port 8080
    char *secret;
    int mqtt_port = 8883; // Default Port
    authen_code_t ret_code = mqtt_get_authen_info(host, 8080, get_ezm_id(), authen_dir, ca_file_name, secret_file_name, &mqtt_port);

    if ((ret_code != NoError) && (ret_code != ServiceIsExisted))
    {
        log_error("Failed to get cafile and secret");
        return -1;
    }

    if (retain)
    {
        secret = read_data(secret_file_name);
        sprintf(cmd, "mosquitto_pub "
                     "-h %s"
                     " -p %d "
                     " --cafile %s"
                     " -t 'hub/event/%s' "
                     " -m '%s' "
                     " --insecure "
                     " -u %s "
                     " -P %s"
                     " -r",
                host, mqtt_port, ca_file_name, ezm_id, msg_str, get_ezm_id(), secret);
    }
    else
    {
        secret = read_data(secret_file_name);
        sprintf(cmd, "mosquitto_pub "
                     "-h %s"
                     " -p %d "
                     " --cafile %s"
                     " -t 'hub/event/%s' "
                     " -m '%s' "
                     " --insecure "
                     " -u %s "
                     " -P %s",
                host, mqtt_port, ca_file_name, ezm_id, msg_str, get_ezm_id(), secret);
    }
    log_info("%s", cmd);
    cmd_ret_t ret = execl_timeout(120000, cmd);
    free(ret.str);
    return 0;
}

authen_code_t mqtt_get_authen_info(char *host, int port, char *id_in, char *authen_dir, char *cafile_name, char *secret_file_name, int *mqtt_port_out)
{
    // Send Pair Event to RE via IP
    char data[4096];
    char *response;
    authen_code_t authen_ret = NoError;
    // 1. Get Token in Authen Service in Target IP
    // 1.1 Register Service
    log_info("Create Register Request");

    sprintf(data, R"({
                        "id": "%s" ,
                        "name": "%s",
                        "software_info": {
                            "version" : "%s"
                        },
                        "hardware_info": {}
                    })",
            id_in, "Ezm", EZM_VERSION_DEFAULT);
    cJSON_Minify(data);
    log_info("%s", data);
    register_client(host, port, data, strlen(data), &response);
    cJSON_Minify(response);
    log_info("%s", response);
    // 1.2 Login with Authen Info
    cJSON *register_response_json, *authen_info_json, *success_json, *id_json, *secret_json;
    char *id = id_in, *secret;
    register_response_json = cJSON_Parse(response);
    if (register_response_json)
    {
        success_json = cJSON_GetObjectItemCaseSensitive(register_response_json, "success");
        if (success_json && cJSON_IsTrue(success_json))
        {
            authen_info_json = cJSON_GetObjectItemCaseSensitive(register_response_json, "authen_info");
            if (authen_info_json)
            {
                id_json = cJSON_GetObjectItemCaseSensitive(authen_info_json, "id");
                secret_json = cJSON_GetObjectItemCaseSensitive(authen_info_json, "secret");
                id = cJSON_GetStringValue(id_json);
                secret = cJSON_GetStringValue(secret_json);
            }
        }
        else
        {
            cJSON *error_json = cJSON_GetObjectItemCaseSensitive(register_response_json, "error_info");
            if (error_json && cJSON_IsObject(error_json))
            {
                cJSON *message_json = cJSON_GetObjectItemCaseSensitive(error_json, "message");
                log_error(cJSON_GetStringValue(message_json));
                cJSON *code_json = cJSON_GetObjectItemCaseSensitive(error_json, "code");
                authen_ret = code_json->valueint;
            }
        }
    }
    else
    {
        log_error("Fail to register Service");
        authen_ret = ServiceIsNotAvailable;
        return authen_ret;
    }
    if (authen_ret == NoError)
    {
        // Write secret to file
        mkdir(authen_dir);
        write_data(secret, strlen(secret), secret_file_name, TRUNCATE);
    }
    else if (authen_ret == ServiceIsExisted)
    {
        // Read secret from file if It's existed
        secret = read_data(secret_file_name);
    }
    else
    {
        log_error("Fail to register Service");
        return authen_ret;
    }
    /* Login */
    memset(data, 0, 4096);
    sprintf(data, R"(
                        {
                            "id": "%s",
                            "secret": "%s"
                        }  
    )",
            id, secret);
    cJSON_Minify(data);
    log_info("%s", data);
    login_client(host, port, data, strlen(data), &response);
    cJSON_Minify(response);
    log_info("%s", response);
    // 1.3 Request MQTT Broker Info
    char *token = NULL;
    cJSON *login_response_json, *jwt_info_json, *token_json;
    login_response_json = cJSON_Parse(response);
    if (login_response_json)
    {
        success_json = cJSON_GetObjectItemCaseSensitive(login_response_json, "success");
        if (success_json && cJSON_IsTrue(success_json))
        {
            jwt_info_json = cJSON_GetObjectItemCaseSensitive(login_response_json, "jwt_info");
            if (jwt_info_json)
            {
                token_json = cJSON_GetObjectItemCaseSensitive(jwt_info_json, "token");
                token = cJSON_GetStringValue(token_json);
            }
        }
        else
        {
            cJSON *error_json = cJSON_GetObjectItemCaseSensitive(login_response_json, "error_info");
            if (error_json && cJSON_IsObject(error_json))
            {
                cJSON *message_json = cJSON_GetObjectItemCaseSensitive(error_json, "message");
                log_error(cJSON_GetStringValue(message_json));
                cJSON *code_json = cJSON_GetObjectItemCaseSensitive(error_json, "code");
                authen_ret = code_json->valueint;
            }
            return authen_ret;
        }
    }
    else
    {
        authen_ret = ServiceIsNotAvailable;
        log_error("Login Response Wrong Format");
        return authen_ret;
    }

    if (token)
    {
        log_info("Create Request Broker Info");
    }
    request_broker_info(host, port, token, &response);
    cJSON_Minify(response);
    log_info("%s", response);
    // 1.4 Connect with Broker Info
    char *cafile, *address;
    int mqtt_port;
    cJSON *response_json, *broker_info_json, *port_json, *ca_file_json, *address_json;
    response_json = cJSON_Parse(response);
    if (response_json)
    {
        success_json = cJSON_GetObjectItemCaseSensitive(response_json, "success");
        if (success_json && cJSON_IsTrue(success_json))
        {
            broker_info_json = cJSON_GetObjectItemCaseSensitive(response_json, "broker_info");
            if (broker_info_json)
            {
                port_json = cJSON_GetObjectItemCaseSensitive(broker_info_json, "port");
                ca_file_json = cJSON_GetObjectItemCaseSensitive(broker_info_json, "cafile");
                address_json = cJSON_GetObjectItemCaseSensitive(broker_info_json, "address");
                mqtt_port = port_json->valueint;
                cafile = cJSON_GetStringValue(ca_file_json);
                address = cJSON_GetStringValue(address_json);
                // Write CAfile to file
                write_data(cafile, strlen(cafile), cafile_name, TRUNCATE);
                // Assign Mqtt port
                *mqtt_port_out = mqtt_port;
            }
        }
        else
        {
            cJSON *error_json = cJSON_GetObjectItemCaseSensitive(register_response_json, "error_info");
            if (error_json && cJSON_IsObject(error_json))
            {
                cJSON *message_json = cJSON_GetObjectItemCaseSensitive(error_json, "message");
                log_error(cJSON_GetStringValue(message_json));
                cJSON *code_json = cJSON_GetObjectItemCaseSensitive(error_json, "code");
                authen_ret = code_json->valueint;
                return authen_ret;
            }
        }
    }
    else
    {
        authen_ret = ServiceIsNotAvailable;
        log_error("Request Broker Response Wrong Format");
        return authen_ret;
    }
    return authen_ret;
}

void mqtt_clean()
{
    mosquitto_lib_cleanup();
}