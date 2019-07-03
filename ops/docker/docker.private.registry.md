# 私有仓库创建及上传

## 1. 安装

- centos  `yum -y install docker-registry`
- ubuntu  `apt -y install docker-registry`
- docker仓库下载安装

    > 私有仓库服务本身也是运行在docker中

    ```bash
    docker pull registry
    docker inspect registry
    docker run -d --name docker_registry -p 5000:5000 -v /opt/registry:/var/lib/registry docker.io/registry
    docker exec -it docker_registry /bin/sh
    ```

    > 防止仓库数据丢失，加了卷标

## 2. 基本测试 —— 修改docker hub官网nginx容器，提交到本地仓库

### 2.1 从docker hub官网下载nginx，及修改

```bash
docker pull nginx
docker run -d --name nginx nginx
docker exec -it nginx /bin/sh
  echo "fuck" > a.txt
  exit
```

### 2.2 提交本地

```bash
docker commit -a "Liruigang <yueyemingming@163.com>" -p nginx liruigang/nginx:v0.1
```

### 2.3 本地hub打标签

```bash
docker tag liruigang/nginx:v0.1 127.0.0.1:5000/liruigang/nginx:v0.1
```

> 注意 docker tag 标签上必须带上服务器地址端口，否则默认上传服务器就是docker.io。

### 2.4 推送到本地hub

```bash
docker push 127.0.0.1:5000/liruigang/nginx:v0.1
```

## 下载新制作的镜像，运行

> 先把本地的镜像删除

```bash
docker stop nginx
docker rm nginx
docker rmi nginx
docker rmi liruigang/nginx:v0.1
docker rmi 127.0.0.1:5000/liruigang/nginx:v0.1
```

```bash
docker pull 127.0.0.1:5000/liruigang/nginx:v0.1
docker run -itd --name liruigang_nginx 127.0.0.1:5000/liruigang/nginx:v0.1
docker exec -it liruigang_nginx /bin/sh
cat ~/a.txt
fuck
```
