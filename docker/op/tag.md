# 打标签 : docker tag

```bash
# docker image ls
REPOSITORY     TAG        IMAGE ID        CREATED         SIZE
`<none>         <none>     b97a6b02560e    3 seconds ago   1.23MB`
ubuntu         16.04      c871c45b1573    7 days ago      127MB
busybox        latest     6858809bf669    2 weeks ago     1.23MB
```

```bash
# docker tag b97a6b02560e ruispace/httpd:v0.1-1
```

```bash
# docker image ls
REPOSITORY     TAG        IMAGE ID        CREATED         SIZE
`ruispace/httpd v0.1-1     b97a6b02560e    3 seconds ago   1.23MB`
ubuntu         16.04      c871c45b1573    7 days ago      127MB
busybox        latest     6858809bf669    2 weeks ago     1.23MB
```

```bash
# docker tag ruispace/httpd:v0.1-1 ruispace/httpd:latest
```

```bash
# docker image ls
REPOSITORY     TAG        IMAGE ID        CREATED         SIZE
**ruispace/httpd latest     b97a6b02560e    5 seconds ago   1.23MB**
ruispace/httpd v0.1-1     b97a6b02560e    3 seconds ago   1.23MB
ubuntu         16.04      c871c45b1573    7 days ago      127MB
busybox        latest     6858809bf669    2 weeks ago     1.23MB
```

