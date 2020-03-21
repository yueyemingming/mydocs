
## 添加到系统的菜单

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
