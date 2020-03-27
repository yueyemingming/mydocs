# docker下安装kafka

## 下载

```bash
docker pull wurstmeister/zookeeper
docker pull wurstmeister/kafka
```

## 启动镜像

```bash
#启动zookeeper
docker run -d --name zookeeper \
--ip 172.18.0.2 \
--publish 2181:2181 \
--volume /etc/localtime:/etc/localtime wurstmeister/zookeeper

#启动kafka
docker run -d --name kafka \
--ip 172.18.0.3 \
--publish 9092:9092 \
--link zookeeper \
--env KAFKA_ZOOKEEPER_CONNECT=192.168.1.181:2181 \
--env KAFKA_ADVERTISED_HOST_NAME=192.168.1.181 \
--env KAFKA_ADVERTISED_PORT=9092  \
--volume /etc/localtime:/etc/localtime \
wurstmeister/kafka

#此命令也可
docker run -d --name kafka \
--ip 172.18.0.3 \
-p 9092:9092 \
-e KAFKA_BROKER_ID=0 \
-e KAFKA_ZOOKEEPER_CONNECT=192.168.1.181:2181 \
-e KAFKA_ADVERTISED_LISTENERS=PLAINTEXT://192.168.1.181:9092 \
-e KAFKA_LISTENERS=PLAINTEXT://192.168.1.181:9092 \
-v /etc/localtime:/etc/localtime \
wurstmeister/kafka
```

## 测试

```bash
docker exec -it kafka /bin/bash
cd /opt/kafka

bin/kafka-topics.sh --create --zookeeper 192.168.1.181:2181 --replication-factor 1 --partitions 3 --topic test
bin/kafka-topics.sh --list --zookeeper 192.168.1.181:2181
bin/kafka-console-producer.sh --broker-list 192.168.1.181:9092 --topic test
bin/kafka-console-consumer.sh --bootstrap-server 192.168.1.181:9092 --topic test --from-beginning


bin/kafka-topics.sh --create --zookeeper localhost:2181 --replication-factor 1 --partitions 3 --topic test
bin/kafka-topics.sh --list --zookeeper localhost:2181
bin/kafka-console-producer.sh --broker-list localhost:9092 --topic test
bin/kafka-console-consumer.sh --bootstrap-server localhost:9092 --topic test --from-beginning
```
