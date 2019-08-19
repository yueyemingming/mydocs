# 基础环境搭建

* [翻墙](../../shadowsocks/)
* [安装docker](../../docker/install.md)
* [安装go](../../golang/install.md)
* [docker镜像下载](docker.download.md)
* [Git安装](../../git/readme.md#安装)

## 1. 下载fabric

* 这是fabric源码

```bash
mkdir -p $GOPATH/src/github.com/hyperledger/
cd $GOPATH/src/github.com/hyperledger/
git clone https://github.com/hyperledger/fabric.git
cd fabric
git checkout v1.3.0
```

### 编译

#### a. 全部编译

```bash
cd $GOPATH/src/github.com/hyperledger/fabric
make release
cp -rvf release/linux-amd64/bin/* /usr/local/bin/

# release/linux-amd64/bin/ 最终生成的可执行文件的目录，拷贝到系统目录中
```

#### b. 子模块编译

```bash
cd $GOPATH/src/github.com/hyperledger/fabric
make configtxgen
make configtxlator
make cryptogen
make discover
make idemixgen
make orderer
make  peer
```

可执行文件生成在 `.build/bin/` 目录中。

## 2. 下载fabric-samples

* 这是已编译的可执行文件及一些示例智能合约

```bash
cd ~
git clone https://github.com/hyperledger/fabric-samples.git
cd fabric-samples
git checkout v1.3.0
```