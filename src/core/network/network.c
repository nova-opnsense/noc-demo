#include "network.h"
#include "config/service_controller/service_controller.h"
#include "utils/log/log.h"

void network_init()
{
    dns_dhcp_reload_from_setting_file();
    iptables_refresh();
    lan_reload_from_setting_file();
    wan_reload_from_setting_file();
}
