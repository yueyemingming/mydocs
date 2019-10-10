# nginx访问状态监控

## 1. 模块支持

要nginx带有监控功能，首先要让nginx支持监控模块 **--with-http_sub_module** 和 **--with-http_stub_status_module** 。

```bash
#查看是否自持
⚡ root@eosio ~ nginx -V

nginx version: nginx/1.10.3 (Ubuntu)
built with OpenSSL 1.0.2g  1 Mar 2016
TLS SNI support enabled
configure arguments: --with-cc-opt='-g -O2 -fPIE -fstack-protector-strong -Wformat -Werror=format-security -Wdate-time -D_FORTIFY_SOURCE=2' --with-ld-opt='-Wl,-Bsymbolic-functions -fPIE -pie -Wl,-z,relro -Wl,-z,now' --prefix=/usr/share/nginx --conf-path=/etc/nginx/nginx.conf --http-log-path=/var/log/nginx/access.log --error-log-path=/var/log/nginx/error.log --lock-path=/var/lock/nginx.lock --pid-path=/run/nginx.pid --http-client-body-temp-path=/var/lib/nginx/body --http-fastcgi-temp-path=/var/lib/nginx/fastcgi --http-proxy-temp-path=/var/lib/nginx/proxy --http-scgi-temp-path=/var/lib/nginx/scgi --http-uwsgi-temp-path=/var/lib/nginx/uwsgi --with-debug --with-pcre-jit --with-ipv6 --with-http_ssl_module --with-http_stub_status_module --with-http_realip_module --with-http_auth_request_module --with-http_addition_module --with-http_dav_module --with-http_geoip_module --with-http_gunzip_module --with-http_gzip_static_module --with-http_image_filter_module --with-http_v2_module --with-http_sub_module --with-http_xslt_module --with-stream --with-stream_ssl_module --with-mail --with-mail_ssl_module --with-threads
```

## 配置页面

```nginx
server{
    listen 8080;
    location /status {
        stub_status on;
        access_log off;
        #allow 127.0.0.1;
        #deny all;
    }
}
```

重启nginx : `nginx -s reload`

```txt
Active connections: 3
server accepts handled requests
 8      8       67
Reading: 0 Writing: 1 Waiting: 2
```

- "Active connections: 3" 表示nginx正在处理的活动连接数3个。

- "server accepts handled requests"  
  "8      8       67"
  - 第一个 server 表示nginx启动到现在共处理了 8 个连接
  - 第二个 accepts 表示nginx启动到现在共成功创建 8 次握手
  - 第三个 handled requests 表示总共处理了 67 次请求
  - 请求丢失数 = 握手数 - 连接数 ，可以看出目前为止没有丢失请求

- "Reading: 0 Writing: 1 Waiting: 2"
  - Reading nginx读取到客户端的 Header 信息数
  - Writing nginx返回给客户端 Header 信息数
  - Waiting nginx已经处理完正在等候下一次请求指令的驻留链接（开启keep-alive的情况下，这个值等于 Active - (Reading+Writing)）
