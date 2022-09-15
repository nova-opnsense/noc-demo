#include "iptables_basic.h"
#include "iptables_common.h"
#include "utils/cmd/cmd.h"
#include "api/error_code/error_code.h"

#define CONCAT(a,b,c) (a " " b " " c)
static char * iptables_default_setting_list[] = {
    "iptables -F",
    "iptables -X",
    "iptables -t nat -F",
    "iptables -t nat -X",
    "iptables -t mangle -F",
    "iptables -t mangle -X",
    "iptables -P FORWARD ACCEPT",
    "iptables -P OUTPUT ACCEPT",
    "iptables -P INPUT ACCEPT",
    "iptables -A INPUT -i lo -j ACCEPT",
    "iptables -A OUTPUT -o lo -j ACCEPT",
    CONCAT("iptables -A OUTPUT -o ", WAN_INTERFACE, " -p tcp --dport 67 -j DROP ") ,
    CONCAT("iptables -A OUTPUT -o ", WAN_INTERFACE, " -p udp --dport 67 -j DROP ") ,
    "iptables -A INPUT -p udp  --dport 67 -j ACCEPT",                                 //dhcp port
    "iptables -A INPUT -p udp  --dport 53 -j ACCEPT",                                 //dns [prt]
    "iptables -A INPUT -p udp  --dport 5353 -j ACCEPT",                                 //dns [prt]
    "iptables -A INPUT -p tcp  --dport 53 -j ACCEPT",                                 //dns port
    "iptables -A INPUT -m conntrack --ctstate ESTABLISHED,RELATED -j ACCEPT",
    "iptables -A OUTPUT -m conntrack --ctstate ESTABLISHED -j ACCEPT",
    "iptables -t nat -A POSTROUTING -j MASQUERADE",
    "iptables -I INPUT 2 -p icmp --icmp-type echo-request -j ACCEPT",
    "iptables -A INPUT -p icmp -j ACCEPT", // allow ping to outside
};


static char * iptables_allow_all_traffic_list[] = {
    "iptables -F",
    "iptables -X",
    "iptables -t nat -F",
    "iptables -t nat -X",
    "iptables -t mangle -F",
    "iptables -t mangle -X",
    "iptables -P INPUT ACCEPT",
    "iptables -P FORWARD ACCEPT",
    "iptables -P OUTPUT ACCEPT"
};

static char * iptable_restrict_basic[] = {
    "iptables -P INPUT DROP"
};


error_code_t iptables_default_setting(){
    int nb_iptables_profiles = sizeof(iptables_default_setting_list) / sizeof(char*);
    for (size_t i = 0; i < nb_iptables_profiles; i++)
    {
        /* code */
        execl_non_timeout(iptables_default_setting_list[i]);
    }
    return ERR_SUCCESS;
}


error_code_t iptables_allow_all_traffic(){
    int nb_iptables_profiles = sizeof(iptables_allow_all_traffic_list) / sizeof(char*);
    for (size_t i = 0; i < nb_iptables_profiles; i++)
    {
        /* code */
        execl_non_timeout(iptables_allow_all_traffic_list[i]);
    }
    return ERR_SUCCESS;
}

error_code_t iptables_restrict_basic(){
    int nb_iptables_profiles = sizeof(iptable_restrict_basic) / sizeof(char*);
    for (size_t i = 0; i < nb_iptables_profiles; i++)
    {
        /* code */
        execl_non_timeout(iptable_restrict_basic[i]);
    }
    return ERR_SUCCESS;
}