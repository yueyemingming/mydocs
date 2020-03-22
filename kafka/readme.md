# docker安装kafka

## 下载

```bash
docker pull wurstmeister/zookeeper
docker pull wurstmeister/kafka
```

## 启动镜像

```bash
docker run -d --name zookeeper --publish 2181:2181 wurstmeister/zookeeper
docker run -d --name kafka --publish 9092:9092 wurstmeister/kafka
```

## 测试

```bash
bin/kafka-topics.sh --create --zookeeper 192.168.1.110:2181 --replication-factor 1 --partitions 1 --topic mykafka
bin/kafka-topics.sh --list --zookeeper 192.168.1.110:2181
bin/kafka-console-producer.sh --broker-list 192.168.1.110:9092 --topic mykafka 
bin/kafka-console-consumer.sh --zookeeper 192.168.1.110:2181 --topic mykafka --from-beginning
```