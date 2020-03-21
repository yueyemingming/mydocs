# 随系统启动

- [1. service方式](#1-service方式)
  - [1.1 /etc/systemd/system/xxx.service](#11-etcsystemdsystemxxxservice)
  - [1.2 /etc/init.d/xxx](#12-etcinitdxxx)
- [2. /etc/rc.local](#2-etcrclocal)
- [3. /etc/xdg/autostart/xxx.desktop](#3-etcxdgautostartxxxdesktop)

## 1. service方式

### 1.1 /etc/systemd/system/xxx.service

```ini
[Unit]
Description=database sync
After=network-online.target
Wants=network-online.target systemd-networkd-wait-online.service

[Service]
Restart=on-failure
ExecStartPre=/bin/sleep 5s
#StartLimitInterval=10
#StartLimitBurst=5
User=lynxapp
Group=lynxapp
ExecStart=/path/process >/path/nohup.out 2>/path/nohup.out
ExecReload=/bin/kill -USR1 $MAINPID
LimitNOFILE=1048576
LimitNPROC=10240

[Install]
WantedBy=multi-user.target
```

```bash
#启动服务方式1
systemctl reload #修改服务文件后，要重新加载
systemctl start xxx.service

#启动服务方式2
service xxx start
```

### 1.2 /etc/init.d/xxx

> /etc/init.d/single

```bash
PATH=/sbin:/bin

. /lib/lsb/init-functions

do_start () {
        log_action_msg "Will now switch to single-user mode"
        exec init -t1 S
}

case "$1" in
start)
        do_start
        ;;
restart|reload|force-reload)
        echo "Error: argument '$1' not supported" >&2
        exit 3
        ;;
stop|status)
        # No-op
        ;;
*)
        echo "Usage: $0 start|stop" >&2
        exit 3
        ;;
esac
```

```bash
#启动服务方式1
/etc/init.d/xxx start

#启动服务方式2
service xxx start
```

## 2. /etc/rc.local

```bash
nohup sslocal -c /etc/sslocal.json >/dev/null 2>%1 &
exit 0
```

## 3. /etc/xdg/autostart/xxx.desktop

这里是带桌面的软件的随系统启动。

- `.desktop` 是linux系统的桌面程序文件后缀名。
- `/usr/share/applications/` 桌面文件安装路径。
- `/etc/xdg/autostart/` 是随系统启动的桌面文件路径。

```ini
[Desktop Entry]
Name=clion
Comment=clion
Exec=/opt/clion-2018.1.1/bin/clion.sh
keywords=clion;
StartupNotify=true
Terminal=false
Type=Application
Categories=Application;System;
Hidden=false
Icon=/opt/clion-2018.1.1/bin/clion.png
```
