# docker命令使用

## 1. 通用命令

```bash
docker info     #查看当前docker运行状态
docker version  #查看版本信息

docker network ls   #查看网络
  NETWORK ID          NAME                DRIVER              SCOPE
  faa550c2be6d        bridge              bridge              local
  12fd8ce8641d        host                host                local
  3c2bbf18acb7        none                null                local
```

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

### 镜像使用大例子 -- web服务 -- 基于容器做镜像

```bash
docker commit -h
docker commit -p h1
docker ls
docker tag 750d9e3dbc63 xxxregistry/httpd:v0.1-1
docker ls
docker tag xxxregistry/httpd:v0.1-1 xxxregistry/httpd:latest
docker tag xxxregistry/httpd:v0.1-1 xxxregistry/httpd:v0.1-2
docker image rm xxxregistiry/httpd:v0.1-2
docker commit -m="has update" -a="runoob" [image-id] runoob/ubuntu:v2

docker commit -a "user<user@email.com>" -c 'CMD ["/bin/httpd","-f","-h","/data/html"]' -p h1 xxxregistry/httpd:v0.2
docker run --name http2 xxxregistry/httpd:v0.2
root@us:~# docker container ls -a
CONTAINER ID        IMAGE                    COMMAND                  CREATED             STATUS                         PORTS               NAMES
4125dc74cc94        xxxregistry/httpd:v0.2   "/bin/httpd -f -h /d…"   19 seconds ago      Up 18 seconds                                      h2
e9ce0b2a26ad        nginx:1.14-alpine        "nginx -g 'daemon of…"   About an hour ago   Up About an hour               80/tcp              web1
750d9e3dbc63        busybox                  "sh"                     About an hour ago   Exited (0) About an hour ago                       http1

root@us:~# docker inspect h2
[
    ......
    {
            "EndpointID": "9b99d1975148cf6066526854879706e4efa974a40576c4bb3ab67b66178c8cde",
            "Gateway": "172.17.0.1",
            "GlobalIPv6Address": "",
            "GlobalIPv6PrefixLen": 0,
            "IPAddress": "172.17.0.3",
            "IPPrefixLen": 16,
            "IPv6Gateway": "",
            "MacAddress": "02:42:ac:11:00:03",
            "Networks": {
                "bridge": {
                    "IPAMConfig": null,
                    "Links": null,
                    "Aliases": null,
                    "NetworkID": "faa550c2be6d4f7352f0a86f11b78692c91566c4f8fea16d82f7d3dfb4bee150",
                    "EndpointID": "9b99d1975148cf6066526854879706e4efa974a40576c4bb3ab67b66178c8cde",
                    "Gateway": "172.17.0.1",
                    "IPAddress": "172.17.0.3",
                    "IPPrefixLen": 16,
                    "IPv6Gateway": "",
                    "GlobalIPv6Address": "",
                    "GlobalIPv6PrefixLen": 0,
                    "MacAddress": "02:42:ac:11:00:03",
                    "DriverOpts": null
                }
            }
        }
    }
]
root@us:~# curl 172.17.0.3
<h1>busybox is running .</h1>

推送镜像

本地的镜像标签，一定要与远程仓库的标签保持一致，否则推送不上去。

docker login -u yueyemingming
  Password: 
  ...
  Login Succeeded

docker push xxxregistry/httpd  #将当前系统中的所有标签是httpd的都推送上去。
docker push xxxregistry/httpd:v0.2  #推送0.2tag上去。

对于默认的docker.io官方仓库，在对镜像打标签时，不需要带上仓库地址，其它所有仓库打标签时，必须带上仓库地址。
```

## 3. 容器

```bash
docker command --help               #某个命令的help
docker ps -a                        #查看所有运行的容器
docker container ls -a              #查看所有运行的容器
docker rm [container]               #删除，停止容器运行后才能删除
docker top [container]              #查看容器内所有运行进程
docker port [container]             #查看端口映射
docker logs [container]             #查看容器内部的标准输出
docker logs -f [container]          #类似于tail -f 始终查看尾端
docker kill -s KILL [container]     #向容器发送一个信号
docker exec -i -t [container] /bin/bash    #交互模式运行容器终端
docker attach [container]           #附着正在运行中的容器
docker stop [container]             #停止
docker inspect [container]          #查看容器状态
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

docker stop $(docker ps -aq)    #停止所有的容器
docker rm $(docker ps -aq)      #删除所有的容器
docker container prune          #删除所有停止的容器
```

### 容器使用大例子 -- web服务

```bash
docker run --name h1 -it busybox:latest   #运行busybox容器。
```

容器中运行

```bash
mkdir -p /data/html/
vi /data/html/index.html
  <h1>busybox is running .</h1>

#启动web服务
httpd -f -h /data/html/
```

```bash
docker ps
  CONTAINER ID    IMAGE       COMMAND     CREATED         STATUS          PORTS   NAMES
  750d9e3dbc63    busybox     "sh"        4 minutes ago   Up 4 minutes            h1

docker inspect h1
[
    {
        "Id": "750d9e3dbc63e0bcfd42f6bc7006f07c754d98d663c9bbea9bcfd911eee722b8",
        "Created": "2019-07-01T12:13:27.463175801Z",
        "Path": "sh",
        "Args": [],
        ... ...
            "MacAddress": "02:42:ac:11:00:02",
            "Networks": {
                "bridge": {
                    "IPAMConfig": null,
                    "Links": null,
                    "Aliases": null,
                    "NetworkID": "faa550c2be6d4f7352f0a86f11b78692c91566c4f8fea16d82f7d3dfb4bee150",
                    "EndpointID": "9e2f97bf05005f0e6de9ddb35dd8430f46c8288ff3b584d8322b32e333ebf806",
                    "Gateway": "172.17.0.1",
                    "IPAddress": "172.17.0.2",  #此处看到容器中的ip
                    "IPPrefixLen": 16,
                    "IPv6Gateway": "",
                    "GlobalIPv6Address": "",
                    "GlobalIPv6PrefixLen": 0,
                    "MacAddress": "02:42:ac:11:00:02",
                    "DriverOpts": null
                }
            }
        }
    }
]
```

在容器中`exit`，则容器h1退出。

```bash
docker container ls -a  #查看所有容器状态
  CONTAINER ID    IMAGE       COMMAND     CREATED         STATUS                     PORTS      NAMES
  750d9e3dbc63    busybox     "sh"        8 minutes ago   Exited (0) 7 seconds ago              h1

docker start -i h1   #再次启动
docker kill h1       #强制终止
docker start -i h1   #再次启动

curl 172.17.0.2
  <h1>busybox is running .</h1>

docker logs h1      #日志来自于哪里没有弄明白呀???
  172.17.0.1 - - [01/Jul/2019:12:27:19 +0000] "GET / HTTP/1.1" 200 612 "-" "curl/7.47.0" "-"
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

### 容器rootfs命令

- commit
- cp
- diff
