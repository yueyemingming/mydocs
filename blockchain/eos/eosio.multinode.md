# EOS多节点环境配置(搭建自己的EOS主网)

本文使用了四台同网段的主机，第一台做为eosio创世用户使用，另外三台做为出块者节点使用，最终实现了EOS多主机多节点的配置。

最后EOSIO创世用户不再出块，由选举出来的各个节点轮流出块，下面将介绍具体的操作流程。

- EOS versio: 1.0.8
- system: ubuntu 18.04
- 四台服务器节点名称与对应IP：

主机名 | ip
:--- | :---
eosio | 192.168.1.120
node1 | 192.168.1.121
node2 | 192.168.1.122  
node3 | 192.168.1.123  

## 1. 安装EOS

### 1.1 编译安装eos

请查看[安装和运行单节点](eosio.start.md)

### 1.2 修改智能合约代码

```bash
git clone https://github.com/EOSIO/eosio.contracts --recursive
vim eosio.contracts/eosio.system/voting.cpp +80
```

```cpp
#将21改成3
top_producers.reserve(3);
```

## 2. 修改配置文件

### 2.1 对四台主机都在命令行先运行nodeos与keosd，让其产生各自的配置文件

- 配置文件路径一般为：~/.local/share/eosio/nodeos/config/config.ini

### 2.2 为三个用户创建三组密钥对

```bash
cleos create key
```

- account node1:  
  Private key: 5KXAsA1nizwEYAaWFA2uBUQqeHNhYA6hZ3dZ3tGb3X6aeW58UXF
  Public key: EOS5nquErGx97QiN6m5DspwLU8wNLmVAm78ccpzV6Ht4nfcJhepyj
- account node2:  
  Private key: 5JSxqFojAkd2HARaxN5rFniK6oEu8YEZ1EcabSoFMhqxTQphKvF
  Public key: EOS5VmHFnwVyiFurmFLw387xRpgXEnyF1yxqZs6JLtVwU1hv9gaEb
- account node3:  
  Private key: 5KHmyWSp6dhEoGRxx1wxAK7kSvtAAy7WDmumMLK3UNg6cqTCWYm
  Public key: EOS8UXVwDadaS8pLDjQ2vRa6Jaqy5k1rqpMHUNCFqzeDF4KfoguJe

### 2.3 修改创世节点配置文件

- eosio sign的密钥对（被记录在./tutorials/bios-boot-tutorial/genesis.json的initial_key里面）  
  EOS8Znrtgwt8TfpmbVpTKvA2oB8Nqey625CLN8bCN3TEbgx86Dsvr
  5K463ynhZoCDDa4RDcr63cUwWLTnKqmdcoTKTHBjqoKfv4u5V7p
- eosio key的密钥对（被记录在默认的eosio配置文件的signature-provider里面）  
  EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
  5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3

> 要改这俩秘钥的话需要同步更改配置文件和创世节点里对应的密钥对

```ini
p2p-server-address = 192.168.1.120:9876
p2p-peer-address = 192.168.1.123:9876
p2p-peer-address = 192.168.1.122:9876
p2p-peer-address = 192.168.1.121:9876
enable-stale-production = true
producer-name = eosio
unlock-timeout = 90000
plugin = eosio::producer_plugin
plugin = eosio::chain_api_plugin
plugin = eosio::chain_plugin
plugin = eosio::http_plugin
plugin = eosio::http_client_plugin
plugin = eosio::bnet_plugin
plugin = eosio::net_plugin
plugin = eosio::net_api_plugin
plugin = eosio::history_api_plugin
plugin = eosio::history_plugin
```

4、修改其他节点配置文件

```ini
p2p-server-address = 192.168.1.122:9876
p2p-peer-address = 192.168.1.123:9876
p2p-peer-address = 192.168.1.120:9876
p2p-peer-address = 192.168.1.121:9876
enable-stale-production = false
producer-name = node2
signature-provider = EOS5VmHFnwVyiFurmFLw387xRpgXEnyF1yxqZs6JLtVwU1hv9gaEb=KEY:5JSxqFojAkd2HARaxN5rFniK6oEu8YEZ1EcabSoFMhqxTQphKvF
plugin = eosio::chain_api_plugin
plugin = eosio::history_api_plugin
plugin = eosio::chain_plugin
plugin = eosio::history_plugin
plugin = eosio::net_plugin
plugin = eosio::net_api_plugin
```

> 不同的节点 p2p-server-address 和 signature-provider 需要改动，对应不同用户的IP和秘钥

## 3. 创建钱包，导入秘钥

### 3.1 创建并解锁钱包

```bash
cleos wallet create
  Creating wallet: default
  Save password to use in the future to unlock this wallet.
  Without password imported keys will not be retrievable.
  "PW5HwJAUXcgD1veDewfEAJAj2MGmfYfCc19Pu5srsnMLhCvFpRQGT"

cleos wallet unlock --password PW5HwJAUXcgD1veDewfEAJAj2MGmfYfCc19Pu5srsnMLhCvFpRQGT
```

### 3.2 导入三个普通用户和创世节点eosio（eosio key ,eosio sign）的秘钥

```bash
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 wallet import 5KXAsA1nizwEYAaWFA2uBUQqeHNhYA6hZ3dZ3tGb3X6aeW58UXF
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 wallet import 5JSxqFojAkd2HARaxN5rFniK6oEu8YEZ1EcabSoFMhqxTQphKvF
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 wallet import 5KHmyWSp6dhEoGRxx1wxAK7kSvtAAy7WDmumMLK3UNg6cqTCWYm
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 wallet import 5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 wallet import 5K463ynhZoCDDa4RDcr63cUwWLTnKqmdcoTKTHBjqoKfv4u5V7p
```

