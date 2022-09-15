#pragma once 
#include "mdns/mdns.h"
#include "core/mesh/topology/device.h"

device_list_t * mdns_query_device_list(char * name , int timeout_second);
void mdns_create_publisher();
void mdns_remove_publisher(char * service_file);
