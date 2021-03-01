# 安装wine稳定

```bash
wget -nc https://dl.winehq.org/wine-builds/Release.key
apt-key add Release.key
apt-add-repository https://dl.winehq.org/wine-builds/ubuntu/
apt update

#安装wine
apt install --allow-unauthenticated --install-recommends winehq-stable

#安装winetricks
apt -y install winetricks

#安装wine-mono,wine-gecko时，翻墙后，按照默认的自动下载安装即可。
```
