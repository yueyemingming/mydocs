# linux安装zookeeper及使用

- [官方网址] —— <http://mirror.bit.edu.cn/apache/zookeeper>

## 下载

```bash
wget http://mirror.bit.edu.cn/apache/zookeeper/zookeeper-3.6.0/apache-zookeeper-3.6.0-bin.tar.gz
tar xzvf apache-zookeeper-3.6.0-bin.tar.gz
```

## 启动

```bash
bin/zkServer.sh start conf/zoo_sample.cfg

 ZooKeeper JMX enabled by default
 Using config: conf/zoo_sample.cfg
 Starting zookeeper ... STARTED
```

或者

```bash
cp conf/zoo_sample.cfg conf/zoo.cfg
bin/zkServer.sh start
```

## 查看

```bash
bin/zkServer.sh status conf/zoo_sample.cfg

 ZooKeeper JMX enabled by default
 Using config: conf/zoo_sample.cfg
 Client port found: 2181. Client address: localhost.
 Mode: standalone
```
