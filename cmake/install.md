# 手动编译安装

```bash
mkdir -p /opt/cmake
mkdir -p ~/software && cd ~/software
wget https://cmake.org/files/v3.11/cmake-3.11.0-Linux-x86_64.sh
chmod +x cmake-3.11.0-Linux-x86_64.sh
bash cmake-3.11.0-Linux-x86_64.sh --prefix=/opt/cmake --skip-license
ln -sfT /opt/cmake/bin/cmake /usr/local/bin/cmake
```