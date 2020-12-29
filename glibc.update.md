# 升级glibc

## 坑：/usr/lib/x86_64-linux-gnu/libstdc++.so.6: version `GLIBCXX_3.4.22' not found

```bash
# 查看glibcxx版本
strings /usr/lib/x86_64-linux-gnu/libstdc++.so.6 | grep GLIBCXX
```


## 升级办法

```bash
# 安装add-apt-repository, 这里针对ubuntu16.04的docker，如果是ubuntu16.04主机不需要
# apt-get install python-software-properties software-properties-common

add-apt-repository -y ppa:ubuntu-toolchain-r/test
apt update
apt -y install gcc-4.9
apt -y upgrade libstdc++6
apt -y dist-upgrade
```bash
