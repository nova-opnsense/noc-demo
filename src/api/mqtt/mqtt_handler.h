#ifndef __MQTT_HANDLE__
#define __MQTT_HANDLE__

#include "mosquitto.h"
#include "config.h"
#include "api/message/message_handler.h"


typedef enum{
    NoError = 0,
    ServiceIsExisted = 1,      // return when a service re-try to register
    ServiceInfoIsMissing = 2,  // return when id or name of service is missing
    AuthenInfoIsMissing = 3,   // return when id or secret is missing
    UnAuthorizedService = 4,   // return when providing wrong id & secret
    GenerateTokenFailed = 5,   // return when system fail to generate token
    InvalidAccessToken = 6,    // return when service provided invalid token
    InvalidRecoverInfo = 7,   // return when service provided invalid recover info
    InvalidHardwareInfo = 8,  // return when service provided invalid info
    ServiceIsNotAvailable = 9        // return when cannot request to authen service
}authen_code_t;

int mqtt_connect(const char *host, int port, int keep_alive);
void mqtt_loop();
void mqtt_clean();
void mqtt_publish_message(msg_respond_t * msg_respond, msg_type_t msg_type , int retain );
authen_code_t mqtt_get_authen_info(char * host , int port , char *id_in,  char * authen_dir,  char * cafile_name , char * secret_file_name , int *mqtt_port_out);
int mqtt_publish_message_to_mqtt_bridge(char* ezm_id,  msg_request_t * msg_request, msg_type_t msg_type , int retain );
int mqtt_publish_message_to_external(char *host , char* ezm_id , msg_request_t * msg_request, msg_type_t msg_type , int retain );

#endif