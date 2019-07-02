# apt使用

## 阿里的16.04源

```config
deb-src http://archive.ubuntu.com/ubuntu xenial main restricted #Added by software-properties
deb http://mirrors.aliyun.com/ubuntu/ xenial main restricted
deb-src http://mirrors.aliyun.com/ubuntu/ xenial main restricted multiverse universe #Added by software-properties
deb http://mirrors.aliyun.com/ubuntu/ xenial-updates main restricted
deb-src http://mirrors.aliyun.com/ubuntu/ xenial-updates main restricted multiverse universe #Added by software-properties
deb http://mirrors.aliyun.com/ubuntu/ xenial universe
deb http://mirrors.aliyun.com/ubuntu/ xenial-updates universe
deb http://mirrors.aliyun.com/ubuntu/ xenial multiverse
deb http://mirrors.aliyun.com/ubuntu/ xenial-updates multiverse
deb http://mirrors.aliyun.com/ubuntu/ xenial-backports main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ xenial-backports main restricted universe multiverse #Added by software-properties
deb http://archive.canonical.com/ubuntu xenial partner
deb-src http://archive.canonical.com/ubuntu xenial partner
deb http://mirrors.aliyun.com/ubuntu/ xenial-security main restricted
deb-src http://mirrors.aliyun.com/ubuntu/ xenial-security main restricted multiverse universe #Added by software-properties
deb http://mirrors.aliyun.com/ubuntu/ xenial-security universe
deb http://mirrors.aliyun.com/ubuntu/ xenial-security multiverse
```

```bash
apt-cache search package    #搜索包
apt-cache show package      #获取包的相关信息，如说明、大小、版本等
apt install package         #安装包
apt install package --reinstall     #重新安装包
apt -f install              #修复安装"-f = ——fix-missing"
apt remove package          #删除包
apt remove package --purge  #删除包，包括删除配置文件等
apt update                  #更新源
apt upgrade                 #更新已安装的包
apt dist-upgrade            #升级系统
apt dselect-upgrade         #使用 dselect 升级
apt-cache depends package   #了解使用依赖
apt-cache rdepends package  #是查看该包被哪些包依赖
apt build-dep package       #安装相关的编译环境
apt source package          #下载该包的源代码
apt clean                   #猛烈的清除无用的包
apt autoclean               #清理无用的包
apt check                   #检查是否有损坏的依赖
```
