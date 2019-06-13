# fabric 单机docker多节点模式安装

* [翻墙](../../../shadowsocks/)
* [安装docker](../../../docker/install.md)
* [安装go](../../../golang/install.md)
* [docker镜像下载](../docker.download.md)

## 1. 启动

```bash
cd fabric-samples/first-network/
./byfn.sh generate
./byfn.sh up
```

## 2. 停止

```bash
./byfn.sh down
```

> 如果是阿里云，部署过程中遇到的坑，请查看**[部署时阿里云存在巨大的坑](../aliyun.keng.md)**