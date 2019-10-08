# 查看进程相关命令

```bash
top -H -p <pid>
fuser filename                  #查看文件被哪个进程占用
program >/dev/null 2>/dev/null  #输出重定向到null
program >nohup.out 2>nohup.out  #输出重定向到某个文件
```
