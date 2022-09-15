#include "error_code.h"
#include "utils/log/log.h"
#include "utils/file/file.h"
#include "utils/mem/mem.h"
#include "config.h"

static cJSON *general_error_tables;
static cJSON *general_mqtt_error_tables;
static cJSON *ezm_error_tables;

cJSON *get_error_json_from_error_table_json(cJSON *error_tables, int code)
{
    // log_info("code: %d",code);
    if (cJSON_IsArray(error_tables))
    {
        size_t error_table_size = cJSON_GetArraySize(error_tables);
        // log_info("%s",cJSON_Print(error_tables));
        for (size_t i = 0; i < error_table_size; i++)
        {
            cJSON *error_index_json = cJSON_GetArrayItem(error_tables, i);
            // log_info("%s",cJSON_Print(error_index_json));
            cJSON *code_json = cJSON_GetObjectItemCaseSensitive(error_index_json, "code");
            cJSON *message_json = cJSON_GetObjectItemCaseSensitive(error_index_json, "message");
            if (cJSON_IsNumber(code_json))
            {
                // log_info("index: %d , code = %d",i,code_json->valueint);
                if (code == code_json->valueint)
                {
                    // log_info("index_result: %d",i);
                    // log_info("message: %s",cJSON_GetStringValue(message_json));
                    return error_index_json;
                }
            }
            else
            {
                log_error("Code is not number");
            }
        }
    }
    return NULL;
}

cJSON *cvt_error_to_json(error_t *ezm_error)
{
    if (ezm_error == NULL)
    {
        return NULL;
    }
    cJSON *ezm_error_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(ezm_error_json, "code", ezm_error->code);
    cJSON_AddStringToObject(ezm_error_json, "message", ezm_error->message);
    return ezm_error_json;
}
error_t *cvt_json_to_error(cJSON *json)
{
    error_t *ezm_error = (error_t *)calloc(1, sizeof(error_t));
    cJSON *error_code_json = cJSON_GetObjectItemCaseSensitive(json, "code");
    if (error_code_json != NULL && cJSON_IsNumber(error_code_json))
    {
        ezm_error->code = error_code_json->valueint;
    }
    cJSON *message_json = cJSON_GetObjectItemCaseSensitive(json, "message");
    if (message_json != NULL && cJSON_IsString(message_json))
    {
        ezm_error->message = app_strdup(cJSON_GetStringValue(message_json));
    }
    return ezm_error;
}

void init_error_table()
{
    general_error_tables = load_json_from_file(get_config().general_error_table_file);
    char *general_error_tables_str = cJSON_Print(general_error_tables);
    cJSON_Minify(general_error_tables_str);
    log_info("%s", general_error_tables_str);
    general_mqtt_error_tables = load_json_from_file(get_config().general_mqtt_error_table_file);
    char *general_mqtt_error_tables_str = cJSON_Print(general_mqtt_error_tables);
    cJSON_Minify(general_mqtt_error_tables_str);
    log_info("%s", general_mqtt_error_tables_str);
    ezm_error_tables = load_json_from_file(get_config().ezm_error_table_file);
    log_info("%s", get_config().ezm_error_table_file);
    char *ezm_error_tables_str = cJSON_Print(ezm_error_tables);
    cJSON_Minify(ezm_error_tables_str);
    log_info("%s", ezm_error_tables_str);
}

error_t *error_table_lookup(int error_code)
{
    cJSON *error_json;
    error_t *error;
    error_json = get_error_json_from_error_table_json(general_error_tables, error_code);
    if (error_json != NULL)
    {
        error = cvt_json_to_error(error_json);
        return error;
    }
    error_json = get_error_json_from_error_table_json(general_mqtt_error_tables, error_code);
    if (error_json != NULL)
    {
        error = cvt_json_to_error(error_json);
        return error;
    }
    error_json = get_error_json_from_error_table_json(ezm_error_tables, error_code);
    if (error_json != NULL)
    {
        error = cvt_json_to_error(error_json);
        return error;
    }
}

void error_free(error_t *error)
{
    if (error == NULL)
    {
        return;
    }
    app_free(error->message);
    app_free(error);
}