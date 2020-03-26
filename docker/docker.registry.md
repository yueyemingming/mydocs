# docker 仓库命令使用方法

## 登录 login

`docker login -u <user>`

## 拉取镜像 pull

`docker pull <registry>[:<port>]/[<namespace>/]<name>:<tag>`

```bash
#默认拉取最新latest，以下二者相同
docker pull quay.io/coreos/flannel
docker pull quay.io/coreos/flannel:latest

#具体某个版本tag
docker pull quay.io/coreos/flannel:v0.9.1
```

## 推送镜像 push

```bash
docker exec -it docker_registry /bin/sh
```

## 镜像查找 search

```bash
docker search [image-name]    #从Docker Hub查找镜像
    --automated    只列出 automated build类型的镜像
    --no-trunc     显示完整的镜像描述
    -s             列出收藏数不小于指定值的镜像
```