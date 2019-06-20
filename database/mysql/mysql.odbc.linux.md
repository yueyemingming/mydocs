# Linux通过ODBC连接MySQL

## 1. 下载MySQL驱动包

下载路径: <http://dev.mysql.com/downloads/connector/odbc/>

已经下载。

我已应用到的我的github库中。

## 2. 解压MySQL驱动包到/usr/local文件下

```bash
wget -c https://github.com/yueyemingming/thirdpart/blob/master/mysql-connector-odbc-8.0.11-linux-ubuntu16.04-x86-64bit.tar.gz
tar xzvf mysql-connector-odbc-8.0.11-linux-ubuntu16.04-x86-64bit.tar.gz -C /usr/local
/usr/local/mysql-connector-odbc-8.0.11-linux-ubuntu16.04-x86-64bit/bin/myodbc-installer -d -a -n "MYSQL" -t "DRIVER=/usr/lib/libmyodbc8a.so;SETUP=/usr/lib/libmyodbc8a.so"

cp -rvf /usr/local/mysql-connector-odbc-8.0.11-linux-ubuntu16.04-x86-64bit/lib/libmyodbc8a.so /usr/lib
cp -rvf /usr/local/mysql-connector-odbc-8.0.11-linux-ubuntu16.04-x86-64bit/lib/libmyodbc8S.so /usr/lib
cp -rvf /usr/local/mysql-connector-odbc-8.0.11-linux-ubuntu16.04-x86-64bit/lib/libmyodbc8w.so /usr/lib
cp -rvf /usr/local/mysql-connector-odbc-8.0.11-linux-ubuntu16.04-x86-64bit/lib/libmyodbc8a.so /usr/lib64
cp -rvf /usr/local/mysql-connector-odbc-8.0.11-linux-ubuntu16.04-x86-64bit/lib/libmyodbc8S.so /usr/lib64
cp -rvf /usr/local/mysql-connector-odbc-8.0.11-linux-ubuntu16.04-x86-64bit/lib/libmyodbc8w.so /usr/lib64

cp -rvf odbcinst.ini /etc/
cp -rvf odbc.ini /etc/
```

> 此处大家注意下5a 5s 5w之间的区别（根据数据库的不同编码格式选择动态库，否则会出现中文乱码）:
> libmyodbc5a.so 是ASCII编码格式
> libmyodbc5w.so 是UNICODE编码格式
> libmyodbc5s.so 是程序开发中配置数据源提供界面的动态链接库

### 2.1 odbcinst.ini

```ini
[MYSQL]
Driver=/usr/lib/libmyodbc8a.so
SETUP=/usr/lib/libmyodbc8a.so
UsageCount   = 1
FileUsage    = 1
```

### 2.2 配置odbc.ini

```ini
[mysql_odbc_test]
Description = mysql_odbc_test
Driver      = /usr/lib/libmyodbc8a.so   //直接跳过odbcinst.ini
Server      = localhost
USER        = root
PASSWORD    = 123456
Database    = test
OPTION      = 3
```

## 3. 查看定义的数据源

```bash
odbcinst -q -s

---------------------
[mysql_odbc_test]
---------------------

isql -v test root 123456

+---------------------------------------+
| Connected!                            |
|                                       |
| sql-statement                         |
| help [tablename]                      |
| quit                                  |
|                                       |
+---------------------------------------+
```
