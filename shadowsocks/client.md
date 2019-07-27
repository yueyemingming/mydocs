# shadowsocks 客户端端配置

## 1. 命令行启动

有两种启动方式，建议使用配置文件的方式启动

### 1.1 直接启动

```bash
sslocal -s x.x.x.x -p 8388 -k "password" -b 127.0.0.1 -l 1080
```

### 1.2 使用配置文件启动

执行 `vim /etc/sslocal.json` 添加如下内容：

```json
{
    "server":"x.x.x.x",
    "server_port":8388,
    "local_address": "127.0.0.1",
    "local_port":1080,
    "password":"password",
    "timeout":300,
    "method":"rc4-md5"
}
```

```bash
sslocal -c /etc/sslocal.json
```

## 自动启动

编辑一下 `vim /etc/rc.local` 文件：

```text
nohup sslocal -c /etc/sslocal.json >/tmp/sslocal 2>/tmp/sslocal &
```

## 2. 全局模式

系统设置 >> 网络 >> 网络代理 >> 方法 >> 手动

![001.png](001.png)

应用到整个系统即可。

## 3. 命令行翻墙

ss客户端已经安装配置完成，但命令行还不能上网，因为ss属于socks5协议，有的系统不能全局使用，所以需要把socks5转为http协议(privoxy)。

### 3.1 安装Privoxy实现Socks5转换为Http

#### 3.1.1 安装

```bash
apt -y install privoxy
```

#### 3.1.2 配置修改

`vim /etc/privoxy/config`

```bash
listen-address 127.0.0.1:8118    # 行783，去掉前面的注释符号, 此处如果是localhost，请改为127.0.0.1，因为可能最后是ipv6的地址，导致127.0.0.1不可用
forward-socks5t / 127.0.0.1:1080   # 行1336，去掉前面的注释符号，后面的1080端口要对应ss服务里面的配置，要一致
```

```bash
/etc/init.d/privoxy start
```

加入系统启动

`vim /etc/rc.local`

```bash
/etc/init.d/privoxy start
```

> 8118端口走http协议，1080走socks5协议，socks5会经过http，这样间接的ss也能支持http协议了.

### 3.2 让终端走代理

```bash
echo "" >> ~/.bashrc
echo "export https_proxy=http://127.0.0.1:8118" >>  ~/.bashrc
echo "export http_proxy=http://127.0.0.1:8118" >>  ~/.bashrc
echo "export ftp_proxy=http://127.0.0.1:8118" >>  ~/.bashrc
source ~/.bashrc
```

### 3.3 结果验证

curl www.google.com
