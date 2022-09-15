#include "stdlib.h"
#include "malloc.h"
#include "string.h"
#include "utils/log/log.h"

#define SPACE " "
#define CMD_LENGTH 200

typedef enum
{
    SECTION_DEBUG = 0,
    SECTION_MD,
    SECTION_EV_LOOP,
    SECTION_IEEE1905,
    SECTION_HY_MANAGER,
    SECTION_WLAN_MANAGER,
    SECTION_MC_MANAGER,
    SECTION_MULTI_AP,
    SECTION_SERVICE_PRIORITY,
    SECTION_TOPOLOGY,
    SECTION_DISPATCH_SERVICE,
    SECTION_DBG_SERVICE,
    SECTION_STADB,
    SECTION_MAP_CTRL,
    SECTION_PC_SERVICE,
    SECTION_STEER_EXEC,
    SECTION_BAND_MON,
    SECTION_ESTIMATOR,
    SECTION_STEER_ALG,
    SECTION_MAX
} section_t;

typedef enum
{
    DEBUG_LEVEL = 0, // [{<module>|all} [[=] {err|info|debug|dump}] -- print/change module debug level
    DEBUG_HERE,      //  copy debug messages to current shell context
    DEBUG_MAX
} debug_action_t;

typedef enum
{
    MD_PRINT = 0, // Print all parameters
    MD_SET,       //  Set md queue limit parameters
    MD_MAX
} md_action_t;

typedef enum
{
    EV_LOOP_PRINT = 0, // Print evloop module status
    EV_LOOP_MAX
} evloop_action_t;

typedef enum
{
    IEEE1905_PRINT = 0, // Parameter access set and display
    IEEE1905_SET,       // Parameter access set and display
    IEEE1905_MAX
} ieee1905_action_t;

typedef enum
{
    HY_MANAGER_PRIND_HD = 0, // print H-Default table
    HY_MANAGER_PRIND_HA,     // print H-Active table
    HY_MANAGER_FLUSH_HA,     // flush (erase/clear) H-Active table
    HY_MANAGER_FLUSH_HD,     // flush (erase/clear) H-Default table
    HY_MANAGER_MAX
} hy_manager_action_t;

typedef enum
{
    WLAN_MANAGER_FREQ = 0, //  Get frequency by IF name
    WLAN_MANAGER_PRINT,    //  Parameters access command (set & display)
    WLAN_MANAGER_MAX
} wlan_manager_action_t;

typedef enum
{
    MC_MANAGER_PRINT_ENCAP = 0,   // Print evloop module status
    MC_MANAGER_PRINT_FLOOD_TABLE, // print flood table
    MC_MANAGER_PRINT_INTERFACE,   // print interface forwarding table
    MC_MANAGER_MAX
} mc_manager_action_t;

typedef enum
{
    MULTI_AP_DISPLAY_STATUS = 0, // Print evloop module status
    MULTI_AP_UNASSOCMETQUERY,    // print flood table
    MULTI_AP_BKSCAN,             // print interface forwarding table
    MULTI_AP_SCANREPORT,         // print interface forwarding table
    MULTI_AP_HOMECHANRANK,       // print flood table
    MULTI_AP_CHANPREFQUERY,      // print interface forwarding table
    MULTI_AP_CHANPREFDUMP,       // print flood table
    MULTI_AP_TRIGGERCHANSEL,     // print interface forwarding table
    MULTI_AP_BSTAMET,            // print flood table
    MULTI_AP_BHS,                // print interface forwarding table
    MULTI_AP_SCANREQ,            // print flood table
    MULTI_AP_NEIGHREPORT,        // print interface forwarding table
    MULTI_AP_CACREQ,             // print flood table
    MULTI_AP_CACTERMINATE,       // print interface forwarding table
    MULTI_AP_MAX
} multi_ap_action_t;

typedef enum
{
    SERVICE_PRIORITY_DISPLAY_SERVICE = 0,    // Display Service Prioritization status
    SERVICE_PRIORITY_BSP,                    // Send Basic Service Priortization request
    SERVICE_PRIORITY_ESP,                    // Send Enhanced Service Priortization request
    SERVICE_PRIORITY_QSP,                    // Send Q Service Priortization request
    SERVICE_PRIORITY_RM_SERVICE_PRIORITY,    // Remove Service Priortization rule
    SERVICE_PRIORITY_PUSH_ALL_RULE_TO_AGENT, // Push all rules on controller to agents
    SERVICE_PRIORITY_MAX
} service_priority_action_t;

