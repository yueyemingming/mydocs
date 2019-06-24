# static ip

- [ubuntu配置文件`/etc/network/interfaces`](#ubuntu配置文件etcnetworkinterfaces)
- [centos配置文件`/etc/sysconfig/network-scripts/ifcfg-eth0`](#centos配置文件etcsysconfignetwork-scriptsifcfg-eth0)

## ubuntu配置文件`/etc/network/interfaces`

```c
auto ens33
iface ens33 inet static
address 192.168.1.112
netmask 255.255.255.0
gateway 192.168.1.1
dns-nameserver 114.114.114.114
```

> 重启服务：/etc/init.d/networking restart

## centos配置文件`/etc/sysconfig/network-scripts/ifcfg-eth0`

```ini
TYPE=Ethernet
DEVICE=eth0
ONBOOT=yes
BOOTPROTO=static
IPADDR=192.168.1.11
NETMASK=255.255.255.0
GATEWAY=192.168.1.1
DNS1=10.203.104.41
HWADDR=00:0C:29:13:5D:74
BROADCAST=192.168.1.255
```

> 重启服务：/etc/init.d/networking reload
