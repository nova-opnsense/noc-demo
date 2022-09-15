#pragma once
#include "stdlib.h"
#include <cjson/cJSON.h>

// [
//   {
//     "message": "Success",
//     "code": 0
//   },
//   {
//     "message": "Unknown",
//     "code": 100001
//   },
//   {
//     "message": "Setting failed",
//     "code": 100002
//   },
//   {
//     "message": "Interface convert failed",
//     "code": 100003
//   },
//   {
//     "message": "Internal Server Error",
//     "code": 100004
//   },
//   {
//     "message": "Unauthorized",
//     "code": 100005
//   },
//   {
//     "message": "ID Mismatched",
//     "code": 100006
//   }
// ]

// [
//   {
//     "message": "MQTT Connection Fail",
//     "code": 101000
//   }
// ]

// [
//   {
//     "message": "Wrong Format Message",
//     "code": 240001
//   },
//   {
//     "message": "WPS Fail",
//     "code": 240002
//   },
//   {
//     "message": "Device Not Found",
//     "code": 240003
//   },
//   {
//     "message": "Mesh is Setuped Before",
//     "code": 240004
//   },
//   {
//     "message": "Mesh not found",
//     "code": 240005
//   },
//   {
//     "message": "Request name invalid",
//     "code": 240006
//   },
//   {
//     "message": "Easymesh Deamon connect fail",
//     "code": 240007
//   }
// ]

typedef enum
{
    ERR_SUCCESS = 0,
    ERR_UNKNOWN = 100001,
    ERR_SETTING_FAILED = 100002,
    ERR_INTERFACE_CONVERT_FAILED = 100003,
    ERR_INTERNAL_SERVER = 100004,
    ERR_UNAUTHORIED = 100005,
    ERR_ID_MISMATCHED = 100006,
    ERR_MQTT_CONNECT_FAIL = 101000,
    ERR_WRONG_FORMAT_MESSAGE = 240001,
    ERR_TIMEOUT = 240002,
    ERR_DEVICE_NOT_FOUND = 240003,
    ERR_MESH_SETUPED_BEFORE = 240004,
    ERR_MESH_NOT_FOUND = 240005,
    ERR_REQUEST_INVALID = 240006,
    ERR_EZMESH_DEAMON_CONNECT_FAIL = 240007
} error_code_t;

typedef struct
{
    error_code_t code;
    char *message;
} error_t;

cJSON *get_error_json_from_error_table_json(cJSON *error_tables, int code);
cJSON *cvt_error_to_json(error_t *ezm_error);
error_t *cvt_json_to_error(cJSON *json);
void err_free(error_t *err);
void init_error_table();
error_t *error_table_lookup(int error_code);

void error_free(error_t *error);