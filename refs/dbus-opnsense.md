## dbus on opnsense

### install

```sh
# enable freebsd repos
echo "FreeBSD: { enabled: yes }" > /usr/local/etc/pkg/repos/FreeBSD.conf

pkg install dbus

# disable freebsd repos
echo "FreeBSD: { enabled: no }" > /usr/local/etc/pkg/repos/FreeBSD.conf

```

usage

```sh
service dbus onestart
```

check

```sh
pgrep -la dbus
```
