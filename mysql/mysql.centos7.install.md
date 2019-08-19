# CentOS7中安装MySQL

因CentOS已经使用MariaDB替代了MySQL，如直接输入“yum install mysql-server”则安装MariaDB.

```bash
wget https://dev.mysql.com/get/mysql57-community-release-el7-9.noarch.rpm
rpm -ivh mysql57-community-release-el7-9.noarch.rpm
yum install mysql-server
systemctl start mysqld
```