> 最后两个秘钥必须导入，不然会提示各种权限不足的错误

## 4. 创建九个系统账户

- [eos 9大系统账户名称及作用](eosio.sysaccount.md)

```bash
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 create account eosio eosio.bpay EOS8Znrtgwt8TfpmbVpTKvA2oB8Nqey625CLN8bCN3TEbgx86Dsvr
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 create account eosio eosio.msig EOS8Znrtgwt8TfpmbVpTKvA2oB8Nqey625CLN8bCN3TEbgx86Dsvr
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 create account eosio eosio.names EOS8Znrtgwt8TfpmbVpTKvA2oB8Nqey625CLN8bCN3TEbgx86Dsvr
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 create account eosio eosio.ram  EOS8Znrtgwt8TfpmbVpTKvA2oB8Nqey625CLN8bCN3TEbgx86Dsvr
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 create account eosio eosio.ramfee EOS8Znrtgwt8TfpmbVpTKvA2oB8Nqey625CLN8bCN3TEbgx86Dsvr
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 create account eosio eosio.saving EOS8Znrtgwt8TfpmbVpTKvA2oB8Nqey625CLN8bCN3TEbgx86Dsvr
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 create account eosio eosio.stake EOS8Znrtgwt8TfpmbVpTKvA2oB8Nqey625CLN8bCN3TEbgx86Dsvr
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 create account eosio eosio.token EOS8Znrtgwt8TfpmbVpTKvA2oB8Nqey625CLN8bCN3TEbgx86Dsvr
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 create account eosio eosio.vpay EOS8Znrtgwt8TfpmbVpTKvA2oB8Nqey625CLN8bCN3TEbgx86Dsvr
```

## 5. 加载四大合约，发行代币

```bash
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 set contract eosio.token eosio.contracts/eosio.token/
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 set contract eosio.msig eosio.contracts/eosio.msig/
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 push action eosio.token create '["eosio", "1000000000.0000 SYS"]' -p eosio.token
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 push action eosio.token issue '["eosio", "1000000000.0000 SYS", "memo"]' -p eosio
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 set contract eosio eosio.contracts/eosio.system/
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 push action eosio setpriv '["eosio.msig", 1]' -p eosio@active
```

## 6. 创建普通用户

加载了eosio.system之后就不能使用create account命令了，只能用system newaccount创建账户

```bash
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 system newaccount --transfer eosio node1 EOS5nquErGx97QiN6m5DspwLU8wNLmVAm78ccpzV6Ht4nfcJhepyj --stake-net "100000000.0000 SYS" --stake-cpu "100000000.0000 SYS" --buy-ram "20000.0000 SYS"
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 transfer eosio node1 "20000.0000 SYS"
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 system newaccount --transfer eosio node2 EOS5VmHFnwVyiFurmFLw387xRpgXEnyF1yxqZs6JLtVwU1hv9gaEb --stake-net "100000000.0000 SYS" --stake-cpu "100000000.0000 SYS" --buy-ram "20000.0000 SYS"
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 transfer eosio node2 "20000.0000 SYS"
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 system newaccount --transfer eosio node3 EOS8UXVwDadaS8pLDjQ2vRa6Jaqy5k1rqpMHUNCFqzeDF4KfoguJe --stake-net "100000000.0000 SYS" --stake-cpu "100000000.0000 SYS" --buy-ram "20000.0000 SYS"
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 transfer eosio node3 "20000.0000 SYS"
```

## 7. 注册节点候选人

```bash
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 system regproducer node1 EOS5nquErGx97QiN6m5DspwLU8wNLmVAm78ccpzV6Ht4nfcJhepyj
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 system regproducer node2 EOS5VmHFnwVyiFurmFLw387xRpgXEnyF1yxqZs6JLtVwU1hv9gaEb
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 system regproducer node3 EOS8UXVwDadaS8pLDjQ2vRa6Jaqy5k1rqpMHUNCFqzeDF4KfoguJe
```

可以用下列命令查看节点候选人：

```bash
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 system listproducers
```

## 8. 给创建的账户转账

```bash
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 push action eosio.token transfer '["eosio", "node1","1000.0000 SYS","vote"]' -p eosio
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 push action eosio.token transfer '["eosio", "node2","1000.0000 SYS","vote"]' -p eosio
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 push action eosio.token transfer '["eosio", "node3","1000.0000 SYS","vote"]' -p eosio
```

## 9. 用命令进行投票（自己给自己投票）

所有代币的15%进行投票后eosio会停止出块，主网会自动启动。在这里我们的代币数据数量总共是10亿，那么我们的主网启动需要1.5亿质押的代币进行投票后才能启动，质押的代币计算方式：staked = cpu + net：

```bash
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 system voteproducer prods node1 node1
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 system voteproducer prods node2 node2
cleos --wallet-url http://127.0.0.1:8900 --url http://192.168.1.120:8888 system voteproducer prods node3 node3
```

过一段时间后就能看到节点轮流产块，创世节点只需同步块，如下所示

## 10. 更改节点出块数目

按上面的步骤可以让节点轮流出块了，但是每个节点出块个数是12，而不是像官网白皮书说的那样是6个，修改下相关代码就可以让节点出块个数变为6。

```bash
vim libraries/chain/include/eosio/chain/config.hpp +99
```

```cpp
//const static int producer_repetitions = 12;
const static int producer_repetitions = 6;
```
