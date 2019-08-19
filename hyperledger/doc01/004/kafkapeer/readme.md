# Kafka模式 - 生产环境

为了能在正式环境中稳定运行，需要对排序（orderer）服务采用集群方式，Hyperledger Fabric采用kafka方式实现排序（orderer）服务的集群，kafka模块被认为是半中心化结构。

Kafka模式通过Kafka集群和zookeeper集群保证数据的一致性，实现排序功能。

Kafka模式由排序（orderer）服务、kafka集群和zookeeper集群组成。每个排序(orderer)服务相互之间不通信，只与kafka集群通信，kafka集群与zookeeper相互连接。

本例部署由 `2orderer + 3kafka + 2zookeeper + org1(peer0,peer1) + org2(peer0,peer1)` 组成，共需六台服务器，配置如下：

名称 | 地址
:--- |:---
peer0.org1 | 10.100.194.211
peer1.org1 | 10.100.194.212
peer0.org2 | 10.100.194.213
peer1.org2 | 10.100.194.214
orderer0 | 10.100.194.215
kafka0 | 10.100.194.215
zookeeper0 | 10.100.194.215
orderer1 | 10.100.194.216
kafka1 | 10.100.194.216
zookeeper1 | 10.100.194.216
kafka2 | 10.100.194.214

结构图如下：

![001.png](001.png)

## 1. orderer0 准备相关文件，及全节点同步

### 1.1 **请先 [搭建基础环境](../../../build.md)**

### 1.2 组织基础文件

文件或目录 | 含义
:--- | :---
[~/kafkapeer/configtx.yaml](configtx.yaml) | 链的公私钥和证书信息配置文件
[~/kafkapeer/crypto-config.yaml](crypto-config.yaml) | 链的创世区块和通道配置文件
[~/kafkapeer/channel-artifacts](channel-artifacts) | 存放公私钥、证书、创世区块的目录
[~/kafkapeer/zookeeper0.yaml](zookeeper0.yaml) | zookeeper0容器配置文件
[~/kafkapeer/kafka0.yaml](kafka0.yaml) | kafka0容器配置文件
[~/kafkapeer/orderer0.yaml](orderer0.yaml) | orderer0容器配置文件
[~/kafkapeer/zookeeper1.yaml](zookeeper1.yaml) | zookeeper1容器配置文件
[~/kafkapeer/kafka1.yaml](kafka1.yaml) | kafka1容器配置文件
[~/kafkapeer/orderer1.yaml](orderer1.yaml) | orderer1容器配置文件
[~/kafkapeer/kafka2.yaml](kafka2.yaml) | kafka2容器配置文件
[~/kafkapeer/peer0.org1.yaml](peer0.org1.yaml) | peer0.org1容器配置文件
[~/kafkapeer/peer1.org1.yaml](peer1.org1.yaml) | peer1.org1容器配置文件
[~/kafkapeer/peer0.org2.yaml](peer0.org2.yaml) | peer0.org2容器配置文件
[~/kafkapeer/peer1.org2.yaml](peer1.org2.yaml) | peer1.org2容器配置文件
[~/kafkapeer/chaincode/go/test.go](chaincode/go/test.go) | 测试代码

#### ! 服务器以root用户登录，将此目录拷贝到服务器的家目录中。

### 1.3 生成公私钥、证书、创世区块、通道配置区块

* **生成公私钥和证书**

```bash
cd ~/kafkapeer/
cryptogen generate --config=crypto-config.yaml
```

* **生成创世区块**

```bash
configtxgen -profile TwoOrgsOrdererGenesis -outputBlock channel-artifacts/genesis.block
```

* **生成通道配置区块**

```bash
configtxgen -profile TwoOrgsChannel -outputCreateChannelTx channel-artifacts/mychannel.tx -channelID mychannel
cd ~
```

### 1.4 全节点同步

```bash
scp -r kafkapeer root@10.100.194.211:~
scp -r kafkapeer root@10.100.194.212:~
scp -r kafkapeer root@10.100.194.213:~
scp -r kafkapeer root@10.100.194.214:~
#scp -r kafkapeer root@10.100.194.215:~  ## orderer0使用此服务器
scp -r kafkapeer root@10.100.194.216:~
```

## 2. 集群启动

! **分别在各自服务节点上启动**

* **Zookeeper集群启动**

| node | ip | script |
| :--- | :--- | :--- |
| zookeeper0 | 10.100.194.215 | `docker-compose -f ~/kafkapeer/zookeeper0.yaml up -d` |
| zookeeper1 | 10.100.194.216 | `docker-compose -f ~/kafkapeer/zookeeper1.yaml up -d` |

