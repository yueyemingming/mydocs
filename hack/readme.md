# 攻防知识

- 系统里一堆没用的账户可以删除

```bash
userdel uname
groupdel gname
```

- 关闭系统一堆没用的服务

```bash
chkconfig bluetooth off
ubuntu上chkconfig已经被 sysv-rc-conf代替了。
apt-get install sysv-rc-conf
sysv-rc-conf nginx on
```

- 修改用户权限

```bash
vim /etc/sudoers
user01  ALL = /bin/more /etc/shadow         #让user01可以访问/etc/shadow这个文件
user01  ALL = NOPASSWD: /etc/init.d/nginx restart   #让user01不需要输入密码的情况下，重启这个服务.
user01  ALL = (ALL) NOPASSWD: ALL           #让user01在不输入密码的情况下，具有root超级权限，这个牛逼。
```

- 登录时控制台的显示信息

```bash
/etc/issue  本地登录时现实
/etc/issue.net  ssh登录时现实，可以配置/etc/ssh/sshd_config中的'#Banner /etc/issue.net'来注释掉

/etc/motd   登录时的公告信息，针对redhat家族
/etc/update-motd.d  登录时的公告信息，针对ubuntu家族

/etc/os-release 操作系统信息
```

- 防火墙
linux防火墙有两层 : 1.iptables 2. tcp_wrappers
  - tcp_wrappers
    - /etc/hosts.allow
    ```txt
    service:host(s) [:action]
    ```

    ```txt
    ALL:ALL EXCEPT 192.168.1.9  #除这个ip，其他都能访问
    sshd: 192.158.1.3           #这个ip可以ssh登录
    ```

    - /etc/hosts.deny

    ```txt
    sshd:ALL                    #所有机器不能ssh登录
    ```

- 文件属性    chattr, lsattr

  ```bash
  chattr i    可以修改文件的权限属性，包括设置文件不能被修改，删除，重命名，设定链接，写入或追加内容等
  ```

- 缓存
  - /dev/shm			共享内存设备，她使用的是tmpfs，tmpfs是内存文件系统。
  - /tmp, /var/tmp	硬盘缓存。

- yum

  | 命令              | 含义                       |
  | ----------------- | -------------------------- |
  | yum info          | 查看可以安装或更新的包信息 |
  | yum info <包名>   | 查看包信息                 |
  | yum info aaa*     | 查看所有aaa开头的包信息    |
  | yum list          | 列出可以安装或跟新的包     |
  | yum search <包名> | 查找某个包                 |
  | yum clean         | 清除所有暂存的包和头文件   |
  | yum clean all     | 清除所有暂存的包和头文件   |
  | yum clean <包名>  | 清除暂存的包               |
  | yum clean headers | 清除暂存的rpm头文件        |

- 不错的yum源
  - epel-release
  - rpmforge-release

- 后门入侵工具 rootkit

  分为应用级和内核级别的rootkit

- rootkit检测工具 chkrootkit, RKHunter

- 查看登录情况
  - w		查看在线用户
  - last	查看用户登录事件

- 检查已安装的软件的包文件是否发生过篡改

```bash
rpm -va <packge>
```

- 查看登录信息

```bash
more /var/log/secure | grep Accepted
ls -al /proc/22765/exec
```

- 通过进程id，找到可执行程序路径

```bash
ls -al /proc/14338/exe
lrwxrwxrwx 1 root root 0 Mar 14 17:15 /proc/14338/exe -> /root/eosio/2.0/bin/nodeos
```

- 监控网卡的实时流量
  - iftop
  - ntop
  - ntopng  ntop的升级版本

他们需要用到redis, ntop家族程序默认服务端口3000

```bash
# 服务端启动    
service redis start
service ntopng start

# 客户端查看
http://ip:3000			# 用户名amdin 密码admin
```

- iperf   带宽测速工具

- nmap 网络探测工具

1. 主机发现
    nmap -sn ip 只发现主机，相当于ping，不扫描端口
    nmap -PE80 ip 通过 ICMP echo方式ping，ping的方式有很多， timstamp, netmask, tcp包方式等。
    nmap -sn -PE22,80 -PU53 www.abc.com
2. 端口扫描
    nmap ip 不加参数，默认端口扫描
    nmap -T4 -A -v ip   T是扫描基本，0-5，级别越高，时间越长，数据越多越准确， -A扫描全部
    nmap -p80 ip 只扫描某个端口是否开放
3. 应用程序和版本侦测
    nmap -sV ip
4. 操作系统和版本侦测
    nmap -O ip

