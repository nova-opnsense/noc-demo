# Sub to Broker
mosquitto_sub -p 1883 -t "ezm/event/ezm_8d10a653-323f-4005-84c9-8d08c5a3b483"

######Mesh

# Test create Mesh
mosquitto_pub -p 1883 -t "hub/event/ezm_8d10a653-323f-4005-84c9-8d08c5a3b483" -m '{"name":"y.x.ezm.mesh.created","scope":"","source":"hub","home_id":"","user_id":"","id":"","auth_token":"","parent_id":"","client_id":"hub_ecfb1cfb-975c-4c96-83bf-2cadc5fca5b8","data":{"mesh_name":"myMesh","type":"SON","rssi_threshhold":{"up":100,"down":80},"antenna_installed":{"up":true,"down":true},"backhaul_channel":"100,104,108,112,116","active_time_after_set":0,"work_mode":"controller","weight_mesh":10,"mesh_link_connection":"auto","minimum_mesh_link_snr":24,"wireless":{"wifi_iface":{"ssid":"NVL_HUB_2.4G_123123123","key":"1234567890"}}},"created_time":1647157599136}'
# Test Request Mesh
mosquitto_pub -p 1883 -t "hub/event/ezm_8d10a653-323f-4005-84c9-8d08c5a3b483" -m '{"name":"x.y.ezm.mesh.requested","scope":"","source":"hub","home_id":"","user_id":"","id":"","auth_token":"","parent_id":"","client_id":"hub_ecfb1cfb-975c-4c96-83bf-2cadc5fca5b8","data":{},"created_time":1647157599136}'
# Test Update Mesh
mosquitto_pub -p 1883 -t "hub/event/ezm_8d10a653-323f-4005-84c9-8d08c5a3b483" -m '{"name":"y.x.ezm.mesh.updated","scope":"","source":"hub","home_id":"","user_id":"","id":"","auth_token":"","parent_id":"","client_id":"hub_ecfb1cfb-975c-4c96-83bf-2cadc5fca5b8","data":{"mesh_id":"mesh_3dc8b43c-c390-410c-9592-f2659f4f4afc","type":"SON","mesh_name":"myMesh","rssi_threshhold":{"up":100,"down":80},"antenna_installed":{"up":true,"down":true},"backhaul_channel":"100,104,108,112,116","active_time_after_set":0,"work_mode":"controller","weight_mesh":10,"mesh_link_connection":"auto","minimum_mesh_link_snr":24,"wireless":{"wifi_iface":{"ssid":"NVL_HUB_2.4G_Floor4","key":"1234567890"}}},"created_time":1647157599136}'
# Test Delete Mesh
mosquitto_pub -p 1883 -t "hub/event/ezm_8d10a653-323f-4005-84c9-8d08c5a3b483" -m '{"name":"y.x.ezm.mesh.deleted","scope":"","source":"hub","home_id":"","user_id":"","id":"","auth_token":"","parent_id":"","client_id":"hub_ecfb1cfb-975c-4c96-83bf-2cadc5fca5b8","data":{"mesh_id":"mesh_3dc8b43c-c390-410c-9592-f2659f4f4afc","type":"SON","mesh_name":"myMesh","rssi_threshhold":{"up":100,"down":80},"antenna_installed":{"up":true,"down":true},"backhaul_channel":"100,104,108,112,116","active_time_after_set":0,"work_mode":"controller","weight_mesh":10,"mesh_link_connection":"auto","minimum_mesh_link_snr":24,"wireless":{"wifi_iface":{"ssid":"NVL_HUB_2.4G_Floor4","key":"1234567890"}}},"created_time":1647157599136}'