* **Kafka集群启动**

| node | ip | script |
| :--- | :--- | :--- |
| kafka0 | 10.100.194.215 | `docker-compose -f ~/kafkapeer/kafka0.yaml up -d` |
| kafka1 | 10.100.194.216 | `docker-compose -f ~/kafkapeer/kafka1.yaml up -d` |
| kafka2 | 10.100.194.214 | `docker-compose -f ~/kafkapeer/kafka2.yaml up -d` |

* **Orderer集群启动**

| node | ip | script |
| :--- | :--- | :--- |
| orderer0 | 10.100.194.215 | `docker-compose -f ~/kafkapeer/orderer0.yaml up -d` |
| orderer1 | 10.100.194.216 | `docker-compose -f ~/kafkapeer/orderer1.yaml up -d` |

* **Peer节点启动**

| node | ip | script |
| :--- | :--- | :--- |
| peer0.org1 | 10.100.194.211 | `docker-compose -f ~/kafkapeer/peer0.org1.yaml up -d` |
| peer1.org1 | 10.100.194.212 | `docker-compose -f ~/kafkapeer/peer1.org1.yaml up -d` |
| peer0.org2 | 10.100.194.213 | `docker-compose -f ~/kafkapeer/peer0.org2.yaml up -d` |
| peer1.org2 | 10.100.194.214 | `docker-compose -f ~/kafkapeer/peer1.org2.yaml up -d` |

## 3. peer0.org1 创建通道 `mychannel.block`，及其他peers同步

* **进入容器**

```bash
docker exec -it cli bash
```

* **创建链接orderer0的channel**

```bash
ORDERER_CA=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/ordererOrganizations/example.com/orderers/orderer0.example.com/msp/tlscacerts/tlsca.example.com-cert.pem
peer channel create -o orderer0.example.com:7050 -c mychannel -f channel-artifacts/mychannel.tx --tls --cafile $ORDERER_CA
```

* **保存mychannel.block**

```bash
exit
docker cp cli:/opt/gopath/src/github.com/hyperledger/fabric/peer/mychannel.block ~/kafkapeer/
```

* **mychannel.block拷贝到其他所有peer服务器中**

```bash
scp ~/kafkapeer/mychannel.block root@10.100.194.212:~/kafkapeer/
scp ~/kafkapeer/mychannel.block root@10.100.194.213:~/kafkapeer/
scp ~/kafkapeer/mychannel.block root@10.100.194.214:~/kafkapeer/
```

## 4. 其他各peer加入通道中

* **拷贝mychannel.block到各个peer cli容器中**

```bash
docker cp ~/kafkapeer/mychannel.block cli:/opt/gopath/src/github.com/hyperledger/fabric/peer/
```

* **各个peer分别加入Channel**

```bash
docker exec -it cli bash
peer channel join -b mychannel.block
```

## 5. 测试智能合约

! **本例，在peer0.org1上初始化智能合约的a和b变量的数值，在peer1.org1上查询合约。**

### 5.1 peer0.org1和peer1.org1上分别 安装智能合约

```bash
docker exec -it cli bash
peer chaincode install -n mycc -p github.com/hyperledger/fabric/kafkapeer/chaincode/go/ -v 1.0
```

### 5.2 peer0.org1 调用智能合约

* 初始化数据为a为100，b为200。

```bash
ORDERER_CA=/opt/gopath/src/github.com/hyperledger/fabric/peer/crypto/ordererOrganizations/example.com/orderers/orderer0.example.com/msp/tlscacerts/tlsca.example.com-cert.pem
peer chaincode instantiate -o orderer0.example.com:7050 --tls --cafile $ORDERER_CA -C mychannel -n mycc -v 1.0 -c '{"Args":["init","a","100","b","200"]}' -P "OR ('Org1MSP.peer','Org2MSP.peer')"
```

* 查询a，显示100

```bash
peer chaincode query -C mychannel -n mycc -c '{"Args":["query","a"]}'
```

* 进行a向b转10交易

```bash
peer chaincode invoke --tls --cafile $ORDERER_CA -C mychannel -n mycc -c '{"Args":["invoke","a","b","10"]}'
```

* 查询b，显示210

```bash
peer chaincode query -C mychannel -n mycc -c '{"Args":["query","b"]}'
```

### 5.3 peer1.org1 查询智能合约

* 查询b，显示210

```bash
docker exec -it cli bash
peer chaincode query -C mychannel -n mycc -c '{"Args":["query","b"]}'
```

> 如果是阿里云，部署过程中遇到的坑，请查看**[部署时阿里云存在巨大的坑](../../../aliyun.keng.md)**