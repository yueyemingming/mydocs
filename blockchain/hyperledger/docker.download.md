
# Docker镜像下载

## 方式1 直接下载方式  `推荐这种方式`

```bash
curl -sSL http://bit.ly/2ysbOFE | bash -s 1.3.0
```

## 方式2 源码下载方式

```bash
git clone https://github.com/hyperledger/fabric.git
cd ~/fabric
git checkout v1.3.0
cd examples/e2e_cli/
source download-dockerimages.sh -c 1.3.0 -f 1.3.0
```

## 方式3 bootstrap.sh下载

在没有翻墙的情况下，国内可以直接使用`bootstrap.sh`下载。

```bash
curl -sSL https://raw.githubusercontent.com/hyperledger/fabric/master/scripts/bootstrap.sh -o bootstrap.sh
./bootstrap.sh 1.3.0 -s -b
```

## 方式4 直接 git pull

```bash
docker pull hyperledger/fabric-peer:1.3.0
docker tag hyperledger/fabric-peer:1.3.0 hyperledger/fabric-peer

docker pull hyperledger/fabric-orderer:1.3.0
docker tag hyperledger/fabric-orderer:1.3.0 hyperledger/fabric-orderer

docker pull hyperledger/fabric-ccenv:1.3.0
docker tag hyperledger/fabric-ccenv:1.3.0 hyperledger/fabric-ccenv

docker pull hyperledger/fabric-javaenv:1.3.0
docker tag hyperledger/fabric-javaenv:1.3.0 hyperledger/fabric-javaenv

docker pull hyperledger/fabric-tools:1.3.0
docker tag hyperledger/fabric-tools:1.3.0 hyperledger/fabric-tools

docker pull hyperledger/fabric-ca:1.3.0
docker tag hyperledger/fabric-ca:1.3.0 hyperledger/fabric-ca

docker pull hyperledger/fabric-couchdb:1.3.0
docker tag hyperledger/fabric-couchdb:1.3.0 hyperledger/fabric-couchdb

docker pull hyperledger/fabric-kafka:1.3.0
docker tag hyperledger/fabric-kafka:1.3.0 hyperledger/fabric-kafka

docker pull hyperledger/fabric-zookeeper:1.3.0
docker tag hyperledger/fabric-zookeeper:1.3.0 hyperledger/fabric-zookeeper
```

## 查看

```bash
docker images
```