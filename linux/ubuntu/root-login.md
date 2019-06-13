# Ubuntu 16.04 root登陆

## 设置root密码

```bash
sudo passwd root
```

## root登陆图形界面

```bash
vim /usr/share/lightdm/lightdm.conf.d/50-unity-greeter.conf
```

```bash
user-session=ubuntu
greeter-show-manual-login=true
all-guest=false
```

## 登陆root时tty 消息错误

```bash
vim ~/.profile
```

```text
mesg n || true 替换为

tty -s && mesg n || true

```text

重启ubuntu，问题解决