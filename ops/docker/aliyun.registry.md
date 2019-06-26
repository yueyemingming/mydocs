# docker 阿里云镜像加速

## docker 下载镜像

```bash
docker pull registry.cn-beijing.aliyuncs.com/mybts_coin/core:v1.0
```

## docker 推送镜像

```bash
docker login -u yueyemingming@163.com registry-vpc.cn-beijing.aliyuncs.com
docker tag [ImageId] registry.cn-beijing.aliyuncs.com/mybts_coin/core:[镜像版本号]
docker push registry.cn-beijing.aliyuncs.com/mybts_coin/core:[镜像版本号]
```

> 除了docker.io，其他的所有仓库服务器都必须是这种方式，即服务器+仓库+版本号，否则push的是默认的是docker.io。
