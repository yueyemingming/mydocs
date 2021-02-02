# linux直接安装kafka

- [官网] —— <http://kafka.apache.org/downloads>

## 安装

```bash
wget https://mirror.bit.edu.cn/apache/kafka/2.4.1/kafka_2.13-2.4.1.tgz
tar xzvf kafka_2.13-2.4.1.tgz 
cd kafka_2.13-2.4.1

#起一个终端, 启动 zookeeper
bin/zookeeper-server-start.sh -daemon config/zookeeper.properties
#开始监听2181端口
[2020-03-23 10:22:06,165] INFO binding to port 0.0.0.0/0.0.0.0:2181 (org.apache.zookeeper.server.NIOServerCnxnFactory)

#起一个终端, 启动 kafka broker
vim config/server.properties
 listeners=PLAINTEXT://0.0.0.0:9092    #将31行的注释去掉,//后面也可以填入ip地址
 advertised.listeners=PLAINTEXT://192.168.1.181:9092

bin/kafka-server-start.sh -daemon config/server.properties
```

## 测试

```bash
#创建一个topic
bin/kafka-topics.sh --zookeeper localhost:2181 --partitions 3 --replication-factor 1 --create --topic test

#查看topic列表
bin/kafka-topics.sh --zookeeper 192.168.1.181:2181 --list

#查看topic描述，可以看到启动了3个partition
bin/kafka-topics.sh --zookeeper localhost:2181 --describe --topic test
 Topic: test	PartitionCount: 3	ReplicationFactor: 1	Configs: 
	Topic: test	Partition: 0	Leader: 0	Replicas: 0	Isr: 0
	Topic: test	Partition: 1	Leader: 0	Replicas: 0	Isr: 0
	Topic: test	Partition: 2	Leader: 0	Replicas: 0	Isr: 0

#启动消费者consumer，输出到命令行
bin/kafka-console-consumer.sh --bootstrap-server 192.168.1.181:9092 --topic test

#启动生产者producer
bin/kafka-console-producer.sh --broker-list 192.168.1.181:9092 --topic test
```
