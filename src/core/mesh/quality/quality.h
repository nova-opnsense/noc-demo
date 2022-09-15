#pragma once
#include "stdint.h"
#include "stdbool.h"
#include <cjson/cJSON.h>

typedef struct
{
    char *dest_link;
    int channel;
    bool up_link;
    bool down_link;
    int tx_link;
    int snr;
    int noise_floor;
    int tx_total;
    int rx_total;
} quality_t;

typedef struct
{
    char *src_link;
    quality_t *quality_list;
    int nb_quality;
} quality_profile_t;

typedef struct
{
    quality_profile_t *quality_profile_list;
    int nb_quality_profiles;
} quality_profile_list_t;

cJSON *cvt_quality_to_json(quality_t *quality);

quality_t *cvt_json_to_quality(cJSON *json);

cJSON *cvt_quality_profile_to_json(quality_profile_t *quality_profile);

quality_profile_t *cvt_json_to_quality_profile(cJSON *json);

cJSON *cvt_quality_profile_list_to_json(quality_profile_list_t *quality_profile_list);

quality_profile_list_t *cvt_json_to_quality_profile_list(cJSON *json);

quality_profile_list_t *get_mesh_quality();

void quality_profile_list_free(quality_profile_list_t *);
void quality_profile_free(quality_profile_t *);