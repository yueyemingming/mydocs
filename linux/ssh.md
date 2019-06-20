# openssh

```bash
#安装
apt -y install openssh-server

vim /etc/ssh/sshd_config
-----------------------
PermitRootLogin yes         #允许root登录
PasswordAuthentication yes  #运行通过密码登录
-----------------------

# 启动服务
/etc/init.d/ssh start
/etc/init.d/ssh restart
```

## ssh无密登陆

```bash
#生成公私钥，-P '' 就表示空密码
ssh-keygen -t rsa -P ''

#私钥为：/home/user/.ssh/id_rsa
#公钥为：/home/user/.ssh/id_rsa.pub
```

把公钥复制到服务器端的 `~/.ssh/authorized_keys` 文件中。
