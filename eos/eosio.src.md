# EOS系列 - 源码框架剖析（v1.7.2）

- [1.编译模块](#1编译模块)
  - [1.1 eos/scripts/eosio_build.sh](#11-eosscriptseosio_buildsh)
  - [1.2 eos/scripts](#12-eosscripts)
  - [1.3 eos/CMakeModules](#13-eoscmakemodules)
- [2.主程序模块](#2主程序模块)
  - [2.1 nodeos](#21-nodeos)
  - [2.2 cleos](#22-cleos)
  - [2.3 keosd](#23-keosd)
  - [2.4 eosio-launcher](#24-eosio-launcher)
- [3.依赖库模块](#3依赖库模块)
  - [3.2 appbase](#32-appbase)
  - [3.3 builtins](#33-builtins)
  - [3.4 chain](#34-chain)
  - [3.5 chainbase](#35-chainbase)
  - [3.6 fc](#36-fc)
  - [3.7 softfloat](#37-softfloat)
  - [3.8 testing](#38-testing)
  - [3.9 wasm-jit](#39-wasm-jit)
- [4.插件模块](#4插件模块)
  - [4.1 插件模板](#41-插件模板)
  - [4.2 基类插件](#42-基类插件)
    - [4.2.1 chain_plugin](#421-chain_plugin)
    - [4.2.2 net_plugin](#422-net_plugin)
    - [4.2.3 http_plugin](#423-http_plugin)
    - [4.2.4 wallet_plugin](#424-wallet_plugin)
    - [4.2.5 state_history_plugin](#425-state_history_plugin)
    - [4.3 派生类插件](#43-派生类插件)
      - [4.3.1 bnet_plugin](#431-bnet_plugin)
      - [4.3.2 faucet_testnet_plugin](#432-faucet_testnet_plugin)
      - [4.3.3 history_plugin](#433-history_plugin)
      - [4.3.4 http_client_plugin](#434-http_client_plugin)
      - [4.3.5 mongo_db_plugin](#435-mongo_db_plugin)
      - [4.3.6 producer_plugin](#436-producer_plugin)
      - [4.3.7 txn_test_gen_plugin](#437-txn_test_gen_plugin)
    - [4.4 封装类插件](#44-封装类插件)
- [5.智能合约模块 (在项目eosio.contracts中)](#5智能合约模块-在项目eosiocontracts中)
  - [5.1 系统合约层](#51-系统合约层)
    - [5.1.1 eosio.bios](#511-eosiobios)
    - [5.1.2 eosio.msig](#512-eosiomsig)
    - [5.1.3 eosio.sudo](#513-eosiosudo)
    - [5.1.4 eosio.system](#514-eosiosystem)
    - [5.1.5 eosio.token](#515-eosiotoken)
  - [5.2 依赖库层](#52-依赖库层)
- [6. 测试模块](#6-测试模块)
- [7. Docker工具模块](#7-docker工具模块)
- [8. 部署与激活模块](#8-部署与激活模块)
- [9. 文档说明模块](#9-文档说明模块)

eos项目主要可以分为以下八个模块：

- 编译模块
  - eos/scripts
  - eos/scripts/eosio_build.sh
  - eos/CMakeModules
- 主程序模块
  - eos/program/nodeos
  - eos/program/cleos
  - eos/program/keosd
  - eos/program/eosio-blocklog
  - eos/program/eosio-launcher
- 依赖库模块
  - eos/libraries/appbase
  - eos/libraries/builtins
  - eos/libraries/chain
  - eos/libraries/fc
  - eos/libraries/chainbase
  - eos/libraries/wasm-jit
- 插件模块
  - eos/plugins/chain_plugin
  - eos/plugins/http_plugin
  - eos/plugins/net_plugin
  - eos/plugins/wallet_plugin
  - eos/plugins/state_history_plugin
- 智能合约模块 (已从eos源码中分离,见eosio.contracts项目)
  - eosio.contracts/contracts/eosio.bios
  - eosio.contracts/contracts/eosio.msig
  - eosio.contracts/contracts/eosio.system
  - eosio.contracts/contracts/eosio.token
  - eosio.contracts/contracts/eosio.wrap
- 测试模块
  - eos/unittests
- Docker工具模块
  - eos/Docker
- 部署与激活模块
  - eos/tutorials
- 文档说明模块
  - eos/README.md
  - eos/LICENSE

## 1.编译模块

eos项目的整体编译、依赖库下载、代码编译等功能，依托于一个完整的脚本体系，主要包括以下三部分：

- eos/scripts
- eos/scripts/eosio_build.sh
- eos/CMakeModules

### 1.1 eos/scripts/eosio_build.sh

eos/scripts/eosio_build.sh是eos项目的主编译脚本，通过运行这个脚本，就可以实现对eos项目的一键编译，脚本运行过程中主要测试了编译所需的硬件与软件环境是否满足要求以及显示编译结果。

### 1.2 eos/scripts

eos/scripts目录中包含了项目编译所需的其他脚本文件，其中：

- abi_is_json.py负责将C++语言编写的智能合约编译成.abi文件，再将.abi文件编译成可执行文件;
- eosio_build_前缀的脚本文件分别是针对不同系统的编译子脚本;
- eosio-tn_前缀的脚本负责自动化运行，关闭节点。

### 1.3 eos/CMakeModules

由于eos项目是基于CMake等工具编译的，所以eos/CMakeModules中主要包含CMake编译所需要使用的一些配置信息。

## 2.主程序模块

eos项目的主程序源代码位于eos/program目录下，包含6个基本功能组件：

- eos/program/nodeos
- eos/program/cleos
- eos/program/keosd
- eos/program/eosio-blocklog
- eos/program/eosio-launcher

### 2.1 nodeos

eos/program/nodeos是eos项目的核心程序源代码，可以配置不同插件来运行不同类型节点。该进程主要负责提供区块生产，封装API接口和本地开发的功能;

### 2.2 cleos

eos/program/cleos是eos项目中与nodeos之间交互信息的命令行工具cleos的源代码，编译后与nodeos公开的REST API进行信息交互。

### 2.3 keosd

eos/program/keosd是eos项目中的内置钱包程序，配合钱包相关插件通过HTTP接口或RPC API与cleos进行通信，完成账户创建，转账等基本钱包功能。

### 2.4 eosio-launcher

eos/program/eosio-launcher是eos项目的P2P网络组成启动器源代码，该部分简化了nodeos节点组网的流程。

## 3.依赖库模块

eos项目的运行依托于许多依赖库，这些依赖库的基本信息位于eos/libraries目录中，包括：

- eos/libraries/appbase
- eos/libraries/builtins
- eos/libraries/chain
- eos/libraries/fc
- eos/libraries/chainbase
- eos/libraries/wasm-jit

### 3.2 appbase

eos/libraries/appbase提供了一个用于从一组插件构建应用程序的基本框架。该模块负责管理插件的生命周期，并确保所有插件按正确的顺序配置，初始化，启动和关闭。
该依赖库包含5个主要特征：

- 动态指定要加载的插件
- 自动加载依赖插件
- 插件可以指定命令行参数和配置文件选项
- 程序正常退出SIGINT和SIGTERM
- 最小依赖（Boost 1.60，c ++ 14）

### 3.3 builtins

eos/libraries/builtins中包含了eos项目编译过程中所需要的compiler-RT编译器（libgcc的替换库）的源代码，包括编译器本身以及相关功能函数的代码描述。

### 3.4 chain

可以说eos/libraries/chain包含有eos项目的核心内容，包括区块，区块链，merkle树等数据结构以及初始区块，控制器等关键算法，这一部分将在后续的教程中展开介绍。

### 3.5 chainbase

eos/libraries/appbase/chainbase中定义了保存eos区块链数据的数据库结构，可以说该数据库旨在满足区块链应用程序的苛刻要求，但适用于任何需要强大的事务数据库的应用程序。

### 3.6 fc

eos/libraries/fc是eos项目的细胞级模块，定义了eos项目中的基本变量数据结构，包括String，Time，Base系列编码等，在后需的具体代码分析中，我们将会遇到很多fc::前缀的引用，届时具体回顾。
在eos/libraries/fc/src目录中我们可以看到具体的代码实现

### 3.7 softfloat

eos/libraries/softfloat包含了一个Berkeley SoftFloat，即符合IEEE浮点运算标准二进制浮点的软件实现。

### 3.8 testing

eos/libraries/testing包含了几个测试实例，包括对区块链数据库的链接测试，P2P网络的链接测试等。

### 3.9 wasm-jit

eos/libraries/wasm-jit中包含了一个WebAssembly的独立VM。 它可以加载标准的二进制格式，也可以加载WebAssembly参考解释器定义的文本格式。 对于文本格式，它可以加载标准堆栈机器语法和参考解释器使用的老式AST语法以及所有测试命令。

## 4.插件模块

可以说eos项目中的节点程序依赖一个完整的插件体系：通过不同的插件组合来提供各种服务功能，如区块链查询，交易验证执行，打包区块，P2P网络通信等服务;应当补充的是，插件的全生命周期包括四个流程：注册、初始化、开启、停止，具体的执行算法将在后续文章展开介绍，本篇教程只介绍不同文件的所有功能。
eos项目的插件代码位于eos/plugins目录下，包括四个层次：

- 模板层
- 基类层
- 派生类层
- 封装层

### 4.1 插件模板

eos/plugins/template_plugin定义了eos项目中所有插件的模板：

### 4.2 基类插件

经总结，eos项目的插件体系中共有5个基类插件：

- eos/plugins/chain_plugin
- eos/plugins/http_plugin
- eos/plugins/net_plugin
- eos/plugins/wallet_plugin
- eos/plugins/state_history_plugin

这5个基类插件承载了eos插件体系中的基本功能：

#### 4.2.1 chain_plugin

eos/plugins/chain_plugin目录定义了链处理插件，这个插件承载了nodeos节点程序与区块链交互的基本功能，包括：

- 读取本地不可逆区块链基本信息;
- 设置本地链检查点;
- 设置本地链参数;
- 设置可逆区块数据库参数;
- 设置账户黑/白名单;
- 设置智能合约黑/白名单;
- 重载区块链初始状态文件;
- 删除、重写、替换本地区块链数据（包括开始、停止等动作）;

#### 4.2.2 net_plugin

eos/plugins/net_plugin目录定义了P2P网络插件，这个插件承载了eos系统的P2P网络中TCP/IP层相关功能，包括4个基本功能：

- 建立节点之间握手并互联;
- 监听/发送/接收新交易请求;
- 监听/发送/接收新区块请求;
- 验证接收数据合法性;

#### 4.2.3 http_plugin

eos/plugins/http_plugin目录定义了网络http插件，这个插件承载了eos系统的P2P网络中http层相关功能，包括3个基本功能：

- 监听/发送/接收新交易请求;
- 监听/发送/接收新区块请求;
- 验证接收数据合法性;

#### 4.2.4 wallet_plugin

eos/plugins/wallet_plugin目录定义了钱包插件，这个插件承载了nodeos节点程序与钱包交互的相关功能，包括3个基本功能：

- 创建/读取钱包文件;
- 设置unlock timeout时间;
- 密钥导入钱包;

- eos/plugins/wallet_plugin/wallet.cpp: 该文件实现了对钱包文件的基本功能，包括创建新钱包、导入密钥等;
- eos/plugins/wallet_plugin/wallet_manager.cpp：该文件实现了对钱包的管理功能，包括设置unlock超时时间、lock指定钱包等;
- eos/plugins/wallet_plugin/wallet_plugin.cpp：该文件对上述功能插件化，包括定义插件参数等，实现了nodeos节点程序通过调用插件处理钱包文件的功能;

#### 4.2.5 state_history_plugin

eos/plugins/state_history_plugin目录定义了历史记录查询插件，该插件承载了节点程序对本地链发起查询的相关功能，包括3个基本功能：

- 指定区块查询;
- 指定账户状态查询;
- 指定交易查询;

### 4.3 派生类插件

经总结，eos项目的插件体系中共有8个派生类插件，这些插件分别继承几个基类插件，实现指定功能。派生类插件包括：

- eos/plugins/bnet_plugin
- eos/plugins/faucet_testnet_plugin
- eos/plugins/history_plugin
- eos/plugins/http_client_plugin
- eos/plugins/mongo_db_plugin
- eos/plugins/producer_plugin
- eos/plugins/template_plugin
- eos/plugins/txn_test_gen_plugin

#### 4.3.1 bnet_plugin

eos/plugins/bnet_plugin目录定义了eos的P2P网络中不同节点之间同步各自本地链数据的算法，简单来讲，包括如下特征：

- 查找本地链上的最后一个区块ID;
- 如果本地产生新区块，则将该区块发送给其他节点;
- 如果本地不产生新区块，则将接受到的未确认交易发送给其他节点;

#### 4.3.2 faucet_testnet_plugin

eos/plugins/faucet_testnet_plugin目录定义了在测试网上快速建立测试帐号的插件，该插件为测试提供便利。

#### 4.3.3 history_plugin

eos/plugins/history_plugin目录定义了查询指定账户中指定公钥的所有历史交易记录。

#### 4.3.4 http_client_plugin

eos/plugins/http_client_plugin目录定义了eos网络http层响应请求，并做相应的安全验证的客户端插件。

#### 4.3.5 mongo_db_plugin

eos/plugins/mongo_db_plugin目录定义了保存并管理本地不可逆转区块链数据的MongoDB数据库基本配置插件。

#### 4.3.6 producer_plugin

eos/plugins/producer_plugin目录定义了区块生产节点的功能插件，其中功能包括：

- 生产/打包新区块数据;
- 对新区块签名;
- 对接收的区块进行验证，包括区块头合法性、签名合法性和交易合法性;

#### 4.3.7 txn_test_gen_plugin

eos/plugins/txn_test_gen_plugin定义了一个每秒自动产生指定数量的交易信息的插件，该插件主要用作对eos网络的吞吐量（TPS）测试。

### 4.4 封装类插件

该部分插件主要用于对上述几个特定插件进行封装，只暴露API，主要包括（不做详细介绍）：

- eos/plugins/chain_api_plugin;
- eos/plugins/db_size_api_plugin;
- eos/plugins/history_api_plugin;
- eos/plugins/net_api_plugin;
- eos/plugins/producer_api_plugin;
- eos/plugins/wallet_api_plugin;

## 5.智能合约模块 (在项目eosio.contracts中)

EOS项目的基本功能是通过系统合约提供的，用户可以调用已经部署上链的智能合约实现特定的功能，也可以通过C++语言自行编辑.cpp智能合约，并通过系统提供的eosiocpp编译器将.cpp、.hpp文件编译成.wasm和.abi文件部署上链，下面简单介绍一下eos项目中智能合约文件夹下的各个合约文件功能，文件结构如下图。
EOS项目的智能合约代码位于eos/contracts目录下，包括三个层次：

- 系统合约层；
- 依赖库层；
- 测试与示例层。

### 5.1 系统合约层

总结下来，EOS项目共有5个系统级智能合约：

- eosio.contracts/contracts/eosio.bios
- eosio.contracts/contracts/eosio.msig
- eosio.contracts/contracts/eosio.system
- eosio.contracts/contracts/eosio.token
- eosio.contracts/contracts/eosio.wrap

#### 5.1.1 eosio.bios

eosio.contracts/contracts/eosio.bios合约用于启动EOS的P2P网络，该合约可以直接控制其他帐户的资源分配并访问其他特权API调用，具体启动流程如下：

- 初始启动节点部署该合约，并设置所需的参数；
- 待连接节点通过初始启动节点的地址与之相连；
- 初始启动节点调用bios合约，为待连接节点设置权限。

#### 5.1.2 eosio.msig

eosio.contracts/contracts/eosio.msig目录定义了多签系统合约，实现了一个多签功能，由于EOS要求系统的每一次更新都需要出块节点完成一次多签，当签名数达到2/3+1时，更新才能生效。所以出块节点可以调用这个合约实现多签功能。

#### 5.1.3 eosio.sudo

eosio.contracts/contracts/eosio.sudo合约实现了创建EOS系统中root账户的功能，用于修改系统代码与更新合约。

#### 5.1.4 eosio.system

eosio.contracts/contracts/eosio.system目录可以算是EOS系统合约中最重要的部分，实现了EOS项目的所有基本功能：

- 创建新账户；
- 部署智能合约；
- 交易RAM；
- 抵押获取资源（net、cpu）；
- 投票；
- 领取节点奖励；

#### 5.1.5 eosio.token

eos/contracts/eosio.token目录实现了发行token的功能，可以说EOS本身代币或后续基于EOS的代币都是通过该合约发行的，主体函数包括：

- 发行新代币；
- 初始分发；
- 转账；
- 查询余额。

### 5.2 依赖库层

上述5个系统合约的实现离不开一个庞大的依赖库，其中包括数据的定义（account、asset等）、还包括一些常用函数（权限管理、序列化等），我们将在后续的算法分析中结合特定示例详细展开。

## 6. 测试模块

eos/unittests

EOS提供了一些测试文件，供用户测试节点是否运行正常，其中测试分两大类：

- 对链功能的测试：包括与区块链之间的数据交互，transaction分发等；
- 对网络层的测试：包括P2P网络传输功能、cleos与nodeos之间的通信等；

## 7. Docker工具模块

eos/Docker

EOS允许通过Docker运行节点或钱包，eos/Docker文件夹下包含了一些通过Docker启动并运行程序的必要文件：

## 8. 部署与激活模块

eos/tutorials

## 9. 文档说明模块

最后一章，我们罗列一下EOS给出的一些说明类文档：

- eos/README.md：节点部署等说明；
- eos/LICENSE：版本号与许可文件；
- eos/tutorials目录：关于主网启动与exchange合约使用教程。
