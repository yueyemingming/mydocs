# openssh

## 1. 安装服务

```bash
apt install openssh-server
```

## 2. 启动服务

```bash
/etc/init.d/ssh start
```

## 3. root允许登陆

```bash
vim /etc/ssh/sshd_config
```

```text
#PermitRootLogin prohibit-password
PermitRootLogin yes
```

```bash
/etc/init.d/ssh restart
```

## 4. 客户端ssh无密登陆

### step1 生成公私钥

```bash
ssh-keygen -t rsa -P ''
```

* -P '' 就表示空密码

它在/home/root下生成.ssh目录，.ssh下有id_rsa和id_rsa.pub。

### step2 公私钥拷贝到服务器端

把client端的 ~/.ssh/id_rsa.pub 内容复制到server端的~/.ssh/authorized_keys文件中。

```bash
vim ~/.ssh/authorized_keys
```

## 5. 允许密码登录

```text
PasswordAuthentication yes
```
