# ubuntu16.04 安装 gcc8

```bash
add-apt-repository ppa:ubuntu-toolchain-r/test
apt update 
apt -y install gcc-8 g++-8

# 配置:将gcc8，g++8作为默认选项
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 100
update-alternatives --config gcc
update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-8 100
update-alternatives --config g++

gcc --version
g++ --version
```
