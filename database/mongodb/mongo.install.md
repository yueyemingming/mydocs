# mongo安装

```bash
curl -O https://fastdl.mongodb.org/linux/mongodb-linux-x86_64-3.0.6.tgz
tar -zxvf mongodb-linux-x86_64-3.0.6.tgz
mv  mongodb-linux-x86_64-3.0.6/ /usr/local/mongodb
```

```bashrc
vim ~/.bashrc                   # 加入如下内容

export PATH=/usr/local/mongodb/mongodb-linux-x86_64-3.0.6/bin:$PATH
```

启动

```bash
mongod              #后台启动
mongo               #登录进mongo
```
