# OpenSSL之命令详解

## 1. OpenSSl命令总览

### 1.1 语法格式

`openssl command [command_opts] [command_args]`

### 1.2 常用command

命令 | 介绍
:--- | :---
version | 用于查看版本信息
enc | 用于加解密
ciphers | 列出加密套件
genrsa | 用于生成私钥
rsa | RSA密钥管理(例如:从私钥中提取公钥)
req | 生成证书签名请求(CSR)
crl | 证书吊销列表(CRL)管理
ca | CA管理(例如对证书进行签名)
dgst | 生成信息摘要
rsautl | 用于完成RSA签名、验证、加密和解密功能
passwd | 生成散列密码
rand | 生成伪随机数
speed | 用于测试加解密速度                    
s_client | 通用的SSL/TLS客户端测试工具
X509 | X.509证书管理
verify | X.509证书验证
pkcs7 | PKCS#7协议数据管理

## 2. OpenSSL命令---enc

对称加密算法工具。它能够运用块或流算法对数据进行加解密。还能够将加解密的结果进行base64编码。

```bash
openssl enc -ciphername [-in filename] [-out filename] [-pass arg] [-e] 
[-d] [-a/-base64] [-A] [-k password] [-kfile filename] [-K key] [-iv IV] [-S salt]
[-salt] [-nosalt] [-z] [-md] [-p] [-P] [-bufsize number] [-nopad] [-debug] [-none] 
[-engine id]
```

参数 | 介绍
:--- | :---
-ciphername | 对称算法名称，此命令有两种使用方式：-ciphername方式或者省略enc直接使用ciphername。
-in filename | 要加密/解密的输入文件，默认为标准输入。
-out filename | 要加密/解密的输出文件，默认为标准输出。
-pass arg | 输入文件如果有密码保护，指定密码来源。
-e | 进行加密操作，默认操作。可以省略
-d | 进行解密操作。
-a | 使用base64编码对加密结果进行处理。加密后进行base64编码，解密前进行base64解密。
-base64 | 同-a选项。
-A | 默认情况下，base64编码为一个多行的文件。使用此选项，可以让生成的结果为一行。解密时，必须使用同样的选项，否则读取数据时会出错。
-k | 指定加密口令，不设置此项时，程序会提示用户输入口令。
-kfile | 指定口令存放文件。可以从这个口令存放文件的第一行读取加密口令。
-K key | 使用一个16进制的输入口令。如果仅指定-K key而没有指定-k password，必须用-iv选项指定IV。当-K key和-k password都指定时，用-K选项给定的key将会被使用，而使用password来产生初始化向量IV。不建议两者都指定。
-iv IV | 手工指定初始化向量(IV)的值。IV值是16进制格式的。如果仅使用-K指定了key而没有使用-k指定password,那么就需要使用-iv手工指定IV值。如果使用-k指定了password，那么IV值会由这个password的值来产生。
-salt | 产生一个随机数，并与-k指定的password串联，然后计算其Hash值来防御字典攻击和rainbow table攻击。
       rainbow table攻击：用户将密码使用单向函数得到Hash摘要并存入数据库中，验证时，使用同一种单向函数对用户输入口令进行Hash得到摘要信息。将得到的摘要信息和数据中该用户的摘要信息进行比对，一致则通过。考虑到多数人使用的密码为常见的组合，攻击者可以将所有密码的常见组合进行单向Hash，得到一个摘要组合。然后与数据库中的摘要进行比对即可获得对应的密码。
      salt将随机数加入到密码中，然后对一整串进行单向Hash。攻击者就很难通过上面的方式来得到密码。
-S salt | 使用16进制的salt。
-nosalt | 表示不使用salt。
-z | 压缩数据(前提是OpenSSL编译时加入了zip库)。
-md | 指定摘要算法。如：MD5  SHA1  SHA256等。
-p | 打印出使用的salt、口令以及初始化向量IV。
-P | 打印出使用的salt、口令以及IV，不做加密和解密操作，直接退出。
-bufsize number | 设置I/O操作的缓冲区大小。因为一个加密的文件可能会很大，每次能够处理的数据是有限的。
-nopad | 没有数据填充(主要用于非对称加密操作)。
-debug | 打印调试信息。
-none | 不对数据进行加密操作。
-engine | 指定硬件引擎。

注意： 密码可以用来产生初始化密钥key和初始化向量IV。
       新版的OpenSSL必须使用-salt选项。

OpenSSL支持的加密算法：des  des3  bf  cast cast5 rc2 rc4 rc5 aes等

使用实例：
对文件进行base64编码：

```bash
which ls
cp /bin/ls .
file ls
openssl base64 -in ls -out ls.b64
file ls.b64
```

对base64编码文件进行解码：

```bash
rm -rf ls
openssl base64 -d -in ls.b64 -out ls
file ls
```

使用des3加密文件并在密码结果中加入salt:


```bash
echo "Hello World" >file.txt
cat file.txt
openssl des3 -salt -k 123456 -in file.txt -out file.des3
cat file.des3
```

使用des3解密文件，并使用-k指定密码：


