# mysql 使用 `mysql-udf-http` 组件，可以注册触发器，主动发送http消息。

## 1. 安装

```bash
apt -y install libcurl4-openssl-dev
apt -y install mysql-server
apt -y install make
apt -y install gcc
apt -y install libmysqlclient-dev
apt -y install pkg-config

echo "/usr/lib/mysql/" > /etc/ld.so.conf.d/mysql.conf
/sbin/ldconfig
git clone https://github.com/yueyemingming/mysql-udf-http
cd mysql-udf-http
chmod +x configure
./configure --with-mysql=/usr/bin/mysql_config
make -j4 && make install
cp -rf /usr/local/lib/mysql-udf-http.* /usr/lib/mysql/plugin
```

## 2. 在数据库中执行此脚本，用于注册sql函数

```sql
create function http_get returns string soname 'mysql-udf-http.so';
create function http_post returns string soname 'mysql-udf-http.so';
create function http_put returns string soname 'mysql-udf-http.so';
create function http_delete returns string soname 'mysql-udf-http.so';
```

## 2. 在数据库中执行此脚本，创建表test

```sql
CREATE TABLE `test` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `name` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8
```

## 3. 在数据库中执行此脚本，创建触发器 test_update_triger

```sql
DELIMITER |  
DROP TRIGGER IF EXISTS test_update_triger;  
CREATE TRIGGER test_update_triger  
AFTER UPDATE ON test  
FOR EACH ROW BEGIN  
    SET @tt_re = (SELECT http_get(CONCAT('http://10.10.10.240:9000/my.do?id=', OLD.id)));  
END |  
DELIMITER ;
```

## 4. 启动web服务器，查看接收的数据

```bash
python -m SimpleHTTPServer 9000
```

此时可以修改test表中的数据，可以看到web服务端终端上接收到消息。
