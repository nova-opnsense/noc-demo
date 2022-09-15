#include "iptables_struct.h"
#include "api/error_code/error_code.h"


error_code_t iptables_dnat_forwarding_create(iptable_list_t * , iptable_list_t **);
error_code_t iptables_dnat_forwarding_request(iptable_list_t **);
error_code_t iptables_dnat_forwarding_update(iptable_list_t * , iptable_list_t **);
error_code_t iptables_dnat_forwarding_delete(iptable_list_t * , iptable_list_t **);

error_code_t iptable_dnat_forwarding_reload_from_setting_file();
error_code_t iptable_dnat_forwarding_request_from_setting_file(iptable_list_t **);