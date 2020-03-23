# docker下安装kafka

## 下载

```bash
docker pull wurstmeister/zookeeper
docker pull wurstmeister/kafka:2.11-0.11.0.3
```

## 启动镜像

```bash
#启动zookeeper
docker run -d --name zookeeper --publish 2181:2181 --volume /etc/localtime:/etc/localtime wurstmeister/zookeeper

#启动kafka
docker run -d --name kafka --publish 9092:9092 \
--link zookeeper \
--env KAFKA_ZOOKEEPER_CONNECT=192.168.1.84:2181 \
--env KAFKA_ADVERTISED_HOST_NAME=192.168.1.84 \
--env KAFKA_ADVERTISED_PORT=9092  \
--volume /etc/localtime:/etc/localtime \
wurstmeister/kafka:2.11-0.11.0.3

```

## 测试

```bash
bin/kafka-topics.sh --create --zookeeper 192.168.1.84:2181 --replication-factor 1 --partitions 1 --topic mykafka
bin/kafka-topics.sh --list --zookeeper 192.168.1.84:2181
bin/kafka-console-producer.sh --broker-list 192.168.1.84:9092 --topic mykafka
bin/kafka-console-consumer.sh --zookeeper 192.168.1.84:2181 --topic mykafka --from-beginning
```
