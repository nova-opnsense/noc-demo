#include "iptables_struct.h"
#include "api/error_code/error_code.h"



error_code_t iptable_nova_service_request_from_setting_file(iptable_nova_service_list_t **);
error_code_t iptable_nova_service_set(iptable_nova_service_list_t *);
error_code_t iptable_nova_service_get(iptable_nova_service_list_t **);
error_code_t iptable_nova_service_reload_from_setting_file();