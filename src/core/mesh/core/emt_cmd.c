#include "emt_cmd.h"

section_mapping_t section_mapping_cmd[] = {
    {SECTION_DEBUG, "dbg"},
    {SECTION_MD, "md"},
    {SECTION_EV_LOOP, "evloop"},
    {SECTION_IEEE1905, "ieee1905"},
    {SECTION_HY_MANAGER, "hy"},
    {SECTION_WLAN_MANAGER, "wlan"},
    {SECTION_MC_MANAGER, "mc"},
    {SECTION_MULTI_AP, "map"},
    {SECTION_SERVICE_PRIORITY, "sp"},
    {SECTION_TOPOLOGY, "td"},
    {SECTION_DISPATCH_SERVICE, "dispatchService"},
    {SECTION_DBG_SERVICE, "dbgService"},
    {SECTION_STADB, "stadb"},
    {SECTION_MAP_CTRL, "mapctrl"},
    {SECTION_PC_SERVICE, "pc"},
    {SECTION_STEER_EXEC, "steerexec"},
    {SECTION_BAND_MON, "bandmon"},
    {SECTION_ESTIMATOR, "estimator"},
    {SECTION_STEER_ALG, "steeralg"}};

action_mapping_t debug_action_cmd[] = {
    {DEBUG_LEVEL, "level"},
    {DEBUG_LEVEL, "here"}};

action_mapping_t md_action_cmd[] = {
    {MD_PRINT, "p"},
    {MD_SET, "s"}};

action_mapping_t evloop_action_cmd[] = {
    {EV_LOOP_PRINT, "s"}};

action_mapping_t ieee1905_action_cmd[] = {
    {IEEE1905_PRINT, "p"},
    {IEEE1905_SET, "s"}};

action_mapping_t hy_manager_action_cmd[] = {
    {HY_MANAGER_PRIND_HD, "hd"},
    {HY_MANAGER_PRIND_HA, "ha"},
    {HY_MANAGER_FLUSH_HA, "fa"},
    {HY_MANAGER_FLUSH_HD, "fd"}};

action_mapping_t wlan_manager_action_cmd[] = {
    {WLAN_MANAGER_FREQ, "freq"},
    {WLAN_MANAGER_PRINT, "p"}};

action_mapping_t mc_manager_action_cmd[] = {
    {MC_MANAGER_PRINT_ENCAP, "encap"},
    {MC_MANAGER_PRINT_FLOOD_TABLE, "flood"},
    {MC_MANAGER_PRINT_INTERFACE, "iffwd"}};

action_mapping_t multi_ap_action_cmd[] = {
    {MULTI_AP_DISPLAY_STATUS, "s"},
    {MULTI_AP_UNASSOCMETQUERY, "unassocmetquery"},
    {MULTI_AP_BKSCAN, "bkscan"},
    {MULTI_AP_SCANREPORT, "scanreport"},
    {MULTI_AP_HOMECHANRANK, "homechanrank"},
    {MULTI_AP_CHANPREFQUERY, "chanprefquery"},
    {MULTI_AP_CHANPREFDUMP, "chanprefdump"},
    {MULTI_AP_TRIGGERCHANSEL, "triggerchansel"},
    {MULTI_AP_BSTAMET, "bstamet"},
    {MULTI_AP_BHS, "bhs"},
    {MULTI_AP_SCANREQ, "scanReq"},
    {MULTI_AP_NEIGHREPORT, "neighReport"},
    {MULTI_AP_CACREQ, "cacreq"},
    {MULTI_AP_CACTERMINATE, "cacterminate"}};

action_mapping_t service_priority_action_cmd[] = {
    {SERVICE_PRIORITY_DISPLAY_SERVICE, "s"},
    {SERVICE_PRIORITY_BSP, "bsp"},
    {SERVICE_PRIORITY_ESP, "esp"},
    {SERVICE_PRIORITY_QSP, "qsp"},
    {SERVICE_PRIORITY_RM_SERVICE_PRIORITY, "rmsprule"},
    {SERVICE_PRIORITY_PUSH_ALL_RULE_TO_AGENT, "pusall"}};

