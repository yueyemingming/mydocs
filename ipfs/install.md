# ipfs 搭建双节点

## ipfs在linux的安装、同步、测试

## 1、安装

机器1

```bash
wget https://github.com/ipfs/go-ipfs/releases/download/v0.4.18/go-ipfs_v0.4.18_linux-amd64.tar.gz
tar xzvf go-ipfs_v0.4.18_linux-amd64.tar.gz
cd go-ipfs
./install.sh

ipfs init   #ipfs初始化
ipfs id     #查看id，publickey等信息

vim .ipfs/config    #将所有的127.0.01改成0.0.0.0
```

> 不要放在 **/usr/bin** 下。  
> **/usr/bin** 下面的都是系统预装的可执行程序，会随着系统升级而改变  
> **/usr/local/bin** 目录是给用户放置自己的可执行程序的地方，推荐放在这里，不会被系统升级而覆盖同名文件。

机器2步骤同上。

## 2、同步

共享KEY用来同步2个主机。

生成共享key

```bash

ipfs bootstrap rm --all   #移除默认的boostrap节点,即同步节点，这时root/.ipfs/config中的bootstrap信息被清空

go get github.com/Kubuxu/go-ipfs-swarm-key-gen
cd $GOPATH/github.com/Kubuxu/go-ipfs-swarm-key-gen     #当前目录下有个main.go
go build    #生成可执行文件ipfs-swarm-key-gen
./ipfs-swarm-key-gen > swarm.key
```

将swarm.key拷贝到两台主机的ipfs仓库中

```bash
cp swarm.key /root/.ipfs
cp swarm.key root@<host2>:/root/.ipfs
```

主机1

```bash
ipfs daemon。
```

主机2

```bash
ipfs bootstrap add /ip4/<host1>/tcp/4001/ipfs/QmVbcrDXC8bnu9HrwRrShCgSri6Dp9C9ZGtdviuuu3yRv2
ipfs add a.txt     #哈希值需要进行保存（查看时使用）
ipfs daemon
```

主机1

```bash
ipfs cat QmQ34xvnJXi9qxUAPMnaMUsTU1ke8JJFs9bEyFDwFy4B3M
```