#Test REquest Device List 
mosquitto_pub -p 1883 -t "hub/event/ezm_8d10a653-323f-4005-84c9-8d08c5a3b483" -m '{"name":"x.y.ezm.mesh.devices.requested","scope":"","source":"hub","home_id":"","user_id":"","id":"","auth_token":"","parent_id":"","client_id":"hub_ecfb1cfb-975c-4c96-83bf-2cadc5fca5b8","data":{},"created_time":1647157599136}'
# Test Request Device List available for pairing
mosquitto_pub -p 1883 -t "hub/event/ezm_8d10a653-323f-4005-84c9-8d08c5a3b483" -m '{"name":"y.x.ezm.mesh.devices.available_for_pairing.requested","scope":"","source":"hub","home_id":"","user_id":"","id":"","auth_token":"","parent_id":"","client_id":"hub_ecfb1cfb-975c-4c96-83bf-2cadc5fca5b8","data":{},"created_time":1647157599136}'
#Test Pair New Device
mosquitto_pub -p 1883 -t "hub/event/ezm_8d10a653-323f-4005-84c9-8d08c5a3b483" -m '{"name":"x.y.ezm.mesh.devices.pair.requested","scope":"","source":"hub","home_id":"","user_id":"","id":"","auth_token":"","parent_id":"","client_id":"hub_ecfb1cfb-975c-4c96-83bf-2cadc5fca5b8","data":[{"hub_id":"hub_ae0ed5e0-6256-4a5f-b92e-885dda83f977","ezm_id":"ezm_ae0ed5e0-6256-4a5f-b92e-885dda83f977","ezm_version":"2.0","ezm_name":"00:03:7f:ba:db:ad","role":"standalone","mac":"00:03:7f:ba:db:ad","ip":"192.168.123.25","channel_selection":{},"status":0}],"created_time":1647157599136}'
#Test Update Device
mosquitto_pub -p 1883 -t "hub/event/ezm_8d10a653-323f-4005-84c9-8d08c5a3b483" -m '{"name":"x.y.ezm.mesh.devices.updated","scope":"","source":"hub","home_id":"","user_id":"","id":"","auth_token":"","parent_id":"","client_id":"hub_ecfb1cfb-975c-4c96-83bf-2cadc5fca5b8","data":[{"hub_id":"hub_5779e411-f6c9-425a-8d42-cf1e27179c80","ezm_id":"ezm_8d10a653-323f-4005-84c9-8d08c5a3b483","ezm_version":"2.0","ezm_name":"Controller2","role":"controller","mac":"c0:1c:30:18:51:11","ip":"192.168.123.1","parent_mac":"none ","channel_selection":{"5g":"144","2g":"11"},"status":1}],"created_time":1647157599136}'
#Test Delete Device
mosquitto_pub -p 1883 -t "hub/event/ezm_8d10a653-323f-4005-84c9-8d08c5a3b483" -m '{"name":"x.y.ezm.mesh.devices.deleted","scope":"","source":"hub","home_id":"","user_id":"","id":"","auth_token":"","parent_id":"","client_id":"hub_ecfb1cfb-975c-4c96-83bf-2cadc5fca5b8","data":[{"ezm_id":"ezm_e64d3084-f8e8-4c04-b714-8981e259e555","ip":"192.168.123.25"}],"created_time":1647157599136}'


#Test Request_activate_re_device
mosquitto_pub -p 1883 -t "hub/event/ezm_8d10a653-323f-4005-84c9-8d08c5a3b483" -m '{"name":"x.y.ezm.mesh.device.activate_agent.requested","scope":"","source":"hub","home_id":"","user_id":"","id":"","auth_token":"","parent_id":"","client_id":"hub_ecfb1cfb-975c-4c96-83bf-2cadc5fca5b8","data":{},"created_time":1647157599136}'
#Test Request_inactivate_re_device
mosquitto_pub -p 1883 -t "hub/event/ezm_8d10a653-323f-4005-84c9-8d08c5a3b483" -m '{"name":"x.y.ezm.mesh.device.inactivate_agent.requested","scope":"","source":"hub","home_id":"","user_id":"","id":"","auth_token":"","parent_id":"","client_id":"hub_ecfb1cfb-975c-4c96-83bf-2cadc5fca5b8","data":{},"created_time":1647157599136}'

#Test Request Mesh Quality
mosquitto_pub -p 1883 -t "hub/event/ezm_8d10a653-323f-4005-84c9-8d08c5a3b483" -m '{"name":"x.y.ezm.mesh.quality.requested","scope":"","source":"hub","home_id":"","user_id":"","id":"","auth_token":"","parent_id":"","client_id":"hub_ecfb1cfb-975c-4c96-83bf-2cadc5fca5b8","data":{},"created_time":1647157599136}'