action_mapping_t topology_action_cmd[] = {
    {TOPOLOGY_S, "s"},
    {TOPOLOGY_S1, "s1"},
    {TOPOLOGY_S2, "s2"},
    {TOPOLOGY_PARAM_ACCESS_CMD, "p"},
    {TOPOLOGY_ADD_ENTRY_TO_TOPO, "a"},
    {TOPOLOGY_AB, "ab"},
    {TOPOLOGY_AI, "ai"},
    {TOPOLOGY_F, "f"},
    {TOPOLOGY_T1, "t1"},
    {TOPOLOGY_T2, "t2"},
    {TOPOLOGY_D, "d"},
    {TOPOLOGY_G, "g"},
    {TOPOLOGY_ME, "me"},
    {TOPOLOGY_DIS, "dis"},
    {TOPOLOGY_DISCOVERY, "dicovery"},
    {TOPOLOGY_NOTIFY, "notify"},
    {TOPOLOGY_RADIOCAP, "radiocap"},
    {TOPOLOGY_RMID, "rmid"},
    {TOPOLOGY_SMID, "smid"}};

action_mapping_t dispatch_service_action_cmd[] = {
    {DISPATCH_SERVICE_PRINT, "s"}};

action_mapping_t dbg_service_action_cmd[] = {
    {DBG_SERVICE_LIST_ALL_EVENTS, "s"},
    {DBG_SERVICE_CREATE_EVENT, "c"}};

action_mapping_t stadb_action_cmd[] = {
    {STADB_PRINT, "s"},
    {STADB_ENA_DIS, "d"},
    {STADB_RSSI, "rssi"},
    {STADB_ACT, "act"},
    {STADB_NOSTEER, "nosteer"},
    {STADB_NOSTEER_LIST_RELOAD, "nosteerlistreload"},
    {STADB_NOSTEER_LIST_SHOW, "nosteerlistshow"},
    {STADB_DIALOG, "diaglog"}};

action_mapping_t mapctrl_action_cmd[] = {
    {MAP_CTRL_PRINT_SUMMARY, "s"},
    {MAP_CTRL_PRINT_NEIGH_REPORT, "d"}};

action_mapping_t pc_service_action_cmd[] = {
    {PC_SERVICE_PRINT, "s"},
    {PC_SERVICE_PARAM_ACCESS, "p"},
    {PC_SERVICE_TRIGGER_STAT, "t"},
    {PC_SERVICE_SET_DEBUG, "d"},
    {PC_SERVICE_SET_MEDIUM_ULTILIZATION, "m"},
    {PC_SERVICE_SET_LINK_CAPACITY, "lc"},
    {PC_SERVICE_PRINT_MODULE_METRIC, "pm"}};

action_mapping_t steer_exec_action_cmd[] = {
    {STEER_EXEC_PRINT, "s"},
    {STEER_EXEC_STEER, "steer"},
    {STEER_EXEC_ALLOW_ASSOC, "allow_assoc"},
    {STEER_EXEC_ABORT, "abort"},
    {STEER_EXEC_DIALOG, "diaglog"}};

action_mapping_t band_mon_action_cmd[] = {
    {BAND_MON_PRINT, "s"},
    {BAND_MON_ENA_DIS_MONITOR, "d"},
    {BAND_MON_UTIL, "util"}};

action_mapping_t estimator_action_cmd[] = {
    {ESTIMATOR_PRINT, "s"},
    {ESTIMATOR_RATE, "rate"},
    {ESTIMATOR_AIRTIME, "airtime"},
    {ESTIMATOR_ENA_DIS, "d"},
    {ESTIMATOR_DETECT, "detect"},
    {ESTIMATOR_STATS, "stats"},
    {ESTIMATOR_DIALOG, "diaglog"}};

action_mapping_t steeralg_action_cmd[] = {
    {STEER_ALG_PRINT_ALG, "s"},
    {STEER_ALG_PRINT_PATH_CAPACITY, "pc"}};

