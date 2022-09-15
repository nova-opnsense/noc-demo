#pragma once
#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"
#include "device.h"
#include "config.h"
#include "stdbool.h"

typedef enum
{
    MAC,
    IP,
    PARENT_MAC,
    CHANNEL_SELECTION_5G,
    CHANNEL_SELECTION_2G
} device_field_t;

typedef struct
{
    device_field_t field;
    char *pattern;
    char *start;
    char *end;
} pattern_t;

device_info_t *topology_handler(char *topology_str, int *number_device);

device_list_t *request_device_list_from_file();

device_list_t *request_device_list_from_file_by_status(int status);

void set_device_list_to_file(device_list_t *);

bool delete_device_list_from_file(device_list_t *);

void update_device_list_to_file_from_emt_device(device_list_t *);

bool update_device_list_to_file_from_web(device_list_t *);

device_info_t *get_device_from_file_by_mac(char *mac);

void device_info_copy(device_info_t *device_dest, device_info_t *device_src);
