# Linux通过ODBC连接MySQL

## 1. 下载MySQL驱动包

下载路径: <http://dev.mysql.com/downloads/connector/odbc/>

已经下载。

```bash
wget -c https://cdn.mysql.com//Downloads/Connector-ODBC/8.0/mysql-connector-odbc-8.0.11-linux-ubuntu16.04-x86-64bit.tar.gz
```

## 2. 解压MySQL驱动包到/usr/local文件下

```bash
mv mysql-connector-odbc-8.0.11-linux-ubuntu16.04-x86-64bit.tar.gz /usr/local
cd /usr/local
tar zxvf mysql-connector-odbc-8.0.11-linux-ubuntu16.04-x86-64bit.tar.gz
```

## 3. 安装MySQL数据库驱动

```bash
cd /usr/local/mysql-connector-odbc-8.0.11-linux-ubuntu16.04-x86-64bit/bin
./myodbc-installer -d -a -n "MYSQL" -t "DRIVER=/usr/lib/libmyodbc8a.so;SETUP=/usr/lib/libmyodbc8a.so"
```

## 4. 拷贝库

包lib下的三个动态库libmysodbc8a.so libmyodbc8s.so libmysodbc8w.so放入/usr/lib 和 /usr/lib64下

```bash
cd ../lib
cp libmyodbc8a.so libmyodbc8S.so libmyodbc8w.so /usr/lib
cp libmyodbc8a.so libmyodbc8S.so libmyodbc8w.so /usr/lib64
```

> 此处大家注意下5a 5s 5w之间的区别（根据数据库的不同编码格式选择动态库，否则会出现中文乱码）:
> libmyodbc5a.so 是ASCII编码格式
> libmyodbc5w.so 是UNICODE编码格式
> libmyodbc5s.so 是程序开发中配置数据源提供界面的动态链接库

## 5. 配置odbcinst.ini 安装MySQL后自动生成

```bash
vim /etc/odbcinst.ini
```

```ini
[MYSQL]
Driver=/usr/lib/libmyodbc8a.so
SETUP=/usr/lib/libmyodbc8a.so
UsageCount   = 1
FileUsage    = 1
```

## 6. 配置odbc.ini

```bash
vim /etc/odbc.ini
```

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

## 7. 查看定义的数据源

```bash
odbcinst -q -s
```

```text
[mysql_odbc_test]
```

## 8. 测试数据源连接情况

```bash
isql -v test root 123456
```

```text
+---------------------------------------+
| Connected!                            |
|                                       |
| sql-statement                         |
| help [tablename]                      |
| quit                                  |
|                                       |
+---------------------------------------+
```