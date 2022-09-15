#include "quality.h"
#include "utils/log/log.h"

static char *mock_quality_profile = R"(
[
    {
      "src_link": "NovaController",
      "quality": [
        {
          "dest_link": "NovaAgent1",
          "channel": 1,
          "up_link": true,
          "downlink": false,
          "tx_link": 12,
          "snr": 13,
          "noise_floor": 11,
          "tx_total": 12,
          "rx_total": 3
        },
        {
          "dest_link": "NovaAgent1",
          "channel": 1,
          "up_link": true,
          "downlink": false,
          "tx_link": 12,
          "snr": 13,
          "noise_floor": 11,
          "tx_total": 12,
          "rx_total": 3
        }
      ]
    }
]
)";

cJSON *cvt_quality_to_json(quality_t *quality)
{
    cJSON *quality_json = cJSON_CreateObject();
    cJSON_AddStringToObject(quality_json, "dest_link", quality->dest_link);
    cJSON_AddNumberToObject(quality_json, "channel", quality->channel);
    cJSON_AddBoolToObject(quality_json, "up_link", quality->up_link);
    cJSON_AddBoolToObject(quality_json, "down_link", quality->down_link);
    cJSON_AddNumberToObject(quality_json, "tx_link", quality->tx_link);
    cJSON_AddNumberToObject(quality_json, "snr", quality->snr);
    cJSON_AddNumberToObject(quality_json, "noise_floor", quality->noise_floor);
    cJSON_AddNumberToObject(quality_json, "tx_total", quality->tx_total);
    cJSON_AddNumberToObject(quality_json, "rx_total", quality->rx_total);
    return quality_json;
}

quality_t *cvt_json_to_quality(cJSON *json)
{
    quality_t *quality = (quality_t *)calloc(1, sizeof(quality_t));

    cJSON *dest_link_json = cJSON_GetObjectItemCaseSensitive(json, "dest_link");
    if (dest_link_json != NULL && cJSON_IsString(dest_link_json))
    {
        quality->dest_link = cJSON_GetStringValue(dest_link_json);
    }
    cJSON *channel_json = cJSON_GetObjectItemCaseSensitive(json, "channel");
    if (channel_json != NULL && cJSON_IsNumber(channel_json))
    {
        quality->channel = channel_json->valueint;
    }
    cJSON *up_link_json = cJSON_GetObjectItemCaseSensitive(json, "up_link");
    if (up_link_json != NULL)
    {
        if (cJSON_IsTrue(up_link_json))
        {
            quality->up_link = true;
        }
        else
        {
            quality->up_link = false;
        }
    }
    cJSON *down_link_json = cJSON_GetObjectItemCaseSensitive(json, "down_link");
    if (down_link_json != NULL)
    {
        if (cJSON_IsTrue(down_link_json))
        {
            quality->down_link = true;
        }
        else
        {
            quality->down_link = false;
        }
    }
    cJSON *tx_link_json = cJSON_GetObjectItemCaseSensitive(json, "tx_link");
    if (tx_link_json != NULL && cJSON_IsNumber(tx_link_json))
    {
        quality->tx_link = tx_link_json->valueint;
    }
    cJSON *snr_json = cJSON_GetObjectItemCaseSensitive(json, "snr");
    if (snr_json != NULL && cJSON_IsNumber(snr_json))
    {
        quality->snr = snr_json->valueint;
    }
    cJSON *noise_floor_json = cJSON_GetObjectItemCaseSensitive(json, "noise_floor");
    if (noise_floor_json != NULL && cJSON_IsNumber(noise_floor_json))
    {
        quality->noise_floor = noise_floor_json->valueint;
    }
    cJSON *tx_total_json = cJSON_GetObjectItemCaseSensitive(json, "tx_total");
    if (tx_total_json != NULL && cJSON_IsNumber(tx_total_json))
    {
        quality->tx_total = tx_total_json->valueint;
    }
    cJSON *rx_total_json = cJSON_GetObjectItemCaseSensitive(json, "rx_total");
    if (rx_total_json != NULL && cJSON_IsNumber(rx_total_json))
    {
        quality->rx_total = rx_total_json->valueint;
    }
    return quality;
}

