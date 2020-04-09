# 查看进程相关命令

## 1. 查看某个进程信息

```bash
top -H -p <pid>
```

## 2. 查看文件被哪个进程占用

```bash
lsof | grep test.sh

fuser filename                  
fuser -v test.sh
```

## 3. 输出重定向

```
program >/dev/null 2>/dev/null	#重定向到null
program >nohup.out 2>nohup.out	#重定向到文件
```

## 4. 通过程序名称找到对应的进程id

```bash
pidof sshd
```

## 5. 通过端口号找到进程id

```bash
fuser -n tcp 111
111/tcp 1579

lsof -i :3000
COMMAND  PID USER   FD   TYPE DEVICE SIZE/OFF NODE NAME
node    6675 root   10u  IPv6 281461      0t0  TCP *:3000 (LISTEN)
```