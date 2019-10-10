# nginx高并发配置

- socket
  - nginx
    - http连接快速关闭(keep_alivetime)
    - 子进程允许打开的连接(worker_connections)
  - 系统层面
    - 最大连接数(somaxconn)
    - 加快tcp连接的回收(recyle)
    - 空的tcp是否允许回收利用(reuse)
    - 洪水攻击 不做洪水抵御(synccookies)
- 文件
  - nginx
    - 子进程允许打开的文件  worker_limit_nofiles
  - 系统
    - ulimit -n 设置一个比较大的值

## 1. socket

### 1.1 nginx

vim /etc/nginx/nginx.conf

#### 1.1.1 http连接快速关闭(keep_alivetime)

```nginx
http {
  keepalive_timeout 0;
  ... ...
}
```

#### 1.1.2 子进程允许打开的连接(worker_connections)

```nginx
events {
  worker_connections 768;
}
```

### 1.2 系统层面

#### 1.2.1 最大连接数(somaxconn)

```bash
echo 65535 > /proc/sys/net/core/somaxconn
```

#### 1.2.2 加快tcp连接的回收(recyle)

```bash
echo 1 > /proc/sys/net/ipv4/tcp_tw_recycle
```

#### 1.2.3 空的tcp是否允许回收利用(reuse)

```bash
echo 1 > /proc/sys/net/ipv4/tcp_tw_reuse
```

#### 1.2.4 洪水攻击 不做洪水抵御(synccookies)

```bash
echo 0 > /proc/sys/net/ipv4/tcp_syncookies
```

测试客户端设置文件最大打开个数

```bash
ulimit -n <num>
```
