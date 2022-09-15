#pragma once
#include "api/error_code/error_code.h"

error_code_t iptables_default_setting();
error_code_t iptables_allow_all_traffic();
error_code_t iptables_restrict_basic();