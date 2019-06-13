# 防火墙

## 关闭防火墙

```bash
/etc/init.d/iptables stop
service iptables stop
setenforce 0
```

## 永久禁止

```bash
vim /etc/sysconfig/selinux

修改为:SELINUX=disabled并且SELINUXTYPE=disabled.
```