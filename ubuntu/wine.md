# 安装wine稳定

```bash
wget -nc https://dl.winehq.org/wine-builds/Release.key
apt-key add Release.key
apt-add-repository https://dl.winehq.org/wine-builds/ubuntu/
apt update

#安装wine
apt install --allow-unauthenticated --install-recommends winehq-stable

#安装winetricks
apt install winetricks

#安装wine-mono
wine start /i wine-mono-6.0.0-x86.msi

#安装wine-gecko
wine start /i wine-gecko-2.47.2-x86.msi 
wine start /i wine-gecko-2.47.2-x86_64.msi 
```
