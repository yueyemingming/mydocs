# Ripple(瑞波币)validator-keys-tool 配置验证器

目录

- Ripple(瑞波币)validator-keys-tool 配置验证器
  - [验证器密钥工具指南](https://www.cnblogs.com/failymao/p/10261453.html#验证器密钥工具指南)
  - [验证器密钥](https://www.cnblogs.com/failymao/p/10261453.html#验证器密钥)
  - [验证器令牌(Validator Keys)](https://www.cnblogs.com/failymao/p/10261453.html#验证器令牌validator-keys)
  - [public_key撤销](https://www.cnblogs.com/failymao/p/10261453.html#public_key撤销)
  - [签名](https://www.cnblogs.com/failymao/p/10261453.html#签名)



## Ripple(瑞波币)`validator-keys-tool `配置验证器

### 验证器密钥工具指南

本指南介绍了如何设置validator，以便在rippled.conf 或服务器受到威胁时不必更改其公钥。

一个validator使用一对公钥/私钥。validator由公钥标识。应严保管私钥。它用于：

- 签署令牌，rippled服务器以此公钥标识为一个validator的节点运行。
- 签署撤销，表明私钥已被泄露，验证器公钥不再受信任。

每个新令牌使验证器公钥的所有先前令牌无效。当前令牌需要存在于rippled.cfg文件中。

做为可信任的validator的服务器将在令牌更改时自动调整。

### 验证器密钥

首次设置`validator`时，请使用该`validator-keys`工具生成其密钥对：

```shell
$ validator-keys create_keys
```

样本输出：

```shell
Validator keys stored in /home/ubuntu/.ripple/validator-keys.json
```

将密钥文件保存在安全但可恢复的位置，例如加密的USB闪存驱动器。不要修改其内容。

### 验证器令牌(Validator Keys)

首次创建[验证器密钥](https://github.com/ripple/validator-keys-tool/blob/master/doc/validator-keys-tool-guide.md#validator-keys)或先前的token已被泄露后，使用该`validator-keys`工具创建新的验证器令牌：

```shell
$ validator-keys create_token
```

样本输出：

```shell
Update rippled.cfg file with these values:

# validator public key: nHUtNnLVx7odrz5dnfb2xpIgbEeJPbzJWfdicSkGyVw1eE5GpjQr

[validator_token]
eyJ2YWxpZGF0aW9uX3NlY3J|dF9rZXkiOiI5ZWQ0NWY4NjYyNDFjYzE4YTI3NDdiNT
QzODdjMDYyNTkwNzk3MmY0ZTcxOTAyMzFmYWE5Mzc0NTdmYT|kYWY2IiwibWFuaWZl
c3QiOiJKQUFBQUFGeEllMUZ0d21pbXZHdEgyaUNjTUpxQzlnVkZLaWxHZncxL3ZDeE
hYWExwbGMyR25NaEFrRTFhZ3FYeEJ3RHdEYklENk9NU1l1TTBGREFscEFnTms4U0tG
bjdNTzJmZGtjd1JRSWhBT25ndTlzQUtxWFlvdUorbDJWMFcrc0FPa1ZCK1pSUzZQU2
hsSkFmVXNYZkFpQnNWSkdlc2FhZE9KYy9hQVpva1MxdnltR21WcmxIUEtXWDNZeXd1
NmluOEhBU1FLUHVnQkQ2N2tNYVJGR3ZtcEFUSGxHS0pkdkRGbFdQWXk1QXFEZWRGdj
VUSmEydzBpMjFlcTNNWXl3TFZKWm5GT3I3QzBrdzJBaVR6U0NqSXpkaXRROD0ifQ==
```

对于新的validator，将[validator_token]值添加到`rippled.cfg`文件中。对于预先存在的验证器，将旧的[validator_token]值替换为新生成的validator。有效的配置文件可能`只包含一个[validator_token]`值。配置更新后，重新启动波纹。

对于给定的验证器密钥对，可以生成4,294,967,293个令牌的硬限制。

### public_key撤销

如果`validator`私钥被泄露，则必须永久撤销密钥。要撤消验证程序密钥，请使用该`validator-keys`工具生成吊销，该吊销向其他服务器(节点)广播消息public_key不再有效：

```shell
$ validator-keys revoke_keys
```

样本输出：

```shell
WARNING: This will revoke your validator keys!

Update rippled.cfg file with these values and restart rippled:

# validator public key: nHUtNnLVx7odrz5dnfb2xpIgbEeJPbzJWfdicSkGyVw1eE5GpjQr

[validator_key_revocation]
JP////9xIe0hvssbqmgzFH4/NDp1z|3ShkmCtFXuC5A0IUocppHopnASQN2MuMD1Puoyjvnr
jQ2KJSO/2tsjRhjO6q0QQHppslQsKNSXWxjGQNIEa6nPisBOKlDDcJVZAMP4QcIyNCadzgM=
```

将`[validator_key_revocation]`值添加到此validatir的配置rippled.cfg并重新启动rippled.service。重命名旧密钥文件并生成新的[验证器密钥](https://github.com/ripple/validator-keys-tool/blob/master/doc/validator-keys-tool-guide.md#validator-keys)和相应的[验证器令牌](https://github.com/ripple/validator-keys-tool/blob/master/doc/validator-keys-tool-guide.md#validator-token)。

### 签名

该`validator-keys`工具可用于使用验证器密钥对任意数据进行签名。

```shell
$ validator-keys sign "your data to sign"
```

`response `：

```shell
B91B73536235BBA028D344B81DBCBECF19C1E0034AC21FB51C2351A138C9871162F3193D7C41A49FB7AABBC32BC2B116B1D5701807BE462D8800B5AEA4F
```

