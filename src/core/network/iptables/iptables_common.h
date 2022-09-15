#pragma once

#include <cjson/cJSON.h>
#include "iptables_struct.h"
#include "api/error_code/error_code.h"

error_code_t iptables_open_port_profiles(iptable_list_t *iptable_list);
error_code_t iptables_open_port_profile(iptable_t *iptables);