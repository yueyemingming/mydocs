# XRP节点部署

目录

- XRP节点部署
  - 准备
    - [硬软件配置(建议)](javascript:void())
    - [安装Rippled服务](javascript:void())
  - 一. 以Stock Server模型运行
    - [在何种情况下运行此模式](javascript:void())
  - 二 .以 Validator模式运行
    - [在何种情况下运行此模式](javascript:void())
    - [ripple集群服务器设置](javascript:void())
    - [生产服务器搭建](javascript:void())
    - [域验证](javascript:void())
    - [测试服务器搭建](javascript:void())
    - [配置历史区块数据](javascript:void())
    - [启用公共签名](javascript:void())
  - 三. 以独立模式(stand-alone)模式运行
    - [以创世节点运行](javascript:void())
    - [从已知账本运行](javascript:void())
  - 四.命令行详解
    - [可用模式](javascript:void())
    - [守护进程模式指令](javascript:void())
    - [独立模式指令](javascript:void())
    - [客户端模式](javascript:void())
    - [单元测试](javascript:void())

# XRP节点部署

`rippled`服务器软件可以根据其配置以多种模式运行，包括：

- `库存服务器(stock server)` - 使用分类帐的本地副本跟随网络
- `验证服务器(validating server)` - 简称验证器(`validator`) - 参与共识（并完成库存服务器所做的一切）。
- `独立模式下(stand-alone mode)`的rippled server - 用于测试 (不与其他rippled节点通信, 私有节点)

还可以将`rippled`可执行文件作为`客户端`应用程序运行，以便在本地访问rippledAPI (在这种情况下，相同二进制文件的两个实例可以并行运行, 一个作为服务端, 另一个作为客户端短暂运行, 然后终止)

## 准备

`rippled`XRP分类帐网络中的每台服务器执行网络的所有交易处理工作。未知数量何时会达到最大网络容量。因此，生产`rippled`服务器的基准硬件应该与Ripple的[性能测试](javascript:void())中使用的类似。

### 硬软件配置(建议)

为了在生产环境中获得最佳性能，建议节点的服务器满足如下配置(裸机)：

```
操作系统：Ubuntu 16.04+
CPU：Intel Xeon 3+ GHz处理器，支持4核和超线程
磁盘：SSD
内存：
    用于测试：8GB +
    对于生产：32GB
网络：主机上具有千兆网络接口的企业数据中心网络
```

#### SSD存储

SSD存储应支持几千个读写IOPS。Ripple工程师观察到的每秒最大读写次数超过每秒10,000次（在重度使用的公共服务器集群中）以及每秒超过7,000次写入（在专用性能测试中）。
CPU利用率和虚拟化

Ripple性能工程已经确定裸机服务器实现最大吞吐量。但是，管理程序很可能会导致性能下降最小化。

#### 网络

对于当前的交易量，最低要求大致为2Mbps传输和2Mbps接收。但是，在提供历史分类帐和交易报告时，这些数据可能会突然传输至100MBps。当一个rippled服务器最初启动时，它可能会突然超过20Mbps的接收。

#### 存储

建议估计存储容量大概为每天大约12GB的数据与NuDB保持联机。RocksDB每天需要8GB左右。但是，每天的数据随网络中的活动而变化。应该提供额外的容量来为未来的增长做准备。截至（2018-03-01），所有XRP账本记录大约为6.8TB。

#### RAM内存

内存要求主要是`node_size`配置设置和检索历史数据的客户端使用。如前所述，生产服务器应该使性能最大化并将此参数设置为`huge`。可以将`node_size`参数设置得较低以减少内存的开销(测试的时候使用)。测试可以使用8GB的内存。

### 安装Rippled服务

#### 在Ubuntu上安装

1. 安装yum-utils和alien：

   ```
   $ sudo apt-get update
   $ sudo apt-get install yum-utils alien
   ```

2. 安装Ripple RPM存储库：

   ```
   $ sudo rpm -Uvh https://mirrors.ripple.com/ripple-repo-el7.rpm
   ```

3. 下载`rippled`软件包：

   ```
   $ yumdownloader --enablerepo=ripple-stable --releasever=el7 rippled
   ```

4. 验证`rippled`软件包上的签名：

   ```
   $ sudo rpm --import https://mirrors.ripple.com/rpm/RPM-GPG-KEY-ripple-release && rpm -K rippled*.rpm
   ```

