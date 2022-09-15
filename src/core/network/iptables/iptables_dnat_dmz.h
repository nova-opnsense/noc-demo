#pragma once
#include "iptables_struct.h"
#include "api/error_code/error_code.h"


error_code_t iptable_dnat_dmz_request_from_setting_file(iptable_t **);

error_code_t iptable_dnat_dmz_update(iptable_t *);
error_code_t iptable_dnat_dmz_request(iptable_t **);

error_code_t iptable_dnat_dmz_reload_from_setting_file();

