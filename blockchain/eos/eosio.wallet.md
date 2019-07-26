# 钱包操作

- 创建钱包

```bash
cleos wallet create --to-console    # 此处创建默认钱包,--to-console代表密钥输出到终端上。

  Creating wallet: default
  Save password to use in the future to unlock this wallet.
  Without password imported keys will not be retrievable.
  "PW5JAyEGnKFwFBTJCKvtMjjYxgzbuftvyECQBkSvEZt4xtQWnf2Wy"   # 这个是当前这个钱包的密码

cleos wallet create -n bob_wallet -f passwd.txt --to-console
  # -n 创建名为mywallet的钱包
  # -f 密钥输出到passwd.txt文件中
```

- 打开钱包

```bash
cleos wallet open
```

- 查看钱包列表

```bash
cleos wallet list

  Wallets:
  [
    "default"   # 代表是锁定状态
  ]
```

- 解锁钱包

需要前面用到的钱包的私钥解密 "PW5JAyEGnKFwFBTJCKvtMjjYxgzbuftvyECQBkSvEZt4xtQWnf2Wy"

```bash
cleos wallet unlock
```

- 查看钱包列表

```bash
cleos wallet list

  Wallets:
  [
    "default *"   # 代表是已解锁状态
  ]
```

- 创建一个key

创建key时，这个key就直接被被导入进钱包了。

```bash
cleos wallet create_key
```

- 将已知key导入钱包

将已知的key导入钱包时，只要提供私钥即。  
比如这里将系统账户eosio的key导入这个钱包，eosio的私钥为："5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3"

```bash
cleos wallet import
```

- 查看当前钱包的所有公钥

```bash
cleos wallet keys
```

- 查看当前钱包的所有私钥

```bash
cleos wallet private_keys
```
