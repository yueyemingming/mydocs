# Ubuntu下Supervisor安装、配置和使用

## 1、认识supervisor

Supervisor（http://supervisord.org/）是用Python开发的一个client/server服务，是Linux/Unix系统下的一个进程管理工具，不支持Windows系统。它可以很方便的监听、启动、停止、重启一个或**多个**进程。用Supervisor管理的进程，当一个进程意外被杀死，supervisord监听到进程死后，会自动将它重新拉起，很方便的做到进程自动恢复的功能，不再需要自己写shell脚本来控制。

## 2、安装supervisor

```bash
apt -y install supervisor
```

- supervisord ：supervisor的守护进程服务
- supervisorctl ：客户端
- echo_supervisord_conf ：生成初始配置文件程序

## 3、配置

使用echo_supervisord_conf生成默认的配置文件

```bash
echo_supervisord_conf > /etc/supervisor/supervisord.conf
```

## 4、配置文件参数说明

> 分号（;）开头的配置表示注释

```ini
[unix_http_server]
file=/tmp/supervisor.sock   ;UNIX socket 文件，supervisorctl 会使用
;chmod=0700                 ;socket文件的mode，默认是0700
;chown=nobody:nogroup       ;socket文件的owner，格式：uid:gid

;[inet_http_server]         ;HTTP服务器，提供web管理界面
;port=0.0.0.0:9001          ;Web管理后台运行的IP和端口，如果开放到公网，需要注意安全性
;username=user              ;登录管理后台的用户名
;password=123               ;登录管理后台的密码

[supervisord]
logfile=/tmp/supervisord.log ;日志文件，默认是 $CWD/supervisord.log
logfile_maxbytes=50MB        ;日志文件大小，超出会rotate，默认 50MB，如果设成0，表示不限制大小
logfile_backups=10           ;日志文件保留备份数量默认10，设为0表示不备份
loglevel=info                ;日志级别，默认info，其它: debug,warn,trace
pidfile=/tmp/supervisord.pid ;pid 文件
nodaemon=false               ;是否在前台启动，默认是false，即以 daemon 的方式启动
minfds=1024                  ;可以打开的文件描述符的最小值，默认 1024
minprocs=200                 ;可以打开的进程数的最小值，默认 200

[supervisorctl]
serverurl=unix:///tmp/supervisor.sock ;通过UNIX socket连接supervisord，路径与unix_http_server部分的file一致
;serverurl=http://127.0.0.1:9001 ; 通过HTTP的方式连接supervisord

; [program:xx]是被管理的进程配置参数，xx是进程的名称
[program:xx]
command=/opt/apache-tomcat-8.0.35/bin/catalina.sh run  ; 程序启动命令
autostart=true       ; 在supervisord启动的时候也自动启动
startsecs=10         ; 启动10秒后没有异常退出，就表示进程正常启动了，默认为1秒
autorestart=true     ; 程序退出后自动重启,可选值：[unexpected,true,false]，默认为unexpected，表示进程意外杀死后才重启
startretries=3       ; 启动失败自动重试次数，默认是3
user=tomcat          ; 用哪个用户启动进程，默认是root
priority=999         ; 进程启动优先级，默认999，值小的优先启动
redirect_stderr=true ; 把stderr重定向到stdout，默认false
stdout_logfile_maxbytes=20MB  ; stdout 日志文件大小，默认50MB
stdout_logfile_backups = 20   ; stdout 日志文件备份数，默认是10
; stdout 日志文件，需要注意当指定目录不存在时无法正常启动，所以需要手动创建目录（supervisord 会自动创建日志文件）
stdout_logfile=/opt/apache-tomcat-8.0.35/logs/catalina.out
stopasgroup=false     ;默认为false,进程被杀死时，是否向这个进程组发送stop信号，包括子进程
killasgroup=false     ;默认为false，向进程组发送kill信号，包括子进程

;包含其它配置文件
[include]
files = /etc/supervisor/conf.d/*.ini    ;可以指定一个或多个以.ini结束的配置文件
```

## 5、配置管理进程

进程管理配置参数，不建议全都写在supervisord.conf文件中，应该每个进程写一个配置文件放在include指定的目录下包含进supervisord.conf文件中。 

```ini
[include]
files = /etc/supervisor/conf.d/*.ini
```

 下面是配置Tomcat进程的一个例子：

```ini
[program:tomcat]
command=/bin/bash -c 'source /var/ftp_root/program/blog/startall.sh'
user=root
autostart=true
autorestart=true
redirect_stderr=True
stdout_logfile=/tmp/blog.log
stderr_logfile=/tmp/blog.err
```

 

## 6、控制进程

### **6.1 交互终端**

```bash
root@localhost ~ $ supervisorctl 	# 直接进入supervisor命令行，直接显示正在运行的已经配置进程
myprogram1                        RUNNING   pid 15137, uptime 0:00:03
myprogram2                        RUNNING   pid 15137, uptime 0:00:03
myprogram3                        RUNNING   pid 15137, uptime 0:00:03
supervisor> help

default commands (type help <topic>):
=====================================
add    exit      open  reload  restart   start   tail   
avail  fg        pid   remove  shutdown  status  update 
clear  maintail  quit  reread  signal    stop    version

supervisor> help stop
stop <name>		    Stop a process
stop <gname>:*		Stop all processes in a group
stop <name> <name>	Stop multiple processes or groups
stop all		    Stop all processes

supervisor> 

```

### **6.2 bash终端**

```
supervisorctl status
supervisorctl stop tomcat
supervisorctl start tomcat
supervisorctl restart tomcat
supervisorctl reread
supervisorctl update
```

 

| **命令**                   | **说明**                                                     |
| -------------------------- | ------------------------------------------------------------ |
| supervisorctl stop prog    | 停止某个进程                                                 |
| supervisorctl start prog   | 启动某个进程                                                 |
| supervisorctl restart prog | 重启某个进程                                                 |
| supervisorctl stop all     | 停止全部进程                                                 |
| supervisorctl reload       | 载入最新的配置文件，停止原有进程并按新的配置启动、管理所有进程 |
| supervisorctl update       | 根据最新的配置文件，启动新配置或有改动的进程，配置没有改动的进程不会受影响而重启 |

## 7、注意点：

1>关于在command中，执行.sh脚本
请参照此配置文件中command的写法，直接写command=source xxx，会报source不认识：

```
[program:blog]
command=/bin/bash -c 'source /var/ftp_root/program/blog/startall.sh'
user=root
autostart=true
autorestart=true
redirect_stderr=True
stdout_logfile=/tmp/blog.log
stderr_logfile=/tmp/blog.err
stopasgroup=true
killasgroup=true
```