typedef enum
{
    TOPOLOGY_S = 0,             // print tdService module status
    TOPOLOGY_S1,                // print tdService module status with more details
    TOPOLOGY_S2,                // print tdService module status with the most details
    TOPOLOGY_PARAM_ACCESS_CMD,  // Parameter access command (set & display)
    TOPOLOGY_ADD_ENTRY_TO_TOPO, // add entry to tdService database
    TOPOLOGY_AB,                // add bridged address information to an entry of the tdService database (original entry already exists, e.g. created by 'a' command)
    TOPOLOGY_AI,                // add interface information to an entry of the tdService database (original entry already exists, e.g. created by 'a' command)
    TOPOLOGY_F,                 // flush tdService database (i.e. erase all entries)
    TOPOLOGY_T1,                //  Transmit ND messages
    TOPOLOGY_T2,                // Transmit BD message
    TOPOLOGY_D,                 // Delete topology entry
    TOPOLOGY_G,                 // Get topology entry by HY MAC
    TOPOLOGY_ME,                // get local information
    TOPOLOGY_DIS,               // disable td service
    TOPOLOGY_DISCOVERY,         // send a discovery message on all interfaces
    TOPOLOGY_NOTIFY,            // send a notification message on all interfaces
    TOPOLOGY_RADIOCAP,          // re-resolve the radio capabilities
    TOPOLOGY_RMID,              // reset local message ID
    TOPOLOGY_SMID,              // set local message ID to a specific value
    TOPOLOGY_MAX
} topology_action_t;

typedef enum
{
    DISPATCH_SERVICE_PRINT = 0, // Display Service Prioritization status
    DISPATCH_SERVICE_MAX
} dispatch_service_action_t;

typedef enum
{
    DBG_SERVICE_LIST_ALL_EVENTS = 0, // list all events
    DBG_SERVICE_CREATE_EVENT,        // [-m ModuleID] [-e EventID] [-p Priority[high,low]] -- create event
    DBG_SERVICE_MAX
} dbg_service_action_t;

typedef enum
{
    STADB_PRINT = 0,           // print station database contents
    STADB_ENA_DIS,             // enable/disable station database debug mode
    STADB_RSSI,                // inject an RSSI measurement
    STADB_ACT,                 // inject an activity status
    STADB_NOSTEER,             // control whether steering is disallowed for a STA
    STADB_NOSTEER_LIST_RELOAD, // control to reload the nosteering STA mac from the file
    STADB_NOSTEER_LIST_SHOW,   // show the list of nosteering STA MAC from the file
    STADB_DIALOG,              // generate diaglog message
    STADB_MAX
} stadb_action_t;

typedef enum
{
    MAP_CTRL_PRINT_SUMMARY = 0,  // Display summary status
    MAP_CTRL_PRINT_NEIGH_REPORT, //  Display scan report
    MAP_CTRL_MAX
} mapctrl_action_t;

typedef enum
{
    PC_SERVICE_PRINT = 0,               // print pcService module status for particular interfaces
    PC_SERVICE_PARAM_ACCESS,            // Parameter access command (set & display)
    PC_SERVICE_TRIGGER_STAT,            //  Trigger stats command
    PC_SERVICE_SET_DEBUG,               //  set pcService module debug mode
    PC_SERVICE_SET_MEDIUM_ULTILIZATION, //  set pcService module medium utilization in debug mode
    PC_SERVICE_SET_LINK_CAPACITY,       // set pcService module link capacity in debug mode
    PC_SERVICE_PRINT_MODULE_METRIC,     // print pcService module metrics in debug mode
    PC_SERVICE_MAX
} pc_service_action_t;

typedef enum
{
    STEER_EXEC_PRINT = 0,   // print steering executor status
    STEER_EXEC_STEER,       //  post-association steer of a STA
    STEER_EXEC_ALLOW_ASSOC, //  pre-association steer of a STA
    STEER_EXEC_ABORT,       //  abort steering a STA
    STEER_EXEC_DIALOG,      //  generate diaglog messages
    STEER_EXEC_MAX
} steer_exec_action_t;

typedef enum
{
    BAND_MON_PRINT = 0,       // print band monitor status
    BAND_MON_ENA_DIS_MONITOR, // enable/disable band monitor debug mode
    BAND_MON_UTIL,            //   inject a utilization measurement
    BAND_MON_MAX
} band_mon_action_t;

typedef enum
{
    ESTIMATOR_PRINT = 0, // display status for all STAs
    ESTIMATOR_RATE,      //  estimate rate for a STA
    ESTIMATOR_AIRTIME,   // estimate airtime for all active STAs on a channel
    ESTIMATOR_ENA_DIS,   // enable/disable debug mode
    ESTIMATOR_DETECT,    //  record whether interference is detected for a given STA
    ESTIMATOR_STATS,     //  inject stats to evaluate interference for a given STA
    ESTIMATOR_DIALOG,    //  generate diaglog message
    ESTIMATOR_MAX
} estimator_action_t;

typedef enum
{
    STEER_ALG_PRINT_ALG = 0,       //  print steering algorithm status
    STEER_ALG_PRINT_PATH_CAPACITY, //  print the path capacity from a STA to CAP in debug mode
    STEER_ALG_MAX
} steeralg_action_t;

typedef struct
{
    section_t section;
    char *section_cmd;
} section_mapping_t;

typedef struct
{
    int action;
    char *action_cmd;
} action_mapping_t;

char *create_cmd(section_t section, int action, char *args);
void free_cmd(char *cmd);