5. 安装`rippled`软件包：

   ```
   sudo alien -i --scripts rippled*.rpm && rm rippled*.rpm
   ```

6. 将`rippled`服务配置为开机启动：

   ```
   sudo systemctl enable rippled.service
   ```

7. 开启 ripple 节点(默认是守护进程)

   ```
   sudo systemctl start rippled.service
   ```

#### 后续

`rippled`与网络的其他部分同步需要几分钟的时间，在此期间，它会输出有关缺失账本的警告。之后，您将拥有一个功能齐全的``stock server`，可以使用该服务器进行本地签名并访问XRP账本的API。

#### 版本更新(ubuntu)

```
sudo rpm -Uvh --replacepkgs https://mirrors.ripple.com/ripple-repo-el7.rpm
yumdownloader --enablerepo=ripple-stable --releasever=el7 rippled
rpm -K rippled*.rpm
sudo alien -i --scripts rippled*.rpm
sudo systemctl daemon-reload
sudo service rippled restart
```

## 一. 以Stock Server模型运行

### 在何种情况下运行此模式

大多数原因时：可以信任自己的服务器(节点)，可以控制其工作负载，并且不会受其他人的支配来决定何时以及如何访问它. 当然，必须考虑器安全性，以保护服务器免受恶意黑客攻击。

需要信任正在使用的rippled 节点。如果连接到恶意节点，很可能导致损失资金。例如：

- 恶意的几点可能会给你播报错误的信息: 在未付款时已付款
- 可以有选择地显示或隐藏支付路径和货币兑换优惠，以保证他们自己(攻击者)的利润，同时不为你提供最优惠的价格。
- 如果发送了地址的密钥(secert)，它可能会被劫持, 从而代表你进行任意交易，甚至转移或销毁地址所持有的所有资金。

此外，运行自己的节点可自己管理控制，以便执行重要的`仅限管理员`的操作指令。如果使用其他共享服务器，则必须担心同一服务器的其他用户与您竞争服务器的计算能力。 WebSocket API中的许多命令会给服务器带来很大压力, 因此，rippled 可以选择在需要时缩减其响应。如果使用共享节点，则可能无法始终获得最佳结果。

最后，如果运行validator 节点，则可以使用库存服务器作为公共网络的代理，同时将validator 服务器保留在私有子网上，使其只能通过库存服务器与外界通信。

## 二 .以 Validator模式运行

### 在何种情况下运行此模式

作为共识节点运行rippled 服务, 不仅可以进行正常p2p节点还参与交易的校验功能

1. rippled 服务器启动验证功能

   首先，您的服务器是一个不受信任的验证器。其他人可以看到您的服务器问题的验证，但是他们在协商一致的过程中忽略了这些问题。

2. 共享公钥，特别是其他受影响的操作人员。

3. 当其他信任的节点将您的公钥添加到它们的受信任服务器列表中时，您就成为了受信任的validator。

   参考 [Properties of a Good Validator](javascript:void())

### ripple集群服务器设置

#### 为何要搭建集群服务器

- rippled集群服务器共享加密的工作。如果一个服务器验证了消息的真实性，集群中的其他服务器也会信任它，不会再次验证。
- 集群服务器共享关于节点和API客户端行为不当或滥用网络, 这使得同时攻击集群中的所有服务器变得更加困难
- 集群服务器总是在集群中传播事务, 即使对其中一些交易不满足当前基于负载的交易费用。

如果运行一个validator的作为私有节点，建议使用集群服务器作为代理服务器。

#### 部署集群

配置集群

1. 对于每个服务器，请注意服务器的IP地址。

2. 对于每个服务器，使用`validation_create`方法生成惟一的种子。

   例如，使用命令行接口:

   ```
   $ rippled validation_create
   Loading: "/etc/rippled.cfg"
   Connecting to 127.0.0.1:5005
   {
      "result" : {
         "status" : "success",
         "validation_key" : "FAWN JAVA JADE HEAL VARY HER REEL SHAW GAIL ARCH BEN IRMA",
         "validation_public_key" : "n9Mxf6qD4J55XeLSCEpqaePW4GjoCR5U1ZeGZGJUCNe3bQa4yQbG",
         "validation_seed" : "ssZkdwURFMBXenJPbrpE14b6noJSu"
      }
   }
   ```

   将每个响应的`validation_seed`和`validation_public_key`参数保存在安全的地方。

3. 在每个服务器上，编辑配置`rippled.cfg`文件，修改以下部分

   1. 在`[ips_fixed]`部分中，列出集群中每个成员的IP地址和端口。对于这些服务器，端口号应该匹配来自该服务器的 `rippled.cfg`.的protocol = peer端口(通常是51235)。例如

      ```
      [ips_fixed]
      192.168.0.1 51235
      192.168.0.2 51235
      ```

      这定义了特定的节点服务器，该服务器应该始终试图维护p2p连接。

   2. 在`[node_seed]`部分中，将服务器的节点种子设置为第2步中使用validation_create方法生成的validation_seed值之一。每个服务器必须使用`唯一的节点种子`。例如:

      ```
      [node_seed]
      ssZkdwURFMBXenJPbrpE14b6noJSu
      ```

      这定义了服务器用于签名p2p通信(不包括验证消息)的密钥对。

   3. 在`[cluster_nodes]`部分中，设置服务器集群的成员，这些成员由它们的`validation_public_key`值标识。每个服务器都应该列出集群中`所有其他成员的公钥`。还可以为每个服务器添加自定义名称。例如:

      ```
      [cluster_nodes]
      n9McNsnzzXQPbg96PEUrrQ6z3wrvgtU4M7c97tncMpSoDzaQvPar keynes
      n94UE1ukbq6pfZY9j54sv2A1UrEeHZXLbns3xK5CzU9NbNREytaa friedman
      ```

      这定义了服务器用来识别集群成员的密钥对

   4. 保存配置文件(rippled.cfg)后，在每个服务器上重新启动。

      ```
      $ systemctl restart rippled
      ```

   5. 要确认每个服务器现在都是集群的成员，使用`[peers method]`。集群字段应该列出每个服务器的公钥和(如果配置了)自定义名称。

      例如，使用命令行接口

      ```
      $ rippled peers
      
      Loading: "/etc/rippled.cfg"
      Connecting to 127.0.0.1:5005
      {
        "result" : {
          "cluster" : {
              "n9McNsnzzXQPbg96PEUrrQ6z3wrvgtU4M7c97tncMpSoDzaQvPar": {
                "tag": "keynes",
                "age": 1
              },
              "n94UE1ukbq6pfZY9j54sv2A1UrEeHZXLbns3xK5CzU9NbNREytaa": {
                "tag": "friedman",
                "age": 1
              }
          },
          "peers" : [
            ... (omitted) ...
          ],
          "status" : "success"
        }
      }
      ```

### 生产服务器搭建

为了防止生产validator服务器受DDoS攻击，可以使用stock `rippled` server服务器作为代理使validator和外部网络之间进行通信。

1. 设置 rippled validator. 参考上文
2. 设置一个或多个 stock ripped servers
3. 配置validator 和服务器，并将分散的服务器彼此集群。
4. 更改以下配置
   ? - 复制`[ips_fixed]`列表并将其粘贴到`[ips]`下。这些字段应该只包含公共的ripple的IP地址和端口。validator 服务器只连接到这些p2p节点。
   ? - 将`[peer_private]`更改为`1`，以防止其他IP地址被转发
   1. 配置validator主机的防火墙，使其仅接受来自其公共节点的入站连接。
   2. 重启rippled 服务器,使配置生效

### 域验证

网络参与者不太可能在不知道谁在操作validator的情况下信任validator。为了解决这个问题，validator节点可以将它们的server与它们控制的web域关联起来。

1. 通过在validator server上运行以下命令来找到您的validator公钥:

   ```
   $ $HOME/opt/ripple/bin/rippled server_info -q | grep pubkey_validator
   ```

2. 使用用于域的SSL私钥对pubkey_validator(来自步骤1)进行签名。SSL私钥文件不需要存储在验证器服务器上。

   ```
   openssl dgst -sha256 -hex -sign /path/to/your/ssl.key <(echo <your-validator-public-key>)
   ```

3. 使用`validator-keys tool`(包含在ripple的RPM中)，对域名进行签名

   ```
   $ $HOME/opt/ripple/bin/validator-keys --keyfile /path/to/your/validator-keys.json sign <your-domain-name>
   ```

4. 要验证validator 域,可在XRP_charts上查看, 将此谷歌表单([Google Form](javascript:void()))与验证器的信息一起提交。

### 测试服务器搭建

Ripple创建了XRP测试网络，为XRP分类账提供了一个测试平台。XRP测试基金不是真正的基金，只用于测试。在连接到正式XRP分类帐网络之前，可以将`rippled server`连接到XRP测试网络，以测试并理解rippled的功能。还可以使用XRP测试网络来验证您自己的代码是否正确地与ripple交互。

**为了rippled 服务器连接到XRP测试网络，更改以下配置 `rippled.cfg`:**

1. 在本地配置文件 `rippled.cfg`

   a. 打开如下配置

   ```
   [ips]
   r.altnet.rippletest.net 51235
   ```

   b. 注释掉以下配置

   ```
   # [ips]
   # r.ripple.com 51235
   ```

2. 在本地 `validators.txt` (与`rippled.cfg`文件同目录) 做如下更改

   a. 打开如下配置

   ```
   
   ```

   b. 注释掉以下配置

   ```
   # [validator_list_sites]
   # https://vl.ripple.com
   #
   # [validator_list_keys]
   # ED2677ABFFD1B33AC6FBC3062B71F1E8397C1505E1C42C64D11AD1B28FF73F4734
   ```

3. 重启 rippled server

4. 验证rippled是否连接到XRP测试网络 :

   在服务器上使用`server_info`方法，并将其与来自测试网络上的公共服务器的结果进行比较。`validated_ledger`对象的`seq`字段在两台服务器上应该是相同的(如果它在您检查时发生了更改，可能会关闭一到两台服务器)

   - 以下命令检查测试网络服务器(`s.altnet.rippletest.net`:)的最新验证分类账

     ```
     $ ./rippled --rpc_ip 34.210.87.206:51234 server_info | grep seq
     ```

   - 下面的命令检查您的本地rippled server 的最新已验证的分类账序列:

     ```
     $ ./rippled server_info | grep seq
     ```

### 配置历史区块数据

`History Sharding`让服务器可以保存历史XRP账本数据，而无需每个服务器存储完整的历史。默认情况下，rippled server 不存储历史碎片(区块)。

*※ 虽然validator 和stock rippled 服务器都可以配置为存储历史区块，但是Ripple建议不要将 validator ripplerd server配置为存储，以减少服务器上的开销。如果运行一个validator并希望存储XRP分类帐历史记录，Ripple建议单独运行一个rippled server 用来存储区块。*

设置存储区块历史, 按以下步骤

1. **明确为区块存储分配多少空间**

   为更好的分配空间, 参考如下几点

   - 账本存储(由`[node_db]`定义)与区块存储是分开的。所有服务器都需要`账本存储`，并且必须包含一系列最近的历史记录，由`online_delete`参数中保持可用的账本数量定义。(默认配置存储最近的2000份账簿)
   - 如果在账本存储中至少保留了2 ^ 15个分类帐（32768份），则可以高效的将分类帐中最近历史记录的块导入到区块中.
   - 历史区块存储(由`[shard_db]`定义)仅用于存储历史区块。不用做stock server省略配置。s区块存储的大小在`max_size_gb`参数中以`GB`为单位定义; 服务器会试图使用尽可能多的空间来存储完整的区块.
   - 一个由2 ^ 14分类帐本(`16384`份)组成的的区块约占200 MB到4 GB的空间。越旧的区块所占的空间越小，因为当时账本中交易量越少。
   - 区块和帐本存储必须存储在`不同的文件路径中`。如果需要，可以将账本和区块配置为位于`不同的磁盘或分区`上。
   - 保存完整的账本历史纪录和分片(区块)是可以的，但是真的是多余的.
   - 获取区块(分片)的时间的长短, rippled server 所需的文件句柄的数量以及内存缓存的使用直接受切片大小的影响。

2. **修改 rippled.cfg**

   打开配置文件 cd `$HOME/.local/ripple/rippled.cfg` 修改如下

   ```
   [shard_db]
   type=NuDB
   path=/var/lib/rippled/db/shards/nudb  # 切片存储路径
   max_size_gb=50      # 单位是GB
   ```

   **`提示`**:Ripple建议对碎片存储使用`NuDB(类型=NuDB)`。与RocksDB相比，NuDB在每个碎片上使用的文件句柄更少。`RocksDB`使用的内存与存储的数据大小成比例，这可能需要过多的内存开销。然而，`NuDB被设计用于SSD驱动器`，不能与`机械磁盘`一起工作。

3. **重启服务**

   ```
   systemctl restart rippled
   ```

4. **等待下载分片数据**

   当服务器通过网络同步后, 会自动下载历史分片数据

### 启用公共签名

默认情况下, rippled 的签名方法被限制使用与管理连接, 如果你想允许 签名方法被用于 公共API 方法( rippled version v1.1.0 以前), 你可以通过配置来启用他.

如果你的服务器接受他们, 这使得以下方法可以公用

- `sign (签名方法)`

  - 默认情况下，此方法仅适用于管理员。如果服务器启用了公共签名，则可以将其用作公共方法。

  - **注意**: 除非本人操作ripple server，否则您应该使用[local signing with RippleAPI](javascript:void()) 来进行签名，而不是使用这个命令。非可信的节点可以在签署之前更改交易，或者使用您的密钥签署其他任意交易，造成虚假签名.

  - 格式如下:

    ```
    {
      "id": 2,
      "command": "sign",
      "tx_json" : {
          "TransactionType" : "Payment",
          "Account" : "rf1BiGeXwwQoi8Z2ueFYTEXSwuJYfV2Jpn",
          "Destination" : "ra5nK24KXen9AHvsdFTKHSANinZseWnPcX",
          "Amount" : {
             "currency" : "USD",
             "value" : "1",
             "issuer" : "rf1BiGeXwwQoi8Z2ueFYTEXSwuJYfV2Jpn"
          }
       },
       "secret" : "s████████████████████████████",
       "offline": false,
       "fee_mult_max": 1000
    }
    ```

- `sign_for` ( 多签名交易的签名)

  - for a [multi-signed transaction](javascript:void()).
  - 默认情况下，此方法仅适用于管理员。如果服务器启用了公共签名，则可以将其用作公共方法。
  - 这个命令需要启用 [MultiSign amendment](javascript:void())

- `submit`

  - submit方法申请一笔交易并将其发送到网络以进行确认(打包)并包含在将来的分类帐本中
  - 有两种模式
    - `submit-only`(只提交)模式, 接受一个已签名和序列化成二进制的交易 ,并提交到网络中,由于签名的事务对象是不可变的，因此在提交后不能修改或自动填写事务的任何部分。
    - `Sign-and-submit`(签名并提交), 接受一个 JSON格式的交易对象,以与 [sign method](javascript:void()),相同的方式完成和签署事务, 然后提交签名的交易。`我们建议只在测试和开发中使用此模式`。

**如果你是管理元连接的该ripple节点, 无需启用 公共签名**

**? :** Ripple不建议启用公共签名。与`wallet_proposal_method`一样，签名命令不执行需要管理级权限的任何操作，但是将它们限制为管理连接可以防止用户通过不安全的通信或不受控制的服务器不负责任地发送或接收密钥。

#### 启用步骤如下

1. 编辑`rippled.cfg`

   ```
   vim $HOME/etc/opt/ripple/rippled.cfg
   ```

2. 添加如下配置

   ```
   [signing_support]
      true
   ```

3. 重启rippled 服务(节点)

   ```
   systemctl restart rippled
   ```

## 三. 以独立模式(stand-alone)模式运行

在没有受信任共识服务器的情况下，可以在独立模式下运行rippled 节点 。 在独立模式下，rippled不与XRP Ledger其他任何p2p服务器通信，所以只能在本地服务器上执行大多数相同的操作。此模式 独立提供了一种测试rippled行为的方法，而不依赖于实时网络。 例如 ,可以测试在Amendments进入区中心网络之前来测试其效果。

在独立模式下，rippled 不会区分`“已关闭”分类帐版本和“已验证”分类帐版本`

在运行该模式之前，必须明确指出从哪个版本的账本开始的：

如下两种 :

1. 从头开始 --- 通过创建创世节点(账本) genesis ledger
2. 从本地已有账本开始

### 以创世节点运行

要使用一个新的genesis分类账以独立模式启动rippled，使用`-a`和`-start`选项:

```
rippled -a --start --conf=/path/to/rippled.cfg  # a 表示alone
```

更多启动参数,参考 命令行使用手册 :[Commandline Usage: Stand-Alone Mode Options](javascript:void()).

在一个创世分类账本中，传世地址拥有全部1000亿XRP。创世地址的关键字硬编码如下：

- **地址:** `rHb9CJAWyB4rj91VRWn96DkukG4bwdtyTh`
- **密钥:** `snoPBrXtMeMyMHUVTgbuqAfg1SUTb` ("masterpassphrase")

#### 设置 Genesis Ledger

默认情况下，一个新的新的创世账本未启用`Amendment`。如果使用`--start`参数启动一个新的创世账本，那么创世总账包含一个`EnableAmendment伪交易`来启用该节点的 `Amendment`, 除了在配置文件中明确禁用 `Amendment`。 （提醒：在独立模式下，必须手动推进分类帐[advance the ledger manually](javascript:void())。）

### 从已知账本运行

如果rippled server 先前已与XRP Ledger的p2p网络（生产网络或测试网络）同步，则可以从存储于磁盘的分类帐版本开始 .

步骤如下

1. 正常启动 rippled (检索账本)

   ```
   rippled --conf=/path/to/rippled.cfg
   ```

2. 等待 同步完成

   使用`server_info`方法检查检查节点相关的状态。当`server_state`值显示以下任何值时，将同步您的服务器：

   - `full`
   - `proposing`
   - `validating`

3. 检索特定的账本版本 (可选)

   - 如果只想要最新的分类帐，则可以跳过此步骤。
   - 如果想加载一个特定的历史分类帐版本，请使用`ledger_request`方法进行提取ripple。如果ripple还没有分类账版本，可能必须多次运行`ledger_request`命令，直到它完成检索分类账为止。
   - 如果想重放一个特定的历史分类帐版本，必须同时获取要重放的分类帐版本和重放之前的分类帐版本。(以前的分类账版本设置了初始状态，在此基础上应用您重播的分类账版本所描述的更改。)

4. 关闭rippled 节点

   使用 `stop`方法

   ```
   rippled stop --conf=/path/to/rippled.cfg
   ```

5. 以独立模式启动rippled 节点

   使用`-a`和`--load`选项启动服务器(默认为最新账本)：

   ```
   rippled -a --load --conf=/path/to/rippled.cfg
   ```

   要加载`特定的历史账本`，使用`--load`和`--ledger`参数启动服务器，提供`分类帐索引`或标识要加载的`分类帐版本的哈希值`：

   ```
   rippled -a --load --ledger 19860944 --conf=/path/to/rippled.cfg
   ```

6. 手动跟进账本

   在独立模式下使用`--ledger`加载分类帐时，它将转到当前的分类帐，因此必须手动跟进分类帐本

   ```
   rippled ledger_accept --conf=/path/to/rippled.cfg
   ```

## 四.命令行详解

### 可用模式

- (**Daemon Mode**)守护程序模式 - 默认值。连接到XRP Ledger以处理交易并构建账本数据库。

- **Daemon Mode**(独立模式) - 使用`-a`或`--standalone`选项。与守护进程模式一样，除了它不连接到其他节点。可以使用此模式测试交易处理或其他功能。

- **Client Mode** (客户端模式) -- 通过API接口的方法连接到又存在的Rippled节点, 作为一个JSON-RPC客户端, 请求完之后就退出 。可以使用此模式来检查节点状态和账本的数据。

- Other Usage

  (其他的用法) --- 以下每个命令都会使rippled 可执行文件打印一些信息，然后退出：

  - 帮助信息 -- 使用 `-h` 或者 `--help` , 输出帮助信息
  - 单元测试 --- 使用`-u` 或者使用`--unittest` 运行单元测试, 有助于确认节点是否成功的进行编译.
  - 版本申明 --- 使用 `--version` ,输出 rippled 的版本信息, 然后退出.

#### 通用指令

此指令适用于大多数的节点模式

`--conf`( 配置文件路径) --- 如果未指定，则rippled首先检查本地工作目录中的rippled.cfg(Uinx 默认再/opt/ripple/etc/rippled.cfg)文件。

#### 可选参数

以下通用参数会影响控制太输出和日志文件的存储

| 指令      | 简写 | 说明                                                         |
| :-------- | :--- | :----------------------------------------------------------- |
| --debug   |      | (弃用)启用跟踪级调试,, 使用[log_level method](javascript:void()) 替代 |
| --silent  |      | 启动时日志不输出, 建议在启动rippled 作为systemd单元时减少多余的日志 |
| --verbose | -v   | 弃用. 使用`log_level`方法替代                                |

### 守护进程模式指令

```
rippled [OPTIONS]
```

默认启动模式。除了通用指令，还可以提供以下任何选项

| 选项              | 说明                                                         |
| :---------------- | :----------------------------------------------------------- |
| --fg              | 将守护程序作为的单个进程运行。否则，在第一个进程作为监视器运行时，rippled会为守护进程分叉第二个进程。 |
| --import          | 在完全启动之前，从另一个节点的账本中导入账本数据。在rippled.cfg中需要有效的[import_db]字段. |
| --nodetoshard     | 在完全启动之前，将任何完整的历史记录分片从账本的存储区复制到分片存储区，直到分片存储区配置的最大磁盘空间 , 会占用大量的CPU 和 I/O , 警告：此命令是复制数据（而不是移动数据），因此必须有足够的磁盘空间来存储分片存储区和账本的数据。 |
| --quorum          | 此选项适用于测试网络 , 通过要求{QUORUM}可信验证者的协议，覆盖最低法定数量以进行验证。默认情况下，验证的仲裁将根据有多少个值自动设置为安全数量的可信验证器。如果对共识有深刻理解并且需要使用非标准配置，请仅使用此选项。 |
| -- validateShards | 查分片存储中的数据是否有效并与网络历史记录一致。有关分片存储的更多信息[History Sharding](javascript:void()) |

### 独立模式指令

```
rippled --standalone/-a [OPTIONS]
```

以独立模式运行。在此模式下，rippled 不会连接p2p或执行共识.

#### 初始化ledger选项

| 指令                | 说明                                                         |
| :------------------ | :----------------------------------------------------------- |
| --ledger {LEDGER}   | 通过指定账本版本来启动, 指定的账本版本必须存在于节点上       |
| --ledgerfile {FILE} | 从指定的文件加载账本的版本，该版本必须包含JSON格式的完整分类帐。有关此类文件的示例，请参阅提供的[`ledger-file.json`](javascript:void()).。 |
| --replay            | 用于调试。使用调试器，您可以添加断点来分析特定的事务处理逻辑。 |
| --start             | 用于调试。从一个新的genesis ledger时启用。                   |

### 客户端模式

```
rippled [OPTIONS] -- {COMMAND} {COMMAND_PARAMETERS}
```

在客户端模式下，充当一个rippled的客户端。 （该节点可能是在本地单独进程中运行的相同可执行文件，也可能是在远程服务器上）

除了单独的命令语法之外，客户端模式还接受通用选项和以下选项：

| 指令                | 说明                                          |
| :------------------ | :-------------------------------------------- |
| --rpc               | 明确指定以客户端的模式启动                    |
| –rpc_ip{IP_ADDRESS} | 明确指定连接到rippled 节点 , 选项报货port端口 |

用法示例（获取最早可用的帐户交易历史记录 和 最新的可用账本版本）

```
rippled -- account_tx rHb9CJAWyB4rj91VRWn96DkukG4bwdtyTh -1 -1
```

### 单元测试

```
rippled --unittest/-u [OPTIONS]
```

单元测试运行的是rippled 的源码，以确认可执行文件按预期执行。运行单元测试后，该过程将显示结果和退出的摘要。

如果单元测试报告失败，则通常表示以下情况之一：

- 编译出错
- 源码有bug
- 单元测试代码错误

单元测试的通用指令

| 指令                             | 说明                                                         |
| :------------------------------- | :----------------------------------------------------------- |
| --unittest-ipv6                  | 使用IPv6连接到本地, 从版本1.1.0开始提供                      |
| --unittest-jobs {NUMBER_OF_JOBS} | 使用指定数量的进程来运行单元测试。这可以在多核系统上更快地完成运行测试。 |
| --unittest-log                   | 即使指定了--quiet，也允许单元测试写入日志。                  |
| --quiet / q                      | 即使指定了--quiet，也允许单元测试写入日志。                  |

