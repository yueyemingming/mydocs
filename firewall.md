# 防火墙

```bash
## 关闭防火墙
/etc/init.d/iptables stop
service iptables stop
setenforce 0

## 永久禁止
vim /etc/sysconfig/selinux

SELINUX=disabled
SELINUXTYPE=disabled
```