######Network
#Create dns_dhcp
mosquitto_pub -p 1883 -t "hub/event/ezm_8d10a653-323f-4005-84c9-8d08c5a3b483" -m '{"name":"hub.ezm.network.dns_dhcp.created","source":"hub","data":{"dns":{"custom_hostname":[{"name":"Authenticate","domain":"auth.nova","ip":"192.168.124.1"},{"name":"Cap","domain":"cap.nova","ip":"192.168.124.1"}],"additional_dns":["8.8.8.8","8.8.4.4"],"forwarding_dns":null,"forwarding_dns_enabled":false},"dhcp4":{"start_ip":"192.168.124.5","pools":100,"relay_enabled":false,"relay_server_address":"192.168.1.1","relay_local_address":"192.168.124.1","static_dhcp4_clients":[{"mac":"aa:bb:cc:dd:ee:ff","ip":"192.168.34.199","host_name":"sample"}]}}}'
#Request dns_dhcp
mosquitto_pub -p 1883 -t "hub/event/ezm_8d10a653-323f-4005-84c9-8d08c5a3b483" -m '{"name":"hub.hub.network.dns_dhcp.requested","source":"hub","data":{}}'
#Request dhcp4_lease
mosquitto_pub -p 1883 -t "hub/event/ezm_8d10a653-323f-4005-84c9-8d08c5a3b483" -m '{"name":"hub.hub.network.dhcp4.leases.requested","source":"hub","data":{}}'
#Create Lan setting 
mosquitto_pub -p 1883 -t "hub/event/ezm_8d10a653-323f-4005-84c9-8d08c5a3b483" -m '{"name":"hub.hub.network.lan.created","source":"hub","data":{"bridge_ap_mode":false,"proto":"static","ipaddr":"192.168.123.1","netmask":"255.255.255.0"}}'
#Request Lan setting 
mosquitto_pub -p 1883 -t "hub/event/ezm_8d10a653-323f-4005-84c9-8d08c5a3b483" -m '{"name":"hub.ezm.network.lan.requested","source":"hub","data":{}}'

##WAN
#Create wan setting 
mosquitto_pub -p 1883 -t "hub/event/ezm_8d10a653-323f-4005-84c9-8d08c5a3b483" -m '{"name":"hub.hub.network.wan.created","source":"hub","data":{"proto":"dhcp","ipaddr":"192.168.1.1"}}'
#Request wan setting 
mosquitto_pub -p 1883 -t "hub/event/ezm_8d10a653-323f-4005-84c9-8d08c5a3b483" -m '{"name":"hub.hub.network.wan.requested","source":"hub","data":{}}'

## ROUTE
#Create route
mosquitto_pub -p 1883 -t "hub/event/ezm_8d10a653-323f-4005-84c9-8d08c5a3b483" -m '{"name":"hub.hub.network.static.routes.created","source":"hub","data":[{"ip_mask":"192.168.4.0/24","target_ip":"192.168.3.98","name":"example2","interface":"eth0"}]}'
#Request route
mosquitto_pub -p 1883 -t "hub/event/ezm_8d10a653-323f-4005-84c9-8d08c5a3b483" -m '{"name":"hub.hub.network.static.routes.requested","source":"hub","data":{}}'
#Update route
mosquitto_pub -p 1883 -t "hub/event/ezm_8d10a653-323f-4005-84c9-8d08c5a3b483" -m '{"name":"hub.hub.network.static.routes.updated","source":"hub","data":[{"id":"route_f5dd2d30-1c91-4a32-a94a-7158e9cfaa94","ip_mask":"192.168.4.0/24","target_ip":"192.168.3.98","name":"updatted","interface":"eth0"}]}'
#Delete Route
mosquitto_pub -p 1883 -t "hub/event/ezm_8d10a653-323f-4005-84c9-8d08c5a3b483" -m '{"name":"hub.hub.network.static.routes.deleted","source":"hub","data":[{"id":"route_a4f52340-690d-4084-b8a6-bcff9f8efd96"}]}'


