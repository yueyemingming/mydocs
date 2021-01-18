# counter智能合约

## 编译

```bash
xdev init counter
cd counter/
cp ../../core/contractsdk/cpp/example/counter.cc src/main.cc

xdev build -o counter.wasm        #编译时找这个文件xdev.toml
```

## 部署及调用

```bash
cd ..
echo "XC1234567890222222@xuper/dpzuVdosQrF2kmzumhVeFQZa1aYcdgFpN" > data/acl/addrs
xchain-cli wasm deploy --account XC1234567890222222@xuper --cname counter -m -a '{"creator":"someone"}' -A data/acl/addrs -o tx.output counter/counter.wasm
xchain-cli multisig sign --tx tx.output --output sign.out
xchain-cli multisig send --tx tx.output sign.out sign.out

# 查看当前账户所有合约
xchain-cli account contracts --account XC1234567890222222@xuper

xchain-cli wasm invoke --method increase -a '{"key":"100"}' --fee 11000 counter
xchain-cli wasm invoke --method get -a '{"key":"100"}' --fee 11000 counter
```

## 设置合约方法的ACL

counter.acl.desc

```json
{
    "module_name":"xkernel",
    "method_name":"SetMethodAcl",
    "args":{
        "contract_name":"counter",
        "method_name":"increase",
        "acl":"{"pm": {"rule": 1,"acceptValue": 1.0},"aksWeight": {"dpzuVdosQrF2kmzumhVeFQZa1aYcdgFpN": 1}}"
    }
}
```

```bash
xchain-cli multisig gen --desc counter/counter.acl.desc --fee 1
xchain-cli multisig sign --output counter/counter.acl.sign.out
xchain-cli multisig send counter/counter.acl.sign.out counter/counter.acl.sign.out
xchain-cli acl query --contract counter --method increase
```
