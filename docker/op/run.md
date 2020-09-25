# 镜像 -->  容器: docker run

```bash
# docker image ls
REPOSITORY     TAG        IMAGE ID        CREATED         SIZE
ruispace/httpd latest     b97a6b02560e    5 seconds ago   1.23MB
ruispace/httpd v0.1-1     b97a6b02560e    3 seconds ago   1.23MB   <---
busybox        latest     6858809bf669    2 weeks ago     1.23MB
```

```bash
# docker container ls -a
CONTAINER ID IMAGE    COMMAND  CREATED       STATUS       PORTS   NAMES
74da64496f33 busybox  "sh"     19 hours ago  Up 25 seconds        b1
```

```bash
docker run --name t1 -it ruispace/httpd:v0.1-1
```

```bash
#docker container ls -a
CONTAINER ID IMAGE    COMMAND  CREATED        STATUS      PORTS   NAMES
325d185dff74 ruispace/httpd:v0.1-1   "sh"  17 seconds ago  Up 16 seconds t1   <---
74da64496f33 busybox  "sh"     19 hours ago  Up 25 seconds        b1
```