# docker

```bash
docker run -itd --name u16 ubuntu:16.04 /bin/bash
docker exec -it u16 /bin/bash
```

- [安装](docker.install.md)
- [基本使用](docker.use.md)
  - [镜像命令](docker.use.md#2-镜像)
  - [容器命令](docker.use.md#3-容器)
- [容器 --> 镜像 : docker commit](op/commit.md)
- [打标签 : docker tag](op/tag.md)
- [范例：下载镜像，修改容器，创建新镜像，推送镜像](docker.eg.md)
- [仓库]
  - **docker官方仓库** —— **[https://hub.docker.com/](https://hub.docker.com/)**
  - **阿里云docker官方仓库** —— **[https://dev.aliyun.com/search.html](https://dev.aliyun.com/search.html)**
  - [配置国内镜像加速器](docker.accelerator.md)
- [我的仓库]
  - [阿里云仓库](aliyun.registry.md)
  - [dockerhub官方仓库](docker.registry.md)
- [仓库操作](docker.registry.md)
  - [命令使用](docker.registry.md)
  - [私有仓库创建](docker.private.registry.md)
  - [harbor搭建docker镜像仓库](docker.harbor.registry.md)
- [错误](error.md)
- [Dockerfile文件详解](dockerfile.md)
