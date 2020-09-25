# 容器 --> 镜像 : docker commit

## 1. 例子1

### 1.1 运行容器

- 运行容器
  ```bash
  # docker run --name b1 -it busybox
  Unable to find image 'busybox:latest' locally
  latest: Pulling from library/busybox
  df8698476c65: Pull complete 
  Digest:   sha256:d366a4665ab44f0648d7a00ae3fae139d55e32f9712c67accd604bb55df9d05a
  Status: Downloaded newer image for busybox:latest
  ```
  
- 启动httpd服务

  ```bash
  / # mkdir -p /data/html
  / # vi /data/html/index.html
  / # cat /data/html/index.html 
  <h1>hello world</h1>
  ```

- 通过容器创建镜像

  ```bash
  # docker image ls
  REPOSITORY   TAG         IMAGE ID            CREATED             SIZE
  ubuntu       16.04       c871c45b1573        7 days ago          127MB
  busybox      latest      6858809bf669        2 weeks ago         1.23MB
  ```

  创建镜像时，不需要停止容器，-p 选项，会暂停容器。

  ```bash
  # docker commit -p b1
  sha256:b97a6b02560e25bc89ee71de1d5794e02996933ddd72a796324a4d8c5d869464
  ```

  ```bash
  # docker image ls
  REPOSITORY     TAG        IMAGE ID        CREATED         SIZE
  <none>         <none>     b97a6b02560e    3 seconds ago   1.23MB  # 新创建
  ubuntu         16.04      c871c45b1573    7 days ago      127MB
  busybox        latest     6858809bf669    2 weeks ago     1.23MB
  ```

## 2. 列子2

