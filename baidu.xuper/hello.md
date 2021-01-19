# 单签 —— hello智能合约

```bash
#编译
xdev init hello-cpp             #自动生成hello示例代码
cd hello-cpp/
xdev build -o hello.wasm        #编译时找这个文件xdev.toml
xdev build --help
cd ..


#部署
xchain-cli wasm deploy --account XC1234567890111111@xuper --cname hello --fee 5200000 --runtime c hello-cpp/hello.wasm
#--runtime虚拟机对应的语言 c,go,java 三种

#调用
xchain-cli wasm invoke --method hello --fee 11000 hello

#查询如此调用是否会成功
xchain-cli wasm query --method hello hello

#升级合约
xchain-cli wasm upgrade --account XC1234567890111111@xuper --fee 5000000 --cname hello hello-cpp/hello.wasm

#查看挂载
xchain-cli account contracts --account XC1234567890111111@xuper
```
