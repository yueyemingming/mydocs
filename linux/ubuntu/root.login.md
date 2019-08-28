# Ubuntu 16.04 root登陆

## 设置root密码

> sudo passwd root

## root登陆图形界面

> vim /usr/share/lightdm/lightdm.conf.d/50-unity-greeter.conf

```ini
user-session=ubuntu
greeter-show-manual-login=true
all-guest=false
```

## 登陆root时tty 消息错误

> vim ~/.profile

```text
mesg n || true 替换为

tty -s && mesg n || true
```

重启ubuntu，问题解决
