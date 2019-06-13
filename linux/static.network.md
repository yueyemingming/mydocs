# static ip

## ubuntu

```bash
vim /etc/network/interfaces
```

```text
auto ens33
iface ens33 inet static
address 192.168.1.112
netmask 255.255.255.0
gateway 192.168.1.1
dns-nameserver 114.114.114.114
```

重启网络

```bash
/etc/init.d/networking restart
```

## centos

```bash
vim /etc/sysconfig/network-scripts/ifcfg-eth0
```

```text
# Intel Corporation 82545EM Gigabit Ethernet Controller (Copper)
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

重新导入ifcfg-eth0 网络配置文件

```bash
/etc/init.d/network reload
```