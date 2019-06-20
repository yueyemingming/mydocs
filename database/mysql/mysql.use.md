# mysql基本使用

```bash
#安装
apt -y install mysql-server libmysqlclient-dev libreadline-dev

#执行脚本
mysql -udbuser -ppasswd dbname < run.sql

#登录
mysql -u root -p

#使用某个库
use mysql;
```

```mysql
--给root赋予牛逼权限
grant all privileges on *.* to 'root'@'%' identified by '123456';
flush privileges;

--创建外网可访问的账户
create user 'test'@'%' identified by '123456';
--创建只能本地访问的账户
create user 'test'@'localhost' identified by '123456';

--授予全部权限
grant all privileges on *.* to 'test'@'%' identified by '123456';
--授予某个库的全部权限
grant all privileges on `testdb`.* to 'test'@'%' identified by '123456';
--授予操作权限
grant select,delete,update,create,drop on *.* to 'test'@'%' identified by '123456';

--删除账户
delete from user where User='test' and Host='localhost';
drop user 'test'@'%';
```

```字符集
latin1 -- cp1252 West European
latin1_swedish_ci
```
