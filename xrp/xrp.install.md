# 瑞波(Ripple)服务器安装(官方文档)

# 1. 简介

XRP Ledger对等网络的核心服务器是[`rippled`](https://ripple.com/build/rippled-apis/)。任何人都可以`rippled`在网络后面运行自己的服务器，并保留XRP分类帐的完整副本。你甚至可以让你的服务器参与一致的过程。

本页面包含以下说明：

- [容量规划 `rippled`](https://ripple.com/build/rippled-setup/#capacity-planning)
- [安装 `rippled`](https://ripple.com/build/rippled-setup/#installing-rippled)
- [参与共识流程](https://ripple.com/build/rippled-setup/#running-a-validator)

# 2. 瑞波服务器的类型

该`rippled`服务器软件可以在多种模式下根据其配置上运行，其中包括：

- **库存服务器** - 使用本地分类帐副本跟随网络。
- **验证服务器**或简称*验证器* - 参与达成共识。
- **独立模式** - 用于测试。不与其他`rippled`服务器通信。

您也可以将该`rippled`可执行文件作为客户端应用程序运行以在本地访问[`rippled`API](https://ripple.com/build/rippled-apis/)。（在这种情况下，相同二进制文件的两个实例可以并排运行;一个作为服务器，另一个作为客户端。）

## 2.1 运行库存服务器的原因

您可能想要运行自己的`rippled`服务器的原因很多，但其中大多数可以概括为：您可以信任自己的服务器，可以控制其工作负载，并且不受其他人的控制决定何时以及如何访问它。当然，你必须练习良好的网络安全来保护你的服务器免受恶意黑客的攻击。

你需要信任`rippled`你使用。如果您连接到恶意服务器，可以通过很多方式利用您或导致您损失资金。例如：

- 恶意服务器可能会在没有付款时报告您已付款。
- 它可以选择性地显示或隐藏付款路径和货币兑换优惠以保证自己的利润，同时不会为您提供最佳交易。
- 如果您将地址的密钥发送给它，它可能代表您进行任意交易，甚至转移或销毁您的地址所持有的所有资金。

此外，运行您自己的服务器可让您对其进行管理控制，从而允许您运行重要的仅限管理员和负载密集型命令。如果您使用共享服务器，则必须担心同一台服务器的其他用户与您竞争服务器的计算能力。WebSocket API中的许多命令可能会给服务器带来很大的压力，因此`rippled`可以在需要时缩减响应。如果您与其他人共享服务器，则可能无法始终获得最佳结果。

最后，如果您运行验证服务器，则可以使用库存服务器作为公用网络的代理，同时将验证服务器保留在只能通过库存服务器访问外部世界的私有子网上。这使得验证服务器的完整性变得更加困难。

## 2.2 运行验证服务器的原因

XRP Ledger的健壮性取决于互相关联的验证者网络，每个验证者信任几个其他验证者*不要串通*。有越多兴趣不同的运营商在运行验证者，那么每个网络成员越确定它会继续公正地运行。如果您或您的组织依赖于XRP分类帐，那么对共识流程作出贡献符合您的利益。

并非所有`rippled`服务器都需要验证器：信任来自同一运营商的更多服务器不会提供更好的防止共谋的保护。在发生自然灾害和其他紧急情况时，一个组织可能会在多个地区运行验证人以实现冗余。

如果您的组织运行验证服务器，那么您也可以运行一个或多个库存服务器，以平衡API访问的计算负载，或者作为验证服务器与外部网络之间的代理。

运行一个比特币完整节点，以方便我自己的比特币交易。

最终，运行验证程序的可能动机是：

1. 如果你需要运行一个Ripple的实例，运行一个验证器的额外成本接近于零。
2. 如果你关心网络的演变，运行一个验证器会让你有发言权。
3. 运行一个验证器宣布你对网络的承诺，并且它的统计数据显示你的运营可行性。

**瑞波公司在[validators.ripple.com上](https://validators.ripple.com/)** 推出 **Ripple Validator Registry**，验证器注册表收集并发布所有网络验证器的数据，使Ripple的运营商能够确定要信任哪些验证器

### 2.2.1 一个好的验证器的属性

有几个属性定义了一个好的验证器。您的服务器所体现的这些属性越多，其他人必须将您的服务器包含在可信验证器列表中的原因越多：

- **可用性**

  - 一个理想的验证器应该始终运行，为每个提议的分类账本提交验证投票。
  - 努力争取100％的正常运行时间。

- **协议**

  - 验证者的投票应该尽可能地与共识过程的结果相匹配。否则可能表明验证者的软件已过时，错误或故意偏倚。
  - 始终运行最新`rippled`版本而无需修改。

- **及时**性
  - 验证者的投票应该很快到达，而不是在一轮达成一致后才能完成。
  - 快速的互联网连接有助于此。

- **确定**
  - 应该清楚谁运行验证器。理想情况下，可信验证人列表应包括不同所有者在多个法律管辖区和地理区域内运行的验证人，以减少任何本地化事件可能干扰验证人的公正操作的可能性。
  - 设置[域验证](https://ripple.com/build/rippled-setup/#domain-verification)是一个好的开始。

目前，Ripple（公司）不能推荐任何验证器，除了默认验证器列表中的验证器。但是，我们正在收集其他验证器和构建工具的数据以报告其性能。有关验证器的指标，请参阅[validators.ripple.com](https://validators.ripple.com/)。

# 3. 容量规划

## 3.1 配置设置

Ripple建议采用以下指导方针来提高性能。您可以在`rippled.cfg`文件中设置以下参数以提高`rippled`服务器的性能。

### 3.1.1 节点大小

该`node_size`参数确定数据库高速缓存的大小。

较大的数据库缓存以较高的内存要求为代价来降低磁盘I / O需求。

Ripple建议您始终使用可用内存可支持的最大数据库缓存。有关建议的设置，请参阅下表。

#### 3.1.1.1 建议

| 可用RAM为 `rippled` | `node_size` 值 | 笔记               |
| :------------------ | :------------- | :----------------- |
| <8GB                | `tiny`         | 不建议             |
| 8GB                 | `low`          |                    |
| 16 GB               | `medium`       |                    |
| 32GB                | `huge`         | 推荐用于生产服务器 |

### 3.1.2 节点数据库类型

`node_db`字段，可以将该值设置为`rocksdb`或`nudb`。

`rippled`提供历史分割功能，允许您将随机范围的分类账本存储在**单独的分仓库**中。您可能希望将分片存储配置为使用与分类帐存储不同类型的键值存储。有关如何使用此功能的更多信息，请参阅[历史记录分片](https://ripple.com/build/history-sharding/#shard-store-configuration)。

#### 3.1.2.1 RocksDB与NuDB

RocksDB需要比NuDB少三分之一的磁盘存储空间，并且相应地改善了I / O延迟。但是，随着存储容量的增长，这需要增加内存利用率。另一方面，NuDB无论[存储](https://ripple.com/build/rippled-setup/#storage)如何，都具有几乎恒定的性能和内存占用[空间](https://ripple.com/build/rippled-setup/#storage)。

`rippled`作为验证器运行的服务器应该只保留几天或更少的数据。

- **验证器节点，Ripple建议使用RocksDB进行验证。**

- **对于所有其他用途，Ripple建议将NuDB用于分类账本本存储。**

RocksDB具有与性能相关的配置选项，您可以修改该选项以实现最大的事务处理吞吐量。（NuDB没有与性能相关的配置选项。）以下是`rippled`使用RocksDB 的服务器的推荐配置示例：



```ini
[node_db]
type=rocksdb
open_files=512
file_size_mb=64
file_size_mult=2
filter_bits=12
cache_mb=512
path={path_to_ledger_store}
```

### 3.1.3 历史数据

`rippled`服务器保持联机的历史数据量是所需存储空间的主要贡献者。在撰写本文时（2018-03-01），一台`rippled`服务器每天存储大约12GB的数据。随着XRP分类账本网络中交易量的增加，您可以预计这一数量会增加。您可以控制您使用`online_delete`和`advisory_delete`字段保存的数据量。

在线删除可以在`rippled`不中断服务的情况下从数据库修剪分类帐。它只会删除不属于当前分类帐的记录。如果没有在线删除，这些数据库会无限制地增长。释放磁盘空间需要停止进程并手动删除数据库文件。

### 3.1.4 日志级别

默认`rippled.cfg`文件将记录详细程度设置为`warning`。此设置通过更详细的日志记录大大减少了磁盘空间和I / O需求。但是，更详细的日志记录可提高故障排除的可见性。

小心：如果您省略节中的`log_level`命令，则将日志写入磁盘的级别并将级别日志输出到控制台。根据事务处理量和客户端活动的不同，日等级日志记录每天需要多个GB的磁盘空间。`[rpc_startup]``rippled``debug``warning``debug``warning`

## 3.2 网络和硬件

`rippled`XRP分类帐网络中的每台服务器执行网络的所有交易处理工作。未知数量何时会达到最大网络容量。因此，生产`rippled`服务器的基准硬件应该与Ripple的[性能测试](https://ripple.com/dev-blog/demonstrably-scalable-blockchain/)中使用的类似。

### 3.2.1 建议

为了在企业生产环境中获得最佳性能，Ripple建议使用`rippled`具有以下特征的裸机运行：

- 操作系统：Ubuntu 16.04+
- CPU：Intel Xeon 3+ GHz处理器，支持4核和超线程
- 磁盘：SSD
- 内存：
  - 用于测试：8GB +
  - 对于生产：32GB
- 网络：主机上具有千兆网络接口的企业数据中心网络

#### 3.2.1.1 SSD存储

SSD存储应支持几千个读写IOPS。Ripple工程师观察到的每秒最大读写次数超过每秒10,000次（在重度使用的公共服务器集群中）以及每秒超过7,000次写入（在专用性能测试中）。

#### 3.2.1.2 CPU利用率和虚拟化

Ripple性能工程已经确定裸机服务器实现最大吞吐量。但是，管理程序很可能会导致性能下降最小化。

#### 3.2.1.3 网络

任何企业或运营商级数据中心都应具有足够的网络带宽来支持正在运行的`rippled`服务器。对于当前的交易量，最低要求大致为2Mbps传输和2Mbps接收。但是，在提供历史分类帐和交易报告时，这些数据可能会突然传输至100MBps。当一个`rippled`服务器最初启动时，它可能会突然超过20Mbps的接收。

#### 3.2.1.4 存储

Ripple建议估计存储容量大概为每天大约12GB的数据与NuDB保持联机。RocksDB每天需要8GB左右。但是，每天的数据随网络中的活动而变化。你应该提供额外的容量来为未来的增长做准备。在撰写本文时（2018-03-01），具有所有XRP分类帐历史记录的服务器需要6.8TB。

#### 3.2.1.5 记忆

内存要求主要是`node_size`配置设置和检索历史数据的客户端流量的函数。如前所述，生产服务器应该使性能最大化并将此参数设置为`huge`。您可以将`node_size`参数设置得较低以使用较少的内存，但您应该只对测试进行此操作。随着`node_size`中`medium`，一个`rippled`服务器可以在测试Linux系统相当稳定与低至8GB的内存。

#### 3.2.1.6 亚马逊网络服务

亚马逊网络服务（AWS）是一种流行的虚拟主机环境。您可以在AWS中运行波动，但Ripple不建议使用弹性块存储（EBS）。弹性块存储的IOPS（5,000）的最大数量对于`rippled`最重的负载是不够的，尽管其非常昂贵。

AWS实例存储（`ephemeral`存储）没有这些约束。因此，Ripple建议部署具有实例存储`rippled`等主机类型的服务器`M3`。该`database_path`和`node_db`路径应每驻留在实例存储。

> AWS实例存储不保证在发生硬盘驱动器故障时提供持久性。此外，实例停止并重新启动时（但不是刚刚重新启动时）会丢失的数据。对于`rippled`服务器而言，这种损失是可以接受的，因为单个服务器通常可以从其对等服务器重新获取该数据。

# 4. 安装

对于开发，您可以[从源代码](https://wiki.ripple.com/Rippled_build_instructions)[编译`rippled`](https://wiki.ripple.com/Rippled_build_instructions)。

生产`rippled`实例可以[使用](https://ripple.com/build/rippled-setup/#installation-on-centosred-hat-with-yum) Ripple [yum](https://en.wikipedia.org/wiki/Yellowdog_Updater,_Modified)版本库中提供的Ripple [二进制可执行文件](https://ripple.com/build/rippled-setup/#installation-on-centosred-hat-with-yum)。

提示：对于超出最低要求的建议，请参阅[容量规划](https://ripple.com/build/rippled-setup/#capacity-planning)。

## 4.1 在yum上安装在CentOS 7/ Red Hat7上

```bash
rpm -Uvh https://mirrors.ripple.com/ripple-repo-el7.rpm
yum install --enablerepo=ripple-stable rippled
systemctl enable rippled.service
systemctl start rippled.service
```

## 4.2 在Ubuntu上用alien来安装 

本节假定您使用的是Ubuntu 16.04或更高版本。

1. 安装yum-utils和alien：

```bash
apt-get update   
apt-get install yum-utils alien
rpm -Uvh https://mirrors.ripple.com/ripple-repo-el7.rpm
yumdownloader --enablerepo=ripple-stable --releasever=el7 rippled

rpm --import https://mirrors.ripple.com/rpm/RPM-GPG-KEY-ripple-release
rpm -K rippled*.rpm

alien -i --scripts rippled*.rpm
rm rippled*.rpm

systemctl enable rippled.service
systemctl start rippled.service
```

## 安装后

`rippled`与网络的其他部分同步可能需要几分钟的时间，在此期间，它会输出有关缺失分类帐的警告。之后，您将拥有一个功能齐全的库存`rippled`服务器，您可以使用该服务器进行本地签名并访问XRP分类帐的API。

[命令](https://ripple.com/build/rippled-apis/#list-of-public-commands)可以在以下情况下运行：

```bash
/opt/ripple/bin/rippled <command>
```


## 更新

您可以订阅[Ripple的Google小组](https://groups.google.com/forum/#!forum/ripple-server)以接收新`rippled`版本的通知。

### 在CentOS / Red Hat上自动更新

可以使用一次性Cron配置启用自动波动更新：

1. 检查是否`/opt/ripple/bin/update-rippled.sh`存在。如果没有，请[手动更新](https://ripple.com/build/rippled-setup/#manual-update-on-centosred-hat)。

2. 安装`crond`：

```bash
yum install cronie
```

3. 打开crontab文件进行编辑

```bash
crontab -e
```

4. 将以下内容添加到crontab文件中。一定要在文件末尾添加一个空白行。

```bash
RANDOM_DELAY=59

0 * * * * /opt/ripple/bin/update-rippled.sh
```

该脚本`rippled`在每个新版本发布后的一小时内更新已安装的软件包。

### 在CentOS / Red Hat上手动更新

运行以下命令以更新到最新版本`rippled`：

```bash
rpm -Uvh --replacepkgs https://mirrors.ripple.com/ripple-repo-el7.rpm
yum update --enablerepo=ripple-stable rippled
systemctl daemon-reload
service rippled restart
```



### Ubuntu上的手动更新

运行以下命令以更新到最新版本`rippled`：

```bash
rpm -Uvh --replacepkgs https://mirrors.ripple.com/ripple-repo-el7.rpm
yumdownloader --enablerepo=ripple-stable --releasever=el7 rippled
rpm -K rippled*.rpm
alien -i --scripts rippled*.rpm
systemctl daemon-reload
service rippled restart
```



# 运行一个验证器

运行`rippled`参与共识流程的验证器非常简单：

1. 在您的服务器上[启用验证](https://ripple.com/build/rippled-setup/#validator-setup)。

```bash
rippled
```

   - 起初，你的服务器是一个*不可信的验证器*。其他人可以看到你的服务器发出的验证，但是他们在共识过程中忽视了他们。

2. 与公众分享公共密钥，尤其是其他`rippled`运营商。

3. 当其他

```bash
rippled
```

操作员将您的公钥添加到受信任服务器列表中时，您已成为值得信赖的验证器  。

   - 另请参阅最佳实践[的良好验证器属性](https://ripple.com/build/rippled-setup/#properties-of-a-good-validator)。

## 验证器设置

该`validator-keys`工具（包含在`rippled`RPM中）是安全生成和管理验证器密钥的推荐方式。

1. [安装`rippled`服务器。](https://ripple.com/build/rippled-setup/#installing-rippled)

2. 生成验证器密钥对：

```bash
/opt/ripple/bin/validator-keys create_keys
```

> 将生成的`validator-keys.json`密钥文件存储在安全但可恢复的位置，如加密的USB闪存驱动器。不要修改其内容。

3. 生成验证器令牌并编辑您的`rippled.cfg`文件以添加`[validator_token]`值。

```bash
/opt/ripple/bin/validator-keys create_token --keyfile /path/to/your/validator-keys.json
```

   如果您以前没有使用该`validator-keys`工具配置验证器，则还必须`[validation_seed]`从`rippled.cfg`文件中删除该验证器。这会更改您的验证器公钥。

4. 开始`rippled`：

```bash
service rippled restart
```

见[的`validator-keys-tool`GitHub的仓库](https://github.com/ripple/validator-keys-tool/blob/master/doc/validator-keys-tool-guide.md)进行有关管理验证密钥的详细信息。

## 面向公众的服务器

为了保护生产验证器免受[DDoS](https://en.wikipedia.org/wiki/Denial-of-service_attack)攻击，您可以使用库存`rippled`服务器作为验证器与外部网络之间的代理。

1. [设置`rippled`验证器。](https://ripple.com/build/rippled-setup/#validator-setup)
2. [设置一个或多个库存`rippled`服务器。](https://ripple.com/build/rippled-setup/#installing-rippled)
3. 将验证器和库存`rippled`服务器配置为相互[集群](https://ripple.com/build/rippled-setup/#clustering)。
4. 对您的验证器进行以下配置更改：
   - 复制`[ips_fixed]`列表并将其粘贴到下`[ips]`。这些字段应仅包含面向公众的波动的IP地址和端口。验证程序只连接到这些同伴。
   - 更改`[peer_private]`为`1`阻止其IP地址被转发。
5. 将验证器主机的防火墙配置为仅接受来自其面向公众的波及的入站连接。

请记住重新启动`rippled`配置更改才能生效。

注意不要发布验证器的IP地址。

## 域验证

网络参与者不可能在不知道谁在操作它们的情况下相信验证者。为了解决这个问题，验证器操作员可以将他们的验证器与他们控制的网域相关联。

1. 通过在验证器服务器上运行以下内容来查找验证器公钥：

```bash
/opt/ripple/bin/rippled server_info -q | grep pubkey_validator
```

2. 使用用于您的域的SSL私钥签署验证器公钥（来自步骤1）。SSL私钥文件不需要存储在验证器服务器上。

```bash
openssl dgst -sha256 -hex -sign /path/to/your/ssl.key <(echo <your-validator-public-key>)
```

3. 使用`validator-keys`工具（包含在`rippled`RPM中），签署域名：

```bash
/opt/ripple/bin/validator-keys --keyfile /path/to/your/validator-keys.json sign <your-domain-name>
```

4. 要在[XRP图表中](https://xrpcharts.ripple.com/#/validators)包含经过验证的验证器域，请将此[Google表单](https://docs.google.com/forms/d/e/1FAIpQLScszfq7rRLAfArSZtvitCyl-VFA9cNcdnXLFjURsdCQ3gHW7w/viewform)与验证器的信息一起提交。

# 其他配置

`rippled`应使用默认配置连接到XRP分类帐。但是，您可以通过编辑`rippled.cfg`文件（位于使用yum进行`/opt/ripple/etc/rippled.cfg`安装`rippled`时）更改设置。有关配置设置的建议，请参阅[容量规划](https://ripple.com/build/rippled-setup/#capacity-planning)。

见[的`rippled`GitHub的仓库](https://github.com/ripple/rippled/blob/release/doc/rippled-example.cfg)对所有配置选项的说明。

更改`[debug_logfile]`或`[database_path]`部分可能需要您给`rippled`用户和组所有权到新配置的路径：

```bash
chown -R rippled:rippled <configured path>
```



重新启动`rippled`以使配置更改生效：

```bash
service rippled restart
```



## 并行网络

大多数时候，我们将XRP分类账本描述为一个单一的实体 - 而且大多数情况都是如此。有一个生产XRP分类帐对等网络，并且所有发生在XRP分类帐上的业务都发生在生产网络中。

但是，有时您可能想要在不与核心网络交互的情况下进行测试和实验。这就是为什么Ripple开创[Ripple Test Net](https://ripple.com/build/ripple-test-net/)这个“替代宇宙”网络的原因，它可以作为应用程序和`rippled`服务器本身的测试平台，而不会影响日常XRP分类账本用户的业务运营。Ripple Test Net（也称为AltNet）单独提供TestNet-only XRP，Ripple [免费赠送](https://ripple.com/build/ripple-test-net/)给有意在测试网络上开发应用程序的各方。

注意：Ripple不保证测试网络的稳定性。它一直并将继续用于测试服务器配置，网络拓扑和网络性能的各种属性。

随着时间的推移，可能还会有更小的临时测试网络用于特定目的。

### 并行网络和共识

没有`rippled`定义它使用哪个网络。相反，它使用它所信任的验证器的共识来知道哪个分类账本被接受为真相。当`rippled`实例的不同共识组仅信任同一组中的其他成员时，每个组继续作为并行网络。即使恶意或行为异常的计算机连接到两个网络，只要每个网络的成员未配置为信任另一个网络的成员超过其仲裁设置，共识流程就会覆盖混淆。

## 聚类

如果您`rippled`在单个数据中心中运行多台服务器，则可以将这些服务器配置为一个群集以最大限度地提高效率。`rippled`在群集中运行服务器可带来以下好处：

- 群集`rippled`服务器共享密码学的工作。如果一台服务器验证了消息的真实性，则集群中的其他服务器会信任它并且不会重新验证。
- 集群服务器共享关于行为不当或滥用网络的对等方和API客户端的信息。这使得一次攻击集群的所有服务器变得更加困难。
- 集群服务器始终在整个集群中传播事务，即使事务在某些集群上不符合当前基于负载的事务处理费用。

要启用群集，请更改每个服务器的[配置文件](https://github.com/ripple/rippled/blob/d7def5509d8338b1e46c0adf309b5912e5168af0/doc/rippled-example.cfg#L297-L346)的以下部分：

- 列出该`[ips_fixed]`部分下的每个其他服务器的IP地址和端口。该端口应该是来自其他服务器`protocol = peer`设置的端口`rippled.cfg`。例：

```ini
[ips_fixed]
192.168.0.1 51235
192.168.0.2 51235
```

- 为每台服务器生成一个唯一的种子（使用该[`validation_create`命令](https://ripple.com/build/rippled-apis/#validation-seed)），并在该`[node_seed]`部分下进行配置。该`rippled`服务器使用该密钥签署的消息发送到对等网络网络中的其他服务器。

- 在该`[cluster_nodes]`部分下添加每个其他服务器的公钥（用于对等通信）。

# 独立模式

您可以`rippled`在独立模式下运行，而不需要获得可信服务器的一致意见。在独立模式下，`rippled`不与XRP分类帐对等网络中的任何其他服务器进行通信，但您只能在本地服务器上执行大部分相同的操作。独立提供了一种测试`rippled`行为的方法，而不受实时网络的束缚。例如，您可以在这些修正案在分散式网络中生效之前[测试](https://ripple.com/build/amendments/#testing-amendments)修正案的效果。

当您`rippled`以独立模式运行时，您必须告诉它从哪个分类帐开始：

  - 从头开始创建一个[新的创始分类帐](https://ripple.com/build/stand-alone-mode/#new-genesis-ledger)。
  - 从磁盘[加载现有账本版本](https://ripple.com/build/stand-alone-mode/#load-saved-ledger)。

  警告：在独立模式下，您必须[手动推进分类帐](https://ripple.com/build/stand-alone-mode/#advancing-ledgers-in-stand-alone-mode)。

## 新的创世分类账本

在独立模式下，您可以`rippled`创建新的创建分类账本。这提供了一个已知的状态，没有生产XRP分类帐的分类帐历史记录。（这对单元测试等非常有用。）

  - 要`rippled`以独立模式开始创建新的分类账本，请使用[`-a`](https://wiki.ripple.com/Rippled#--standalone.2C_-a)和[`--start`](https://wiki.ripple.com/Rippled#--start)选项：

```bash
rippled -a --start --conf=/path/to/rippled.cfg
```

  

在创世总帐中，[创世地址](https://ripple.com/build/accounts/#special-addresses)占据了1000亿XRP。起始地址的关键字如下所示进行[硬编码](https://github.com/ripple/rippled/blob/94ed5b3a53077d815ad0dd65d490c8d37a147361/src/ripple/app/ledger/Ledger.cpp#L184)：

地址： `rHb9CJAWyB4rj91VRWn96DkukG4bwdtyTh`

密钥:( `snoPBrXtMeMyMHUVTgbuqAfg1SUTb` “masterpassphrase”）

> 如果你创建一个新的成因分类帐，硬编码的默认[储备](https://ripple.com/build/reserves/)是200 XRP最低为资助一个新的地址，用增量50 XRP每个对象的分类帐。这些值高于生产网络的当前储备要求。（另见：[费用投票](https://ripple.com/build/fee-voting/)）

[![新增功能：波纹0.50.0](https://img.shields.io/badge/New%20in-rippled%200.50.0-blue.svg)](https://github.com/ripple/rippled/releases/tag/0.50.0)如果您创建新的创建分类帐`--start`，则所有创建分类帐都包含一个[EnableAmendment伪事务，](https://ripple.com/build/transactions/#enableamendment)以打开服务器本机支持的所有[修订](https://ripple.com/build/amendments/)`rippled`，但您在配置文件中明确禁用的修订除外。这些修正案的影响从下一个分类账本版本开始。

## 加载已保存的帐簿

如果您的`rippled`服务器先前与XRP分类帐对等网络（生产网络或[测试网络](https://ripple.com/build/rippled-setup/#parallel-networks)）同步，则可以从保存到磁盘的分类帐版本开始。

### 1. `rippled`正常启动。

要加载现有分类帐，您必须先从网络中检索该分类帐。`rippled`正常情况下，以在线模式启动：

```bash
rippled --conf=/path/to/rippled.cfg
```

  

### 2.等到`rippled`同步。

使用该[`server_info`命令](https://ripple.com/build/rippled-apis/#server-info)检查您的服务器相对于网络的状态。当`server_state`值显示以下任何值时，您的服务器将同步：

  - `full`
  - `proposing`
  - `validating`

欲了解更多信息，请参阅[可能的服务器状态](https://ripple.com/build/rippled-apis/#possible-server-states)。

### 3.（可选）检索特定分类账本版本。

如果您只想要最近的分类帐，则可以跳过此步骤。

如果您想加载特定的历史分类帐版本，请使用该[`ledger_request`命令](https://ripple.com/build/rippled-apis/#ledger-request)进行`rippled`提取。如果`rippled`尚未拥有分类帐版本，则可能必须`ledger_request`多次运行该命令，直至完成检索分类帐。

如果您想要重放特定的历史分类帐版本，则必须先取回分类帐版本以及之前的分类帐版本。（之前的分类帐版本设置了您应用您重放的分类帐版本所描述的更改的初始状态。）

### 4.关机`rippled`。

使用[`stop`命令](https://ripple.com/build/rippled-apis/#stop)：

```bash
rippled stop --conf=/path/to/rippled.cfg
```

  

### 5.开始`rippled`独立模式。

要加载最新的分类帐版本，可以在启动服务器时使用[`-a`](https://wiki.ripple.com/Rippled#--standalone.2C_-a)和[`--load`](https://wiki.ripple.com/Rippled#--load)选项：

```bash
rippled -a --load --conf=/path/to/rippled.cfg
```

  

要改为加载特定的历史分类帐，请将[`--load`](https://wiki.ripple.com/Rippled#--load)参数与参数一起使用`--ledger`，提供分类帐索引或标识要加载的分类帐版本的散列值：

```bash
rippled -a --load --ledger 19860944 --conf=/path/to/rippled.cfg
```



### 6.手动推进分类帐。

当您以`--ledger`独立模式加载分类账本时，它会转到当前的开立分类帐，因此您必须[手动推进分类帐](https://ripple.com/build/stand-alone-mode/#advancing-ledgers-in-stand-alone-mode)：

```bash
rippled ledger_accept --conf=/path/to/rippled.cfg
```



## 以独立模式推进总帐

在独立模式下，`rippled`不会与对等网络的其他成员进行通信或参与共识流程。相反，您必须使用以下[`ledger_accept`命令](https://ripple.com/build/rippled-apis/#ledger-accept)手动推进分类帐索引：

```bash
rippled ledger_accept --conf=/path/to/rippled.cfg
```



在独立模式下，`rippled`不区分“已关闭”分类账本版本和“已验证”分类账本版本。（有关差异的更多信息，请参阅[XRP分类帐共识流程](https://ripple.com/build/xrp-ledger-consensus-process/)。）

每当`rippled`关闭分类账本时，它都会根据确定性但难以游戏的算法对交易进行重新排序。（这是共识的重要组成部分，因为交易可能以不同的顺序到达网络的不同部分。）`rippled`在独立模式下使用时，应在提交取决于交易结果的交易之前手动提前分类帐从一个不同的地址。否则，当分类账本被关闭时，这两笔交易可能会以相反的顺序执行。注意：您可以安全地从一个单一的地址，提交多笔交易的明细帐单，因为`rippled`从升序排列相同的地址各种交易[`Sequence`号码](https://ripple.com/build/transactions/#common-fields)。