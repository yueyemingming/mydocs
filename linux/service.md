# 系统服务命令

## 方法一

```bash
service network stop     #关闭网络服务
service network start    #启动网络服务
service network restart  #重启网络服务
```

## 方法二

```bash
/etc/init.d/network stop
/etc/init.d/network start
/etc/init.d/network restart
```

## 网卡状态查询

```bash
service network status
````
