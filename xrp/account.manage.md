# [XRP(瑞波币)账户管理系统](https://www.cnblogs.com/failymao/p/10261421.html)

目录

- 账户管理
  - 分配常规密钥对
    - [1.生成一对密钥](https://www.cnblogs.com/failymao/p/10261421.html#1生成一对密钥)
    - \2. 将密钥对作为常规密钥对分配给帐户
      - [签名交易](https://www.cnblogs.com/failymao/p/10261421.html#签名交易)
      - [提交交易](https://www.cnblogs.com/failymao/p/10261421.html#提交交易)
    - \3. 验证常规密钥对
      - [签名交易](https://www.cnblogs.com/failymao/p/10261421.html#签名交易)
      - [提交交易](https://www.cnblogs.com/failymao/p/10261421.html#提交交易)
  - 修改或移除常规密钥对
    - [更改常规密钥对](https://www.cnblogs.com/failymao/p/10261421.html#更改常规密钥对)
    - 删除常规密钥对
      - [签名交易](https://www.cnblogs.com/failymao/p/10261421.html#签名交易)
      - [提交交易](https://www.cnblogs.com/failymao/p/10261421.html#提交交易)
  - 设置多重签名
    - [先决条件](https://www.cnblogs.com/failymao/p/10261421.html#先决条件)
    - [1. 准备有资金的账户地址](https://www.cnblogs.com/failymao/p/10261421.html#1-准备有资金的账户地址)
    - [2. 准备成本密钥](https://www.cnblogs.com/failymao/p/10261421.html#2-准备成本密钥)
    - [3. 发送SignerListSet交易](https://www.cnblogs.com/failymao/p/10261421.html#3-发送signerlistset交易)
    - [4. 关闭ledger](https://www.cnblogs.com/failymao/p/10261421.html#4-关闭ledger)
    - [5. 确认新的签名列表](https://www.cnblogs.com/failymao/p/10261421.html#5-确认新的签名列表)
  - 发送多签名交易
    - [先决条件](https://www.cnblogs.com/failymao/p/10261421.html#先决条件)
    - [1. 创建一笔交易](https://www.cnblogs.com/failymao/p/10261421.html#1-创建一笔交易)
    - [2 . 获得一个签名](https://www.cnblogs.com/failymao/p/10261421.html#2--获得一个签名)
    - [3 .获取辅助签名](https://www.cnblogs.com/failymao/p/10261421.html#3-获取辅助签名)
    - [4. 结合签名并提交](https://www.cnblogs.com/failymao/p/10261421.html#4-结合签名并提交)
    - [5. 关闭ledger](https://www.cnblogs.com/failymao/p/10261421.html#5-关闭ledger)
    - [6. 确认交易结果](https://www.cnblogs.com/failymao/p/10261421.html#6-确认交易结果)



## 账户管理

### 分配常规密钥对

XRP Ledger允许帐户授权二级密钥对（称为常规密钥对）来对未来的交易进行签名, 如果常规密钥对的私钥被泄露，可以删除或替换它，而无需更改帐户的其余部分并重新建立与其他帐户的关系。还可以`主动轮换常规密钥对`。 （对于帐户的`主密钥对`，这两种情况都不可能，这与帐户的地址有内在联系。）

有关主密钥对和常规密钥对的详细信息，请参阅[Cryptographic Keys](https://developers.ripple.com/cryptographic-keys.html).

常规密钥对分配的步骤

#### 1.生成一对密钥

使用`wallet_propose`方法生成作为常规密钥对分配给帐户

*请求格式*

```shell
rippled wallet_propose   # COmmindline
# json-rpc
{
  "method": "wallet_propose"
}
#Websocket
{
  "command": "wallet_propose"
}
```

*响应格式*

```shell
{
   "result" : {
      "account_id" : "rsprUqu6BHAffAeG4HpSdjBNvnA6gdnZV7",
      "key_type" : "secp256k1",
      "master_key" : "KNEW BENT LYNN LED GAD BEN KENT SHAM HOBO RINK WALT ALLY",
      "master_seed" : "sh8i92YRnEjJy3fpFkL8txQSCVo79",
      "master_seed_hex" : "966C0F68643EFBA50D58D191D4CA8AA7",
      "public_key" : "aBRNH5wUurfhZcoyR6nRwDSa95gMBkovBJ8V4cp1C1pM28H7EPL1",
      "public_key_hex" : "03AEEFE1E8ED4BBC009DE996AC03A8C6B5713B1554794056C66E5B8D1753C7DD0E",
      "status" : "success"
   }
}
```

#### 2. 将密钥对作为常规密钥对分配给帐户

使用`SetRegularKey transaction `将步骤1中生成的密钥对作为常规密钥对分配给所在账户的帐户。

 在第一次为帐户分配常规密钥对时，`SetRegularKey transaction`需要使用帐户的主私钥（私密）进行签名。传输主私钥(master private key)是危险的，因此我们将分两步完成此事务，以使交易签名与提交分离到网络。

 发送后续`SetRegularKey transaction` 时，您可以使用现有的常规私钥(regular key )进行签名以替换或删除自身。请注意，仍然不应该通过网络提交常规私钥

##### 签名交易

交易签名的最安全方式是 **使用本地签名库进行操作, 如[RippleAPI](https://developers.ripple.com/rippleapi-reference.html). **

或者也可以使用 `sign`方法对交易进行签名，但必须通过`可信和加密`的连接来完成, 或通过`本地连接`，仅限于自己控制的服务器。

*请求格式 (JSON-RPC)*

```json
{
   "method": "sign",
   "params": [
      {
         "tx_json": {
            "TransactionType": "SetRegularKey",
            "Account": "rUAi7pipxGpYfPNg3LtPcf2ApiS8aw9A93",
            "RegularKey": "rsprUqu6BHAffAeG4HpSdjBNvnA6gdnZV7"
         },
         "secret": "ssCATR7CBvn4GLd1UuU2bqqQffHki"
      }
   ]
}
```

*响应格式*

```json
{
    "result": {
        "status": "success",
        "tx_blob": "1200052280000000240000000768400000000000000A73210384CA3C528F10C75F26E0917F001338BD3C9AA1A39B9FBD583DFFFD96CF2E2D7A7446304402201453CA3D4D17F0EE3828B9E3D6ACF65327F5D4FC2BA30953CACF6CBCB4145E3502202F2154BED1D7462CAC1E3DBB31864E48C3BA0B3133ACA5E37EC54F0D0C339E2D8114830923439D307E642CED308FD91EF701A7BAA74788141620D685FB08D81A70D0B668749CF2E130EA7540",
        "tx_json": {
            "Account": "rUAi7pipxGpYfPNg3LtPcf2ApiS8aw9A93",
            "Fee": "10",
            "Flags": 2147483648,
            "RegularKey": "rsprUqu6BHAffAeG4HpSdjBNvnA6gdnZV7",
            "Sequence": 4,
            "SigningPubKey": "0384CA3C528F10C75F26E0917F001338BD3C9AA1A39B9FBD583DFFFD96CF2E2D7A",
            "TransactionType": "SetRegularKey",
            "TxnSignature": "304402201453CA3D4D17F0EE3828B9E3D6ACF65327F5D4FC2BA30953CACF6CBCB4145E3502202F2154BED1D7462CAC1E3DBB31864E48C3BA0B3133ACA5E37EC54F0D0C339E2D",
            "hash": "AB73BBF7C99061678B59FB48D72CA0F5FC6DD2815B6736C6E9EB94439EC236CE"
        }
    }
}
```

##### 提交交易

从离线签名响应( `offline signing response`)中获取signedTransaction值或从sign命令响应中获取tx_blob值，并使用`submit方法`将其作为tx_blob值提交

*请求格式(JSON-RPC)*

```json
{
   "method":"submit",
   "params": [
      {
         "tx_blob": "1200052280000000240000000468400000000000000A73210384CA3C528F10C75F26E0917F001338BD3C9AA1A39B9FBD583DFFFD96CF2E2D7A7446304402204BCD5663F3A2BA02D2CE374439096EC6D27273522CD6E6E0BDBFB518730EAAE402200ECD02D8D2525D6FA4642613E71E395ECCEA01C42C35A668BF092A00EB649C268114830923439D307E642CED308FD91EF701A7BAA74788141620D685FB08D81A70D0B668749CF2E130EA7540"
      }
   ]
}
```

*响应格式*

```json
{
    "result": {
       "engine_result": "tesSUCCESS",
       "engine_result_code": 0,
       "engine_result_message": "The transaction was applied. Only final in a validated ledger.",
        "status": "success",
        "tx_blob": "1200052280000000240000000468400000000000000A73210384CA3C528F10C75F26E0917F001338BD3C9AA1A39B9FBD583DFFFD96CF2E2D7A7446304402204BCD5663F3A2BA02D2CE374439096EC6D27273522CD6E6E0BDBFB518730EAAE402200ECD02D8D2525D6FA4642613E71E395ECCEA01C42C35A668BF092A00EB649C268114830923439D307E642CED308FD91EF701A7BAA74788141620D685FB08D81A70D0B668749CF2E130EA7540",
        "tx_json": {
            "Account": "rUAi7pipxGpYfPNg3LtPcf2ApiS8aw9A93",
            "Fee": "10",
            "Flags": 2147483648,
            "RegularKey": "rsprUqu6BHAffAeG4HpSdjBNvnA6gdnZV7",
            "Sequence": 4,
            "SigningPubKey": "0384CA3C528F10C75F26E0917F001338BD3C9AA1A39B9FBD583DFFFD96CF2E2D7A",
            "TransactionType": "SetRegularKey",
            "TxnSignature": "304402204BCD5663F3A2BA02D2CE374439096EC6D27273522CD6E6E0BDBFB518730EAAE402200ECD02D8D2525D6FA4642613E71E395ECCEA01C42C35A668BF092A00EB649C26",
            "hash": "AB73BBF7C99061678B59FB48D72CA0F5FC6DD2815B6736C6E9EB94439EC236CE"
        }
    }
}
```

**请注意**，响应包含交易的哈希值`"hash"`，可以使用该哈希值查找事务的最终结果。

#### 3. 验证常规密钥对

为了验证帐户是否正确设置了常规密钥对，从自己账户中的提交一笔`AccountSet`的交易，并使用在步骤2中分配给帐户的常规密钥(`master_seed`)对其进行签名。

如步骤2中所述，任何情况下**传输主(常规)私钥都是危险**的。因此如步骤二中一样, 将分两步完成此交易，以使`交易签名(sign)`与`事务提交(submit)`分离到网络

##### 签名交易

使用步骤二中的方式进行签名 (`TransactionType`不同)

*请求参数(JSON-RPC)*

```JSON
{
   "method": "sign",
   "params": [
      {
         "tx_json": {
            "TransactionType": "AccountSet",   
            "Account": "rUAi7pipxGpYfPNg3LtPcf2ApiS8aw9A93"
         },
         "secret": "sh8i92YRnEjJy3fpFkL8txQSCVo79"
      }
   ]
}
```

*响应格式*

```json
{
    "result": {
        "status": "success",
        "tx_blob": "1200032280000000240000000468400000000000000A73210330E7FC9D56BB25D6893BA3F317AE5BCF33B3291BD63DB32654A313222F7FD02074473045022100A50E867D3B1B5A39F23F1ABCA5C7C3EC755442FDAA357EFD897B865ACA7686DB02206077BF459BCE39BCCBFE1A128DA986D1E00CBEC5F0D6B0E11710F60BE2976FB88114623B8DA4A0BFB3B61AB423391A182DC693DC159E",
        "tx_json": {
            "Account": "rUAi7pipxGpYfPNg3LtPcf2ApiS8aw9A93",
            "Fee": "10",
            "Flags": 2147483648,
            "Sequence": 4,
            "SigningPubKey": "0330E7FC9D56BB25D6893BA3F317AE5BCF33B3291BD63DB32654A313222F7FD020",
            "TransactionType": "AccountSet",
            "TxnSignature": "3045022100A50E867D3B1B5A39F23F1ABCA5C7C3EC755442FDAA357EFD897B865ACA7686DB02206077BF459BCE39BCCBFE1A128DA986D1E00CBEC5F0D6B0E11710F60BE2976FB8",
            "hash": "D9B305CB6E861D0994A5CDD4726129D91AC4277111DC444DE4CEE44AD4674A9F"
        }
    }
}
```

##### 提交交易

从离线签名响应( `offline signing response`)中获取signedTransaction值或从sign命令响应中获取tx_blob值，并使用`submit方法`将其作为tx_blob值提交

*请求参数(JSON-RPC)*

```json
{
   "method":"submit",
   "params": [
      {
         "tx_blob": "1200032280000000240000000468400000000000000A73210330E7FC9D56BB25D6893BA3F317AE5BCF33B3291BD63DB32654A313222F7FD02074473045022100A50E867D3B1B5A39F23F1ABCA5C7C3EC755442FDAA357EFD897B865ACA7686DB02206077BF459BCE39BCCBFE1A128DA986D1E00CBEC5F0D6B0E11710F60BE2976FB88114623B8DA4A0BFB3B61AB423391A182DC693DC159E"
      }
   ]
}
```

*响应格式*

```json
{
    "result": {
        "engine_result": "tesSUCCESS",
        "engine_result_code": 0,
        "engine_result_message": "The transaction was applied. Only final in a validated ledger.",
        "status": "success",
        "tx_blob": "1200032280000000240000000468400000000000000A73210330E7FC9D56BB25D6893BA3F317AE5BCF33B3291BD63DB32654A313222F7FD02074473045022100A50E867D3B1B5A39F23F1ABCA5C7C3EC755442FDAA357EFD897B865ACA7686DB02206077BF459BCE39BCCBFE1A128DA986D1E00CBEC5F0D6B0E11710F60BE2976FB88114623B8DA4A0BFB3B61AB423391A182DC693DC159E",
        "tx_json": {
            "Account": "rUAi7pipxGpYfPNg3LtPcf2ApiS8aw9A93",
            "Fee": "10",
            "Flags": 2147483648,
            "Sequence": 4,
            "SigningPubKey": "0330E7FC9D56BB25D6893BA3F317AE5BCF33B3291BD63DB32654A313222F7FD020",
            "TransactionType": "AccountSet",
            "TxnSignature": "3045022100A50E867D3B1B5A39F23F1ABCA5C7C3EC755442FDAA357EFD897B865ACA7686DB02206077BF459BCE39BCCBFE1A128DA986D1E00CBEC5F0D6B0E11710F60BE2976FB8",
            "hash": "D9B305CB6E861D0994A5CDD4726129D91AC4277111DC444DE4CEE44AD4674A9F"
        }
    }
}
```

### 修改或移除常规密钥对

如果帐户的常规密钥对遭到入侵，或者只是想要定期更改常规密钥对作为安全措施，请使用`SetRegularKey` 交易来删除或更改帐户的常规密钥对。

#### 更改常规密钥对

更改现有常规密钥对的步骤与首次分配常规密钥的步骤几乎相同,

但是，主要区别在于，在更改现有常规密钥对时，可以使用`现有的常规私钥`来替换自身 , 而在第一次为帐户分配常规密钥对时，您必须使用帐户的`主私钥`来执行此操作.

#### 删除常规密钥对

如果您只想从帐户中删除受损的常规密钥对，则无需先生成密钥对。发送一笔 `[SetRegularKey]`交易, `省略RegularKey字段`。请注意，如果没有其他方式为您当前启用的帐户（主密钥对或签名者列表）签名，则交易将失败。

将常规密钥对移除帐户时，`SetRegularKey`交易需要使用帐户的主私钥（秘密）或现有常规密钥对进行签名。传输主私钥和常规私钥是危险的, 同样分两步完成此交易，以便将`交易签名(sign)`与交易提交(submit)分离到网络

##### 签名交易

方法同上,

*请求参数*

```json
{
    "method": "sign",
    "params": [
        {
        "secret" : "snoPBrXtMeMyMHUVTgbuqAfg1SUTb",
        "tx_json" : {
            "TransactionType" : "SetRegularKey",
            "Account" : "r9xQZdFGwbwTB3g9ncKByWZ3du6Skm7gQ8"
            }
        }
    ]
}
```

*响应参数*

```json
{
    "result": {
        "status": "success",
        "tx_blob": "1200052280000000240000000268400000000000000A73210330E7FC9D56BB25D6893BA3F317AE5BCF33B3291BD63DB32654A313222F7FD02074473045022100CAB9A6F84026D57B05760D5E2395FB7BE86BF39F10DC6E2E69DC91238EE0970B022058EC36A8EF9EE65F5D0D8CAC4E88C8C19FEF39E40F53D4CCECBB59701D6D1E838114623B8DA4A0BFB3B61AB423391A182DC693DC159E",
        "tx_json": {
            "Account": "r9xQZdFGwbwTB3g9ncKByWZ3du6Skm7gQ8",
            "Fee": "10",
            "Flags": 2147483648,
            "Sequence": 2,
            "SigningPubKey": "0330E7FC9D56BB25D6893BA3F317AE5BCF33B3291BD63DB32654A313222F7FD020",
            "TransactionType": "SetRegularKey",
            "TxnSignature": "3045022100CAB9A6F84026D57B05760D5E2395FB7BE86BF39F10DC6E2E69DC91238EE0970B022058EC36A8EF9EE65F5D0D8CAC4E88C8C19FEF39E40F53D4CCECBB59701D6D1E83",
            "hash": "59BCAB8E5B9D4597D6A7BFF22F6C555D0F41420599A2E126035B6AF19261AD97"
        }
    }
}
```

##### 提交交易

从离线签名响应( `offline signing response`)中获取signedTransaction值或从sign命令响应中获取tx_blob值，并使用`submit方法`将其作为tx_blob值提交

*请求格式*

```json
{
    "command": "submit",
    "tx_blob": "1200052280000000240000000268400000000000000A73210330E7FC9D56BB25D6893BA3F317AE5BCF33B3291BD63DB32654A313222F7FD02074473045022100CAB9A6F84026D57B05760D5E2395FB7BE86BF39F10DC6E2E69DC91238EE0970B022058EC36A8EF9EE65F5D0D8CAC4E88C8C19FEF39E40F53D4CCECBB59701D6D1E838114623B8DA4A0BFB3B61AB423391A182DC693DC159E"
}
```

*响应格式*

```
成功的响应
{
    "result": {
        "engine_result": "tesSUCCESS",
        "engine_result_code": 0,
        "engine_result_message": "The transaction was applied. Only final in a validated ledger.",
        "status": "success",
        "tx_blob": "1200052280000000240000000268400000000000000A73210330E7FC9D56BB25D6893BA3F317AE5BCF33B3291BD63DB32654A313222F7FD02074473045022100CAB9A6F84026D57B05760D5E2395FB7BE86BF39F10DC6E2E69DC91238EE0970B022058EC36A8EF9EE65F5D0D8CAC4E88C8C19FEF39E40F53D4CCECBB59701D6D1E838114623B8DA4A0BFB3B61AB423391A182DC693DC159E",
        "tx_json": {
            "Account": "r9xQZdFGwbwTB3g9ncKByWZ3du6Skm7gQ8",
            "Fee": "10",
            "Flags": 2147483648,
            "Sequence": 2,
            "SigningPubKey": "0330E7FC9D56BB25D6893BA3F317AE5BCF33B3291BD63DB32654A313222F7FD020",
            "TransactionType": "SetRegularKey",
            "TxnSignature": "3045022100CAB9A6F84026D57B05760D5E2395FB7BE86BF39F10DC6E2E69DC91238EE0970B022058EC36A8EF9EE65F5D0D8CAC4E88C8C19FEF39E40F53D4CCECBB59701D6D1E83",
            "hash": "59BCAB8E5B9D4597D6A7BFF22F6C555D0F41420599A2E126035B6AF19261AD97"
        }
    }
}
```

验证常规密钥对删除成功的方法 : `是确认无法使用删除的常规私钥发送交易`

下面是使用上面的`SetRegularKey`交易删除的常规私钥签名的AccountSet交易的示例错误响应。

```json
{
    "result": {
        "error": "badSecret",
        "error_code": 41,
        "error_message": "Secret does not match account.",
        "request": {
            "command": "submit",
            "secret": "snoPBrXtMeMyMHUVTgbuqAfg1SUTb",
            "tx_json": {
                "Account": "r9xQZdFGwbwTB3g9ncKByWZ3du6Skm7gQ8",
                "TransactionType": "AccountSet"
            }
        },
        "status": "error"
    }
}
```

**在某些情况下，甚至可以使用SetRegularKey交易发送密钥重置事务，而无需支付交易成本, 通过启用FeeEscalation修正，即使密钥重置交易的交易成本为零，rippled也会将密钥重置的交易优先于其他交易。**

### 设置多重签名

多重签名(Multi-Signing)是授权XRP Ledger交易的三种方式之一，同时使用常规密钥和主密钥进行签名。可以配置地址以允许三种方法的任意组合来授权交易。

#### 先决条件

- 必须拥有一个有`资金`的XRP Ledger地址
- 必须有权访问可以生成XRP Ledger格式的密钥对的工具。如果使用rippled服务器，则必须具有管理员权限，因为`wallet_propose`方法仅限管理员
- 多重签名可用。自2016-06-27以来，XRP Ledger Consensus Protocol的修正案已经实现了多重签名

#### 1. 准备有资金的账户地址

需要一个可以发送交易的XRP Ledger地址，并且有足够的XRP币可用! 对于帐户储备金和交易成本，多重签名需要超过通常的XRP数量，并会随着您使用的签名者和签名数量的增加而增加。

如果以创始账本通过独立模式下开始rippled 服务，则必须

1. 为新地址生成key，或重用已有的密钥
2. 从创世纪帐户为新地址发起一笔支付交易 （发送至少100,000,000XRP)
3. 手动关闭ledger

#### 2. 准备成本密钥

需要几组XRP Ledger密钥（地址和密钥）作为`SignerList`的成员包含在内。这些可以是分类帐中存在的资金地址，也可以使用`wallet_propose`方法生成新地址。例如：

```shell
$ rippled wallet_propose
Loading: "/home/mduo13/.config/ripple/rippled.cfg"
Connecting to 127.0.0.1:5005
{
    "result" : {
        "account_id" : "rnRJ4dpSBKDR2M1itf4Ah6tZZm5xuNZFPH",
        "key_type" : "secp256k1",
        "master_key" : "FLOG SEND GOES CUFF GAGE FAT ANTI DEL GUM TIRE ISLE BEAR",
        "master_seed" : "snheH5UUjU4CWqiNVLny2k21TyKPC",
        "master_seed_hex" : "A9F859765EB8614D26809836382AFB82",
        "public_key" : "aBR4hxFXcDNHnGYvTiqb2KU8TTTV1cYV9wXTAuz2DjBm7S8TYEBU",
        "public_key_hex" : "03C09A5D112B393D531E4F092E3A5769A5752129F0A9C55C61B3A226BB9B567B9B",
        "status" : "success"
    }
}
```

记下生成的每个帐户的account_id（XRP ledger地址）和master_seed（密钥）

#### 3. 发送`SignerListSet`交易

按 `sign` 和`submit` 分步的方式完成交易. 这会将SignerList与自己的XRP Ledger地址相关联，以便来自该`SignerList成员的签名组合`可以代替以后的交易进行多重的签名。

```
如下实例
```

在此示例中，SignerList具有3个成员(member)，其中成员的签名权重(`SignerWeight`)和所要求签名数量(`SignerQuorum`)的设置使得此交易满足的多签名如下 : 除此成员rsA2LpzuawewSBQXkiju3YQTMzW13pAAdW`的签名(权重为2)`以外, 还需要来自列表的其他两个成员的至少一个签名(其他成员的权重都为1)。

```json
$ rippled submit shqZZy2Rzs9ZqWTCQAdqc3bKgxnYq '{
>     "Flags": 0,
>     "TransactionType": "SignerListSet",
>     "Account": "rnBFvgZphmN39GWzUJeUitaP22Fr9be75H",
>     "Fee": "10000",
>     "SignerQuorum": 3,
>     "SignerEntries": [
>         {
>             "SignerEntry": {
>                 "Account": "rsA2LpzuawewSBQXkiju3YQTMzW13pAAdW",
>                 "SignerWeight": 2
>             }
>         },
>         {
>             "SignerEntry": {
>                 "Account": "rUpy3eEg8rqjqfUoLeBnZkscbKbFsKXC3v",
>                 "SignerWeight": 1
>             }
>         },
>         {
>             "SignerEntry": {
>                 "Account": "raKEEVSGnKSD9Zyvxu4z6Pqpm4ABH8FS6n",
>                 "SignerWeight": 1
>             }
>         }
>     ]
> }'
Loading: "/home/mduo13/.config/ripple/rippled.cfg"
Connecting to 127.0.0.1:5005
{
   "result" : {
      "engine_result" : "tesSUCCESS",
      "engine_result_code" : 0,
      "engine_result_message" : "The transaction was applied. Only final in a validated ledger.",
      "status" : "success",
      "tx_blob" : "12000C2200000000240000000120230000000368400000000000271073210303E20EC6B4A39A629815AE02C0A1393B9225E3B890CAE45B59F42FA29BE9668D74473045022100BEDFA12502C66DDCB64521972E5356F4DB965F553853D53D4C69B4897F11B4780220595202D1E080345B65BAF8EBD6CA161C227F1B62C7E72EA5CA282B9434A6F04281142DECAB42CA805119A9BA2FF305C9AFA12F0B86A1F4EB1300028114204288D2E47F8EF6C99BCC457966320D12409711E1EB13000181147908A7F0EDD48EA896C3580A399F0EE78611C8E3E1EB13000181143A4C02EA95AD6AC3BED92FA036E0BBFB712C030CE1F1",
      "tx_json" : {
         "Account" : "rnBFvgZphmN39GWzUJeUitaP22Fr9be75H",
         "Fee" : "10000",
         "Flags" : 0,
         "Sequence" : 1,
         "SignerEntries" : [
            {
               "SignerEntry" : {
                  "Account" : "rsA2LpzuawewSBQXkiju3YQTMzW13pAAdW",
                  "SignerWeight" : 2
               }
            },
            {
               "SignerEntry" : {
                  "Account" : "rUpy3eEg8rqjqfUoLeBnZkscbKbFsKXC3v",
                  "SignerWeight" : 1
               }
            },
            {
               "SignerEntry" : {
                  "Account" : "raKEEVSGnKSD9Zyvxu4z6Pqpm4ABH8FS6n",
                  "SignerWeight" : 1
               }
            }
         ],
         "SignerQuorum" : 3,
         "SigningPubKey" : "0303E20EC6B4A39A629815AE02C0A1393B9225E3B890CAE45B59F42FA29BE9668D",
         "TransactionType" : "SignerListSet",
         "TxnSignature" : "3045022100BEDFA12502C66DDCB64521972E5356F4DB965F553853D53D4C69B4897F11B4780220595202D1E080345B65BAF8EBD6CA161C227F1B62C7E72EA5CA282B9434A6F042",
         "hash" : "3950D98AD20DA52EBB1F3937EF32F382D74092A4C8DF9A0B1A06ED25200B5756"
      }
   }
}
```

确保交易结果是`tesSUCCESS`。否则，交易失败。如果您在独立模式或非生产网络中出现问题，请检查是否已启用多重登录。

**※** `SignerList`中的成员越多，自身的地址必须具有的XRP越多，以用于所有者保留。如果您的地址没有足够的XRP，则交易将失败并显示`tecINSUFFICIENT_RESERVE`。另请参见：`SignerLists和Reserves`

#### 4. 关闭ledger

在实时网络上，可以等待4-7秒以使ledger 自动关闭

如果在独立模式下运行rippled，请使用`ledger_accept`方法手动关闭分类帐：

```shell
$ rippled ledger_accept
Loading: "/home/mduo13/.config/ripple/rippled.cfg"
Connecting to 127.0.0.1:5005
{
   "result" : {
      "ledger_current_index" : 6,
      "status" : "success"
   }
}
```

#### 5. 确认新的签名列表

使用`account_objects`方法确认`SignerList`与`最新验证的分类帐中的地址`相关联

通常，帐户可以拥有许多不同类型的对象（例如信任行和要约）

```shell
$ rippled account_objects rEuLyBCvcw4CFmzv8RepSiAoNgF8tTGJQC validated
Loading: "/home/mduo13/.config/ripple/rippled.cfg"
Connecting to 127.0.0.1:5005
{
   "result" : {
      "account" : "rEuLyBCvcw4CFmzv8RepSiAoNgF8tTGJQC",
      "account_objects" : [
         {
            "Flags" : 0,
            "LedgerEntryType" : "SignerList",
            "OwnerNode" : "0000000000000000",
            "PreviousTxnID" : "8FDC18960455C196A8C4DE0D24799209A21F4A17E32102B5162BD79466B90222",
            "PreviousTxnLgrSeq" : 5,
            "SignerEntries" : [
               {
                  "SignerEntry" : {
                     "Account" : "rsA2LpzuawewSBQXkiju3YQTMzW13pAAdW",
                     "SignerWeight" : 2
                  }
               },
               {
                  "SignerEntry" : {
                     "Account" : "raKEEVSGnKSD9Zyvxu4z6Pqpm4ABH8FS6n",
                     "SignerWeight" : 1
                  }
               },
               {
                  "SignerEntry" : {
                     "Account" : "rUpy3eEg8rqjqfUoLeBnZkscbKbFsKXC3v",
                     "SignerWeight" : 1
                  }
               }
            ],
            "SignerListID" : 0,
            "SignerQuorum" : 3,
            "index" : "79FD203E4DDDF2EA78B798C963487120C048C78652A28682425E47C96D016F92"
         }
      ],
      "ledger_hash" : "56E81069F06492FB410A70218C08169BE3AB3CFD5AEA20E999662D81DC361D9F",
      "ledger_index" : 5,
      "status" : "success",
      "validated" : true
   }
}
```

如果`SignerList`与预期内容一起出现，那么您的地址已准备好了进行多重签名。

### 发送多签名交易

#### 先决条件

- 必须拥有一个已经设置了多重签名交易的 XRP地址
- Rippled 版本支持多重签名

#### 1. 创建一笔交易

创建一个代表要提交的交易的的JSON对象。必须指定此交易的所有内容，包括费用和顺序。还包括`SigningPubKey`字段作为`空字符串`，以指明交易是多重签名的。

请记住，多签名交易的费用明显高于常规签名的交易。它应该至少是正常交易成本的(`N + 1`)倍,其中N是设置的签名数量。由于从多个来源收集签名有时需要一段时间，因此在交易成本增加的情况下，可能需要指定超过当前最小值(交易手续费)的内容。

进行多重签名交易的实例

```shell
{
    "TransactionType": "TrustSet",
    "Account": "rEuLyBCvcw4CFmzv8RepSiAoNgF8tTGJQC",  
    "Flags": 262144,
    "LimitAmount": {
        "currency": "USD",
        "issuer": "rHb9CJAWyB4rj91VRWn96DkukG4bwdtyTh",
        "value": "100"
    },
    "Sequence": 2,         # 顺序
    "SigningPubKey": "",   # value为空字符串,指明为多重签名交易
    "Fee": "30000"		   # 手续费	
}
```

> 此笔多重签名交易说明: 从地址rEuLyBCvcw4CFmzv8RepSiAoNgF8tTGJQC 到地址rHb9CJAWyB4rj91VRWn96DkukG4bwdtyTh的关系, 最高余额为100 $

#### 2 . 获得一个签名

使用`sign_for`方法和`SignerList`中的一个成员的密钥和地址来获取该成员的签名。

**⚠** 切勿向无法控制的服务器提交密钥。不要通过网络发送未加密的密钥。

*请求 commandline*

```shell
$ rippled sign_for rsA2LpzuawewSBQXkiju3YQTMzW13pAAdW <rsA2L..'s secret> '{
     "TransactionType": "TrustSet",
     "Account": "rEuLyBCvcw4CFmzv8RepSiAoNgF8tTGJQC",
     "Flags": 262144,
     "LimitAmount": {
         "currency": "USD",
         "issuer": "rHb9CJAWyB4rj91VRWn96DkukG4bwdtyTh",
         "value": "100"
     },
     "Sequence": 2,
     "SigningPubKey": "",
     "Fee": "30000"
 }'
```

*响应格式*

```json
Loading: "/home/mduo13/.config/ripple/rippled.cfg"
Connecting to 127.0.0.1:5005
{
   "result" : {
      "status" : "success",
      "tx_blob" : "1200142200040000240000000263D5038D7EA4C680000000000000000000000000005553440000000000B5F762798A53D543A014CAF8B297CFF8F2F937E868400000000000753073008114A3780F5CB5A44D366520FC44055E8ED44D9A2270F3E010732102B3EC4E5DD96029A647CFA20DA07FE1F85296505552CCAC114087E66B46BD77DF744730450221009C195DBBF7967E223D8626CA19CF02073667F2B22E206727BFE848FF42BEAC8A022048C323B0BED19A988BDBEFA974B6DE8AA9DCAE250AA82BBD1221787032A864E58114204288D2E47F8EF6C99BCC457966320D12409711E1F1",
      "tx_json" : {
         "Account" : "rEuLyBCvcw4CFmzv8RepSiAoNgF8tTGJQC",
         "Fee" : "30000",
         "Flags" : 262144,
         "LimitAmount" : {
            "currency" : "USD",
            "issuer" : "rHb9CJAWyB4rj91VRWn96DkukG4bwdtyTh",
            "value" : "100"
         },
         "Sequence" : 2,
         "Signers" : [
            {
               "Signer" : {
                  "Account" : "rsA2LpzuawewSBQXkiju3YQTMzW13pAAdW",
                  "SigningPubKey" : "02B3EC4E5DD96029A647CFA20DA07FE1F85296505552CCAC114087E66B46BD77DF",
                  "TxnSignature" : "30450221009C195DBBF7967E223D8626CA19CF02073667F2B22E206727BFE848FF42BEAC8A022048C323B0BED19A988BDBEFA974B6DE8AA9DCAE250AA82BBD1221787032A864E5"
               }
            }
         ],
         "SigningPubKey" : "",
         "TransactionType" : "TrustSet",
         "hash" : "A94A6417D1A7AAB059822B894E13D322ED3712F7212CE9257801F96DE6C3F6AE"
      }
   }
}
```

保存响应的`tx_json`字段：它在`Signers`字段中具有新签名。可以`丢弃tx_blob`字段的值

如果在独立模式或非生产网络中出现问题，检查是`否已启用多重登录`

#### 3 .获取辅助签名

可以并行或串行收集其他签名

- 并行 : 将`sign_for`命令与原始JSON一起用于交易。每个响应在`Signers`数组中都有一个签名。
- 串行: 使用`sign_for`命令和前一个`sign_for`响应中的`tx_json`值。每个响应都会为现有的`Signers数组`添加一个新签名。

*请求方式 commandline*

```json
$ rippled sign_for rUpy3eEg8rqjqfUoLeBnZkscbKbFsKXC3v <rUpy..'s secret> '{
    "Account" : "rEuLyBCvcw4CFmzv8RepSiAoNgF8tTGJQC",
    "Fee" : "30000",
    "Flags" : 262144,
    "LimitAmount" : {
       "currency" : "USD",
       "issuer" : "rHb9CJAWyB4rj91VRWn96DkukG4bwdtyTh",
       "value" : "100"
    },
    "Sequence" : 2,
    "Signers" : [
       {
          "Signer" : {
             "Account" : "rsA2LpzuawewSBQXkiju3YQTMzW13pAAdW",
             "SigningPubKey" : "02B3EC4E5DD96029A647CFA20DA07FE1F85296505552CCAC114087E66B46BD77DF",
             "TxnSignature" : "30450221009C195DBBF7967E223D8626CA19CF02073667F2B22E206727BFE848FF42BEAC8A022048C323B0BED19A988BDBEFA974B6DE8AA9DCAE250AA82BBD1221787032A864E5"
          }
       }
    ],
    "SigningPubKey" : "",
    "TransactionType" : "TrustSet",
    "hash" : "A94A6417D1A7AAB059822B894E13D322ED3712F7212CE9257801F96DE6C3F6AE"
 }'
Loading: "/home/mduo13/.config/ripple/rippled.cfg"
Connecting to 127.0.0.1:5005
```

*响应格式*

```json
{
   "result" : {
      "status" : "success",
      "tx_blob" : "1200142200040000240000000263D5038D7EA4C680000000000000000000000000005553440000000000B5F762798A53D543A014CAF8B297CFF8F2F937E868400000000000753073008114A3780F5CB5A44D366520FC44055E8ED44D9A2270F3E010732102B3EC4E5DD96029A647CFA20DA07FE1F85296505552CCAC114087E66B46BD77DF744730450221009C195DBBF7967E223D8626CA19CF02073667F2B22E206727BFE848FF42BEAC8A022048C323B0BED19A988BDBEFA974B6DE8AA9DCAE250AA82BBD1221787032A864E58114204288D2E47F8EF6C99BCC457966320D12409711E1E0107321028FFB276505F9AC3F57E8D5242B386A597EF6C40A7999F37F1948636FD484E25B744630440220680BBD745004E9CFB6B13A137F505FB92298AD309071D16C7B982825188FD1AE022004200B1F7E4A6A84BB0E4FC09E1E3BA2B66EBD32F0E6D121A34BA3B04AD99BC181147908A7F0EDD48EA896C3580A399F0EE78611C8E3E1F1",
      "tx_json" : {
         "Account" : "rEuLyBCvcw4CFmzv8RepSiAoNgF8tTGJQC",
         "Fee" : "30000",
         "Flags" : 262144,
         "LimitAmount" : {
            "currency" : "USD",
            "issuer" : "rHb9CJAWyB4rj91VRWn96DkukG4bwdtyTh",
            "value" : "100"
         },
         "Sequence" : 2,
         "Signers" : [
            {
               "Signer" : {
                  "Account" : "rsA2LpzuawewSBQXkiju3YQTMzW13pAAdW",
                  "SigningPubKey" : "02B3EC4E5DD96029A647CFA20DA07FE1F85296505552CCAC114087E66B46BD77DF",
                  "TxnSignature" : "30450221009C195DBBF7967E223D8626CA19CF02073667F2B22E206727BFE848FF42BEAC8A022048C323B0BED19A988BDBEFA974B6DE8AA9DCAE250AA82BBD1221787032A864E5"
               }
            },
            {
               "Signer" : {
                  "Account" : "rUpy3eEg8rqjqfUoLeBnZkscbKbFsKXC3v",
                  "SigningPubKey" : "028FFB276505F9AC3F57E8D5242B386A597EF6C40A7999F37F1948636FD484E25B",
                  "TxnSignature" : "30440220680BBD745004E9CFB6B13A137F505FB92298AD309071D16C7B982825188FD1AE022004200B1F7E4A6A84BB0E4FC09E1E3BA2B66EBD32F0E6D121A34BA3B04AD99BC1"
               }
            }
         ],
         "SigningPubKey" : "",
         "TransactionType" : "TrustSet",
         "hash" : "BD636194C48FD7A100DE4C972336534C8E710FD008C0F3CF7BC5BF34DAF3C3E6"
      }
   }
}
```

**根据配置的签名者列表，可能需要多次重复此步骤，以便从所有必要的方获得签名 **

#### 4. 结合签名并提交

如果以**串行**方式收集签名，则最后一个`sign_for`响应中的`tx_json`会汇集所有签名，因此可以将其用作`submit_multisigned`方法的参数。

如果是**并行**收集签名，则必须手动构造包含所有签名的`tx_json`对象。从所有`sign_for`响应中获取Signers数组，并将它们的内容组合到一个具有`每个签名`的Signer数组中。

将组合的`Signers数组`添加到原始交易`JSON`值，并将其用作`submit_multisigned`方法的参数。

*请求格式*

```shell
$ rippled submit_multisigned '{
              "Account" : "rEuLyBCvcw4CFmzv8RepSiAoNgF8tTGJQC",
              "Fee" : "30000",
              "Flags" : 262144,
              "LimitAmount" : {
                 "currency" : "USD",
                 "issuer" : "rHb9CJAWyB4rj91VRWn96DkukG4bwdtyTh",
                 "value" : "100"
              },
              "Sequence" : 2,
              "Signers" : [
                 {
                    "Signer" : {
                       "Account" : "rsA2LpzuawewSBQXkiju3YQTMzW13pAAdW",
                       "SigningPubKey" : "02B3EC4E5DD96029A647CFA20DA07FE1F85296505552CCAC114087E66B46BD77DF",
                       "TxnSignature" : "30450221009C195DBBF7967E223D8626CA19CF02073667F2B22E206727BFE848FF42BEAC8A022048C323B0BED19A988BDBEFA974B6DE8AA9DCAE250AA82BBD1221787032A864E5"
                    }
                 },
                 {
                    "Signer" : {
                       "Account" : "rUpy3eEg8rqjqfUoLeBnZkscbKbFsKXC3v",
                       "SigningPubKey" : "028FFB276505F9AC3F57E8D5242B386A597EF6C40A7999F37F1948636FD484E25B",
                       "TxnSignature" : "30440220680BBD745004E9CFB6B13A137F505FB92298AD309071D16C7B982825188FD1AE022004200B1F7E4A6A84BB0E4FC09E1E3BA2B66EBD32F0E6D121A34BA3B04AD99BC1"
                    }
                 }
              ],
              "SigningPubKey" : "",
              "TransactionType" : "TrustSet",
              "hash" : "BD636194C48FD7A100DE4C972336534C8E710FD008C0F3CF7BC5BF34DAF3C3E6"
           }'
```

*响应格式*

```json
Loading: "/home/mduo13/.config/ripple/rippled.cfg"
Connecting to 127.0.0.1:5005
{
    "result": {
        "engine_result": "tesSUCCESS",
        "engine_result_code": 0,
        "engine_result_message": "The transaction was applied. Only final in a validated ledger.",
        "status": "success",
        "tx_blob": "1200142200040000240000000263D5038D7EA4C680000000000000000000000000005553440000000000B5F762798A53D543A014CAF8B297CFF8F2F937E868400000000000753073008114A3780F5CB5A44D366520FC44055E8ED44D9A2270F3E010732102B3EC4E5DD96029A647CFA20DA07FE1F85296505552CCAC114087E66B46BD77DF744730450221009C195DBBF7967E223D8626CA19CF02073667F2B22E206727BFE848FF42BEAC8A022048C323B0BED19A988BDBEFA974B6DE8AA9DCAE250AA82BBD1221787032A864E58114204288D2E47F8EF6C99BCC457966320D12409711E1E0107321028FFB276505F9AC3F57E8D5242B386A597EF6C40A7999F37F1948636FD484E25B744630440220680BBD745004E9CFB6B13A137F505FB92298AD309071D16C7B982825188FD1AE022004200B1F7E4A6A84BB0E4FC09E1E3BA2B66EBD32F0E6D121A34BA3B04AD99BC181147908A7F0EDD48EA896C3580A399F0EE78611C8E3E1F1",
        "tx_json": {
            "Account": "rEuLyBCvcw4CFmzv8RepSiAoNgF8tTGJQC",
            "Fee": "30000",
            "Flags": 262144,
            "LimitAmount": {
                "currency": "USD",
                "issuer": "rHb9CJAWyB4rj91VRWn96DkukG4bwdtyTh",
                "value": "100"
            },
            "Sequence": 2,
            "Signers": [{
                "Signer": {
                    "Account": "rsA2LpzuawewSBQXkiju3YQTMzW13pAAdW",
                    "SigningPubKey": "02B3EC4E5DD96029A647CFA20DA07FE1F85296505552CCAC114087E66B46BD77DF",
                    "TxnSignature": "30450221009C195DBBF7967E223D8626CA19CF02073667F2B22E206727BFE848FF42BEAC8A022048C323B0BED19A988BDBEFA974B6DE8AA9DCAE250AA82BBD1221787032A864E5"
                }
            }, {
                "Signer": {
                    "Account": "rUpy3eEg8rqjqfUoLeBnZkscbKbFsKXC3v",
                    "SigningPubKey": "028FFB276505F9AC3F57E8D5242B386A597EF6C40A7999F37F1948636FD484E25B",
                    "TxnSignature": "30440220680BBD745004E9CFB6B13A137F505FB92298AD309071D16C7B982825188FD1AE022004200B1F7E4A6A84BB0E4FC09E1E3BA2B66EBD32F0E6D121A34BA3B04AD99BC1"
                }
            }],
            "SigningPubKey": "",
            "TransactionType": "TrustSet",
            "hash": "BD636194C48FD7A100DE4C972336534C8E710FD008C0F3CF7BC5BF34DAF3C3E6"
        }
    }
}
```

**记下响应中的哈希值，以便稍后检查交易的结果 **

#### 5. 关闭ledger

如果使用的是实时网络，则可以等待4-7秒以使分类帐自动关闭。

如果在独立模式下运行rippled，使用`ledger_accept`方法手动关闭ledger：

```shell
$ rippled ledger_accept
Loading: "/home/mduo13/.config/ripple/rippled.cfg"
Connecting to 127.0.0.1:5005
{
   "result" : {
      "ledger_current_index" : 7,
      "status" : "success"
   }
}
```

#### 6. 确认交易结果

使用对`submit_multisigned`命令的响应中的哈希值，使用`tx`方法查找交易结果。特别是，检查`TransactionResult`是字符串`tesSUCCESS`。

在实时网络上，还必须确认`validated`的字段设置为布尔值true, 如果该字段不正确，可能需要等待更长时间`才能完成共识流程`;或者交易可能由于某种原因无法打包进账本中(区块)。

在独立模式下，如果已手动关闭分类帐，则服务器会自动认为该分类帐已经过验证。

```SHEll
$ rippled tx BD636194C48FD7A100DE4C972336534C8E710FD008C0F3CF7BC5BF34DAF3C3E6  # 交易哈希
Loading: "/home/mduo13/.config/ripple/rippled.cfg"
Connecting to 127.0.0.1:5005
{
    "result": {
        "Account": "rEuLyBCvcw4CFmzv8RepSiAoNgF8tTGJQC",
        "Fee": "30000",
        "Flags": 262144,
        "LimitAmount": {
            "currency": "USD",
            "issuer": "rHb9CJAWyB4rj91VRWn96DkukG4bwdtyTh",
            "value": "100"
        },
        "Sequence": 2,
        "Signers": [{
            "Signer": {
                "Account": "rsA2LpzuawewSBQXkiju3YQTMzW13pAAdW",
                "SigningPubKey": "02B3EC4E5DD96029A647CFA20DA07FE1F85296505552CCAC114087E66B46BD77DF",
                "TxnSignature": "30450221009C195DBBF7967E223D8626CA19CF02073667F2B22E206727BFE848FF42BEAC8A022048C323B0BED19A988BDBEFA974B6DE8AA9DCAE250AA82BBD1221787032A864E5"
            }
        }, {
            "Signer": {
                "Account": "rUpy3eEg8rqjqfUoLeBnZkscbKbFsKXC3v",
                "SigningPubKey": "028FFB276505F9AC3F57E8D5242B386A597EF6C40A7999F37F1948636FD484E25B",
                "TxnSignature": "30440220680BBD745004E9CFB6B13A137F505FB92298AD309071D16C7B982825188FD1AE022004200B1F7E4A6A84BB0E4FC09E1E3BA2B66EBD32F0E6D121A34BA3B04AD99BC1"
            }
        }],
        "SigningPubKey": "",
        "TransactionType": "TrustSet",
        "date": 512172510,
        "hash": "BD636194C48FD7A100DE4C972336534C8E710FD008C0F3CF7BC5BF34DAF3C3E6",
        "inLedger": 6,
        "ledger_index": 6,
        "meta": {
            "AffectedNodes": [{
                "ModifiedNode": {
                    "LedgerEntryType": "AccountRoot",
                    "LedgerIndex": "2B6AC232AA4C4BE41BF49D2459FA4A0347E1B543A4C92FCEE0821C0201E2E9A8",
                    "PreviousTxnID": "B7E1D33DB7DEA3BB65BFAB2C80E02125F47FCCF6C957A7FDECD915B3EBE0C1DD",
                    "PreviousTxnLgrSeq": 4
                }
            }, {
                "CreatedNode": {
                    "LedgerEntryType": "RippleState",
                    "LedgerIndex": "93E317B32022977C77810A2C558FBB28E30E744C68E73720622B797F957EC5FA",
                    "NewFields": {
                        "Balance": {
                            "currency": "USD",
                            "issuer": "rrrrrrrrrrrrrrrrrrrrBZbvji",
                            "value": "0"
                        },
                        "Flags": 2162688,
                        "HighLimit": {
                            "currency": "USD",
                            "issuer": "rHb9CJAWyB4rj91VRWn96DkukG4bwdtyTh",
                            "value": "0"
                        },
                        "LowLimit": {
                            "currency": "USD",
                            "issuer": "rEuLyBCvcw4CFmzv8RepSiAoNgF8tTGJQC",
                            "value": "100"
                        }
                    }
                }
            }, {
                "ModifiedNode": {
                    "FinalFields": {
                        "Account": "rEuLyBCvcw4CFmzv8RepSiAoNgF8tTGJQC",
                        "Balance": "999960000",
                        "Flags": 0,
                        "OwnerCount": 6,
                        "Sequence": 3
                    },
                    "LedgerEntryType": "AccountRoot",
                    "LedgerIndex": "A6B1BA6F2D70813100908EA84ABB7783695050312735E2C3665259F388804EA0",
                    "PreviousFields": {
                        "Balance": "999990000",
                        "OwnerCount": 5,
                        "Sequence": 2
                    },
                    "PreviousTxnID": "8FDC18960455C196A8C4DE0D24799209A21F4A17E32102B5162BD79466B90222",
                    "PreviousTxnLgrSeq": 5
                }
            }, {
                "ModifiedNode": {
                    "FinalFields": {
                        "Flags": 0,
                        "Owner": "rEuLyBCvcw4CFmzv8RepSiAoNgF8tTGJQC",
                        "RootIndex": "C2728175908D82FB1DE6676F203D8D3C056995A9FA9B369EF326523F1C65A1DE"
                    },
                    "LedgerEntryType": "DirectoryNode",
                    "LedgerIndex": "C2728175908D82FB1DE6676F203D8D3C056995A9FA9B369EF326523F1C65A1DE"
                }
            }, {
                "CreatedNode": {
                    "LedgerEntryType": "DirectoryNode",
                    "LedgerIndex": "D8120FC732737A2CF2E9968FDF3797A43B457F2A81AA06D2653171A1EA635204",
                    "NewFields": {
                        "Owner": "rHb9CJAWyB4rj91VRWn96DkukG4bwdtyTh",
                        "RootIndex": "D8120FC732737A2CF2E9968FDF3797A43B457F2A81AA06D2653171A1EA635204"
                    }
                }
            }],
            "TransactionIndex": 0,
            "TransactionResult": "tesSUCCESS"
        },
        "status": "success",
        "validated": true
    }
}
```

### 