cJSON *cvt_quality_profile_to_json(quality_profile_t *quality_profile)
{
    cJSON *quality_profile_json = cJSON_CreateObject();
    log_info("Test: %s", quality_profile->src_link);
    cJSON_AddStringToObject(quality_profile_json, "src_link", quality_profile->src_link);
    if (quality_profile->nb_quality)
    {
        cJSON *quality_list_json = cJSON_CreateArray();
        for (size_t i = 0; i < quality_profile->nb_quality; i++)
        {
            /* code */
            // log_info("1");
            cJSON *quality_json = cvt_quality_to_json(&(quality_profile->quality_list[i]));
            cJSON_AddItemToArray(quality_list_json, quality_json);
        }
        cJSON_AddItemToObject(quality_profile_json, "quality", quality_list_json);
    }
    return quality_profile_json;
}
quality_profile_t *cvt_json_to_quality_profile(cJSON *json)
{
    quality_profile_t *quality_profile = (quality_profile_t *)calloc(1, sizeof(quality_profile_t));
    cJSON *src_link_json = cJSON_GetObjectItemCaseSensitive(json, "src_link");
    log_info("Getin");
    if (src_link_json != NULL && cJSON_IsString(src_link_json))
    {
        quality_profile->src_link = cJSON_GetStringValue(src_link_json);
        log_info("124: %s", quality_profile->src_link);
    }
    cJSON *quality_json = cJSON_GetObjectItemCaseSensitive(json, "quality");
    if (quality_json != NULL && cJSON_IsArray(quality_json))
    {
        int nb_quality = cJSON_GetArraySize(quality_json);
        quality_t *quality_list = (quality_t *)calloc(nb_quality, sizeof(quality_t));
        for (size_t i = 0; i < nb_quality; i++)
        {
            /* code */
            quality_list[i] = *cvt_json_to_quality(cJSON_GetArrayItem(quality_json, i));
        }
        quality_profile->quality_list = quality_list;
        quality_profile->nb_quality = nb_quality;
    }
    return quality_profile;
}

cJSON *cvt_quality_profile_list_to_json(quality_profile_list_t *quality_profile_list)
{
    cJSON *cvt_quality_profile_list_json = cJSON_CreateArray();
    if (quality_profile_list->nb_quality_profiles)
    {
        for (size_t i = 0; i < quality_profile_list->nb_quality_profiles; i++)
        {
            cJSON *quality_profile_json = cvt_quality_profile_to_json(&(quality_profile_list->quality_profile_list[i]));
            cJSON_AddItemToArray(cvt_quality_profile_list_json, quality_profile_json);
        }
    }
    return cvt_quality_profile_list_json;
}

quality_profile_list_t *cvt_json_to_quality_profile_list(cJSON *json)
{
    quality_profile_list_t *quality_profile_list = (quality_profile_list_t *)calloc(1, sizeof(quality_profile_list_t));
    int nb_quality_profiles = cJSON_GetArraySize(json);
    quality_profile_t *quality_profile = (quality_profile_t *)calloc(nb_quality_profiles, sizeof(quality_profile_t));
    for (size_t i = 0; i < nb_quality_profiles; i++)
    {
        quality_profile[i] = *cvt_json_to_quality_profile(cJSON_GetArrayItem(json, i));
    }
    quality_profile_list->quality_profile_list = quality_profile;
    quality_profile_list->nb_quality_profiles = nb_quality_profiles;
    return quality_profile_list;
}

quality_profile_list_t *get_mesh_quality()
{
    quality_profile_list_t *quality_profile_list;
    cJSON *quality_profile_list_json = cJSON_Parse(mock_quality_profile);
    quality_profile_list = cvt_json_to_quality_profile_list(quality_profile_list_json);
    return quality_profile_list;
}

void quality_free(quality_t *quality)
{
    if (quality)
    {
        app_free(quality->dest_link);
    }
}

void quality_profile_free(quality_profile_t *quality_profile)
{
    if (quality_profile)
    {
        app_free(quality_profile->src_link);
        for (size_t i = 0; i < quality_profile->nb_quality; i++)
        {
            /* code */
            quality_free(&quality_profile->quality_list[i]);
        }
        app_free(quality_profile->quality_list);
    }
}

void quality_profile_list_free(quality_profile_list_t *quality_profile_list)
{
    if (quality_profile_list)
    {
        for (size_t i = 0; i < quality_profile_list->nb_quality_profiles; i++)
        {
            quality_profile_free(&quality_profile_list->quality_profile_list[i]);
        }
        app_free(quality_profile_list->quality_profile_list);
        app_free(quality_profile_list);
    }
}
