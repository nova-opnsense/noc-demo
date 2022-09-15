#ifndef __SERVICE_CONTROLLER_H__
#define __SERVICE_CONTROLLER_H__



void start_service (char * service_name);
void stop_service (char * service_name);
void restart_service(char * service_name);
void disable_service(char * service_name);
void enable_service(char * service_name);


#endif