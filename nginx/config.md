# nginx配置

## 1. 基本结构

```nginx
#全局区
worker_processes 1;             #有1个工作的子进程,可以自行修改,但太大无益,因为要争夺CPU,一般设置为 CPU数*核数

#一般是配置nginx连接的特性
Event {
    worker_connections  1024;   #一个worker子进程最大允许连1024个连接
}

#这是配置http服务器的主要段
http {
    Server1 {                   #这是虚拟主机段
        Location {              #定位,把特殊的路径或文件再次定位 ,如image目录单独处理
        }                       #/ 如.php单独处理
    }

    Server2 {
    }
}
```

### 1.1 基于域名的虚拟主机

```nginx
server {
    listen 80;                  #监听端口
    server_name a.com;          #监听域名

    location / {
        root /var/www/a.com;    #根目录定位
        index index.html;
    }
}
```

### 1.2 基于端口的虚拟主机配置

```nginx
server {
    listen 8080;
    server_name 192.168.1.204;

    location / {
        root /var/www/html8080;
        index index.html;
    }
}
```

## 2. 日志管理

### 2.1 main日志格式

```nginx
log_format main '$remote_addr - $remote_user [$time_local] "$request" '
                '$status $body_bytes_sent "$http_referer" '
                '"$http_user_agent" "$http_x_forwarded_for"';

access_log /var/log/nginx/access.log;   #访问日志文件,默认main格式
error_log /var/log/nginx/error.log;     #错误日志文件,默认main格式
```

- 远程IP
- 远程用户/用户时间
- 请求方法(如GET/POST)
- 请求体body长度
- referer来源信息
- http-user-agent 用户代理/蜘蛛, 被转发的请求的原始IP
- http_x_forwarded_for 在经过代理时,代理把你的本来IP加在此头信息中,传输你的原始IP

### 2.2 自定义日志格式

```nginx
log_format mylog '$remote_addr- "$request" '
                 '$status $body_bytes_sent "$http_referer" '
                 '"$http_user_agent" "$http_x_forwarded_for"';

access_log /var/log/nginx/access.log mylog;   #访问日志文件
error_log /var/log/nginx/error.log mylog;     #错误日志文件
```

### 2.3 日志按日期备份存储 ———— shell+定时任务+nginx信号管理

- 分析思路: 凌晨00:00:01,把昨天的日志重命名,放在相应的目录下。再用USR1信息号控制nginx重新生成新的日志文件。
- 具体脚本

```bash
#!/bin/bash
base_path='/usr/local/nginx/logs'
log_path=$(date -d yesterday +"%Y%m")
day=$(date -d yesterday +"%d")
mkdir -p $base_path/$log_path
mv $base_path/access.log $base_path/$log_path/access_$day.log
echo $base_path/$log_path/access_$day.log
kill -USR1 `cat /usr/local/nginx/logs/nginx.pid`
```

- 定时任务

```bash
vim /etc/crontab

01 00 * * * /xxx/path/b.sh  每天0时1分(建议在02-04点之间,系统负载小)
```

## 3. location

```nginx
location [=|~|~*|^~] patt {
}
```

- 一般匹配 `location patt {}`
- 精准匹配 `location = patt {}`
- 正则匹配 `location ~ patt {}` , ~区分大小写，~*不区分大小写。

![location](location的解析过程.tif)