# 多签 —— counter智能合约

## 编译

```bash
xdev init counter
cd counter/
cp ../../core/contractsdk/cpp/example/counter.cc src/main.cc
xdev build -o counter.wasm
cd ..
```

## 部署

```bash
#需要添加一个多签名的地址合集文件
echo "XC1234567890111111@xuper/dpzuVdosQrF2kmzumhVeFQZa1aYcdgFpNaaaaaa" >> acl.addrs
echo "XC1234567890222222@xuper/dpzuVdosQrF2kmzumhVeFQZa1aYcdgFpNbbbbbb" >> acl.addrs

#生成交易文件
xchain-cli wasm deploy --account XC1234567890111111@xuper --cname counter -m -A acl.addrs -a '{"creator":"someone"}' -o tx.output counter/counter.wasm
#-m代表多签
#-A多签名时用到的多签地址文件
#-a合约初始化参数
#-o生成的交易文件


#各个签名
xchain-cli multisig sign --tx tx.output --output sign1.out
xchain-cli multisig sign --tx tx.output --output sign2.out

#发送签名交易
xchain-cli multisig send --tx tx.output sign1.out,sign2.out sign1.out,sign2.out

#调用合约
xchain-cli wasm invoke --method increase -a '{"key":"100"}' --fee 11000 counter
xchain-cli wasm invoke --method get -a '{"key":"100"}' --fee 11000 counter

#查看当前账户挂载的所有合约
xchain-cli account contracts --account XC1234567890111111@xuper
```

## 设置合约方法的ACL

```bash
vim setMethodACL.desc
{
    "module_name": "xkernel",
    "method_name": "SetMethodAcl",
    "args" : {
        "contract_name": "counter",
        "method_name": "increase",
        "acl": "{\"pm\": {\"rule\": 1,\"acceptValue\": 1.0},\"aksWeight\": {\"UU4kyZcQinAMsBSPRLUA34ebXrfZtB4Z8\": 1}}"
        }
}

#看样子这里只能多签了。
echo "XC1111111111111111@xuper/dpzuVdosQrF2kmzumhVeFQZa1aYcdgFpN" > data/acl/addrs
xchain-cli multisig gen --desc ./setMethodACL.desc --fee 1
xchain-cli multisig sign --output sign.out
xchain-cli multisig send sign.out sign.out

#查看合约方法ACL
xchain-cli acl query --contract counter --method increase
{
    "pm": {
        "rule": 1,
        "acceptValue": 1
    },
    "aksWeight": {
        "UU4kyZcQinAMsBSPRLUA34ebXrfZtB4Z8": 1
    }
}
```
