# 镜像打包 save, load

## 1. 打包

- 查看镜像

  ```bash
  # docker image ls
  REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
  ruispace/httpd      v0.2                da9b69f6dcbd        2 hours ago         1.23MB
  ruispace/httpd      latest              b97a6b02560e        22 hours ago        1.23MB
  ruispace/httpd      v0.1-1              b97a6b02560e        22 hours ago        1.23MB
  busybox             latest              6858809bf669        2 weeks ago         1.23MB
  ```

- 打包

  ```bash
  docker save -o myimages.gz ruispace/httpd:v0.2 ruispace/httpd:v0.1-1
  ```

## 2. 解包

- 解包

  ```bash
  docker load -i myimages.gz
  ```

- 查看镜像

  ```bash
  # docker image ls
  REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
  ruispace/httpd      v0.2                da9b69f6dcbd        2 hours ago         1.23MB
  ruispace/httpd      v0.1-1              b97a6b02560e        22 hours ago        1.23MB
  ```