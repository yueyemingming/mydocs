# OpenSSL之编译安装

[![img](https://ucenter.51cto.com/images/noavatar_middle.gif)](https://blog.51cto.com/shjia)

[shenjia_009](https://blog.51cto.com/shjia)1人评论[3038人阅读](javascript:;)[2014-06-17 01:50:04](javascript:;)

**OpenSSL编译安装for Linux:**

```php
安装环境：
         操作系统：CentOS release 6.5 
         OpenSSL版本：openssl-1.0.1h
下载源码包：
         源码包名称：openssl-1.0.1h.tar.gz
         下载地址：http://www.openssl.org/source/
         校验源码包：
                 [root@localhost ~]# sha1sum openssl-1.0.1h.tar.gz
                 b2239599c8bf8f7fc48590a55205c26abe560bf8  openssl-1.0.1h.tar.gz
                 [root@localhost ~]# cat openssl-1.0.1h.tar.gz.sha1 
                 b2239599c8bf8f7fc48590a55205c26abe560bf8
解压源码包：
                 [root@localhost ~]# tar -zxvf openssl-1.0.1h.tar.gz
开始安装：
         ./config --prefix=/opt/openssl shared zlib-dynamic enable-camellia
         make
         make install
         问题：make时出现了错误，make[2]: *** [c_zlib.o] Error 1等好几个依赖关系的失败
               原来是zlib-devel这个软件包没装，yum -y install zlib-devel,于是make clean ,再次make，问题即可迎刃而解！
               
验证安装： /opt/openssl/bin/openssl version
临时设置PATH：
              export PATH="/opt/openssl/bin/:${PATH}"
              which openssl
配置文件中设置PATH：
              echo 'export PATH="/opt/openssl/bin/:${PATH}"' >>~/.bash_profile 
              logout重新登录测试一下
              which openssl
```

**OpenSSL编译安装for Windows:**

```php
安装环境：Windows 7 旗舰版
          openssl-1.0.0m
相关软件下载：
          编译器：C++编译器：VC++ 6.0
                  http://pan.baidu.com/share/link?shareid=108824630&uk=2283015311
                         提取码：9ig9
          Perl:   ActivePerl
                  http://www.activestate.com/activeperl/downloads/
          OpenSSL: 
                  http://www.openssl.org/source/
环境准备：
          安装VC++ 6.0
          安装Perl
          解压OpenSSL到C:\openssl
```

 测试Perl:  

​     [![wKiom1OfatvAhls2AACkfPAuOHg698.jpg](https://s3.51cto.com/wyfs02/M02/2F/4E/wKiom1OfatvAhls2AACkfPAuOHg698.jpg)](https://s3.51cto.com/wyfs02/M02/2F/4E/wKiom1OfatvAhls2AACkfPAuOHg698.jpg)

​     注：如果不能执行，请设置Perl的环境变量。



 初始化VC++ 6.0的环境变量：

​     [![wKioL1Ofa_HwGujxAAA5sTc3Apc195.jpg](https://s3.51cto.com/wyfs02/M01/2F/4F/wKioL1Ofa_HwGujxAAA5sTc3Apc195.jpg)](https://s3.51cto.com/wyfs02/M01/2F/4F/wKioL1Ofa_HwGujxAAA5sTc3Apc195.jpg) 

 

 测试VC++ 6.0命令行工具：

​     [![wKiom1OfbI2DwlErAACH5uk6vgY209.jpg](https://s3.51cto.com/wyfs02/M00/2F/4E/wKiom1OfbI2DwlErAACH5uk6vgY209.jpg)](https://s3.51cto.com/wyfs02/M00/2F/4E/wKiom1OfbI2DwlErAACH5uk6vgY209.jpg)  



 开始安装OpenSSL:

​     ***执行config命令：**

​     [![wKioL1Ofbm3g5zgHAAFljJxv1Ko662.jpg](https://s3.51cto.com/wyfs02/M02/2F/4F/wKioL1Ofbm3g5zgHAAFljJxv1Ko662.jpg)](https://s3.51cto.com/wyfs02/M02/2F/4F/wKioL1Ofbm3g5zgHAAFljJxv1Ko662.jpg)



​     ***运行ms\do_ms.bat**

​     [![wKioL1OfceqSjN-uAABhFHaXrsQ792.jpg](https://s3.51cto.com/wyfs02/M01/2F/4F/wKioL1OfceqSjN-uAABhFHaXrsQ792.jpg)](https://s3.51cto.com/wyfs02/M01/2F/4F/wKioL1OfceqSjN-uAABhFHaXrsQ792.jpg)   

​     [![wKiom1OfckzSzulVAACDgD_Tk3w684.jpg](https://s3.51cto.com/wyfs02/M01/2F/4F/wKiom1OfckzSzulVAACDgD_Tk3w684.jpg)](https://s3.51cto.com/wyfs02/M01/2F/4F/wKiom1OfckzSzulVAACDgD_Tk3w684.jpg)



​     *编译源代码：nmake -f ms\ntdll.mak

​        注：编译中如出现 'ml.exe' 不是内部或外部命令(ml.exe 是微软汇编编译器)

​          请下载MASM6.15 V1.1，并设置好环境变量。下载地址：

​          http://download.csdn.net/download/davelv/366293



​     *测试：nmake -f ms\ntdll.mak test



​     *安装：nmake -f ms\ntdll.mak install