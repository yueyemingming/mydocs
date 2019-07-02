# 国内的镜像加速器

docker-cn  
阿里云加速器  
中国科技大学

1. 加速器配置文件    /etc/docker/daemon.json

    ```json
    {
        "registry-mirrors":["https://registry.docker-cn.com"]
    }
    ```

2. 重启服务

    /etc/init.d/docker restart

3. 查看信息

    ```bash
    docker info
    ... ...
    Registry: https://index.docker.io/v1/
    Labels:
    Experimental: false
    Insecure Registries:
    127.0.0.0/8
    Registry Mirrors:
    https://registry.docker-cn.com/      #这里标识加速器已经配置成功
    Live Restore Enabled: false
    Product License: Community Engine
    ```
