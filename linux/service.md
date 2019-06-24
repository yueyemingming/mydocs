# 系统服务命令

```bash
#命令启动
service network stop    #关闭网络服务
service network start   #启动网络服务
service network restart #重启网络服务
systemctl start <xxx>   #较新的方式

#直接启动脚本
/etc/init.d/network stop
/etc/init.d/network start
/etc/init.d/network restart

#查看状态
service network status

#随系统启动
chkconfig smbd on       ## 早期命令
systemctl enable smbd   ## 现在命令
```
