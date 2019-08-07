# eos 同步数据及支持mongodb存储节点搭建

两台主机，第一台上跑出块节点，第二台上跑同步节点数据

## 1. 启动第一个节点，用来出块

```bash
nodeos -e -p eosio \
--p2p-listen-endpoint=0.0.0.0:9876 \
--http-server-address=0.0.0.0:8888 \
--plugin eosio::producer_plugin \
--plugin eosio::chain_api_plugin \
--plugin eosio::http_plugin \
--plugin eosio::history_plugin \
--plugin eosio::history_api_plugin \
--access-control-allow-origin='*' \
--contracts-console \
--http-validate-host=false \
--verbose-http-errors >> /data/nodeos.log 2>&1 &
```

参数 | 描述
:--- | :---
-p eosio | 代表eosio系统账户出块
--p2p-listen-endpoint=0.0.0.0:9876 | p2p节点数据同步服务

## 2. 启动第二个节点，同步数据及支持mongodb存储节点搭建

若要支持mongodb持久化存储数据，需要在编译eosio时添加上支持mongodb plugin才行。编译命令如下 :  

### 2.1 编译eosio

```bash
./script/eosio_build.sh -m
./script/eosio_install.sh
```

### 2.2 安装启动mongodb

- [安装启动mongodb](../../database/mongodb/mongo.install.md)

### 2.3 启动节点

```bash
nodeos -e \
--p2p-peer-address=192.168.1.123:9876 \
--http-server-address=0.0.0.0:8888 \
--plugin eosio::chain_api_plugin \
--plugin eosio::http_plugin \
--plugin eosio::history_plugin \
--plugin eosio::history_api_plugin \
--plugin eosio::mongo_db_plugin \
--mongodb-uri=mongodb://127.0.0.1:27017/eos \
--mongodb-filter-on=eosio.token:: \
--chain-state-db-size-mb=2048 \
--access-control-allow-origin='*' \
--filter-on='*' \
--contracts-console \
--http-validate-host=false \
--verbose-http-errors >> /data/nodeos.log 2>&1 &
```

参数 | 描述
:--- | :---
-p eosio | 代表eosio系统账户出块
--p2p-listen-endpoint=0.0.0.0:9876 | p2p节点数据同步服务
--plugin eosio::mongo_db_plugin | 启动mongodb插件
--mongodb-uri=mongodb://127.0.0.1:27017/eos | 连接mongodb服务。
--mongodb-filter-on=eosio.token:: | 支持查询eosio.token这个合约的操作历史

- [坑:generic server error](../../database/mongodb/mongo.keng1.md)

### 2.4 查看mongodb数据

```bash
mongo localhost:27017

> show dbs
admin  0.000GB
eos    0.024GB
local  0.000GB
> use eos
switched to db eos
> show tables
pub_keys
block_states
transaction_traces
blocks
transactions
action_traces
account_controls
accounts
> db.accounts.find()
{ "_id" : ObjectId("5d4a48c77b284e1551607d22"), "name" : "eosio", "createdAt" : ISODate("2019-08-07T03:43:03.502Z") }
> db.accounts.find({name:{$eq:'eosio'}})
{ "_id" : ObjectId("5d4a48c77b284e1551607d22"), "name" : "eosio", "createdAt" : ISODate("2019-08-07T03:43:03.502Z") }
>
```

## mongodb插件的其他参数

参数 | 描述
:--- | :---
--mongodb-store-blocks | 存储所有块数据, 默认 `=1` 开启
--mongodb-store-block-states | 存储所有块状态, 默认 `=1` 开启
--mongodb-store-transactions | 存储所有交易, 默认 `=1` 开启
--mongodb-store-transaction-traces | 存储所有交易痕迹, 默认 `=1` 开启
--mongodb-store-action-traces | 存储所有action痕迹, 默认 `=1` 开启

> --mongodb-store-* **默认都是开启 **  
> --mongodb-filter-* **目前默认只支持action_traces**

> 建议头两项设为0，禁掉，这些区块数据造成占用空间最大，不停地往mongodb里写入，同时造成cpu基本满负荷的跑。