```bash
openssl des3 -d -salt -k 123456 -in file.des3 -out file2.txt
cat file2.txt
```

使用bf算法加密文件，并将加密结果进行base64编码：

```bash
openssl bf -a -salt -k 123456 -in file.txt -out file.bf
file file.bf
cat file.bf
```

先用base64解码文件，再解密：


```bash
openssl bf -d -salt -a -k 123456 -in file.bf -out file3.txt
cat file3.txt
```

从密码文件中读取密码进行加解密：


```bash
echo "123456" >123.txt
cat 123.txt
openssl des3 -salt -kfile 123.txt -in file.txt -out file.des3      //加密
cat file.des3
openssl des3 -d -salt -k 123456 -in file.des3 -out file2.txt       //解密
cat file2.txt
openssl des3 -d -salt -kfile 123.txt -in file.des3 -out file3.txt  
cat file3.txt
echo "654321" >>123.txt
cat 123.txt
openssl des3 -salt -kfile 123.txt -in file.txt -out file3.des3
cat file3.des3
openssl des3 -d -salt -k 654321 -in file3.des4 -out file4.txt
   //解密失败 因为kfile只会读取密码文件的第一行作为密码进行加密
openssl des3 -d -salt -k 654321 -in file3.des4 -out file4.txt
cat file4.txt
```

加密文件并进行压缩


```bash
dd if=/dev/zero of=/tmp/123 bs=100M count=10     //生成一个100M的文件
openssl des3 -salt -k 123456 -in 123 -out 123.des3    //普通加密
openssl des3 -salt -k 123456 -z -in 123 -out 123.des3.z   //带压缩的加密
du -sh /tmp/123  /tmp/123.des3  /tmp/123.des3.z      //比较文件大小
```

打印salt、key和IV的信息：

```bash
rm -rf 123.des3
openssl des3 -salt -k 123456 -p -in 123 -out 123.des3        //打印信息 并加密
openssl des3 -salt -k 123456 -P -in 123 -out 123.des3.P     //仅打印信息 不加密
```

## 3. OpenSSL命令---ciphers

用来展示加密算法套件的工具。它能够把所有OpenSSL支持的加密算法按照一定规律排列(一般是加密强度)。

`openssl ciphers [-v] [-ssl2] [-ssl3] [-tls1] [cipherlist]`


参数 | 介绍
:--- | :---
-v | 详细列出所有加密套件。包括SSL版本(SSLv2、SSLv3以及TLS)、密钥交换算法、身份验证算法、对称算法、摘要算法以及该算法是否允许出口。
-ssl2 | 只列出sslv2使用的加密套件。
-ssl3 | 只列出sslv3使用的加密套件。
-tls1 | 只列出tls使用的加密套件。
cipherlist | 列出一个cipher list的详细内容。此项能列出所有符合规则的加密套件，如果不加-v选项，它只会显示各个套件名称。

cipherlist格式：

 openssl ciphers ‘cipherstring1:cipherstring2.....'
          1)可以显示一个或多个cipher string套件。多个cipher string直接使用分隔符分割。
          2)分隔符：通常使用冒号(:)分割，也可以使用逗号或空格来分割。
          3)每一个cipher string前面都可以加上!、-、+
            -：表示将这个cipher从List中删除。以后也可以选择再添加回来。
            +: 表示将这个cipher移动到List的底部
               注意：这个选项不是添加任何新的cipher,而只是移动匹配到的cipher。
            !：表示将这个cipher从List中永久删除。
          4)@STRENGTH 表示使用加密算法key的长度进行排序。

```bash
openssl ciphers -h          //列出ciphers参数简要帮助
openssl ciphers -v -ssl2 
openssl ciphers -v -ssl3
openssl ciphers -v -tls1
openssl ciphers 'SHA1'     //列出所有使用SHA1算法的套件
openssl ciphers 'SSLv3'    //列出SSLv3所使用的算法
openssl ciphers 'SHA1+DES'  //列出所有包含SHA1和DES的套件
openssl ciphers -v 'ALL:eNULL' //详细列出所有非加密算法
openssl ciphers -v 'ALL:!ADH:@STRENGTH' //列出所有不包含匿名DH的算法并排序
openssl ciphers -v '3DES:+RSA' //仅列出包含3DES和RSA算法的套件且将包含RSA的显示在底部
```

## 4. OpenSSL命令---genrsa

用来产生RSA私钥。

```bash
openssl genrsa [-out filename] [-passout arg] [-des] [-des3] [-idea] [-f4] [-3] 
[-rand   file(s)] [-engine id] [numbits]
```

参数 | 介绍
:--- | :---
-out fiename | 指定输出文件。如果没有设定此选项，将会输出到标准输出。
-passout arg | 指定密码来源。
-des|-des3|-idea | 用来加密私钥文件的三种对称加密算法。
-F4|-3 | 指定指数。-f4为0x1001  
-rand file(s) | 指定随机种子。
-engine id | 硬件引擎。
numbits | 生成的密钥位数。必须是本指令的最后一个参数。默认为512bits。


```bash
openssl genrsa -des3 -out prikey.pem -f4 1024
```
