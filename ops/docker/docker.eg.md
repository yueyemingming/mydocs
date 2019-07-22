# 范例：下载镜像，修改容器，创建新镜像，推送镜像

## 1. 下载镜像及运行容器

```bash
docker run --name h1 -it busybox:latest
```

## 2. 修改容器

容器中运行

```bash
mkdir -p /data/html/
vi /data/html/index.html
  busybox is running .

#启动web服务
httpd -f -h /data/html/
```

## 3. 提交容器

```bash
docker commit -a "user<user@email.com>" -c 'CMD ["/bin/httpd","-f","-h","/data/html"]' -p h1 ruitest/httpd:v0.1

docker images
  REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
  ruitest/httpd       v0.1                988a144e6316        4 seconds ago       1.22MB
  busybox             latest              e4db68de4ff2        2 weeks ago         1.22MB
```

## 4. 打latest镜像标签

```bash
docker tag ruitest/httpd:v0.1 ruitest/httpd:latest

docker images
  REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
  ruitest/httpd       latest              988a144e6316        4 seconds ago       1.22MB
  ruitest/httpd       v0.1                988a144e6316        4 seconds ago       1.22MB
  busybox             latest              e4db68de4ff2        2 weeks ago         1.22MB
```

## 5. 推送镜像

本地的镜像标签，一定要与远程仓库的标签保持一致，否则推送不上去。

- 登录

```bash
docker login -u yueyemingming
```

```text
  Password:
  ...
  Login Succeeded
```

- 将标签为ruitest/httpd:0.1的镜像推送上去

```bash
docker push ruitest/httpd:v0.1
```

```text
  The push refers to repository [docker.io/ruitest/httpd]
  3567e20cd636: Pushed
  6194458b07fc: Layer already exists
  v0.1: digest: sha256:d37bf36ed7fc3a5c0b5ac9de4e37d12c2ab4eff6ac9791374acbcd804fd492b6 size: 734
```

- 将当前系统中的所有ruitest/httpd标签镜像推送上去

```bash
docker push ruitest/httpd
```

```text
  The push refers to repository [docker.io/ruitest/httpd]
  6194458b07fc: Mounted from library/busybox
  v0.1-1: digest: sha256:bf510723d2cd2d4e3f5ce7e93bf1e52c8fd76831995ac3bd3f90ecc866643aff size: 527
```
