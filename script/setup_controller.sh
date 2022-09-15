wifimeshconfig map
uci set dhcp.lan.ignore=0
uci commit dhcp

# set br-lan to static ip
uci set network.lan.proto='static'

# Add wan interface
uci set network.wan=interface
uci set network.wan.ifname='eth0'
uci set network.wan.proto='dhcp'


uci set wireless.wifi0.repacd_create_ctrl_fbss=1
uci set wireless.wifi0.repacd_create_ctrl_bbss=1
uci set wireless.wifi1.repacd_create_ctrl_fbss=1
uci set wireless.wifi1.repacd_create_ctrl_bbss=1
uci set wireless.qcawifi=qcawifi
uci set wireless.qcawifi.samessid_disable=1
uci commit wireless
uci set repacd.repacd.Ezmesh=1
uci set repacd.repacd.Enable=1
uci set repacd.repacd.ConfigREMode='son'
uci set repacd.MAPConfig.Enable=1
uci set repacd.MAPConfig.FirstConfigRequired=1
uci set repacd.MAPConfig.BSSInstantiationTemplate='scheme-a.conf'
uci set repacd.MAPConfig.MapVersionEnabled=3
uci set repacd.MAPConfig.FronthaulSSID=$1
uci set repacd.MAPConfig.FronthaulKey=$2
uci set repacd.MAPConfig.BackhaulSSID='EzMeshBackhaul'
uci set repacd.MAPConfig.BackhaulKey='QCOM#1WiFi'
# uci set repacd.MAPConfig.BackhaulSSID='meshR3'
# uci set repacd.MAPConfig.BackhaulKey='MatMaBiMat'
# uci set repacd.MAPConfig.OnboardingType='dpp'
uci set repacd.MAPConfig.FronthaulAuthType=1
uci set ezlbd.Estimator_Adv.ActDetectMinInterval='10'
uci set ezlbd.Estimator_Adv.ActDetectMinPktPerSec='5'
uci set ezlbd.APSteer.LowRSSIAPSteerThreshold_SIG='35'
uci set ezlbd.APSteer.APSteerMaxRetryCount='20'
uci set ezlbd.StaDB.MarkAdvClientAsDualBand=1
uci set ezlbd.SteerExec_Adv.StartInBTMActiveState=1
uci set ezlbd.config.InactDetectionFromTx='1'
uci set ezmesh.MultiAP.EnableChannelSelection=1
uci set ezmesh.MultiAP.BkScanIntervalMin=0
uci set ezmesh.MultiAP.ChannelSelectionDelaySec=0
uci set ezmesh.MultiAP.ChannelSelectionOnGlobalPref=0
uci set ezmesh.MultiAP.EnableTopologyOpt=1
uci commit ezlbd
uci commit ezmesh
uci commit repacd
/etc/init.d/repacd start


 # Config Bridge WAN Interface
# Create Bridge WWAN
sleep 5
uci set network.wwan=interface
uci set network.wwan.ifname='eth0'
uci set network.wwan.proto='dhcp'
uci set network.wwan.type='bridge'
uci set network.wwan6=interface
uci set network.wwan6.ifname='eth0'
uci set network.wwan6.proto='dhcp6'

uci commit network
/etc/init.d/network restart