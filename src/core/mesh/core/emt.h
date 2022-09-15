#include "stdlib.h"
#include "../topology/topology.h"
#include "emt_cmd.h"


char *emt_debug(debug_action_t debug_action, char* args);
char *emt_md(md_action_t md_action, char* args);
char *emt_ieee1905(ieee1905_action_t ieee1905_action, char* args);
char *emt_hy_manager(hy_manager_action_t hy_manager_action, char* args);
char *emt_wlan_manager(wlan_manager_action_t wlan_manager_action, char* args);
char *emt_mc_manager(mc_manager_action_t mc_manager_action, char* args);
// char *emt_multi_ap(mullti)
char *emt_service_priority(service_priority_action_t service_priority_action, char* args);
device_info_t *emt_topology(topology_action_t topology_action, char* args, int* nb_devices);
char *emt_dispatch_service(dispatch_service_action_t dispatch_service_action, char* args);
char *emt_dbg_service(dbg_service_action_t dbg_service_action, char* args);
char *emt_stadb(stadb_action_t stadb_action, char* args);
char *emt_map_ctrl(mapctrl_action_t mapctrl_action, char* args);
char *emt_pc_service(pc_service_action_t pc_service_action, char* args);
char *emt_steer_exec(steer_exec_action_t steer_exec_action, char* args);
char *emt_band_mon(band_mon_action_t band_mon_action, char* args);
char *emt_estimator(estimator_action_t estimator_action, char* args);
char *emt_steer_alg(steeralg_action_t steeralg_action, char* args);

void emt_connect(char *telnet_host , int telnet_port);
char*  emt_send_cmd(char *cmd);
int ezm_deamon_is_running();