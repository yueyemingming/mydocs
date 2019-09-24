# mongo ubuntu 安装

## 1. 社区版本安装使用

- [社区版本安装教程](https://docs.mongodb.com/manual/tutorial/install-mongodb-on-ubuntu/)  
        <https://docs.mongodb.com/manual/tutorial/install-mongodb-on-ubuntu/>

### 1.1 安装

```bash
wget -qO - https://www.mongodb.org/static/pgp/server-4.0.asc | sudo apt-key add -
echo "deb [ arch=amd64,arm64 ] https://repo.mongodb.org/apt/ubuntu xenial/mongodb-org/4.0 multiverse" | sudo tee /etc/apt/sources.list.d/mongodb-org-4.0.list
apt-get update

#安装最新版本
apt-get install -y mongodb-org

#安装指定版本
apt-get install -y mongodb-org=4.0.11 mongodb-org-server=4.0.11 mongodb-org-shell=4.0.11 mongodb-org-mongos=4.0.11 mongodb-org-tools=4.0.11
```

### 1.2 运行

```bash
# 服务端
service mongod start
service mongod stop
service mongod restart

# 客户端
mongo
```

### 1.3 卸载

```bash
apt-get purge mongodb-org*
rm -r /var/log/mongodb
```

### 1.4 包介绍

包名 | 描述
:--- | :---
mongodb-org | 总体包，包含以下四个包，安装这个包会自动安装以下四个包
mongodb-org-server | 包含mongod daemon、初始化脚本、配置文件等
mongodb-org-mongos | 只包含mongod daemon
mongodb-org-shell | 包含初始化脚本和配置文件
mongodb-org-tools | 管理工具及客户端工具<br>  mongoimport bsondump<br>  mongodump<br>  mongoexport<br>  mongofiles<br>  mongorestore<br>  mongostat<br>  mongotop

## 2. 第二种安装方式

- [社区版本选择下载链接](https://www.mongodb.com/download-center/community)  
        <https://www.mongodb.com/download-center/community>

```bash
curl -O https://fastdl.mongodb.org/linux/mongodb-linux-x86_64-3.0.6.tgz
tar -zxvf mongodb-linux-x86_64-3.0.6.tgz
mv  mongodb-linux-x86_64-3.0.6/ /usr/local/mongodb

echo "export PATH=/usr/local/mongodb/mongodb-linux-x86_64-3.0.6/bin:$PATH" >> ~/.bashrc
source ~/.bashrc

# 启动
mongod

# 客户端
mongo
```
