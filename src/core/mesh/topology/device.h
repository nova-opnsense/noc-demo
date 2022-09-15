#pragma once
#include "stdint.h"
#include "stdio.h"
#include <cjson/cJSON.h>

#define DEVICE_OFF 0
#define DEVICE_ON 1
#define DEVICE_PAIRING 2
#define DEVICE_DELETED 3

typedef struct
{
    char *hub_id;
    char *ezm_id;
    char *ezm_version;
    char *ezm_name;
    char *role;
    char *mac;
    char *ip;
    char *parent_mac;
    char *channel_selection_5g;
    char *channel_selection_2g;
    int status;
} device_info_t;

typedef struct
{
    device_info_t *device_list;
    int nb_device;
} device_list_t;

cJSON *cvt_device_list_to_json(device_list_t *device_list);
device_list_t *cvt_json_to_device_list(cJSON *json);

cJSON *cvt_device_info_to_json(device_info_t *device);
device_info_t *cvt_json_to_device_info(cJSON *json);
device_info_t *get_current_device_info();

void device_info_free(device_info_t *device_info);
void device_list_free(device_list_t *device_list);