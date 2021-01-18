# 链启动

## 下载编译

$ cd $HOME
$ git clone https://github.com/xuperchain/xuperchain.git  xuperchain
$ cd xuperchain && make

## 配置环境变量

```bash
export PATH=$HOME/xuperchain/output:$PATH
export XDEV_ROOT=$HOME/xuperchain/core/contractsdk/cpp
```

## single模式启动

–vm ixvm参数是选择ixvm合约虚拟机，开发合约过程中使用ixvm虚拟机能加快合约部署

```bash
cd output
## 首先创建链
xchain-cli createChain
## 后台启动xuperchain节点
nohup ./xchain --vm ixvm &
```
