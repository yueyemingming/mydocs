# docker

```bash
docker run -itd --name u16 ubuntu:16.04 /bin/bash
docker exec -it u16 /bin/bash
```

---

- [安装](docker.install.md)

- [基本使用](docker.use.md)
  - [镜像命令](docker.use.md#2-镜像)
  - [容器命令](docker.use.md#3-容器)
  
---

- [容器 --> 镜像 : commit](op/commit.md)
- [镜像 -->  容器: run](op/run.md)
- [打标签 : tag](op/tag.md)
- [仓库 : login,pull,push,search](op/registory.md)
- [镜像导入导出 : save, load](op/save.load.md)

---

- [范例：下载镜像，修改容器，创建新镜像，推送镜像](docker.eg.md)
- 仓库
  - **docker官方仓库** —— **[https://hub.docker.com/](https://hub.docker.com/)**
  - **阿里云docker官方仓库** —— **[https://dev.aliyun.com/search.html](https://dev.aliyun.com/search.html)**
  - [配置国内镜像加速器](docker.accelerator.md)
- 我的仓库
  - [阿里云仓库](aliyun.registry.md)
  - [docker hub官方仓库](docker.registry.md)
- 私有仓库操作
  - [私有仓库创建](docker.private.registry.md)
  - [harbor搭建docker镜像仓库](docker.harbor.registry.md)
- [错误](error.md)
- [Dockerfile文件详解](dockerfile.md)
  - [RUN与CMD的区别](dockerfile.cmd.run.md)
  - [CMD与ENTRYPOINT的区别](dockerfile.cmd.entrypoint.md)
