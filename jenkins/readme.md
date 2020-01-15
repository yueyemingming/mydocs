# jenkins

## 安装

```bash
apt add-apt-repository ppa:openjdk-r/ppa
apt update
apt -y install openjdk-8-jdk

apt -y install python-software-properties
wget -q -O - http://pkg.jenkins-ci.org/debian-stable/jenkins-ci.org.key | apt-key add -
sh -c 'echo deb http://pkg.jenkins-ci.org/debian-stable binary/ > /etc/apt/sources.list.d/jenkins.list'
apt update
apt -y install jenkins
```

## 启动

```bash
systemctl start jenkins
```

Jenkins启动后，默认的日志会输出到/var/log/jenkins/jenkins.log文件中，启动后默认会创建一个管理员账号admin，以及为此账号产生一个密码，密码会写入到文件 **/var/lib/jenkins/secrets/initialAdminPassword** 中，可通过vim或vi查看日志文件和密码文件。

## 访问

在浏览器地址栏输入http://IP:8080/ 就可以访问

Jenkins默认端口为8080
