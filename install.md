# shadowsocks 安装

[TOC]

## 1. 安装python-pip

### 1.1 debian/ubuntu

```bash
apt -y install python-pip
pip install shadowsocks -i http://mirrors.aliyun.com/pypi/simple/ --trusted-host mirrors.aliyun.com
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

### 1.2 CentOS

```bash
yum install python-setuptools && easy_install pip
pip install shadowsocks -i http://mirrors.aliyun.com/pypi/simple/
```

## 2. pip国内源

- 清华：https://pypi.tuna.tsinghua.edu.cn/simple
- 阿里云：http://mirrors.aliyun.com/pypi/simple/
- 中国科技大学 https://pypi.mirrors.ustc.edu.cn/simple/
- 华中理工大学：http://pypi.hustunique.com/
- 山东理工大学：http://pypi.sdutlinux.org/
- 豆瓣：http://pypi.douban.com/simple/
