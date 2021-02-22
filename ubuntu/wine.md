# 安装wine稳定

```bash
wget -nc https://dl.winehq.org/wine-builds/Release.key
apt-key add Release.key
apt-add-repository https://dl.winehq.org/wine-builds/ubuntu/
apt-get update
apt-get install --allow-unauthenticated --install-recommends winehq-stable
```
