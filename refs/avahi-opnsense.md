## avahi on opnsense

### install

from pkg

```sh
# enable freebsd repos
echo "FreeBSD: { enabled: yes }" > /usr/local/etc/pkg/repos/FreeBSD.conf

pkg install avahi

# disable freebsd repos
echo "FreeBSD: { enabled: no }" > /usr/local/etc/pkg/repos/FreeBSD.conf

```

from source (not recommended)

```sh
opnsense-code ports
cd /usr/ports/net/avahi
make install clean
```

### config

FILES

- /usr/local/etc/avahi/avahi-daemon.conf: the default configuration file for avahi-daemon, avahi-daemon.conf(5) for more information.

- /usr/local/etc/avahi/hosts: additional static hostname mappings to publish in mDNS, see avahi.hosts(5) for more information.

- /usr/local/etc/avahi/services/\*.service: static service definitions, see avahi.service(5) for more information.

noc.service

```xml
<?xml version="1.0" standalone='no'?>
<!--*-nxml-*-->
<!DOCTYPE service-group SYSTEM "avahi-service.dtd">

<!--
  This file is part of avahi.

  avahi is free software; you can redistribute it and/or modify it
  under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2 of the
  License, or (at your option) any later version.

  avahi is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with avahi; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307 USA.
-->

<!-- See avahi.service(5) for more information about this configuration file -->

<service-group>

    <name replace-wildcards="yes">Printer on %h</name>

    <service>
        <type>_printer._tcp</type>
        <port>515</port>
    </service>

    <service>
        <type>_ipp._tcp</type>
        <subtype>_colour._sub._ipp._tcp</subtype>
        <port>631</port>
    </service>

    <service protocol="ipv6">
        <type>_uberprinter._tcp</type>
        <domain-name>local</domain-name>
        <host-name>quux.local</host-name>
        <port>4711</port>
        <txt-record>this=is</txt-record>
        <txt-record>a=really</txt-record>
        <txt-record>bad=example</txt-record>
    </service>

</service-group>

```

```xml

<?xml version="v1.0.0" standalone='no'?><!--*-nxml-*-->
<!DOCTYPE service-group SYSTEM "avahi-service.dtd">
<service-group>
    <name>NOC Service</name>
    <service>
        <type>_noc._tcp</type>
        <txt-record>hub_id=hub_id_test_1</txt-record>
        <txt-record>noc_id=noc_id_test_1</txt-record>
        <txt-record>noc_version=1.0.0</txt-record>
        <txt-record>noc_name=test</txt-record>
        <txt-record>role=1</txt-record>
        <txt-record>mac=0:0:0:0:0:0</txt-record>
        <txt-record>ip=0.0.0.0</txt-record>
    </service>
</service-group>

```

usage

```sh
service avahi-daemon onestart
```

check

```sh
pgrep -la avahi
```

### discovery tools

for windows

[Bonjour browser for Windows](https://hobbyistsoftware.com/bonjourbrowser)
