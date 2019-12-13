# 修改系统时间命令

```bash
systemctl disable systemd-timesyncd
date -s "2018-05-17 09:51:50"
hwclock -w
```
