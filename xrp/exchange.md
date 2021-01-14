# 瑞波币交易

作者：[@failymao](https://github.com/evilmao)
本文为作者原创，转载请注明出处：https://www.cnblogs.com/failymao/p/10261402.html

------

目录

- 交易系统基础
  - 账户
    - [创建账户](https://www.cnblogs.com/failymao/p/10261402.html#创建账户)
    - [Addresses](https://www.cnblogs.com/failymao/p/10261402.html#addresses)
    - [特殊地址](https://www.cnblogs.com/failymao/p/10261402.html#特殊地址)
    - [账户的持久性](https://www.cnblogs.com/failymao/p/10261402.html#账户的持久性)
    - [交易历史](https://www.cnblogs.com/failymao/p/10261402.html#交易历史)
    - [地址编码](https://www.cnblogs.com/failymao/p/10261402.html#地址编码)
  - 加密密钥
    - [生成密钥对](https://www.cnblogs.com/failymao/p/10261402.html#生成密钥对)
    - [主密钥对](https://www.cnblogs.com/failymao/p/10261402.html#主密钥对)
    - [常规密钥对](https://www.cnblogs.com/failymao/p/10261402.html#常规密钥对)
  - 账户资金储备
    - [基本和私人备用金](https://www.cnblogs.com/failymao/p/10261402.html#基本和私人备用金)
    - [私人备用金](https://www.cnblogs.com/failymao/p/10261402.html#私人备用金)
  - 交易费用
    - [当前交易成本](https://www.cnblogs.com/failymao/p/10261402.html#当前交易成本)
    - [特殊交易成本](https://www.cnblogs.com/failymao/p/10261402.html#特殊交易成本)
    - [用户交易产生的交易成本的归谁所有](https://www.cnblogs.com/failymao/p/10261402.html#用户交易产生的交易成本的归谁所有)
    - [加载成本和打开账本成本](https://www.cnblogs.com/failymao/p/10261402.html#加载成本和打开账本成本)
    - [本地加载成本](https://www.cnblogs.com/failymao/p/10261402.html#本地加载成本)
    - [创建新账本的成本](https://www.cnblogs.com/failymao/p/10261402.html#创建新账本的成本)
    - [交易排队](https://www.cnblogs.com/failymao/p/10261402.html#交易排队)
    - 查询交易成本
      - [1. Server_info方法](https://www.cnblogs.com/failymao/p/10261402.html#1-server_info方法)
      - [2. Server_state方法](https://www.cnblogs.com/failymao/p/10261402.html#2-server_state方法)
    - [指定交易成本](https://www.cnblogs.com/failymao/p/10261402.html#指定交易成本)
    - [自动指定交易成本](https://www.cnblogs.com/failymao/p/10261402.html#自动指定交易成本)
  - 提交可靠的交易
    - [背书](https://www.cnblogs.com/failymao/p/10261402.html#背书)
    - [交易时间表](https://www.cnblogs.com/failymao/p/10261402.html#交易时间表)
    - 交易
      - [1.提交](https://www.cnblogs.com/failymao/p/10261402.html#1提交)
      - [2.验证](https://www.cnblogs.com/failymao/p/10261402.html#2验证)
      - [3.Ledger 缺失账本](https://www.cnblogs.com/failymao/p/10261402.html#3ledger-缺失账本)
    - [开发相关](https://www.cnblogs.com/failymao/p/10261402.html#开发相关)
    - rippled - 提交和验证交易
      - [1. 确认账户序列号](https://www.cnblogs.com/failymao/p/10261402.html#1-确认账户序列号)
      - [2. 确定最后验证的账本](https://www.cnblogs.com/failymao/p/10261402.html#2-确定最后验证的账本)
      - [3. 构建交易](https://www.cnblogs.com/failymao/p/10261402.html#3-构建交易)
      - [4. 提交交易](https://www.cnblogs.com/failymao/p/10261402.html#4-提交交易)
      - [5.验证交易](https://www.cnblogs.com/failymao/p/10261402.html#5验证交易)
      - [6.验证缺失的交易](https://www.cnblogs.com/failymao/p/10261402.html#6验证缺失的交易)
  - [取消或忽略一笔交易](https://www.cnblogs.com/failymao/p/10261402.html#取消或忽略一笔交易)
  - [查找交易结果](https://www.cnblogs.com/failymao/p/10261402.html#查找交易结果)



## 交易系统基础

### 账户

XRP Ledger中的`“帐户”`代表XRP的持有者和交易的发送者的地址。帐户的核心要素是：

- 识别地址 (**address**)，例如`rf1BiGeXwwQoi8Z2ueFYTEXSwuJYfV2Jpn`
- XRP余额(**XRP balance**)。一部分用于备用([Reserve](https://developers.ripple.com/reserves.html)).
- 序列号(**sequence number**)，从1开始，随着从该帐户发送的每笔交易而增加。除非交易的序列号与其发件人的下一个序列号匹配，否则任何交易都不能包含在账本中。
- 交易历史(**history of transactions**) —- 影响此帐户及其余额的交易历史记录.
- 一种或多种授权交易的方式，可能包括:
  - 帐户固有的主密钥对.(这可以禁用但不能更改。)
  - 可以轮换的`“常规密钥”`对 。
  - 用于多签名(`multi-siging`)的签名地址列表 (与帐户的核心数据分开存储)

在账本的数据树(Milkre tire)中，帐户的核心数据存储在`AccountRoot`账本对象类型中。帐户也可以是其他几种类型数据的所有者（或部分所有者）

#### 创建账户

没有专门的`创建帐户`交易。 如果付款将XRP等于或大于帐户备用金发送到不存在的账户，则付款交易会自动创建新帐户。这称为资金帐户，并在账本中创建`AccountRoot`对象。没有其他交易可以创建帐户.

**⚠** 为帐户注资(充值)并不会为该帐户的赋予任何特权。 拥有与帐户地址对应的密钥(私钥)的人可以完全控制帐户及其包含的所有XRP。

在XRP Ledger中获取帐户的方法如下：

1. 从强大的随机数据源生成密钥对，并计算该密钥对的地址。 （例如，您可以使用`wallet_propose`方法执行此操作。）

2. 让已在XRP Ledger中拥有帐户的人将XRP发送给你生成的地址。

   - 例如，可以在交易所购买XRP，交易所会分配一个可用的账户地址。

     第一次在自己的XRP Ledger地址收到XRP时，必须支付账户储备金（目前为20 XRP）-- 这部分资金会无期限锁定 , 相比之下，交易所通常将所有客户的XRP保存在`企业XRP Ledger账户`中，因此客户无需为交易所的个人账户支付准备金。

#### Addresses

XRP分类帐中的Addresses由base58 XRP账本地址标识。该地址来自帐户的主公钥(master [public key](https://en.wikipedia.org/wiki/Public-key_cryptography))，该公钥又来自密钥。`An Addresses`在JSON中表示为字符串，具有以下特征：

- 长度在25到35个字符之间
- 以字符 ` r` 开头
- 使用字母数字字符，不包括数字`“0”`大写字母`“O”`，大写字母`“I”`和小写字母`“l”`
- 区分大小写
- 包括一个4字节的校验和，以便从随机字符(概率约为2 ^ 32)生成有效地址

#### 特殊地址

某些地址在XRP Ledger中具有特殊含义或历史用途。在许多情况下，这些是“黑洞”地址，意味着地址不是从已知的密钥导出的, 由于实际上不可能从地址推导出密钥，因此黑洞地址所拥有的任何XRP都将永远丢失。

| Address                            | Name           | 含义                                                         | 备注     |
| ---------------------------------- | -------------- | ------------------------------------------------------------ | -------- |
| rrrrrrrrrrrrrrrrrrrrrhoLvTp        | ACCOUNT_ZERO   | 它是值为0的base58编码。p2p中，rippled使用此地址作为XRP的发布者 | 黑洞地址 |
| rrrrrrrrrrrrrrrrrrrrBZbvji         | CCOUNT_ONE     | 它是值1的base58编码。在账本中，RippleState条目使用此地址作为信任行余额发行者的占位符。 | 黑洞地址 |
| rHb9CJAWyB4rj91VRWn96DkukG4bwdtyTh | 创世块地址     | 独立节点模式下,作为xrp发布者地址使用                         | 否       |
| rrrrrrrrrrrrrrrrrNAMEtxvNvQ        | RippleName预留 | Ripple要求用户将XRP发送到此帐户以保留Ripple Names。          | 是       |
| rrrrrrrrrrrrrrrrrrrn5RM1rHd        | NaN地址        | 当base58编码值NaN时，先前版本的ripple-lib会生成此地址。      | 是       |

#### 账户的持久性

账户一旦创建后，XRP Ledger的数据树(默克尔树)中将永远存在一个帐户。这是因为这个账户必须永久跟踪该账户当前交易的序列号，以便不能再次处理旧交易。

与比特币和许多其他加密货币不同，`XRP Ledger`公共账本的链的每个新账本都包含账本的完整状态，每个新账户的大小都会增加。因此，除非完全有必要，否则Ripple不鼓励创建新帐户。代表许多用户发送和接收价值的机构(交易所或者其他商业机构 )可以使用源标签和目标标签( [**Source Tags** and **Destination Tags**](https://developers.ripple.com/become-an-xrp-ledger-gateway.html#source-and-destination-tags) )来区分与客户的付款，同时仅使用XRP Ledger中的一个（或少数）帐户

#### 交易历史

在XRP Ledger中，交易历史记录由交易的标识hash值和账本索引(`ledger index`)链接的交易的“线程”跟踪。`AccountRoot`账本对象具有最近修改它的交易的标记Hash值账本; 该交易的元数据包括AccountRoot节点的先前状态，因此可以通过这种方式迭代单个帐户的历史记录。此交易历史记录包括直接修改AccountRoot节点的所有交易，包括：

- 帐户发送的交易，因为它们修改了帐户的序列号。因为是交易就会存在成本( 手续费), 这些交易还会修改帐户的XRP余额。
- 修改帐户XRP余额的交易 , 包括收款付,款交易和其他类型的交易，如`PaymentChannelClaim`和`EscrowFinish`。

帐户交易历史记录的概念还包括修改帐户拥有的对象和非XRP余额的交易。这些对象有单独账本的对象，每个对象都有自己的影响它们的交易线程。如果拥有帐户的完整的账本历史，则可以向前跟踪以查找由其创建或修改的所有账本对象.

`“完整”交易历史记录`包括交易 拥有的对象的历史记录, 如

- 连接到帐户的`RippleState对`象（Trust Lines）。
- `DirectoryNode`对象，尤其是跟踪帐户拥有的对象的所有者目录。
- `Offer`对象，代表去中心化交易所中账户的未提现的订单交易
- `PayChannel`对象，表示与帐户之间的异步支付渠道
- `Escrow`(托管)对象，表示按时间或加密条件锁定的帐户持有的付款。
- `SignerList`对象，表示可以通过多重签名授权帐户交易的的地址列表。

#### 地址编码

XRP Ledger地址使用base58和Ripple字典进行编码：`rpshnaf39wBUDNEGHJKLM4PQRST7VWXYZ2bcdeCg65jkm8oFqi1tuvAxyz` 由于XRP Ledger使用base58对几种类型的密钥进行编码，因此它使用一个字节的“类型前缀”（也称为“版本前缀”）为编码数据添加前缀以区分它们. 类型前缀导致地址通常以base58格式的不同字母开头。

下图显示了键和地址之间的关系

![img](http://ww1.sinaimg.cn/large/8599e4cfly1fxwtqxj31uj20n60czjs8.jpg)

计算XRP分地址的公式如下。有关完整的示例代码，请参阅encode_address.js。

1. 导入所需的算法：SHA-256，RIPEMD160和base58。为base58设置字典。

   ```js
   'use strict';
   const assert = require('assert');
   const crypto = require('crypto');
   const R_B58_DICT = 'rpshnaf39wBUDNEGHJKLM4PQRST7VWXYZ2bcdeCg65jkm8oFqi1tuvAxyz';
   const base58 = require('base-x')(R_B58_DICT);
   
   assert(crypto.getHashes().includes('sha256'));
   assert(crypto.getHashes().includes('ripemd160'));
   ```

2. 首先使用33字节的ECDSA secp256k1公钥或32字节的Ed25119公钥。对于Ed25519密钥，在密钥前面加上字节0xED。

   ```js
   const pubkey_hex =
     'ED9434799226374926EDA3B54B1B461B4ABF7237962EAE18528FEA67595397FA32';
   const pubkey = Buffer.from(pubkey_hex, 'hex');
   assert(pubkey.length == 33);
   ```

3. 计算公钥的SHA-256哈希的RIPEMD160哈希值。该值是“帐户ID”。

   ```js
   const pubkey_inner_hash = crypto.createHash('sha256').update(pubkey);
   const pubkey_outer_hash = crypto.createHash('ripemd160');
   pubkey_outer_hash.update(pubkey_inner_hash.digest());
   const account_id = pubkey_outer_hash.digest();
   ```

4. 计算帐户ID的SHA-256哈希的SHA-256哈希值;取前4个字节。该值是“校验和”。

   ```js
   const address_type_prefix = Buffer.from([0x00]);
   const payload = Buffer.concat([address_type_prefix, account_id]);
   const chksum_hash1 = crypto.createHash('sha256').update(payload).digest();
   const chksum_hash2 = crypto.createHash('sha256').update(chksum_hash1).digest();
   const checksum =  chksum_hash2.slice(0,4);
   ```

5. 连接有效负载和校验和。计算连接缓冲区的base58值。结果是地址。

   ```js
   const dataToEncode = Buffer.concat([payload, checksum]);
   const address = base58.encode(dataToEncode);
   console.log(address);
   // rDTXLQ7ZKZVKz33zJbHjgVShjsBnqMBhmN
   ```

### 加密密钥

在XRP Ledger中，数字签名证明交易被授权执行一组特定的操作。只有已签名的交易才能提交到网络并记录进经过验证的账本.

每个数字签名都是 由与交易发送帐户关联的加密密钥对生成。 可以使用任何XRP Ledger支持的加密签名算法生成密钥对。无论使用何种算法生成密钥对，密钥对都可以用作主密钥对，常规密钥对或签名者列表的成员

#### 生成密钥对

您使用`wallet_propose`方法生成密钥对 , 以下是一个成功生成密钥对的响应

```json
{
  "result": {
    "account_id": "rDGnaDqJczDAjrKHKdhGRJh2G7zJfZhj5q",
    "key_type": "secp256k1",
    "master_key": "COON WARN AWE LUCK TILE WIRE ELI SNUG TO COVE SHAM NAT",
    "master_seed": "sstV9YX8k7yTRzxkRFAHmX7EVqMfX",
    "master_seed_hex": "559EDD35041D3C11F9BBCED912F4DE6A",
    "public_key": "aBQXEw1vZD3guCX3rHL8qy8ooDomdFuxZcWrbRZKZjdDkUoUjGVS",
    "public_key_hex": "0351BDFB30E7924993C625687AE6127034C4A5EBA78A01E9C58B0C46E04E3A4948"
  },
  "status": "success",
  "type": "response"
}
```

响应包含`密钥对`（私钥和公钥，各种格式）, `account_id`, `key_type(加密算法类型)`。

**Private Key(私钥)**

`master_key`，`master_seed`和`master_seed_hex`是各种格式的私钥，所有这些都可用于交易签名 . 尽管以`master_`为前缀，但这些密钥不一定是帐户的主密钥。此响应中，`master_`前缀更多地指代 密钥作为`私钥的角色。 `master_seed`是主种子，从中可以派生有关此帐户的所有其他信息。

**Public Key(公钥)**

`public_key`和`public_key_hex`是公钥的两种不同格式，`public_key_hex`是与交易签名的私钥对应的公钥。`public_key`和`public_key_hex`都直接从`master_seed`推导出来。此外，account_id在收到资金并`创建帐户`的`交易`之后才能发送其他交易。

但是，account_id（无资金帐户）可以用作常规密钥或签名者列表的成员，以授权存在的另一个帐户的交易。

要创建存储在分账本中的资金帐户，account_id必须接收支付交易，该交易需提供足够的XRP以满足保留要求。

**account_id(用户ID)**

`account_id`由公钥推导而来，并指定在XRP Ledger中创建帐户的可能性。重要的是，虽然存在account_id，但在account_id收到其`首次XRP付款`之前，XRP Ledger中不存在实际帐户。

生成的`密钥对`可以在以下三种情况下使用 : `主密钥对`, `常规密钥对`, `签名组成员`

**Key Type(加密算法类型)**

字段key_type指示使用哪种加密签名算法来生成此密钥对。当您使用wallet_propose方法生成密钥对的请求时，可以指定key_type.

#### 主密钥对

主密钥对由私钥和公钥组成。

除了能够签名常规密钥对可以进行的所有交易之外，主密钥对的私钥是唯一可用于执行以下操作的密钥：

- 禁用主公钥(master_public_key)。
- 永久放弃冻结的能力。
- 发送 0 手续费的 `密钥重置交易(常规密钥)`

#### 常规密钥对

XRP Ledger允许帐户授权一个二级密钥对，称为常规密钥对，用于签署未来的交易，同时保持主密钥对脱机。如果常规密钥对的私钥被泄露，可以删除或替换它，而无需更改帐户的其余部分并重新建立与其他帐户的关系。还可以主动轮换常规密钥对(对于帐户的主密钥对，这两种情况都不可能，这与帐户的地址有内在联系。）

使用`wallet_propose`方法生成密钥对以用作常规密钥对。但是，与主密钥对不同，主密钥对与其支持的帐户的account_id并行生成, 必须明确创建常规密钥对与您希望其为其签名交易的帐户之间的关系。使用`SetRegularKey`方法为帐户分配常规密钥对。

查看 [Assign a Regular Key Pair](https://developers.ripple.com/assign-a-regular-key-pair.html) 为账户分配常规密钥对教程.

将常规密钥对分配给帐户后，该帐户有两个与之关联的密钥对：

- 离线保存的主密钥对
- 已明确分配给该帐户的常规密钥对，以及用于为该帐户签署交易的常规密钥对。

可以为帐户分配一个常规密钥对，使用它对所有交易进行签名，但保留主密钥对的交易除外。

可以随时删除或更改常规密钥对。这意味着如果常规私钥被泄露（但主私钥不被破坏），您只需删除或更改常规密钥对即可重新获得对帐户的控制权。

### 账户资金储备

要提交交易，账户地址必须在共享全局账本中保留最小量的XRP, 意味着无法将此XRP发送到其他地址。要为新地址提供资金，您必须发送足够的XRP以满足保留要求。

目前的最低储备要求是20 XRP。 （这是账本中在不拥有其他 账本对象的最低成本)

#### 基本和私人备用金

储备金要求分为两部分：

- 基本储备是分类帐中每个地址所需的最小XRP数量。目前，这是20 XRP（20000000 drops）
- 私人储备是为了满足地址拥有足够的空间来存储不同的账本对象。目前，每一个账本对象需要5 XRP（5000000 drops)

#### 私人备用金

从分类帐中删除对象时，它们不再计入其所有者的保留要求。

- 所有者可以通过发送`OfferCancel`交易或通过发送包含`OfferSequence`参数的`OfferCreate`交易来取消记录账本
- `Trust line`在两个地址之间共享
- 出于所有者保留的目的，单个SignerList的数量为3到10个对象，具体取决于它具有多少成员。另请参见：SignerLists和Reserves。
- [Held Payments (Escrow)](https://developers.ripple.com/escrow-object.html) 由放置它们的地址所有
- [Payment Channels](https://developers.ripple.com/use-payment-channels.html) 由创建它们的地址所有。
- [Owner directories](https://developers.ripple.com/directorynode.html) 列出了有助于地址所有者保留的所有分类帐对象。但是，所有者目录本身不计入保留。
- [Checks](https://developers.ripple.com/checks.html) 由创建它们的地址（发件人，而不是目的地）拥有。

### 交易费用

为保护XRP Ledger免受垃圾邮件和拒绝服务攻击的干扰，每个事务必须消耗少量的XRP。该交易成本旨在维护整个网络的健康发展.

#### 当前交易成本

目前，标准交易需要花费的最低交易成本为0.00001 XRP（10drops）会因网络负载而增加。

查询交易成本: [query `rippled` for the current transaction cost](https://developers.ripple.com/transaction-cost.html#querying-the-transaction-cost).

#### 特殊交易成本

不同类型的交易由不同的交易成本

| 交易类型                             | 正常网络下的交易成本                     |
| ------------------------------------ | ---------------------------------------- |
| 普通交易(reference Transction)       | 10 drops                                 |
| 密钥重置(Key reset Transaction)      | 0                                        |
| 多签名交易(Multi-sigled Transaction) | 10 drops * (1 + 签名人数)                |
| EscrowFinish交易                     | 10 drops * (33 + (文本大小(字节) ÷ 16 )) |

#### 用户交易产生的交易成本的归谁所有

交易手续费不会归任何人所有: 不像类似BItcoin及其他的数字货币, 手续费是奖励给打包者, XRP 产生的交易费直接被销毁掉. 由于不会有新的XRP产生, 这会使得现有的XRP更加稀缺，并使XRP更有价值，从而使持有XRP的用户受益。

#### 加载成本和打开账本成本

启用`FeeEscalation amendment` 后，交易成本有两个阈值

- 如果交易成本不满足rippled 服务器的基于负载的交易成本阈值，则服务器将完全忽略该交易
- 如果交易成本不符合rippled server 的开放式分类帐成本阈值，则服务器会将该交易排队等待后续账本。

将交易分为大致三类：

- 指定较低的交易成本使交易拒绝
- 指定交易成本高到足以包含在当前未结分类帐中的交易。
- 中间的交易，排队等待后来的分类帐版本

#### 本地加载成本

每个rippled 节点根据其当前网络的负载维护一个成本阈值 , 如果提交的交易的费用值低于rippled server的当前基于负载的交易成本 , 该服务将无效 . (注意: 如果通过管理员连接提交交易，只要交易符合未按比例的最低交易成本，服务器就会应用并转发交易) . 除非其费用值满足大多数服务器的要求，否则交易不太可能在共识流程中存活

#### 创建新账本的成本

rippled 具有用于强制执行交易成本的第二种机制，称为开放式分类帐成本。如果交易符合XRP中的开放式分类帐成本要求，则该交易只能包含在开放式分类帐中。不符合新建账本成本的交易可能会排队等待下一个账本。

对于每个新的账本，服务器根据前一个账本中的交易数量选择要包含在open ledger 中的交易数量的限制。之后，open ledger中包含的每个交易的开放式分类帐成本呈指数级增长。对于下一个账本，如果当前账本包含的交易数多于软限制，则服务器(节点)会增加限制，如果共识过程需要超过5秒，则会降低软限制。

#### 交易排队

当rippled 收到满足服务器本地负载成本而非 `open ledger`的交易时，服务器会预估该交易是否“可能包含在后续账本中”。 大多数交易的成本与 `reference Transaction`成本相同, 某些交易（例如`multi-signed`交易）需要此费用的倍数。当open ledger成本上升时，需求与交易的基本成本成比例

#### 查询交易成本

rippled APIs 有两种方法来查询 `local load-based`交易成本: 使用`server_info`和`server_state`命令

如果启用了 `FeeEscalation amendment` , 可以使用`fee` 方法查询`open ledger`的交易手续费.

##### 1. Server_info方法

`server_info`方法 返回的响应中位于`validated_ledger.base_fee_xrp`字段。通过将`base_fee_xrp`值乘以响应(response)中`load_factor`参数来缩放中继交易所需的实际成本，该响应表示服务器的当前负载级别 .

```
Current Transaction Cost in XRP = base_fee_xrp × load_factor
```

##### 2. Server_state方法

server_state方法返回rippled内部负载计算的直接表示。在这种情况下，有效负载率是当前load_factor与load_base的比率。

```
Current Transaction Cost in Drops = (base_fee × load_factor) ÷ load_base
```

通常，XRP Ledger完全按照签名执行交易。

#### 指定交易成本

每个签过名的交易必须在`Fee `字段中包含交易成本。与已签名交易的所有字段一样，如果签名有效，则无法更改此字段。因此，每笔交易的手续费用都会被销毁，即使自定义的的手续飞数量远远超过网络中可执行的最低易交成本。

在交易进行签名之前, 建议先查询`查询当前交易手续费`. 如果不打算立即提交交易，建议您设定稍高的交易成本，因为可能在未来由于网络负载增加导致交易成本上升, 使当前的交易无法成功提交。

#### 自动指定交易成本

在在正式rippled 网络上发起交易时，可以省略`Fee`字段。在这种情况下，rippled或RippleAPI会自动检查p2p的状态以获取当前可提交交易的成本，并在交易签名之前添加Fee字段值, 但是这种方式有几点限制和不足:

- 如果在签名和构建交易的过程中网络的交易成本上升，则可能无法确认交易。
  - 最坏的情况是，交易可能会陷入既未明确确认或拒绝的状态，除非它包含`LastLedgerSequence`参数或直到您使用相同序列号的新的交易取消这笔交易。
- 事先并不知道此笔交易的手续费
  - 如果使用的是 rippled，还可以使用`sign`方法的`fee_mult_max`和`fee_div_max`参数来设置交易手续费的最大值。
- 无法从离线的节点中查找当前交易的手续费。
- `multi-signed`交易 , 无法使用自动指定交易成本XRP简单的交易

### 提交可靠的交易

#### 背书

XRP Ledger协议提供了在网络中所有服务器之间共享的分类帐本(分布式账本) 。通过共识和验证的过程，网络同意将交易应用于（或省略）分类账的顺序。

提交给可信任的XRP Ledger 服务器的格式良好的交易通常在几秒钟内就会得到验证或者拒绝, 然而有些情况下 , 即使是格式良好的交易既没有得到验证也没有被迅速拒绝!如果 程序发送交易后, 全局的交易成本增加, 则可能会法政这种特殊的情况. 如果 交易的成本高于交易中指定的 成本 (手续费), 则该交易无法被打包到新的账本中(区块中) . 如果稍晚日期的全局交易成本降低, 则该笔交易可以被打包进新的账本中, 如果交易未指定过期时间, 则此后该笔交易发生的时间没有限制.

如果发生电源或网络中断, 应用程序将面临更多的挑战, 以查找提交交易的状态. 应用程序( 钱包等)必须注意正确的提交交易, 然后才能正确的获取可信的结果

#### 交易时间表

XRP Ledger 为提交交易提供了几个API , 包含`rippled API, and RippleAPI. `无论使用何种API，交易都应用于ledger，如下所示。

1. 账户所有者创建并签名交易
2. 所有者将 交易作为候选交易提交给网络
   - 格式错误或者无意义的交易会立即被拒绝
   - 格式良好的交易可能暂时成功,但是随后失败
   - 格式良好的交易可能暂时失败, 但是随后成功
   - 格式良好的交易可能暂时成功了, 随后以稍微不同的方式取得成功(如通过消费不同的报价并实现比临时执行更好或更差的汇率)
3. 通过共识和验证,交易将应用与 ledger, 甚至应用了一些失败的交易, 以强制通过网络传播
4. 经过验证的账本包括交易 , 其影响反映在分类帐状态中,'
   - 交易结果不再是临时的, 结果不管是成功还是失败都无法改变

#### 交易

提交和验证是两个单独的程序，可以使用本文档中描述的逻辑来实现。

1. 提交 - 将交易提交到网络, 并返回临时结果
2. 验证 - 可信任的结果的通过经过验证的账本被得以确认

##### 1.提交

如果在提交完成之前出现电源故障或网络故障，请在提交前保留交易的详细信息。重新启动时，持久值可以验证交易的状态。

提交的流程:

1. 构建并签名交易
   - 包括 `LastLedgerSequence`参数
2. 保留交易详情 ;

- 交易hash
- `LastLedgerSequence`
- 发送者地址和序号
- 提交时的最新验证 Ledger 索引
- 特定于应用程序的数据, 根据需要

1. 提交 交易

##### 2.验证

在正常操作期间，应用程序可以通过交易哈希检查提交的交易的状态;或者，根据所使用的API，在验证（或失败）交易时接收通知。 正常的操作可能由于网络错误或者电源故障中断, 在这种中断的情况下，应用程序需要可靠地验证在中断之前可能已经或可能未提交给网络的交易的状态。

重新启动或确定新的最后验证ledger（伪代码)

```c#
For each persisted transaction without validated result:
    Query transaction by hash
    If (result appears in validated ledger)
        # Outcome is final
        Persist the final result
        If (result code is tesSUCCESS)
            Application may act based on successful transaction
        Else
            The transaction failed (1)
            If appropriate for the application and failure type, submit with
                new LastLedgerSequence and Fee

    Else if (LastLedgerSequence > newest validated ledger)
        # Outcome is not yet final
        Wait for more ledgers to be validated

    Else
        If (server has continuous ledger history from the ledger when the
              transaction was submitted up to and including the ledger
              identified by LastLedgerSequence)
            The transaction failed (2)
            If appropriate for the application, submit with
                new LastLedgerSequence and Fee

        Else
            # Outcome is final, but not known due to a ledger gap
            Wait to acquire continuous ledger history
```

##### 3.Ledger 缺失账本

rippled节点应该在有备用资源（CPU / RAM /磁盘IO）的情况下自动获取缺少的分类帐本，除非分类帐的存储历史记录量超过其配置的历史记录量。根据差额的大小和服务器的资源使用情况，获取丢失的分类帐应该需要几分钟时间。还可以使用ledger_request方法手动请求服务器获取历史分类帐版本。

或者，还可以使用已具有所需账本历史记录的不同rippled 节点查找交易的状态，例如`s2.ripple.com`节点(拥有完整的账本历史) , 恶意的节点可以被编程为 提供关于交易的状态和结果的错误信息。

#### 开发相关

要实现交易提交和验证最佳方式，应用程序需要执行以下操作：

1. 确定 签名账户的下一个序列号

a. 每笔交易都有一个`特定于账户的序列号`。这保证了帐户签署的交易的执行顺序，并且可以安全地重新提交交易，而不会有多次将同一笔交易记录在账本中。

1. 决定一个 `LastLedgerSequence`
   - 交易的`LastLedgerSequence`是根据上次验证的分类账本序列号计算而得。
2. 构建并签名交易
   - 在提交之前保留已签名交易的详细信息.
3. 签名此交易
   - 初始的结果是临时的，可能会有变化。
4. 确认一笔交易的最终结果
   - 最终结果将被记录进账本中, 不可修改

应用程序如何实现这些操作, 取决于程序使用的API。应用程序可以使用以下任何接口：

1. The [`rippled` API](https://developers.ripple.com/rippled-api.html)
2. [RippleAPI](https://developers.ripple.com/rippleapi-reference.html)
3. 其他构建与rippled 分层之上的API

#### rippled - 提交和验证交易

##### 1. 确认账户序列号

rippled 提供 `account_info` 方法获得在最后经验证过账本中帐户的序列号.

```
JSON-RPC请求格式
{
  "method": "account_info",   #JSON-RPC method
  "params": [
    {
      "account": "rG5Ro9e3uGEZVCh3zu5gB9ydKUskCs221W", #账户地址
      "ledger": "validated"						       # 账本状态: 验证过
    }
  ]
}
响应结果
{
    "result": {
        "validated": true,			   # validated状态
        "status": "success",
        "ledger_index": 10266396,
        "account_data": {
            "index": "96AB97A1BBC37F4F8A22CE28109E0D39D709689BDF412FE8EDAFB57A55E37F38",
            "Sequence": 4,				# 序列号
            "PreviousTxnLgrSeq": 9905632,
            "PreviousTxnID": "CAEE0E34B3DB50A7A0CA486E3A236513531DE9E52EAC47CE4C26332CC847DE26",
            "OwnerCount": 2,
            "LedgerEntryType": "AccountRoot",
            "Flags": 0,
            "Balance": "49975988",
            "Account": "rG5Ro9e3uGEZVCh3zu5gB9ydKUskCs221W"
        }
    }
}
```

此例: 此账户的上次已验证账本的序列号为4, 注意返回的参数中, `"validated" `值为 true , 说明已经验证

如果程序提交由此帐户签名的三笔交易 , 这三笔交易将会使用序号为4,5,6 . 提交多个交易而无需等待每个交易的验证, 一个程序应该保留正在运行的账户序列号.

##### 2. 确定最后验证的账本

使用`server_state`方法将返回上次 已经验证过的账本索引, 示例如下

```
JSON-RPC 请求
{
  "id": "client id 1",
  "method": "server_state"
}
响应格式
{
    "result": {
        "status": "success",  #状态
        "state": {
            "validation_quorum": 3,
            "validated_ledger": {
                "seq": 10268596,   # 分类账本序列号
                "reserve_inc": 5000000,
                "reserve_base": 20000000,
                "hash": "0E0901DA980251B8A4CCA17AB4CA6C3168FE83FA1D3F781AFC5B9B097FD209EF",
                "close_time": 470798600,
                "base_fee": 10
            },
            "server_state": "full",
            "published_ledger": 10268596,
            "pubkey_node": "n9LGg37Ya2SS9TdJ4XEuictrJmHaicdgTKiPJYi8QRSdvQd3xMnK",
            "peers": 58,
            "load_factor": 256000,
            "load_base": 256,
            "last_close": {
                "proposers": 5,
                "converge_time": 3004
            },
            "io_latency_ms": 2,
            "fetch_pack": 10121,
            "complete_ledgers": "10256331-10256382,10256412-10268596",
            "build_version": "0.26.4-sp3-private"
        }
    }
}
```

此例中表明 ,上一个已经验证的账本序列号为: `10268596` . 另请注意，此示例表示分类帐历史记录中的间隙 (`complete_ledgers`字段)。此节点无法提供账本索引从10256383到10256411中的交易信息. 如果配置如此，节点最终将检索分类帐历史记录的这一部分

##### 3. 构建交易

rippled 为准备提交一笔交易提供了`sign`方法 , 此方法需要一个账户的密钥, 该密钥只能传递给受信任的rippled 节点(实例)。 如下实例展示了 10个法币到另一个XRP账户地址

```
JSON-RPC请求
{
    "method": "sign",						          # sign方法
    "params": [
        {
            "offline": true,
            "secret": "sssssssssssssssssssssssssssss",  #密钥
            "tx_json": {
               "Account": "rG5Ro9e3uGEZVCh3zu5gB9ydKUskCs221W",
                "Sequence": 4,                         # 从account_info中获得
                "LastLedgerSequence": 10268600,
                "Fee": 10000,
                "Amount": {
                    "currency": "FOO",
                    "issuer": "rG5Ro9e3uGEZVCh3zu5gB9ydKUskCs221W", #本地址
                    "value": "10"
                },
                "Destination": "rawz2WQ8i9FdTHp4KSNpBdyxgFqNpKe8fM", #目标地址
                "TransactionType": "Payment"
            }
        }
    ]
}
```

**注意** : 应用程序请求参数指定账户的序列号`Sequence:4`, 由 `account_info`方法获得, 避免`tefPAST_SEQ`错误.

还要注意 `LastLedgerSequence`的值是通过`server_state`方法基于最进的经过验证的账本中获得的。后端开发程序的建议是使用(*last validated ledger sequence + 4*)。或者, 使用(`当前ledger +3`) 的值 . 如果`LastLedgerSequence`计算错误，且小于最后一个验证的账本 , 交易将会出现``tefMAX_LEDGER` `错误.

```
响应格式
{
    "result": {
        "tx_json": {
            "hash": "395C313F6F11F70FEBAF3785529A6D6DE3F44C7AF679515A7EAE22B30146DE57",
            "TxnSignature": "304402202646962A21EC0516FCE62DC9280F79E7265778C571E9410D795E67BB72A2D8E402202FF4AF7B2E2160F5BCA93011CB548014626CAC7FCBEBDB81FE8193CEFF69C753",
            "TransactionType": "Payment",
            "SigningPubKey": "0267268EE0DDDEE6A862C9FF9DDAF898CF17060A673AF771B565AA2F4AE24E3FC5",
            "Sequence": 4,
            "LastLedgerSequence": 10268600,
            "Flags": 2147483648,
            "Fee": "10000",
            "Destination": "rawz2WQ8i9FdTHp4KSNpBdyxgFqNpKe8fM",
            "Amount": {
                "value": "10",
                "issuer": "rG5Ro9e3uGEZVCh3zu5gB9ydKUskCs221W",
                "currency": "FOO"
            },
            "Account": "rG5Ro9e3uGEZVCh3zu5gB9ydKUskCs221W"
        },
        "tx_blob": "12000022800000002400000004201B009CAFB861D4C38D7EA4C68000000000000000000000000000464F4F0000000000AC5FA3BB28A09BD2EC1AE0EED2315060E83D796A68400000000000271073210267268EE0DDDEE6A862C9FF9DDAF898CF17060A673AF771B565AA2F4AE24E3FC57446304402202646962A21EC0516FCE62DC9280F79E7265778C571E9410D795E67BB72A2D8E402202FF4AF7B2E2160F5BCA93011CB548014626CAC7FCBEBDB81FE8193CEFF69C7538114AC5FA3BB28A09BD2EC1AE0EED2315060E83D796A831438BC6F9F5A6F6C4E474DB0D59892E90C2C7CED5C",
        "status": "success"
    }
}
```

程序应该在提交交易之前之前。保存`tx_json`哈希值

##### 4. 提交交易

rippled 提供 `submit`方法, 允许我们提交`已经签名过的交易`, 添加`tx_blob`(用sign方法返回的值)参数请求服务器.

```
JSON-RPC请求
{
    "method": "submit",
    "params": [
        {
        "tx_blob": "12000022800000002400000004201B009CAFB861D4C38D7EA4C68000000000000000000000000000464F4F0000000000AC5FA3BB28A09BD2EC1AE0EED2315060E83D796A68400000000000271073210267268EE0DDDEE6A862C9FF9DDAF898CF17060A673AF771B565AA2F4AE24E3FC57446304402202646962A21EC0516FCE62DC9280F79E7265778C571E9410D795E67BB72A2D8E402202FF4AF7B2E2160F5BCA93011CB548014626CAC7FCBEBDB81FE8193CEFF69C7538114AC5FA3BB28A09BD2EC1AE0EED2315060E83D796A831438BC6F9F5A6F6C4E474DB0D59892E90C2C7CED5C"
        }
    ]
}
*响应格式*
{
    "result": {
        "tx_json": {
            "hash": "395C313F6F11F70FEBAF3785529A6D6DE3F44C7AF679515A7EAE22B30146DE57",
            "TxnSignature": "304402202646962A21EC0516FCE62DC9280F79E7265778C571E9410D795E67BB72A2D8E402202FF4AF7B2E2160F5BCA93011CB548014626CAC7FCBEBDB81FE8193CEFF69C753",
            "TransactionType": "Payment",
            "SigningPubKey": "0267268EE0DDDEE6A862C9FF9DDAF898CF17060A673AF771B565AA2F4AE24E3FC5",
            "Sequence": 4,
            "LastLedgerSequence": 10268600,
            "Flags": 2147483648,
            "Fee": "10000",
            "Destination": "rawz2WQ8i9FdTHp4KSNpBdyxgFqNpKe8fM",
            "Amount": {
                "value": "10",
                "issuer": "rG5Ro9e3uGEZVCh3zu5gB9ydKUskCs221W",
                "currency": "FOO"
            },
            "Account": "rG5Ro9e3uGEZVCh3zu5gB9ydKUskCs221W"
        },
        "tx_blob": "12000022800000002400000004201B009CAFB861D4C38D7EA4C68000000000000000000000000000464F4F0000000000AC5FA3BB28A09BD2EC1AE0EED2315060E83D796A68400000000000271073210267268EE0DDDEE6A862C9FF9DDAF898CF17060A673AF771B565AA2F4AE24E3FC57446304402202646962A21EC0516FCE62DC9280F79E7265778C571E9410D795E67BB72A2D8E402202FF4AF7B2E2160F5BCA93011CB548014626CAC7FCBEBDB81FE8193CEFF69C7538114AC5FA3BB28A09BD2EC1AE0EED2315060E83D796A831438BC6F9F5A6F6C4E474DB0D59892E90C2C7CED5C",
        "status": "success",
        "engine_result_message": "The transaction was applied.",
        "engine_result_code": 0,
        "engine_result": "tesSUCCESS"
    }
}
```

**注意**: 这只是一个初始的结果. 最终的结果只能从 `经验证过的账本(区块)`中获得 . 返回的结果 缺少 `validated: true`的字段说明这不是一个不可更改的结果.

##### 5.验证交易

交易的哈希是在交易签名(`sign`方法)时生成的，它被传递给`tx`方法以检索交易的结果.

```
JSON-RPC请求
{
    "method": "tx",
    "params": [
        {
            "transaction": "395C313F6F11F70FEBAF3785529A6D6DE3F44C7AF679515A7EAE22B30146DE57", # 交易哈希值
            "binary": false
        }
    ]
}
*响应结果*
{
    "result": {
        "validated": true,  # 说明已经确认过此账本
        "status": "success",
        "meta": {
            "TransactionResult": "tesSUCCESS",
            "TransactionIndex": 2,
            "AffectedNodes": [...]
        },
        "ledger_index": 10268599[d],
        "inLedger": 10268599,
        "hash": "395C313F6F11F70FEBAF3785529A6D6DE3F44C7AF679515A7EAE22B30146DE57",
        "date": 470798270,
        "TxnSignature": "304402202646962A21EC0516FCE62DC9280F79E7265778C571E9410D795E67BB72A2D8E402202FF4AF7B2E2160F5BCA93011CB548014626CAC7FCBEBDB81FE8193CEFF69C753",
        "TransactionType": "Payment",
        "SigningPubKey": "0267268EE0DDDEE6A862C9FF9DDAF898CF17060A673AF771B565AA2F4AE24E3FC5",
        "Sequence": 4,
        "LastLedgerSequence": 10268600,
        "Flags": 2147483648,
        "Fee": "10000",
        "Destination": "rawz2WQ8i9FdTHp4KSNpBdyxgFqNpKe8fM",
        "Amount": {
            "value": "10",
            "issuer": "rG5Ro9e3uGEZVCh3zu5gB9ydKUskCs221W",
            "currency": "FOO"
        },
        "Account": "rG5Ro9e3uGEZVCh3zu5gB9ydKUskCs221W"
    }
}
```

返回的结果中`validated:true`指明此交易已被记录进账本中, 说明此笔交易不可变 . 此外，返回数据还包括`“TransactionResult”:“tesSUCCESS”`，表示该交易已应用于账本中。

如果结果中没有`validator:ture`的字段 , 说明结果是临时的, 结果可能会变 . 要检查交易最终结果，程序必须再次调用`tx`方法，以便网络有足够的时间验证更多的账本。可能需要等待`LastLedgerSequence`中指定的账本被验证，尽管如果交易包含在更早的验证分类账中，说明此交易已经是不可变的.

##### 6.验证缺失的交易

程序必须处理调用`tx`方法返回`txnNotFound`错误的情况。

```
请求方法JSON-RPC
{
    "result": {
        "status": "error",
        "request": {
            "transaction": "395C313F6F11F70FEBAF3785529A6D6DE3F44C7AF679515A7EAE22B30146DE56",
            "command": "tx",
            "binary": false
        },
        "error_message": "Transaction not found.",
        "error_code": 24,
        "error": "txnNotFound"
    }
}
```

`txnNotFound`结果代码不会发生在任何账本中。然而, 它也可能发生在rippled节点没有完整账本历史时，或者交易还没有传播到rippled节点时 , 程序应该做进一步检查以做出应对措施

使用`server_state`方法 (在确定最后验证账本之前使用) 从返回结果(`result.state.complete_ledgers`)中获得账本历史是否完整.

```
响应结果
{
    "result": {
        "status": "success",
        "state": {
            "validation_quorum": 3,
            "validated_ledger": {
                "seq": 10269447,
                "reserve_inc": 5000000,
                "reserve_base": 20000000,
                "hash": "D05C7ECC66DD6F4FEA3A6394F209EB5D6824A76C16438F562A1749CCCE7EAFC2",
                "close_time": 470802340,
                "base_fee": 10
            },
            "server_state": "full",
            "pubkey_node": "n9LJ5eCNjeUXQpNXHCcLv9PQ8LMFYy4W8R1BdVNcpjc1oDwe6XZF",
            "peers": 84,
            "load_factor": 256000,
            "load_base": 256,
            "last_close": {
                "proposers": 5,
                "converge_time": 2002
            },
            "io_latency_ms": 1,
            "complete_ledgers": "10256331-10256382,10256412-10269447",
            "build_version": "0.26.4-sp3-private"
        }
    }
}
```

我们的示例交易指定`LastLedgerSequence` 10268600，基于当时最后一个经过验证的分类帐+4. 为了确定我们丢失的交易是否永久失败，rippled 节点必须有分类账10268597到10268600 .如果节点在其历史记录中具有那些经过验证的账本，并且`tx返回txnNotFound`，则交易已失败，并且不会出现在未来任何账本中.。在这种情况下，程序逻辑可以指明使用`相同的帐户序列`构建(sign)和提交(submit)替换交易并更新`LastLedgerSequence`

节点可能返回最后验证的账本序列号小于指定的`LastLedgerSequence` , 如果是，则`txnNotFound`表示(a)提交的交易尚未分发到网络，或（b）交易已分发到网络但尚未处理。为了处理前一种情况(`交易未发布到网络`)，程序可以再次提交相同的签名交易。由于交易具有唯一的帐户序列号，因此`最多可以处理一次`。

最后，节点服务器可以在交易历史中显示一个或多个间隙(Ledger Caps) . 上面响应中显示的`completed_ledgers`字段表示此rippled 实例中缺少从10256383到10256411的账本。以上示例交易只能出现在分类帐10268597 - 10268600中（基于提交时间和`LastLedgerSequence`），因此此处显示的差距不相关。但是，如果间隙指示该范围内的账本丢失，则程序需要查询另一个ripple服务器（或等待此服务器检索丢失的账本）以确定`txnNotFound`结果是不可变的.

### 取消或忽略一笔交易

`XRP Ledger`的一个重要且有意的特征是，一旦交易被打包进到经过验证的账本中，交易就是最终的(不可更改)。

但是，如果交易尚未打包进已验证的账本中，则可以通过将其设置为无效来有效取消该交易。通常，这意味着从同一帐户发送具有相同序列值(`Sequence`)的另一个交易。如果不想替换交易执行任何操作，发送没有选项的`AccountSet`的交易.

例如，如果尝试提交序列号(sequence)为11,12和13的三笔交易，但序号未11的交易以某种方式丢失或者没有足够的成本(手续费)广播到p2p网络上, 然后，可以通过提交`没有选项`和`序列号11`的`AccountSet`的交易来取消序号为11的交易。此交易除了破坏序号为11的交易成本外不会起任何作用, 但它间接的允许交易12和13可执行。

这种方法优于重新编号和重新提交12和13的交易，因为它可以防止交易在不同的序列号下有效地复制。

通过这种方式，没有选项的`AccountSet`交易是规范的“no-op”交易。

### 查找交易结果

使用`tx`和`account_tx`方法来查看交易的最终结果. 或者通过其他方法 的响应中 查找如果存在`“validated”：true`字段则表示此响应已经通过共识算法记录进 账本中(节点)

```
响应中包含以下字段
"hash": "E08D6E9754025BA2534A78707605E0601F03ACE063687A0CA1BDDACFCD1698C7",
"meta": {
    ...
    "TransactionResult": "tesSUCCESS"
},
"validated": true
```

分类: [XRP瑞波币](https://www.cnblogs.com/failymao/category/1382015.html)

标签: [XRP](https://www.cnblogs.com/failymao/tag/XRP/), [账户](https://www.cnblogs.com/failymao/tag/账户/), [XRP交易费用](https://www.cnblogs.com/failymao/tag/XRP交易费用/)

[好文要顶](javascript:void(0);) [关注我](javascript:void(0);) [收藏该文](javascript:void(0);) [![img](https://common.cnblogs.com/images/icon_weibo_24.png)](javascript:void(0);) [![img](https://common.cnblogs.com/images/wechat.png)](javascript:void(0);)

[![img](https://pic.cnblogs.com/face/sample_face.gif)](https://home.cnblogs.com/u/failymao/)

[failymao](https://home.cnblogs.com/u/failymao/)
[关注 - 3](https://home.cnblogs.com/u/failymao/followees/)
[粉丝 - 9](https://home.cnblogs.com/u/failymao/followers/)

[+加关注](javascript:void(0);)

0

0

[« ](https://www.cnblogs.com/failymao/p/9814032.html)上一篇： [Bitcoin区块验证](https://www.cnblogs.com/failymao/p/9814032.html)
[» ](https://www.cnblogs.com/failymao/p/10261412.html)下一篇： [XRP共识算法](https://www.cnblogs.com/failymao/p/10261412.html)

posted @ 2019-01-12 22:58 [failymao](https://www.cnblogs.com/failymao/) 阅读(766) 评论(0) [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=10261402) [收藏](javascript:void(0))





[刷新评论](javascript:void(0);)[刷新页面](https://www.cnblogs.com/failymao/p/10261402.html#)[返回顶部](https://www.cnblogs.com/failymao/p/10261402.html#top)

登录后才能发表评论，立即 [登录](javascript:void(0);) 或 [注册](javascript:void(0);)， [访问](https://www.cnblogs.com/) 网站首页

[【推荐】阿里出品，对标P7！限时免费七天玩转 PostgreSQL 创新营报名开启](https://developer.aliyun.com/learning/trainingcamp/mysql/1?utm_content=g_1000228441)
[【推荐】变强吧2021！零门槛速抢阿里云开发者新年加油包，最高6000元](https://developer.aliyun.com/topic/2021act?utm_content=g_1000227941)
[【推荐】与开发者在一起，云计算领导者AWS入驻博客园品牌专区](https://brands.cnblogs.com/aws)
[【推荐】大型组态、工控、仿真、CADGIS 50万行VC++源码免费下载](http://www.softbam.com/index.htm)
[【推荐】第一个NoSQL数据库，在大规模和一致性之间找到了平衡](https://brands.cnblogs.com/aerospike)
[【推荐】了不起的开发者，挡不住的华为，园子里的品牌专区](https://brands.cnblogs.com/huawei)
[【推荐】未知数的距离，毫秒间的传递，声网与你实时互动](https://brands.cnblogs.com/agora)

<iframe id="google_ads_iframe_/1090369/C1_0" title="3rd party ad content" name="google_ads_iframe_/1090369/C1_0" width="300" height="250" scrolling="no" marginwidth="0" marginheight="0" frameborder="0" srcdoc="" data-google-container-id="1" data-load-complete="true" style="margin: 0px; padding: 0px; border: 0px; vertical-align: bottom;"></iframe>

**相关博文：**
· [凑硬币](https://www.cnblogs.com/zbr162/p/11822676.html)
· [UVA11300分金币](https://www.cnblogs.com/csnd/p/12062678.html)
· [UVA11300分金币](https://www.cnblogs.com/csnd/p/12062679.html)
· [白话比特币](https://www.cnblogs.com/stack010/p/12068307.html)
· [门罗币_挖矿](https://www.cnblogs.com/x-hong/p/12083611.html)
» [更多推荐...](https://recomm.cnblogs.com/blogpost/10261402)

<iframe id="google_ads_iframe_/1090369/C2_0" title="3rd party ad content" name="google_ads_iframe_/1090369/C2_0" width="468" height="60" scrolling="no" marginwidth="0" marginheight="0" frameborder="0" srcdoc="" data-google-container-id="2" data-load-complete="true" style="margin: 0px; padding: 0px; border: 0px; vertical-align: bottom;"></iframe>

**最新 IT 新闻**:
· [抢夺UP主、围剿B站 谁是最有力的竞争对手？](https://news.cnblogs.com/n/685292/)
· [视频号要往哪里去？一场全新热战才刚刚开始](https://news.cnblogs.com/n/685291/)
· [一键查询！春节临近：各地返乡规定看这里](https://news.cnblogs.com/n/685290/)
· [进化后的小鹏NGP变“稳”了 似人般稳重](https://news.cnblogs.com/n/685289/)
· [Windows 10X即将向OEM渠道推送：微软引入防盗功能](https://news.cnblogs.com/n/685288/)
» [更多新闻...](https://news.cnblogs.com/)

### 公告

昵称： [failymao](https://home.cnblogs.com/u/failymao/)
园龄： [4年1个月](https://home.cnblogs.com/u/failymao/)
粉丝： [9](https://home.cnblogs.com/u/failymao/followers/)
关注： [3](https://home.cnblogs.com/u/failymao/followees/)

[+加关注](javascript:void(0))

| [<](javascript:void(0);)2021年1月[>](javascript:void(0);) |      |      |      |      |      |      |
| --------------------------------------------------------- | ---- | ---- | ---- | ---- | ---- | ---- |
| 日                                                        | 一   | 二   | 三   | 四   | 五   | 六   |
| 27                                                        | 28   | 29   | 30   | 31   | 1    | 2    |
| 3                                                         | 4    | 5    | 6    | 7    | 8    | 9    |
| 10                                                        | 11   | 12   | 13   | 14   | 15   | 16   |
| 17                                                        | 18   | 19   | 20   | 21   | 22   | 23   |
| 24                                                        | 25   | 26   | 27   | 28   | 29   | 30   |
| 31                                                        | 1    | 2    | 3    | 4    | 5    | 6    |

### 搜索

 

 

### 我的标签

- [golang基础(12)](https://www.cnblogs.com/failymao/tag/golang基础/)
- [Pow(4)](https://www.cnblogs.com/failymao/tag/Pow/)
- [比特币(4)](https://www.cnblogs.com/failymao/tag/比特币/)
- [python装饰器(3)](https://www.cnblogs.com/failymao/tag/python装饰器/)
- [go闭包(2)](https://www.cnblogs.com/failymao/tag/go闭包/)
- [python(2)](https://www.cnblogs.com/failymao/tag/python/)
- [go函数(2)](https://www.cnblogs.com/failymao/tag/go函数/)
- [interface(2)](https://www.cnblogs.com/failymao/tag/interface/)
- [python算法(2)](https://www.cnblogs.com/failymao/tag/python算法/)
- [go(2)](https://www.cnblogs.com/failymao/tag/go/)
- [更多](https://www.cnblogs.com/failymao/tag/)

### 随笔分类 (46)

- [django restful 知识点(1)](https://www.cnblogs.com/failymao/category/1608200.html)
- [Golang基础知识系列(13)](https://www.cnblogs.com/failymao/category/1241442.html)
- [Js前端(1)](https://www.cnblogs.com/failymao/category/1607719.html)
- [python 小脚本(3)](https://www.cnblogs.com/failymao/category/1762973.html)
- [python精耕细作(5)](https://www.cnblogs.com/failymao/category/1408647.html)
- [python算法(4)](https://www.cnblogs.com/failymao/category/1405104.html)
- [XRP瑞波币(5)](https://www.cnblogs.com/failymao/category/1382015.html)
- [开发规范(1)](https://www.cnblogs.com/failymao/category/1464148.html)
- [区块链底层(1)](https://www.cnblogs.com/failymao/category/1269404.html)
- [设计模式(2)](https://www.cnblogs.com/failymao/category/1664419.html)
- [深入浅出区块链(8)](https://www.cnblogs.com/failymao/category/1202641.html)
- [细说Linux(1)](https://www.cnblogs.com/failymao/category/1192379.html)
- [一个程序员的花花世界(1)](https://www.cnblogs.com/failymao/category/1416311.html)

### 随笔档案 (47)

- [2020年5月(4)](https://www.cnblogs.com/failymao/archive/2020/05.html)
- [2020年3月(3)](https://www.cnblogs.com/failymao/archive/2020/03.html)
- [2019年12月(2)](https://www.cnblogs.com/failymao/archive/2019/12.html)
- [2019年5月(1)](https://www.cnblogs.com/failymao/archive/2019/05.html)
- [2019年3月(6)](https://www.cnblogs.com/failymao/archive/2019/03.html)
- [2019年2月(2)](https://www.cnblogs.com/failymao/archive/2019/02.html)
- [2019年1月(5)](https://www.cnblogs.com/failymao/archive/2019/01.html)
- [2018年10月(6)](https://www.cnblogs.com/failymao/archive/2018/10.html)
- [2018年8月(1)](https://www.cnblogs.com/failymao/archive/2018/08.html)
- [2018年7月(12)](https://www.cnblogs.com/failymao/archive/2018/07.html)
- [2018年6月(2)](https://www.cnblogs.com/failymao/archive/2018/06.html)
- [2018年5月(1)](https://www.cnblogs.com/failymao/archive/2018/05.html)
- [2018年4月(2)](https://www.cnblogs.com/failymao/archive/2018/04.html)

### 文章分类 (3)

- [Golang基础知识系列(1)](https://www.cnblogs.com/failymao/category/1241441.html)
- [Python高能知识点(1)](https://www.cnblogs.com/failymao/category/1192039.html)
- [区块链技术(1)](https://www.cnblogs.com/failymao/category/1249651.html)

### 最新评论

- [1. Re:不管人生怎么走，都需要实时回头看看](https://www.cnblogs.com/failymao/p/8699180.html)
- @ Firelin嗯嗯, 一定会坚持下去的!...
- --failymao
- [2. Re:不管人生怎么走，都需要实时回头看看](https://www.cnblogs.com/failymao/p/8699180.html)
- 一定要坚持下去哦
- --Firelin
- [3. Re:python线程入门](https://www.cnblogs.com/failymao/p/10505911.html)
- @ super_cj谢谢点赞......
- --failymao
- [4. Re:python线程入门](https://www.cnblogs.com/failymao/p/10505911.html)
- 讲的很清楚，通俗易懂 点个赞
- --super_cj
- [5. Re:golang基础--strcut结构体](https://www.cnblogs.com/failymao/p/9310107.html)
- @ 牛腩谢谢 谢谢...
- --failymao

### 阅读排行榜

- [1. python之首字母大写(4474)](https://www.cnblogs.com/failymao/p/10476161.html)
- [2. XRP(瑞波币)账户管理系统(1629)](https://www.cnblogs.com/failymao/p/10261421.html)
- [3. 不管人生怎么走，都需要实时回头看看(1180)](https://www.cnblogs.com/failymao/p/8699180.html)
- [4. 区块链之工作量证明(1097)](https://www.cnblogs.com/failymao/p/8975934.html)
- [5. python字典列表过滤器(1045)](https://www.cnblogs.com/failymao/p/12874623.html)

### 评论排行榜

- [1. 不管人生怎么走，都需要实时回头看看(39)](https://www.cnblogs.com/failymao/p/8699180.html)
- [2. python线程入门(2)](https://www.cnblogs.com/failymao/p/10505911.html)
- [3. golang基础--strcut结构体(2)](https://www.cnblogs.com/failymao/p/9310107.html)
- [4. golang基础--map字典(2)](https://www.cnblogs.com/failymao/p/9286858.html)

### 推荐排行榜

- [1. 不管人生怎么走，都需要实时回头看看(13)](https://www.cnblogs.com/failymao/p/8699180.html)
- [2. python重试次数装饰器(1)](https://www.cnblogs.com/failymao/p/12874250.html)
- [3. python线程入门(1)](https://www.cnblogs.com/failymao/p/10505911.html)

Copyright © 2021 failymao
Powered by .NET 5.0 on Kubernetes