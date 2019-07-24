# 钱包操作

```bash
# 创建钱包
cleos wallet create --to-console

# 打开钱包
cleos wallet open

# 查看钱包列表
cleos wallet list

  Wallets:
  [
    "default"   # 代表是锁定状态
  ]

# 解锁钱包
cleos wallet unlock

# 查看钱包列表
cleos wallet list

  Wallets:
  [
    "default *"   # 代表是已解锁状态
  ]

# 创建一个key
cleos wallet create_key

# 将key导入钱包
cleos wallet import
```
