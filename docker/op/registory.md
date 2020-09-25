# docker 仓库命令 login,pull,push,search

## - login 登录

```bash
docker login -u <user>
```

## - pull 拉取镜像

```bash
docker pull <registry>[:<port>]/[<namespace>/]<name>:<tag>
```

```bash
#默认拉取最新latest，以下二者相同
docker pull quay.io/coreos/flannel
docker pull quay.io/coreos/flannel:latest

#具体某个版本tag
docker pull quay.io/coreos/flannel:v0.9.1
```

## - push 推送镜像

```bash
docker push ruispace/httpd			#推送httpd所有标签镜像
docker push ruispace/httpd:v0.1-2	#推送httpd指定标签镜像
```

## - search 镜像查找

```bash
docker search [image-name]    #从Docker Hub查找镜像
    --automated    只列出 automated build类型的镜像
    --no-trunc     显示完整的镜像描述
    -s             列出收藏数不小于指定值的镜像
```