## IPTABLE
# create dnat.forwarding
mosquitto_pub -p 1883 -t "hub/event/ezm_8d10a653-323f-4005-84c9-8d08c5a3b483" -m '{"name":"hub.hub.iptables.dnat.port_forwarding.profiles.created","source":"hub","data":[{"id":"","name":"SSH Nano forwarding","port_range":"8999:8999","ip_mask":"","protocol":"tcp","dnat_ip":"192.168.1.67","dnat_port_range":"22:22","Enable":true}]}'
# request dnat.forwarding
mosquitto_pub -p 1883 -t "hub/event/ezm_8d10a653-323f-4005-84c9-8d08c5a3b483" -m '{"name":"hub.hub.iptables.dnat.port_forwarding.profiles.requested","source":"hub","data":{}}'
# update dnat.forwarding
mosquitto_pub -p 1883 -t "hub/event/ezm_8d10a653-323f-4005-84c9-8d08c5a3b483" -m '{"name":"hub.hub.iptables.dnat.port_forwarding.profiles.updated","source":"hub","data":[{"id":"iptable_55a39222-48b8-4a7a-9b6d-0eb143cde4d3","name":"SSH Update Nano forwarding","port_range":"8999:8999","ip_mask":"","protocol":"tcp","dnat_ip":"192.168.1.67","dnat_port_range":"22:22","enable":false}]}'
# delete dnat.forwarding
mosquitto_pub -p 1883 -t "hub/event/ezm_8d10a653-323f-4005-84c9-8d08c5a3b483" -m '{"name":"hub.hub.iptables.dnat.port_forwarding.profiles.deleted","source":"hub","data":[{"id":"iptable_2d1b498e-273c-4641-aa80-812fda9ee006","name":"SSH Nano forwarding","port_range":"8999:8999","ip_mask":"","protocol":"tcp","dnat_ip":"192.168.1.67","dnat_port_range":"22:22","enable":false}]}'
# create dnat.dmz
mosquitto_pub -p 1883 -t "hub/event/ezm_8d10a653-323f-4005-84c9-8d08c5a3b483" -m '{"name":"hub.hub.iptables.dnat.dmz.profiles.created","source":"hub","data":[{"name":"SSH Nano DMZ2","ip_mask":"","dnat_ip":"192.168.1.67","enable":false}]}'
# request dnat.dmz
mosquitto_pub -p 1883 -t "hub/event/ezm_8d10a653-323f-4005-84c9-8d08c5a3b483" -m '{"name":"hub.hub.iptables.dnat.dmz.profiles.requested","source":"hub","data":{}}'
# create nova_service
mosquitto_pub -p 1883 -t "hub/event/ezm_8d10a653-323f-4005-84c9-8d08c5a3b483" -m '{"name":"hub.hub.iptables.nova-services.port-profiles.created","source":"hub","data":[{"name":"ssh","port_profiles":[{"id":"","name":"aloha","port_range":"22:22","ip_mask":"192.168.0.0/16","protocol":"tcp","interface":"","enable":true}]},{"name":"broker","port_profiles":[{"id":"","name":"","port_range":"8883:8883","ip_mask":"192.168.0.0/16","protocol":"tcp","interface":"","enable":true}]},{"name":"web","port_profiles":[{"id":"","name":"","port_range":"8100:8101","ip_mask":"192.168.0.0/16","protocol":"tcp","interface":"","enable":true}]},{"name":"old_web","port_profiles":[{"id":"","name":"","port_range":"80:80","ip_mask":"192.168.0.0/16","protocol":"tcp","interface":"","enable":true},{"id":"","name":"","port_range":"8080:8080","ip_mask":"192.168.0.0/16","protocol":"tcp","interface":"","enable":true},{"id":"","name":"","port_range":"9010","ip_mask":"192.168.0.0/16","protocol":"tcp","interface":"","enable":true}]}]}'
# request nova_service
mosquitto_pub -p 1883 -t "hub/event/ezm_8d10a653-323f-4005-84c9-8d08c5a3b483" -m '{"name":"hub.hub.iptables.nova-services.port-profiles.requested","source":"hub","data":{}}'

