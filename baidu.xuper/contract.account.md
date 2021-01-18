# 创建合约账户

### 方式1

### acl描述文件

account.1234567890111111.des

```json
{
    "module_name": "xkernel",
    "method_name": "NewAccount",
    "args" : {
        "account_name": "1234567890111111",
        "acl": "{\"pm\": {\"rule\": 1,\"acceptValue\": 1.0},\"aksWeight\": {\"dpzuVdosQrF2kmzumhVeFQZa1aYcdgFpN\": 1.0}}"
    }
}
```

### 创建

```bash
xchain-cli account new --desc account.1234567890111111.des --fee 1000
```

## 方式2

```bash
xchain-cli account new --account 1234567890111111 --fee 1000
```

## 查询acl

```bash
xchain-cli acl query --account XC1234567890111111@xuper 
```
