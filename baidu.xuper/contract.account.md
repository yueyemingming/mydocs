# 创建合约账户

## 方式1 —— 调用合约的方式

```bash
#创建新秘钥
xchain-cli account newkeys --output data/1234567890111111

#创建调用创建账户用的acl描述文件
vim account.1234567890111111.des
{
    "module_name": "xkernel",       #调用此合约
    "method_name": "NewAccount",    #调用次方法
    "args" : {
        "account_name": "1234567890111111",
        "acl": "{\"pm\": {\"rule\": 1,\"acceptValue\": 1.0},\"aksWeight\": {\"dpzuVdosQrF2kmzumhVeFQZa1aYcdgFpN\": 1.0}}"
        #dpzuVdosQrF2kmzumhVeFQZa1aYcdgFpN这个地址就是data/1234567890111111/address内容
    }
}

#调用合约的方式链上创建账户
xchain-cli account new --desc account.1234567890111111.des --fee 1000
```

## 方式2 —— 直接创建账户，创建后在关联acl

```bash
#直接创建账户，默认acl为 data/keys
xchain-cli account new --account 1234567890111111 --fee 1000
```

### 修改账户acl

由于通过此方式创建的合约账户acl默认就是 data/keys ，因此需要修改账户acl

```bash
vim acl_new.json
{
    "module_name": "xkernel",
    "method_name": "SetAccountAcl",  # 这里的方法有了变更
    "args" : {
        "account_name": "XC1234567890111111@xuper", #account_name在此处一定要写成XC.....@xuper的形式
        # acl字段为要修改成的新ACL
        "acl": "{\"pm\": {\"rule\": 1,\"acceptValue\": 0.6},\"aksWeight\": {\"AK3\": 0.3,\"AK4\": 0.3}}"
    }
}

#生成交易
xchain-cli multisig gen --desc acl_new.json --from XC1234567890111111@xuper

#多签名
xchain-cli multisig sign --keys data/account/AK1 --output AK1.sign
xchain-cli multisig sign --keys data/account/AK2 --output AK2.sign

#发送交易
xchain-cli multisig send --tx tx.out AK1.sign,AK2.sign AK1.sign,AK2.sign
```

## 查询账户acl

```bash
xchain-cli acl query --account XC1234567890111111@xuper
```
