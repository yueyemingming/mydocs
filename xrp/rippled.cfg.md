# rippled节点配置文件详解

### 文章目录

- [一、简介](https://blog.csdn.net/cljdsc/article/details/111657824#_1)
- [二、配置文件示例](https://blog.csdn.net/cljdsc/article/details/111657824#_13)
- [三、配置项说明](https://blog.csdn.net/cljdsc/article/details/111657824#_89)
- - [[server\]](https://blog.csdn.net/cljdsc/article/details/111657824#server_90)
  - [[port_rpc_admin_local\]](https://blog.csdn.net/cljdsc/article/details/111657824#port_rpc_admin_local_96)
  - [[port_peer\]](https://blog.csdn.net/cljdsc/article/details/111657824#port_peer_122)
  - [[port_ws_admin_local\]](https://blog.csdn.net/cljdsc/article/details/111657824#port_ws_admin_local_153)
  - [node_size](https://blog.csdn.net/cljdsc/article/details/111657824#node_size_172)
  - [node_db](https://blog.csdn.net/cljdsc/article/details/111657824#node_db_198)
  - [ledger_history](https://blog.csdn.net/cljdsc/article/details/111657824#ledger_history_254)
  - [database_path](https://blog.csdn.net/cljdsc/article/details/111657824#database_path_273)
  - [debug_logfile](https://blog.csdn.net/cljdsc/article/details/111657824#debug_logfile_286)
  - [sntp_servers](https://blog.csdn.net/cljdsc/article/details/111657824#sntp_servers_293)
  - [validators_file](https://blog.csdn.net/cljdsc/article/details/111657824#validators_file_305)
  - [rpc_startup](https://blog.csdn.net/cljdsc/article/details/111657824#rpc_startup_316)
  - [ssl_verify](https://blog.csdn.net/cljdsc/article/details/111657824#ssl_verify_327)
  - [信任节点列表文件](https://blog.csdn.net/cljdsc/article/details/111657824#_343)
- [四、测试网及开发网配置文件](https://blog.csdn.net/cljdsc/article/details/111657824#_362)
- - - [rippled.cfg 文件修改](https://blog.csdn.net/cljdsc/article/details/111657824#rippledcfg__365)
    - [validators.txt文件修改](https://blog.csdn.net/cljdsc/article/details/111657824#validatorstxt_391)



# 一、简介

- .cfg 是ripple节点的配置文件，文件名默认为 ripple.cfg
- 文档中以 # 开头的内容是注释
- 文档中以 [] 修饰一个配置项单元的名称
- 文档中需要空格的地方只允许一个空格，多于一个空格会识别不了

rippled.cfg配置示例：

- rippled.cfg配置示例1：https://github.com/ripple/rippled/blob/master/cfg/rippled-example.cfg
- rippled.cfg配置示例2：https://github.com/ripple/rippled/blob/8429dd67e60ba360da591bfa905b58a35638fda1/cfg/rippled-example.cfg#L1050-L1073
- rippled服务器无法启动，可能的配置原因以及修复方法：https://xrpl.org/server-wont-start.html#cannot-create-database-path
- chainsql配置文件详解（仅用于参考）：http://docs.chainsql.net/theory/cfg.html#id4

# 二、配置文件示例

```
#端口配置列表
[server]
port_rpc_admin_local
port_peer
port_ws_admin_local


#http端口配置，用于rpc请求
[port_rpc_admin_local]
port = 5005
ip = 0.0.0.0
admin = 127.0.0.1
protocol = http

#peer端口配置，用于p2p节点发现
[port_peer]
port = 51235
ip =  0.0.0.0
protocol = peer

#websocket端口配置
[port_ws_admin_local]
port = 6006
ip = 0.0.0.0
admin = 127.0.0.1
protocol = ws

# 缓存级别
[node_size]
medium


#区块数据存储配置，windows下用NuDB,Linux/Mac下用RocksDB
[node_db]
type=RocksDB
path=/opt/docker/ripple/data/rocksdb
open_files=2000
filter_bits = 12
cache_mb = 256
file_size_mb = 8
file_size_mult = 2
online_delete=200000000000
advisory_delete=0

#是否全节点
[ledger_history]
full

#sqlite数据库（存储区块头数据，交易概要数据）
[database_path]
/opt/docker/ripple/data

#日志目录
[debug_logfile]
/opt/docker/ripple/logs/debug.log

#时间服务器，用于不同节点单时间同步
[sntp_servers]
time.windows.com
time.apple.com
time.nist.gov
pool.ntp.org

#信任节点列表文件
[validators_file]
validators.txt

#日志级别，一般设置为warning级别
[rpc_startup]
{ "command": "log_level", "severity": "warning" }
[ssl_verify]
1
123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566676869707172
```

# 三、配置项说明

## [server]

端口配置列表

- 端口列表，ripple 会查找文件中具有与列表项相同名称的配置项，并用这些配置荐创建监听端口。
- 列表中配置项的名称不会影响功配置功能

## [port_rpc_admin_local]

- 字段内容：

```
[port_rpc_admin_local]
port = 5005  
ip = 0.0.0.0
admin = 127.0.0.1
protocol = http
12345
```

- 字段含义：

```
[port_rpc_admin_local]：http端口配置
port： 配置端口，此端口为rpc服务的端口
ip： 哪个ip可以连接这一端口，0.0.0.0 代表任意ip可以连接这一端口
admin： ripple中有一些命令只有拥有admin权限的ip才能调用，配置方法与 ip 相同
protocol： 协议名称，rippple中支持协议有 http,https,ws,wss,peer（此处为http，可调用rpc接口）
12345
```

注：rippled API方法分为“公共方法”和“管理方法”，要访问管理方法或公共方法的管理功能，必须在服务器的配置文件中标记为admin的端口和IP地址上连接到API，公共方法为ip字段的IP地址可以请求，管理方法为admin字段的IP地址可以请求

- 公共方法接口：https://xrpl.org/public-rippled-methods.html
- 管理方法接口：https://xrpl.org/admin-rippled-methods.html

## [port_peer]

- 字段内容：

```
[port_peer]
port = 51235
ip =  0.0.0.0
protocol = peer
1234
```

- 字段含义：

```
[port_peer]：peer端口配置，用于p2p节点发现，区块链节点间的数据通信
port： rippled服务器使用p2p协议的端口号，通常为51235
ip： 任何rippled服务器，按主机名或IP地址，0.0.0.0 代表任意ip可以连接这一端口
protocol： 协议名称，rippple中支持协议有 http,https,ws,wss,peer（此处为peer，可用于区块链节点间通信）
1234
```

注：向Ripple的一台公共服务器请求节点程序信息。

```
curl --insecure https://172.31.56.172:51235/crawl
1
```

例如：

```
rpc：curl --insecure https://s1.ripple.com:51235/crawl
http：GET https://localhost:51235/crawl
12
```

参考文献：https://xrpl.org/peer-crawler.html#request-format

## [port_ws_admin_local]

- 字段内容：

```
[port_ws_admin_local]
port = 6006
ip = 0.0.0.0
admin = 127.0.0.1
protocol = ws
12345
```

- 字段含义：

```
[port_ws_admin_local]：websocket端口配置
port： 配置端口，此端口为websocket服务的端口
ip： 哪个ip可以连接这一端口，0.0.0.0 代表任意ip可以连接这一端口
admin： ripple中有一些命令只有拥有admin权限的ip才能调用，配置方法与 ip 相同
protocol： 协议名称，rippple中支持协议有 http,https,ws,wss,peer（此处为ws，可调用websocket接口）
12345
```

## node_size

- 字段内容：

```
[node_size]
medium
12
```

node_size 用于设置和检索历史数据的客户端使用，应该根据服务器目前的可用内存进行设置，以下是建议设置的参数说明

| 可用内存 | node_size | 备注                                                         |
| -------- | --------- | ------------------------------------------------------------ |
| < 8GB    | tiny      | 不建议用于测试或生产服务器。如果在rippled.cfg配置文件中未指定值，则此参数为默认值 |
| 8 GB     | small     | 建议用于测试服务器                                           |
| 16 GB    | medium    | rippled-example.cfg文件使用此值（官方配置文件）              |
| 32 GB    | huge      | 推荐用于生产服务器                                           |

注：

- 每个node_size 都有对可用内存的相应要求。例如，如果设置node_size为huge，则应至少具有32 GB的可用内存，以帮助确保其rippled可以平稳运行
- 如果将node_size参数设置为无效值，则服务器无法启动，可能会出现以下错误

```
Terminating thread rippled: main: unhandled N5beast14BadLexicalCastE 'std::bad_cast'
1
```

参考文献：https://xrpl.org/capacity-planning.html#node-size

## node_db

- 字段内容：

```
[node_db]
type=RocksDB
path=/opt/docker/ripple/data/rocksdb
open_files=2000
filter_bits = 12
cache_mb = 256
file_size_mb = 8
file_size_mult = 2
online_delete=200000000000
advisory_delete=0
12345678910
```

- 字段含义：

```
[node_db] 
type：节点数据库类型，可以将该值设置为rocksdb或nudb
path：存放数据的位置
open_files：打开的文件列表，仅适用于RocksDB
filter_bits：仅适用于RocksDB
cache_mb：缓存，仅适用于RocksDB
file_size_mb：仅适用于RocksDB
file_size_mult：仅适用于RocksDB
online_delete：设置在线删除时保留的最新的ripple账本数量，如果使用自动删除（默认设置），则服务器通常会在账本累积到参数的两倍左右时运行删除，此值必须大于或等于ledger_history

advisory_delete：是否接受管理员手动删除账本数量，online_delete若要自动运行，则必须设置advisory_delete=0；参数值为1，就意味着可手动删除
1234567891011
```

注：节点数据库类型

- 可将type的值设置为RocksDB或NuDB
- 如果rippled作为验证运行的节点，应该只保留几天或更少的数据。Ripple建议使用RocksDB进行验证
- 在大多数情况下，请使用NuDB，因为即使磁盘上有大量数据，它的性能也是恒定的。但它需要快速的SSD
- 一般建议windows下用NuDB,Linux/Mac下用RocksDB
- 若使用RocksDB，rippled官方推荐推荐以下配置：

```
[node_db]
type=RocksDB
path=/opt/docker/ripple/data/rocksdb
open_files=2000
filter_bits = 12
cache_mb = 256
file_size_mb = 8
file_size_mult = 2
online_delete=200000000000
advisory_delete=0
12345678910
```

可根据自身情况调整存储目录（path），根据配置的需要调整online_delete和advisory_delete设置。

参考文献：

- 数据容量规划：https://xrpl.org/capacity-planning.html#more-about-using-rocksdb
- 配置在线删除：https://xrpl.org/configure-online-deletion.html#configure-online-deletion
- 配置手动删除：https://xrpl.org/configure-advisory-deletion.html
- 在线删除配置不生效：https://github.com/ripple/rippled/issues/2306

## ledger_history

- 字段内容：

```
[ledger_history]
full
12
```

指定本节点是否为保存全部历史，如果是，则修改为full,默认为256，意为保存最近256个区块。

注意：如果使用了online_delete，此值必须小于或等于online_delete ！！！

注：xrp并不需要完整的历史记录节点也可参与网络，验证交易或了解网络的当前状态。完整历史记录仅对了解过去发生的交易的结果或过去给定时间的分类帐状态有用。

参考文献：

- 配置完整历史记录：https://xrpl.org/configure-full-history.html
- 账本历史数据：https://xrpl.org/ledger-history.html#ledger-history
- online_delete与ledger_history关系：https://xrpl.org/online-deletion.html

## database_path

- 字段内容：

```
[database_path]
/opt/docker/ripple/data
12
```

此配置为要存储其他数据库数据的位置。（这包括带有配置数据的SQLite数据库，通常在[node_db]path字段上方一层。

参考文献：

- rippled配置：https://xrpl.org/build-run-rippled-ubuntu.html#2-configure-rippled
- 修复SQLite事务数据库页面大小问题：https://xrpl.org/fix-sqlite-tx-db-page-size-issue.html#migration-process

## debug_logfile

```
[debug_logfile]
/opt/docker/ripple/logs/debug.log
12
```

ripple日志文件保存路径

## sntp_servers

- 字段内容

```
[sntp_servers]
time.windows.com
time.apple.com
time.nist.gov
pool.ntp.org
12345
```

时间服务器，用于不同节点单时间同步，以上默认配置即可

## validators_file

- 字段内容

```
[validators_file]
validators.txt
12
```

包含受信任的验证者密钥或验证者列表发布者的文件，此文件应与rippled.cfg配置文件在同一级目录中

- ripple信任节点密钥替换：https://xrpl.org/blog/2019/labeling-the-internet-of-value.html?q=validators_file#validators

## rpc_startup

- 字段内容

```
[rpc_startup]
{ "command": "log_level", "severity": "warning" }
12
```

记录本地程序运行产生的日志级别，默认为：debug，此配置可减少磁盘空间以及磁盘IO使用率，若需要查看ripple故障原因，可设置更加详细的日志级别

日志级别分别为： trace（跟踪）, debug（调试）, info（信息）, warning（警告）, error（错误）, fatal （严重）

## ssl_verify

- 字段内容

```
[ssl_verify]
1
12
```

参数为：0或1

```
0：HTTPS客户端连接不会验证证书。 
1：将检查证书以获取HTTPS客户端连接。 

内部使用，一般填写为0
如果未指定，则此参数默认为1。
12345
```

## 信任节点列表文件

- 文件内容

```
# cat validators.txt 
[validator_list_sites]
https://vl.ripple.com
[validator_list_keys]
ED2677ABFFD1B33AC6FBC3062B71F1E8397C1505E1C42C64D11AD1B28FF73F4734
12345
```

这种需要有一个信任的服务器，该服务器对外公开发布信任度高的节点公钥列表，同时提供一个列表有效时间点。这种配置为远程配置。

- [validator_list_sites]：配置服务器获取信任列表的URL
- [validator_list_keys]：配置服务器的公钥用于验签
- 设置可信列表：https://xrpl.org/connect-your-rippled-to-the-xrp-test-net.html#2-set-your-trusted-validator-list
- ripple信任列表配置方式：https://blog.csdn.net/lc315yuhuofei/article/details/104251722

# 四、测试网及开发网配置文件

注意：部署测试网及开发网与主网的方式一样，仅在配置文件中修改以下内容即可

### rippled.cfg 文件修改

rippled.cfg 配置文件中添加以下配置：

```
#设置为测试网
[ips]
s.altnet.rippletest.net 51235

# 或设置为设置为开发网，根据个人节点类型进行选择
[ips]
s.devnet.rippletest.net 51235



[features]
NegativeUNL
123456789101112
```

features：要强制启用某个功能，[features]请在rippled.cfg文件中添加一节。在本节中，添加要启用的功能的简称，每行一个

NegativeUNL：此功能在rippled1.6.0版本中，可以在测试网络中使用，它提高了网络在某些类型的部分中断期间宣布结果为最终结果的能力

参考文献：

- NegativeUNL详解：https://xrpl.org/negative-unl.html#negative-unl

### validators.txt文件修改

validators.txt 文件中修改为以下配置，根据个人节点类型进行选择

```
测试网：
[validator_list_sites]
https://vl.altnet.rippletest.net

[validator_list_keys]
ED264807102805220DA0F312E71FC2C69E1552C9C5790F6C25E3729DEB573D5860

开发网：
[validator_list_sites]
https://vl.devnet.rippletest.net

[validator_list_keys]
EDDF2F53DFEC79358F7BE76BC884AC31048CFF6E2A00C628EAE06DB7750A247B12

主网：
[validator_list_sites]
https://vl.ripple.com

[validator_list_keys]
ED2677ABFFD1B33AC6FBC3062B71F1E8397C1505E1C42C64D11AD1B28FF73F4734
1234567891011121314151617181920
```

参考文献：

- xrp主网、测试网、开发网信任节点配置：https://xrpl.org/connect-your-rippled-to-the-xrp-test-net.html
- xrp主网、测试网、开发网浏览器：https://devnet.xrpl.org/
- xrp测试网、开发网水龙头：https://xrpl.org/xrp-testnet-faucet.html

以上就是ripple网络部署节点时的配置文件详解，因为ripple官方网站中的配置参数解释有时会令人感到晦涩难懂，所以小帅杰这次也是花了非常多的心思来整理这篇文章，以便让我们在部署rippled节点时，不再担心因为配置文件的问题导致部署报错。