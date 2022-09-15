#ifndef __MQTT_BRIDGE_H__
#define __MQTT_BRIDGE_H__


#define DEFAULT_MOSQUITTO_IP_CONTROLLER        "192.168.123.1"
#define DEFAULT_MOSQUITTO_PORT_CONTROLLER     "1884"
#define DEFAULT_MOSQUITTO_FILE                "/etc/mosquitto/mosquitto.conf"
#define DEFAULT_MOSQUITTO_PASSWORD_FILE      "/etc/mosquitto/password.pwd"
#define DEFAULT_MOSQUITTO_USERNAME          "mqtt-bridge"
#define DEFAULT_MOSQUITTO_PASSWD            "novaintechs"


typedef enum{
    MOSQUITTO_FILE_NOT_FOUND,
    MOSQUITTO_SERVER_BRIDGE_NOT_FOUND,
    MOSQUITTO_SERVER_BRIDGE_EXISTED,
    MOSQUITTO_CLIENT_BRIDGE_EXISTED,
    MOSQUITTO_CLIENT_BRIDGE_NOT_FOUND,
    MOSQUITTO_SUCCESS
}mqtt_bridge_ret_t;




// Create Bridge to MQTT Broker IP
mqtt_bridge_ret_t create_mqtt_bridge_client();

// Create Bridge to MQTT Broker IP
mqtt_bridge_ret_t create_mqtt_bridge_server();

// If dont have bridge before so return ip NULL and port 0 and
char* request_mqtt_bridge_client();

// If dont have bridge before so return ip NULL and port 0 and
char* request_mqtt_bridge_server();

// Return 0 if bridge is up and 
mqtt_bridge_ret_t update_mqtt_bridge_client();

mqtt_bridge_ret_t update_mqtt_bridge_server();

mqtt_bridge_ret_t delete_mqtt_bridge_client();

mqtt_bridge_ret_t delete_mqtt_bridge_server();

void restart_mqtt_service();

#endif