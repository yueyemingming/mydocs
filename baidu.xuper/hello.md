# hello智能合约

```bash
xdev init hello-cpp
cd hello-cpp/
xdev build -o hello.wasm        #编译时找这个文件xdev.toml
cd ..
xchain-cli wasm deploy --account XC1234567890222222@xuper --cname hello --fee 5200000 --runtime c hello-cpp/hello.wasm
xchain-cli wasm invoke --method hello --fee 11000 hello

xchain-cli account contracts --account XC1234567890222222@xuper
```
