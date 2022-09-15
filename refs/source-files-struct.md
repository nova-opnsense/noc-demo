    .
    ├── build                   # Compiled files (alternatively `dist`)
    ├── docs                    # Documentation files (alternatively `doc`)
    ├── libs                    # Lib files
    ├── config                  # Configuration File (Network Default Config)
    ├── script                  # Script File for manual config
    ├── src                     # Source files 
    ├── test                    # Automated tests (alternatively `spec` or `tests`)
    ├── CHANGELOG.md            # CHANGELOG for ezm release version
    └── README.md               # 


    src
    ├── api                     #
    │   ├── error_code          # Contain all message error code
    │   ├── mesh                # Mesh API
    │   ├── message             # Message struct and handler
    │   ├── mqtt                # MQTT Connection (Publish, Subcribe, Connect)
    │   └── network             # All Network API
    │       ├── dhcp            # DNS-DHCP API Handler
    │       ├── iptables        # Iptable API Handler
    │       ├── lan             # Lan API Handler
    │       ├── route           # Route API Handler
    │       └── wan             # WAN API handler
    ├── config                  
        ├── mqtt_bridge         # Config Mqtt-bridge
        ├── service_controller  # Config Service controller
        └── service_info        # Get Service Info
    ├── core
    │   ├── mesh                # Mesh Core Function
    │   │   ├── core            # Deamon Function (emt, emt-cmd)
    │   │   ├── pair_device     # Pair_device Core Function (WPS)
    │   │   ├── setting         # Mesh Setting Core Function (Config CAP, RE Mode)
    │   │   └── topology        # Topology Core Function (Get List AP in Mesh)
    │   └── network
    │       ├── dhcp            # DNS-DHCP Core Function
    │       ├── iptables        # Iptable Core Function
    │       ├── lan             # Lan Core Function
    │       ├── route           # Static Route Core Function
    │       ├── wan             # Wan Core Function
    │       └── wlan            # Wlan Core Function
    └── utils
        ├── avahi               # Avahi Client
        ├── cmd                 # Command Lib
        ├── file                # Read, Write File
        ├── logger              # Wrapper Logger to Syslog-NG
        ├── queue               # Queue Messsage
        ├── split               # Split string
        ├── timer               # Thread Timer
        ├── uci                 # Uci lib 
        └── uuid                # UUID 