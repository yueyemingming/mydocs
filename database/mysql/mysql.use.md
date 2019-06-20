# mysql基本使用


```bash
apt -y install mysql-server libmysqlclient-dev libreadline-dev

mysql -udbuser -ppasswd dbname < run.sql

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
