# mysql 基本使用

## 授权

```mysql
grant all privileges on *.* to 'root'@'%' identified by '123456';
flush privileges;
```

## 字符集

```字符集
latin1 -- cp1252 West European
latin1_swedish_ci
```

## 安装

```bash
apt install -y mysql-server libmysqlclient-dev libreadline-dev
```

## 1. root登录MySQL

```bash
mysql -u root -p
```

```mysql
mysql> use mysql;
```

## 2. 添加新用户

### 2.1 运行localhost访问

```mysql
mysql> create user 'test'@'localhost' identified by '123456';
mysql> flush privileges;
```

### 2.2 允许外网 IP 访问

```mysql
mysql> create user 'test'@'%' identified by '123456';
mysql> flush privileges;
```

## 3. 为用户分配权限

### 3.1 授予所有库的权限

```mysql
mysql> grant all privileges on *.* to 'test'@'%' identified by '123456';
mysql> flush privileges;
```

### 3.2 授予某库的权限

```mysql
mysql> grant all privileges on `testdb`.* to 'test'@'%' identified by '123456';
mysql> flush privileges;
```

### 3.3 授予某库的某权限

```mysql
mysql> grant select,delete,update,create,drop on *.* to 'test'@'%' identified by '123456';
mysql> flush privileges;
```

### 3.4 test登录MySQL

```mysql
test@localhost:~$ mysql -u test -p
```

## 4. 删除用户

### 4.1 删除用户

```mysql
mysql> delete from user where User='test' and Host='localhost';
mysql> flush privileges;
```

### 4.2 删除账户及权限

```mysql
mysql> drop user 'test'@'%';
```
