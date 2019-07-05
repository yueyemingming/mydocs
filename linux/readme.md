# linux

- [ubuntu](ubuntu/readme.md)
  - [root登陆](ubuntu/root.login.md)
  - [apt](ubuntu/apt.md)
  - [配置系统包](ubuntu/ubuntu.package.md)
- [redhat](redhat/readme.md)
- [系统配置](readme.md)
  - [网络配置](linux.net.md)
  - [系统服务命令](service.md)
  - [防火墙](#防火墙)
- [命令使用](cmd/readme.md)  
  - [vim](cmd/vim/readme.md)
  - [ssh](cmd/ssh.md)
  - [find](cmd/find.md)
  - [netstat](cmd/netstat.md)
  - [netcat](cmd/netcat.md)
  - [tcpdump](cmd/tcpdump/readme.md)
  - [wget](cmd/wget/readme.md)
- [系统配置](readme.md)  
  - [source, sh, exec, .号的区别](#source-sh-exec-号的区别)
  - [解决sudo时的“sudo: unable to resolve host xxxx”的问题](#解决sudo时的sudo-unable-to-resolve-host-xxxx的问题)
  - [随系统启动](linux.autostart.md)
  - [mplayer开启硬解码的方面](mplayer/readme.md)
- **清华大学开源软件镜像站** —— **[https://mirrors.tuna.tsinghua.edu.cn/](https://mirrors.tuna.tsinghua.edu.cn/)**
- **阿里云开源软件镜像站** —— **[https://mirrors.aliyun.com/](https://mirrors.aliyun.com/)**

## 防火墙

```bash
## 关闭防火墙
/etc/init.d/iptables stop
service iptables stop
setenforce 0

## 永久禁止
vim /etc/sysconfig/selinux

SELINUX=disabled
SELINUXTYPE=disabled
```

## source, sh, exec, .号的区别

命令 | 含义
:--- | :---
source和.号 | shell中使用source run.sh，是直接运行run.sh的命令，不创建子shell，类似与html中include。  .号跟source基本相同，用来引用其他文件(一般定义了一些function，和变量)，跟source基本相同。
sh | sh是则创建子shell，子shell里面 的变量父shell无法使用，对环境变量的修改也不影响父shell。父shell中的局部变量，子shell也无法使用，只有父shell的环境变量， 子shell能够使用。
exec | 还有一个exec run.sh，这个与source类似，区别是exec执行完，不再执行后面的语句。

## 解决sudo时的“sudo: unable to resolve host xxxx”的问题

> vim /etc/hosts

```bash
127.0.0.1       localhost xxxx   #这后面添上这个主机名就行
```
