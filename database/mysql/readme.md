# MySQL

* [mysql ubuntu16.04 安装](mysql.ubuntu16.04.install.md)
* [mysql CentOS7 安装](mysql.centos7.install.md)
* [Linux通过ODBC连接MySQL](mysql.odbc.linux.md)
* [mysql 获取本周本月本季开始结束时间](mysql.zhou.yue.ji.md)
* [mysql 使用 `mysql-udf-http` 组件，可以注册触发器，主动发送http消息。](mysql.udf-http.md)

```bash
apt install -y mysql-server libmysqlclient-dev libreadline-dev
mysql -u root -p

use mysql;
```

```mysql
grant all privileges on *.* to 'root'@'%' identified by '123456';
flush privileges;

create user 'test'@'%' identified by '123456';
create user 'test'@'localhost' identified by '123456';
grant all privileges on *.* to 'test'@'%' identified by '123456';
grant all privileges on `testdb`.* to 'test'@'%' identified by '123456';
grant select,delete,update,create,drop on *.* to 'test'@'%' identified by '123456';

delete from user where User='test' and Host='localhost';
drop user 'test'@'%';
```

```字符集
latin1 -- cp1252 West European
latin1_swedish_ci
```
