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

### 3.1 命中规则

1. 判断精准命中，如果命中，立即返回结果并结束解析过程
2. 判断普通命中，如果有多个命中，"记录"下来"最长"的命中结果（注意：记录但不结束，最长的为准）
3. 继续判断正则表达式的解析结果，按配置里的正则表达式顺序为准，由上到下开始匹配，一旦匹配成功 1 个，立即返回结果，并结束解析过程．
4. 延伸分析
  - 普通命中, 顺序无所谓，按长短确定命中，长的有限命中。  
  - 正则命中, 顺序有所谓，按从前往后去定命中。

![location](location.png)

### 3.2 例子1：精准命中和普通命中

```nginx
location = / {
    root   /var/www/html/;
    index  index.htm index.html;
}

location / {
    root   /usr/local/nginx/html;
    index  index.html index.htm;
}
```

如果访问　　http://xxx.com/

1. 精准匹配中"/", 得到index页为index.htm
2. 再次访问"/index.htm", 此次命中普通命中，内部转跳uri已经是"/index.htm", 根目录为/usr/local/nginx/html
3. 最终结果,访问了 /usr/local/nginx/html/index.htm

### 3.2 例子2：普通命中和正则命中

```nginx
location / {
    root   /usr/local/nginx/html;
    index  index.html index.htm;
}

location ~ image {
    root /var/www;
    index index.html;
}
```

如果我们访问 "http://xx.com/image/logo.png" ，此时, "/" 与 "/image/logo.png" 匹配，同时 "image"正则 与"image/logo.png"也能匹配,谁发挥作用? 正则表达式的成果将会使用，优先命中长的. 图片真正会访问 /var/www/image/logo.png 

### 3.3 例子3: 普通命中

```nginx
location / {
    root   /usr/local/nginx/html;
    index  index.html index.htm;
}

location /foo {
    root /var/www/html;
    index index.html;
}
```

访问 http://xxx.com/foo

对于uri "/foo", 两个location的patt,都能匹配他们, 即 '/'能从左前缀匹配 '/foo', '/foo'也能左前缀匹配'/foo', 此时, 真正访问 /var/www/html/index.html , '/foo'匹配的更长,因此使用之.
