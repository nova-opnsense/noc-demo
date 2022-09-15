wifimeshconfig map
uci set network.lan=interface
uci set network.lan.force_link='1'
uci set network.lan.type='bridge'
uci set network.lan.ipaddr='192.168.123.1'
uci set network.lan.netmask='255.255.255.0'
uci set network.lan.ip6assign='60'
uci set network.lan.multicast_querier='0'
uci set network.lan.igmp_snooping='0'
uci set network.lan.ieee1905managed='1'
uci set network.lan.proto='dhcp'
uci set network.lan.ifname='eth1 eth2 eth3 eth4'

#delete WAN for ignore Relaying Agent
uci delete network.wan;
uci commit network
uci set dhcp.lan.ignore=1
uci commit dhcp
/etc/init.d/network restart
/etc/init.d/dnsmasq restart
uci set wireless.wifi0.repacd_map_bsta_preference=1
uci set wireless.wifi1.repacd_map_bsta_preference=2
uci set wireless.qcawifi=qcawifi
uci set wireless.qcawifi.samessid_disable=1
uci commit wireless
uci set wsplcd.config.MapMaxBss=6
uci commit wsplcd

uci set repacd.repacd.Enable=1
uci set repacd.repacd.ConfigREMode='son'
uci set repacd.MAPConfig.FirstConfigRequired=1
uci set repacd.MAPConfig.Enable=1
uci set repacd.MAPConfig.MapVersionEnabled=3
uci set repacd.MAPConfig.OnboardingType='none'
uci set repacd.MAPConfig.FronthaulSSID='NOVA'
uci set repacd.MAPConfig.FronthaulKey='password'
uci set repacd.MAPConfig.FronthaulAuthType=1
uci commit repacd
/etc/init.d/repacd start