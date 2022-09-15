#include "iptables_common.h"
#include "utils/split/split.h"
#include "utils/log/log.h"
#include "utils/cmd/cmd.h"

error_code_t iptables_open_port_profiles(iptable_list_t *iptable_list)
{
    char cmd[200];
    int port_profiles = iptable_list->nb_iptable;
    for (size_t i = 0; i < port_profiles; i++)
    {
        /* code */
        // log_info("1");
        iptable_t iptables = iptable_list->iptable_list[i];
        if (iptables.enable)
        {
            // log_info("1");
            if (!iptables.protocol || !iptables.port_range)
            {
                break;
            }
            sprintf(cmd, "iptables -A INPUT -p %s --dport %s", iptables.protocol, iptables.port_range);
            if (iptables.ip_mask && strcmp(iptables.ip_mask, "") != 0)
            {
                // ip_mask not empty
                strcat(cmd, " -s ");
                strcat(cmd, iptables.ip_mask);
            }
            // log_info("1");
            if (iptables.interface && strcmp(iptables.interface, "") != 0)
            {
                strcat(cmd, " -i ");
                strcat(cmd, iptables.interface);
            }
            // log_info("1");
            strcat(cmd, " -j ACCEPT");
            // Exec iptable command
            // log_info("1");
            //  log_info("%s", cmd);
            execl_non_timeout(cmd);
            // log_info("1");
        }
    }
    // log_info("1");
    return ERR_SUCCESS;
}

error_code_t iptables_open_port_profile(iptable_t *iptables)
{
    char cmd[200];
    /* code */
    // log_info("1");
    if (iptables->enable)
    {
        // log_info("1");
        sprintf(cmd, "iptables -A INPUT -p %s --dport %s", iptables->protocol, iptables->port_range);
        if (iptables->ip_mask && strcmp(iptables->ip_mask, "") != 0)
        {
            // ip_mask not empty
            strcat(cmd, " -s ");
            strcat(cmd, iptables->ip_mask);
        }
        // log_info("1");
        if (iptables->interface && strcmp(iptables->interface, "") != 0)
        {
            strcat(cmd, " -i ");
            strcat(cmd, iptables->interface);
        }
        // log_info("1");
        strcat(cmd, " -j ACCEPT");
        // Exec iptable command
        // log_info("1");
        log_info("%s", cmd);
        execl_non_timeout(cmd);
        // log_info("1");
    }
    // log_info("1");
    return ERR_SUCCESS;
}
