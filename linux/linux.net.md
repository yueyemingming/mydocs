# linux 网络配置

- [1. 配置静态ip](#1-配置静态ip)
  - [ubuntu配置文件`/etc/network/interfaces`](#ubuntu配置文件etcnetworkinterfaces)
  - [centos配置文件`/etc/sysconfig/network-scripts/ifcfg-eth0`](#centos配置文件etcsysconfignetwork-scriptsifcfg-eth0)
- [2. 配置dns](#2-配置dns)

## 1. 配置静态ip

### ubuntu配置文件`/etc/network/interfaces`

1. 配置

    ```c
    auto ens33
    iface ens33 inet static
    address 192.168.1.112
    netmask 255.255.255.0
    gateway 192.168.1.1
    dns-nameserver 114.114.114.114
    ```

2. 重启服务

    ```bash
    /etc/init.d/networking restart
    /etc/init.d/network-manager restart #桌面版时要加这个
    ```

### centos配置文件`/etc/sysconfig/network-scripts/ifcfg-eth0`

1. 配置

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

2. 重启服务

    /etc/init.d/networking reload

## 2. 配置dns

1. 临时修改 /etc/resolv.conf
2. 永久修改 /etc/resolvconf/resolv.conf.d/base

    ```ini
    search localdomain      #如果本Server为DNS服务器，可以加上这一句，如果不是，可以不加
    nameserver 172.16.3.4   #希望修改成的DNS
    nameserver 172.16.3.3   #希望修改成的DNS
    ```

3. 重启服务

    ```bash
    /etc/init.d/networking restart  #使网卡配置生效
    /etc/init.d/resolvconf restart  #使DNS生效
    ```
