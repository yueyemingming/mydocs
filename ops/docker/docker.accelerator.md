# 国内的镜像加速器

docker-cn  
阿里云加速器  
中国科技大学

```bash
#加速器配置文件
vim /etc/docker/daemon.json
  {
      "registry-mirrors":["https://registry.docker-cn.com"]
  }

#重启服务
/etc/init.d/docker restart

#查看信息
docker info
  Kernel Version: 4.4.0-142-generic
  Operating System: Ubuntu 16.04.6 LTS
  OSType: linux
  Architecture: x86_64
  CPUs: 8
  Total Memory: 1.936GiB
  Name: us16046
  ID: RD4X:2DMA:IEV2:QFF2:V37E:5SDF:N6MM:KNBX:GCGG:CRU6:VFEL:ER2E
  Docker Root Dir: /var/lib/docker
  Debug Mode (client): false
  Debug Mode (server): false
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
