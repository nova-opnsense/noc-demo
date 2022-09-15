#pragma once
#include <cjson/cJSON.h>
#include "stddef.h"
#include "stdbool.h"
#include "core/network/wlan/wireless.h"

#define STANDALONE_MODE 0
#define AGENT_MODE 1
#define CONTROLLER_MODE 2

typedef struct
{
    int up;
    int down;
} rssi_threshhold_t;

typedef struct
{
    bool up;
    bool down;
} antenna_installed_t;

typedef struct
{
    char *mesh_id;
    char *mesh_name;
    char *type;
    rssi_threshhold_t *rssi_threshhold;
    antenna_installed_t *antenna_installed;
    char *backhaul_channel;
    int active_time_after_set;
    char *work_mode;
    int weight_mesh;
    char *mesh_link_connection;
    int minimum_mesh_link_snr;
    wireless_t *wireless;
} mesh_setting_t;

cJSON *cvt_rssi_threshhold_to_json(rssi_threshhold_t *rssi_threshhold);
cJSON *cvt_antenna_installed_to_json(antenna_installed_t *antenna_installed);
cJSON *cvt_mesh_setting_to_json(mesh_setting_t *mesh_data);

rssi_threshhold_t *cvt_json_to_rssi_threshhold(cJSON *json);
antenna_installed_t *cvt_json_to_antenna_installed(cJSON *json);
mesh_setting_t *cvt_json_to_mesh_setting(cJSON *json);

int get_work_mode();
char *get_work_mode_str();
int run_standalone_mode();
int run_agent_mode();
int run_controller_mode();

void mesh_setting_free(mesh_setting_t *mesh_data);
