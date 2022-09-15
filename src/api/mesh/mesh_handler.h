#ifndef __MESH_HANDLER_H__
#define __MESH_HANDLER_H__

#include <cjson/cJSON.h>
#include "core/mesh/topology/topology.h"
#include "core/mesh/setting/mesh_setting.h"

/* Mesh Settting API */

// Creat mesh and active HUB to CAP
void create_mesh(cJSON *msg_json);

// Request current mesh_setting
void request_mesh(cJSON *msg_json);

// Update mesh_setting in CAP
void update_mesh(cJSON *msg_json);

// Delete mesh and reactive HUB to AP Mode
void delete_mesh(cJSON *msg_json);

/* Mesh Device API */
// Get Topology device list
void request_mesh_devices(cJSON *msg_json);

// Request pair device with ip and active device to RE
void request_pair_mesh_devices(cJSON *msg_json);

// Get all device in network
void request_devices_available_for_pairing(cJSON *msg_json);

// Update device from mesh and send it to agent device
void update_mesh_devices(cJSON *msg_json);

// Remove device from mesh and reactive device to AP Mode
void delete_mesh_devices(cJSON *msg_json);

// Request activate RE
void request_activate_agent_device(cJSON *msg_json);

// Request inactivate
void request_inactivate_agent_device(cJSON *msg_json);

// Update Agent Device Info
void update_agent_device(cJSON *msg_json);

// Get Mesh Quality
void request_mesh_quality(cJSON *msg_json);

#endif