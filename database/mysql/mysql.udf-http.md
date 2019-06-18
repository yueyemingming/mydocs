# mysql 主动触发器发送http消息 mysql-udf-http

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

```sql
create function http_get returns string soname 'mysql-udf-http.so';
create function http_post returns string soname 'mysql-udf-http.so';
create function http_put returns string soname 'mysql-udf-http.so';
create function http_delete returns string soname 'mysql-udf-http.so';
```

创建表test。

```sql
CREATE TABLE `test` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `name` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8
```

创建触发器 test_update;

```sql
DELIMITER |  
DROP TRIGGER IF EXISTS test_update;  
CREATE TRIGGER test_update  
AFTER UPDATE ON test  
FOR EACH ROW BEGIN  
    SET @tt_re = (SELECT http_get(CONCAT('http://10.10.10.240:9000/my.do?id=', OLD.id)));  
END |  
DELIMITER ;
```

```bash
python -m SimpleHTTPServer 9000
```
