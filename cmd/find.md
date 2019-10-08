# find

## 常用

| 示例 | 介绍 |
| :-- | :-- |
find / -name "*xxx*"   | 通过文件名查找法
find . -name "xxx" -exec rm -rf {} \;  | 找到当前目录下所有的xxx文件或目录，直接删除。
find / -amin -10       | 查找最后10分钟访问的文件
find / -atime -2       | 查找最后48(2*24)小时访问的文件
find / -empty          | 查找为空的文件或者文件夹
find / -mmin -5        | 查找最后5分钟里修改过的文件
find / -mtime -1       | 查找最后24小时里修改过的文件
find / -nouser         | 查找属于作废用户的文件
find / -user fred      | 查找属于FRED这个用户的文件
find /tmp -size +10000000c -and -mtime +2  | `-and`, 查找大于100000000字节并且在48小时内修改的文件
find / -user fred -or -user george         | `-or`, 查找属于fred或者george这两个用户的文件
find /tmp ! -user panda                    | `!`, 查找所有不属于panda的文件
find / -name "httpd.conf" -ls              | 找到文件后显示文件属性。<br>`12063 34 -rw-r--r-- 1 root root 33545 Dec 30 15:36 /etc/httpd/conf/httpd.conf`

## find用法

| 选项 | 用途描述 |
| :-- | :-- |
-amin n                | 查找最后N分钟访问的文件
-atime n               | 查找最后n*24小时访问的文件
-cmin n                | 查找最后N分钟被改变状态的文件
-ctime n               | 查找最后n*24小时被改变状态的文件
-empty                 | 查找空白的文件，或空白的文件目录，或目录中没有子目录的文件夹
-false                 | 查找总是错误的文件
-fstype type           | 查找存在于指定文件系统的文件，例如：ext2
-gid n                 | 查找文件数字组 ID 为 n的文件
-group gname           | 查找文件属于gname文件组，并且指定组和ID的文件

### 控制选项

Find命令也提供给用户一些特有的选项来控制查找操作。下表就是我们总结出的最基本，最常用的find命令的控制选项及其用法。

| 选项 |用途描述 |
| :-- | :-- |
-daystart             | 测试系统从今天开始24小时以内的文件，用法类似-amin
-depth                | 使用深度级别的查找过程方式,在某层指定目录中优先查找文件内容
-follow               | 遵循通配符链接方式查找
-maxdepth levels<br>find . -maxdepth 2 -name fred    | 递归leves层目录中查找
-mount                | 不在文件系统目录中查找， 用法类似 -xdev.
-noleaf               | 禁止在非UNUX文件系统，MS-DOS系统，CD-ROM文件进行最优化查找

### 查找并显示文件的方法

| 选项 |用途描述 |
| :-- | :-- |
-exec command            | 查找并执行命令
-fprint file             | 打印文件完整文件名
-fprint0 file            | 打印文件完整文件名包括空的文件
-fprintf file format     | 打印文件格式
-ok command              | 给用户命令执行操作，根据用户的Y 确认输入执行
-printf format           | 打印文件格式
-ls                      | 打印同种文件格式的文件