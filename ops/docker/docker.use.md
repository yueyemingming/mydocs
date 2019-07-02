# docker命令使用

## 2. 镜像

```bash
docker images               #查看本地所有
docker rmi [image-id]       #删除本地某个
docker rmi $(docker images -q)  #删除所有的镜像
docker image prune --force --all    #删除所有不使用的镜像
docker image prune -f -a        #删除所有不使用的镜像
docker pull [image-name]    #拉取

# 提交容器, -m 信息; -a 作者;  runoob/ubuntu:v2 提交的容器名称
docker commit -m="has update" -a="runoob" [image-id] runoob/ubuntu:v2
```

## 3. 容器

```bash
docker command --help               #某个命令的help
docker ps -a                        #查看所有运行的容器
docker rm [container]               #删除，停止容器运行后才能删除
docker top [container]              #查看容器内所有运行进程
docker port [container]             #查看端口映射
docker logs [container]             #查看容器内部的标准输出
docker logs -f [container]          #类似于tail -f 始终查看尾端
docker kill -s KILL [container]     #向容器发送一个信号
docker exec -i -t [container] /bin/bash    #交互模式运行容器终端
docker attach [container]           #附着正在运行中的容器
docker stop [container-id]          #停止
```

```bash
docker run -it --name [container]  ubuntu:latest /bin/bash     运行
    -d  让容器在后台运行。
    -P  将容器内部使用的网络端口映射到我们使用的主机上。
    -p 5000:5000  映射容器的5000到本地主机的5000端口上。
    -t  在新容器内指定一个伪终端或终端。
    -i  允许你对容器内的标准输入 (STDIN) 进行交互。
    -v [host-path]:[container-path]    将主机目录[host-path]挂载到容器的[container-path]
    -v [file]:[file]                   将主机文件[file]挂载到容器文件的[file]

docker ps -aq    #列出所有的容器
docker stop $(docker ps -aq)    #停止所有的容器
docker rm $(docker ps -aq)      #删除所有的容器
docker container prune          #删除所有停止的容器
```

## 4. 创建新容器

`vim Dockerfile`

```text
FROM    ubuntu:16.04
MAINTAINER      rui "yueyemingming@163.com"

EXPOSE  22                         #默认打开的端口
EXPOSE  80                         #默认打开的端口
CMD     /usr/sbin/sshd -D          #默认打开的服务
```

```bash
docker create --name mydocker nginx:latest      #创建但不启动
docker build -t rui/ubuntu:16.04 .              #此处镜像名称rui/ubuntu
docker build -t rui-ubuntu:16.04 .              #此处镜像名称rui-ubuntu
docker build -t myfuck:16.04 .                  #此处镜像名称myfuck
```

## 5. 命令一览

### 容器操作

* ps
* inspect
* top
* attach
* events
* logs
* wait
* export
* port

### 容器rootfs命令

* commit
* cp
* diff