char *create_cmd(section_t section, int action, char *args)
{
    char *cmd = (char *)calloc(CMD_LENGTH, sizeof(char));
    if (cmd == NULL)
    {
        log_error("Out of Memory\n");
        return NULL;
    }
    if (section >= SECTION_MAX)
    {
        log_error("Wrong SECTION\n");
        return NULL;
    }
    char *section_cmd = section_mapping_cmd[section].section_cmd;
    char *action_cmd;
    switch (section)
    {
    case SECTION_DEBUG:
        /* code */
        if (action >= DEBUG_MAX)
        {
            log_error("Wrong action\n");
            return NULL;
        }
        action_cmd = debug_action_cmd[action].action_cmd;
        break;
    case SECTION_MD:
        /* code */
        if (action >= MD_MAX)
        {
            log_error("Wrong action\n");
            return NULL;
        }
        action_cmd = md_action_cmd[action].action_cmd;
        break;
    case SECTION_EV_LOOP:
        /* code */
        if (action >= EV_LOOP_MAX)
        {
            log_error("Wrong action\n");
            return NULL;
        }
        action_cmd = evloop_action_cmd[action].action_cmd;
        break;
    case SECTION_IEEE1905:
        /* code */
        if (action >= IEEE1905_MAX)
        {
            log_error("Wrong action\n");
            return NULL;
        }
        action_cmd = ieee1905_action_cmd[action].action_cmd;
        break;
    case SECTION_HY_MANAGER:
        /* code */
        if (action >= HY_MANAGER_MAX)
        {
            log_error("Wrong action\n");
            return NULL;
        }
        action_cmd = hy_manager_action_cmd[action].action_cmd;
        break;
    case SECTION_WLAN_MANAGER:
        /* code */
        if (action >= WLAN_MANAGER_MAX)
        {
            log_error("Wrong action\n");
            return NULL;
        }
        action_cmd = wlan_manager_action_cmd[action].action_cmd;
        break;
    case SECTION_MC_MANAGER:
        /* code */
        if (action >= MC_MANAGER_MAX)
        {
            log_error("Wrong action\n");
            return NULL;
        }
        action_cmd = mc_manager_action_cmd[action].action_cmd;
        break;
    case SECTION_MULTI_AP:
        /* code */
        if (action >= MULTI_AP_MAX)
        {
            log_error("Wrong action\n");
            return NULL;
        }
        action_cmd = multi_ap_action_cmd[action].action_cmd;
        break;
    case SECTION_SERVICE_PRIORITY:
        /* code */
        if (action >= SECTION_MAX)
        {
            log_error("Wrong action\n");
            return NULL;
        }
        action_cmd = service_priority_action_cmd[action].action_cmd;
        break;
    case SECTION_TOPOLOGY:
        /* code */
        if (action >= TOPOLOGY_MAX)
        {
            log_error("Wrong action\n");
            return NULL;
        }
        action_cmd = topology_action_cmd[action].action_cmd;
        break;
    case SECTION_DISPATCH_SERVICE:
        /* code */
        if (action >= DISPATCH_SERVICE_MAX)
        {
            log_error("Wrong action\n");
            return NULL;
        }
        action_cmd = dispatch_service_action_cmd[action].action_cmd;
        break;
    case SECTION_DBG_SERVICE:
        /* code */
        if (action >= DBG_SERVICE_MAX)
        {
            log_error("Wrong action\n");
            return NULL;
        }
        action_cmd = dbg_service_action_cmd[action].action_cmd;
        break;
    case SECTION_STADB:
        /* code */
        if (action >= STADB_MAX)
        {
            log_error("Wrong action\n");
            return NULL;
        }
        action_cmd = stadb_action_cmd[action].action_cmd;
        break;
    case SECTION_MAP_CTRL:
        /* code */
        if (action >= MAP_CTRL_MAX)
        {
            log_error("Wrong action\n");
            return NULL;
        }
        action_cmd = mapctrl_action_cmd[action].action_cmd;
        break;
    case SECTION_PC_SERVICE:
        /* code */
        if (action >= PC_SERVICE_MAX)
        {
            log_error("Wrong action\n");
            return NULL;
        }
        action_cmd = pc_service_action_cmd[action].action_cmd;
        break;
    case SECTION_STEER_EXEC:
        /* code */
        if (action >= STEER_EXEC_MAX)
        {
            log_error("Wrong action\n");
            return NULL;
        }
        action_cmd = steer_exec_action_cmd[action].action_cmd;
        break;
    case SECTION_BAND_MON:
        /* code */
        if (action >= BAND_MON_MAX)
        {
            log_error("Wrong action\n");
            return NULL;
        }
        action_cmd = band_mon_action_cmd[action].action_cmd;
        break;
    case SECTION_ESTIMATOR:
        /* code */
        if (action >= ESTIMATOR_MAX)
        {
            log_error("Wrong action\n");
            return NULL;
        }
        action_cmd = estimator_action_cmd[action].action_cmd;
        break;
    case SECTION_STEER_ALG:
        /* code */
        if (action >= STEER_ALG_MAX)
        {
            log_error("Wrong action\n");
            return NULL;
        }
        action_cmd = steeralg_action_cmd[action].action_cmd;
        break;
    default:
        break;
    }

    strcat(cmd, section_cmd);
    strcat(cmd, SPACE);
    strcat(cmd, action_cmd);
    if (args != NULL)
    {
        strcat(cmd, SPACE);
        strcat(cmd, args);
    }
    strcat(cmd, "\n");

    return cmd;
}

void free_cmd(char *cmd)
{
    free(cmd);
}
