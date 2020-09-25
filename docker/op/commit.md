# 容器 --> 镜像 : docker commit

## 1. 普通制作镜像

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
  
- 编辑httpd网页文件

  ```bash
  / # mkdir -p /data/html
  / # vi /data/html/index.html
  / # cat /data/html/index.html 
  <h1>hello world</h1>
  ```

- 查看镜像列表

  ```bash
  # docker image ls
  REPOSITORY   TAG         IMAGE ID            CREATED       SIZE
  busybox      latest      6858809bf669        2 weeks ago   1.23MB
  ```

- **通过容器创建镜像，创建镜像时，不需要停止容器，-p 选项，会暂停容器。**

  ```bash
  # docker commit -p b1
  sha256:b97a6b02560e25bc89ee71de1d5794e02996933ddd72a796324a4d8c5d869464
  ```

- 查看镜像列表

  ```bash
  # docker image ls
  REPOSITORY     TAG        IMAGE ID        CREATED         SIZE
  <none>         <none>     b97a6b02560e    3 seconds ago   1.23MB   <---
  busybox        latest     6858809bf669    2 weeks ago     1.23MB
  ```

## 2. 修改默认的命令制作的镜像

- 查看镜像列表

  ```bash
  # docker image ls
  REPOSITORY     TAG        IMAGE ID            CREATED         SIZE
  ruispace/httpd latest     b97a6b02560e        5 seconds ago   1.23MB   <---
  ruispace/httpd v0.1-1     b97a6b02560e        3 seconds ago   1.23MB
  busybox        latest     6858809bf669        2 weeks ago     1.23MB
  ```

- 查看容器

  ```bash
  # docker container ls -a
  CONTAINER ID  IMAGE                 COMMAND  NAMES
  325d185dff74  ruispace/httpd:v0.1-1 "sh"     t1   <---
  74da64496f33  busybox               "sh"     b1
  ```

- **可以看到容器中启动的都是 "sh"。 现在修改掉这个"sh"，改为启动httpd服务。制作镜像。**

  ```bash
  docker commit -a "user<user@email.com>" -c 'CMD ["/bin/httpd","-f","-h","/data/html"]' -p t1 ruispace/httpd:v0.2
  ```

- 查看镜像列表
  
  ```bash
  # docker image ls
  REPOSITORY          TAG            IMAGE ID            SIZE
  ruispace/httpd      v0.2           da9b69f6dcbd        1.23MB   <---
  ruispace/httpd      latest         b97a6b02560e        1.23MB
  ruispace/httpd      v0.1-1         b97a6b02560e        1.23MB
  busybox             latest         6858809bf669        1.23MB
  ```

- **后台启动容器**

  ```bash
  docker run --name t2 -itd ruispace/httpd:v0.2
  ```
  
- 查看容器列表

  ```bash
  # docker container ls -a
  CONTAINER ID  IMAGE                 COMMAND                 NAMES
  7373696fbde1  ruispace/httpd:v0.2   "/bin/httpd -f -h /d…"  t2   <---
  325d185dff74  ruispace/httpd:v0.1-1 "sh"                    t1
  74da64496f33  busybox               "sh"                    b1
  ```



- 查看容器ip地址

  ```bash
  # docker inspect t2
  
  ...
  "Networks": {
                  "bridge": {
                      "IPAMConfig": null,
                      "Links": null,
                      "Aliases": null,
                      "NetworkID": "93568b54bd94c3dbfadc038edb798dae896e5c7de8430437bd1e0b5895c51652",
                      "EndpointID": "8c5d6589a0dc03cc6a3433b748d4b92cda5c390c5e1068eeefee159996255a8c",
                      "Gateway": "172.17.0.1",
                      "IPAddress": "172.17.0.5",                      <---
                      "IPPrefixLen": 16,
                      "IPv6Gateway": "",
                      "GlobalIPv6Address": "",
                      "GlobalIPv6PrefixLen": 0,
                      "MacAddress": "02:42:ac:11:00:05",
                      "DriverOpts": null
                  }
              }
          }
      }
  ]
  ```
- 访问网站

  ```bash
  # curl 172.17.0.5
  <h1>hello world</h1>
  ```