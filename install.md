# shadowsocks 安装

## 1. Debian / Ubuntu

```bash
apt -y install python-pip
pip install shadowsocks
```

> pip错误 ImportError: No module named _internal

```text
Traceback (most recent call last):
File "/home/ubuntu/.local/bin/pip", line 7, in <module>

from pip._internal import main

ImportError: No module named _internal
```

> 强制重新安装pip3

```bash
wget https://bootstrap.pypa.io/get-pip.py  --no-check-certificate
python3 get-pip.py --force-reinstall
```

## 2. CentOS

```bash
yum install python-setuptools && easy_install pip
pip install shadowsocks
```
