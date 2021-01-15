# OpenSSL之SSL协议的Web安全实现

[![img](https://ucenter.51cto.com/images/noavatar_middle.gif)](https://blog.51cto.com/shjia)

[shenjia_009](https://blog.51cto.com/shjia)0人评论[2070人阅读](javascript:;)[2014-06-15 23:44:49](javascript:;)

我们在了解完[PKI体系结构](http://shjia.blog.51cto.com/2476475/1426682)之后，下面我们着手利用OpenSSL来构建一个安全的基于SSL协议的Web通讯。



**目标**

   1，构建一个CA；

   2，Web服务器制作证书签署请求；

   3，CA完成对Web服务器证书的签名；

   4，实现客户端和服务器端的双向验证以及两者之间的加密通讯。



**SSL协议简介**

   SSL(Secure Sockets Layer 安全套接层),及其继任者传输层安全（Transport Layer Security，TLS）是为网络通信提供安全及数据完整性的一种安全协议。TLS与SSL在传输层对网络连接进行加密。

   **SSL提供的服务：** 

​      1）认证用户和服务器，确保数据发送到正确的客户机和服务器；

​      2）加密数据以防止数据中途被窃取；

​      3）维护数据的完整性，确保数据在传输过程中不被改变。

   **SSL在TCP/IP协议模型中的位置图：**

   [![wKiom1Oe95qjwrQOAADn3R7TFcw361.jpg](https://s3.51cto.com/wyfs02/M02/2F/4C/wKiom1Oe95qjwrQOAADn3R7TFcw361.jpg)](https://s3.51cto.com/wyfs02/M02/2F/4C/wKiom1Oe95qjwrQOAADn3R7TFcw361.jpg)

   图解：

​      SSL协议位于TCP/IP协议与各种应用层协议之间，为数据通讯提供安全支持。SSL协议可分为两层:SSL记录协议（SSL Record Protocol):它建立在可靠的传输协议（如TCP）之上,为高层协议提供数据封装、压缩、加密等基本功能的支持。SSL握手协议（SSL Handshake Protocol):它建立在SSL记录协议之上,用于在实际的数据传输开始前,通讯双方进行身份认证、协商加密算法、交换加密密钥等。



   **SSL可以实现众多通讯协议的安全传输。常见的协议如下：**

```php
               http:80/tcp------------->https:443/tcp
               ftp:21/tcp----------->ftps
               smtp:25/tcp---------->smtps:465/tcp
               pop3:110/tcp--------->pops:995/tcp
               ............
```

   

   **SSL的版本：**

​        SSL v2

​        SSL v3

​        TLS v1 (相当于SSL v3.1) IETF

​        WTLS   Mobile and wireless version of the TLS protocol



   **SSL通讯过程阶段一图：**

   [![wKioL1Oe-qnR-udlAACyO3uVv6A827.jpg](https://s3.51cto.com/wyfs02/M00/2F/4C/wKioL1Oe-qnR-udlAACyO3uVv6A827.jpg)](https://s3.51cto.com/wyfs02/M00/2F/4C/wKioL1Oe-qnR-udlAACyO3uVv6A827.jpg)

   阶段一图解：

​       1，Client端主动和Server端建立TCP连接；

​       2，Client向Server发送Hello信息，告知Server,我想和你建立安全连接。这是我支持的SSL版本和加密算法。

​       3，Server接收之后，会给Client一个响应。告知Client,我同意和你建立安全连接。我们就使用这个SSL版本和加密套件进行安全连接吧！

​       4，如果Client向Server提出需要验证Server端，那么Server就会将自己的经过CA签过名的数字证书文件发送给Client。Client端收到Server端发送的数字证书之后，使用CA的公钥来验证Server端的合法性。

​        注：Server Key Exchange表示Server端没有证书时，Server端就会把自己的公钥发送给Client端。但是这样没办法保证Server端的合法性。

​       5，反过来，Server端也可以验证Client端。这时候Server端会要求Client提供它指定CA颁发的证书。否则验证不通过。

​       6，至此，阶段一结束。Client和Server端已经协商出使用的SSL版本和加解密套件等信息。而且Client端已经得到Server端的公钥。



   **SSL通讯过程阶段二图：**

   [![wKiom1OfEkCgKpvEAADXQmYOLs4135.jpg](https://s3.51cto.com/wyfs02/M02/2F/4E/wKiom1OfEkCgKpvEAADXQmYOLs4135.jpg)](https://s3.51cto.com/wyfs02/M02/2F/4E/wKiom1OfEkCgKpvEAADXQmYOLs4135.jpg)

   阶段二图解：

​         1，如果Server端要求验证Client端，那么Client端此时会将自己的证书发送给Server端。

​         2，Client端会随机产生一个Session Key，然后用Server端的公钥加密这个Session Key并发送给Server端。

​         3，Client端使用自己的私钥加密一些信息完成签名，并发送给Server端。Server端使用Client端的公钥来验证签名，完成对客户端的验证。

​         4，安全通讯开始，通讯双方使用Session Key利用对称加密算法加密数据并进行传输。接收方就可以使用Session Key进行解密。

​         5，至此，SSL安全通讯完成。



 

**基于SSL的Web通讯**

```php
      1，构建CA：
                1）生成密钥对
                (umask 077;openssl genrsa -out /etc/pki/CA/private/cakey.pem 2048)
                     说明：CA私要保存路径是根据openssl的配置文件来设定的
                           openssl的配置文件：/etc/pki/tls/openssl.cnf
                2）生成自签名证书
                openssl req -new -x509 -key /etc/pki/CA/private/cakey.pem -out cacert.pem-days 3656
                     说明：-x509 签发X.509格式证书
                           -key  指定签名的私钥
                           -days 表示有效天数 3656也就是10年
                3）创建序列号文件和数据库文件
                   touch serial index.txt
                   echo 01 > serial
                      说明：这个两个文件的文件名也是在配置文件中定义的。
                            echo 01 > serial 表示给它一个初始序列号。          
                注：CA私钥和自签名证书可以同时生成：
                    openssl req -new -x509 -days 3656 -nodes -keyout /etc/pki/CA/private/cakey.pem  -out cacert.pem -subj 'CN=Test Only'
                    
                        
      2，Web服务器制作证书签署请求：
                1）生成密钥对
                   (umask 077;openssl genrsa -out /root/Server.key 2048)
                2）制作证书签署请求：
                   openssl req -new -key /root/Server.key -out /root/Serreq.csr
                   注意：Common Name必须和客户端需要访问的FQDN或IP地址对应
                   
      3，CA完成对Web服务器证书的签名：
                openssl ca -in Serreq.csr -out Server.crt -days 3656
                
      4，实现客户端和服务器端的双向验证以及两者之间的加密通讯：
                客户端验证服务器：
                           1）编辑apache主配置文件：
                              Include conf/extra/httpd-ssl.conf  #开启ssl配置文件
                           2）编辑httpd-ssl.conf文件：
                              SSLCertificateFile "/usr/local/apache/conf/Server.crt"
                              #指定证书文件
                              SSLCertificateKeyFile "/usr/local/apache/conf/Server.key" 
                           3）启动apache:
                              /usr/local/apache/bin/apachectl start
                           4）客户端测试：
                              https://192.168.1.109
                              openssl s_client -connect 192.168.1.109:443
                服务器验证客户端：
                            ......
                              
      5，小技巧：
         如果你的CA和Web服务器是同一台，你也可以这么来：
                    openssl req -new -x509 -days 3656 -sha1 -nodes -newkey rsa:2048 -keyout /etc/pki/CA/private/cakey.pem  -out cacert.pem -subj '/O=Seccure/OU=Seccure Labs/CN=ww.ssldemo.com'
                      说明： -nodes 表示不用des算法对私钥文件进行加密
                             CN:Common Name必须和客户端需要访问的FQDN或IP地址对应 
                             Web服务器可以之间使用cacert.pem这个证书文件
                             客户端也可以直接使用https://www.ssldemo.com来访问
```

 

**OpenSSL客户端工具**

```php
       1，openssl s_client -connect HOST:443
       
       2，ethereal 抓包工具
       
       3，ssldump  专门抓取和分析ssl协议包
```