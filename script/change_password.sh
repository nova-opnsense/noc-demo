uci set repacd.MAPConfig.FronthaulSSID='ezmesh1'
uci set repacd.MAPConfig.FronthaulKey='password'
uci set repacd.MAPConfig.FirstConfigRequired='1'
uci commit repacd
/etc/init.d/repacd restart