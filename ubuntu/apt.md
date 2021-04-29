# apt使用

## 阿里的16.04源

```bash
echo "" > /etc/apt/sources.list
echo "deb http://mirrors.aliyun.com/ubuntu/ xenial main restricted" >> /etc/apt/sources.list
echo "deb-src http://mirrors.aliyun.com/ubuntu/ xenial main restricted multiverse universe #Added by software-properties" >> /etc/apt/sources.list
echo "deb http://mirrors.aliyun.com/ubuntu/ xenial-updates main restricted" >> /etc/apt/sources.list
echo "deb-src http://mirrors.aliyun.com/ubuntu/ xenial-updates main restricted multiverse universe" >> /etc/apt/sources.list
echo "deb http://mirrors.aliyun.com/ubuntu/ xenial universe" >> /etc/apt/sources.list
echo "deb http://mirrors.aliyun.com/ubuntu/ xenial-updates universe" >> /etc/apt/sources.list
echo "deb http://mirrors.aliyun.com/ubuntu/ xenial multiverse" >> /etc/apt/sources.list
echo "deb http://mirrors.aliyun.com/ubuntu/ xenial-updates multiverse" >> /etc/apt/sources.list
echo "deb http://mirrors.aliyun.com/ubuntu/ xenial-backports main restricted universe multiverse" >> /etc/apt/sources.list
echo "deb-src http://mirrors.aliyun.com/ubuntu/ xenial-backports main restricted universe multiverse" >> /etc/apt/sources.list
echo "deb http://mirrors.aliyun.com/ubuntu/ xenial-security main restricted" >> /etc/apt/sources.list
echo "deb-src http://mirrors.aliyun.com/ubuntu/ xenial-security main restricted multiverse universe" >> /etc/apt/sources.list
echo "deb http://mirrors.aliyun.com/ubuntu/ xenial-security universe" >> /etc/apt/sources.list
echo "deb http://mirrors.aliyun.com/ubuntu/ xenial-security multiverse" >> /etc/apt/sources.list

apt update
```

```bash
apt install package         #安装包
apt install package --reinstall     #重新安装包
apt -f install              #修复安装"-f = ——fix-missing"
apt remove package          #删除包
apt remove package --purge  #删除包，包括删除配置文件等
apt update                  #更新源
apt upgrade                 #更新已安装的包
apt dist-upgrade            #升级系统
apt dselect-upgrade         #使用 dselect 升级
apt build-dep package       #安装相关的编译环境
apt source package          #下载该包的源代码
apt clean                   #猛烈的清除无用的包
apt autoclean               #清理无用的包
apt check                   #检查是否有损坏的依赖

apt-cache search package    #搜索包
apt-cache show package      #获取包的相关信息，如说明、大小、版本等
apt-cache depends package   #了解使用依赖
apt-cache rdepends package  #是查看该包被哪些包依赖
```
