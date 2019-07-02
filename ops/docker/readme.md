# docker

```bash
docker run -itd --name u16 ubuntu:16.04 /bin/bash
docker exec -it u16 /bin/bash
```

* [安装](docker.install.md)
* [基本使用](docker.use.md)
* [仓库操作](docker.registry.md)
  * docker 官方仓库 —— **[https://hub.docker.com/](https://hub.docker.com/)**
  * 阿里云docker 官方仓库 —— **[https://dev.aliyun.com/search.html](https://dev.aliyun.com/search.html)**
  * [命令使用](docker.registry.command.md)
  * [我的阿里云镜像加速仓库](aliyun.registry.md)
  * [私有仓库创建](private.registry.md)
  * [harbor搭建docker镜像仓库](docker.harbor.registry.md)
* [错误](error.md)
* [Dockerfile文件详解](dockerfile.md)

国内的镜像加速器
docker-cn
阿里云加速器
中国科技大学

/etc/docker/daemon.json
{
    "registry-mirrors":["https://registry.docker-cn.com"]
}

docker info #查看当前docker运行状态
docker version #查看版本信息

docker network ls

NETWORK ID          NAME                DRIVER              SCOPE
faa550c2be6d        bridge              bridge              local
12fd8ce8641d        host                host                local
3c2bbf18acb7        none                null                local

docker run --name h1 -it busybox:latest

/ # httpd -h
httpd: option requires an argument -- h
BusyBox v1.31.0 (2019-06-14 22:02:40 UTC) multi-call binary.

Usage: httpd [-ifv[v]] [-c CONFFILE] [-p [IP:]PORT] [-u USER[:GRP]] [-r REALM] [-h HOME]
or httpd -d/-e/-m STRING

Listen for incoming HTTP requests

	-i		Inetd mode
	-f		Don't daemonize
	-v[v]		Verbose
	-p [IP:]PORT	Bind to IP:PORT (default *:80)
	-u USER[:GRP]	Set uid/gid after binding to port
	-r REALM	Authentication Realm for Basic Authentication
	-h HOME		Home directory (default .)
	-c FILE		Configuration file (default {/etc,HOME}/httpd.conf)
	-m STRING	MD5 crypt STRING
	-e STRING	HTML encode STRING
	-d STRING	URL decode STRING

mkdir -p /data/html/
vi /data/html/index.html

<h1>busybox is running .</h1>

httpd -f -h /data/html/


root@us:~# docker ps
CONTAINER ID        IMAGE               COMMAND             CREATED             STATUS              PORTS               NAMES
750d9e3dbc63        busybox             "sh"                4 minutes ago       Up 4 minutes                            h1
root@us:~# docker inspect h1
[
    {
        "Id": "750d9e3dbc63e0bcfd42f6bc7006f07c754d98d663c9bbea9bcfd911eee722b8",
        "Created": "2019-07-01T12:13:27.463175801Z",
        "Path": "sh",
        "Args": [],
        "State": {
            "Status": "running",
            "Running": true,
            "Paused": false,
            "Restarting": false,
            "OOMKilled": false,
            "Dead": false,
            "Pid": 10822,
            "ExitCode": 0,
            "Error": "",
            "StartedAt": "2019-07-01T12:13:28.258182134Z",
            "FinishedAt": "0001-01-01T00:00:00Z"
        },
        "Image": "sha256:e4db68de4ff27c2adfea0c54bbb73a61a42f5b667c326de4d7d5b19ab71c6a3b",
        "ResolvConfPath": "/var/lib/docker/containers/750d9e3dbc63e0bcfd42f6bc7006f07c754d98d663c9bbea9bcfd911eee722b8/resolv.conf",
        "HostnamePath": "/var/lib/docker/containers/750d9e3dbc63e0bcfd42f6bc7006f07c754d98d663c9bbea9bcfd911eee722b8/hostname",
        "HostsPath": "/var/lib/docker/containers/750d9e3dbc63e0bcfd42f6bc7006f07c754d98d663c9bbea9bcfd911eee722b8/hosts",
        "LogPath": "/var/lib/docker/containers/750d9e3dbc63e0bcfd42f6bc7006f07c754d98d663c9bbea9bcfd911eee722b8/750d9e3dbc63e0bcfd42f6bc7006f07c754d98d663c9bbea9bcfd911eee722b8-json.log",
        "Name": "/h1",
        "RestartCount": 0,
        "Driver": "overlay2",
        "Platform": "linux",
        "MountLabel": "",
        "ProcessLabel": "",
        "AppArmorProfile": "docker-default",
        "ExecIDs": null,
        "HostConfig": {
            "Binds": null,
            "ContainerIDFile": "",
            "LogConfig": {
                "Type": "json-file",
                "Config": {}
            },
            "NetworkMode": "default",
            "PortBindings": {},
            "RestartPolicy": {
                "Name": "no",
                "MaximumRetryCount": 0
            },
            "AutoRemove": false,
            "VolumeDriver": "",
            "VolumesFrom": null,
            "CapAdd": null,
            "CapDrop": null,
            "Dns": [],
            "DnsOptions": [],
            "DnsSearch": [],
            "ExtraHosts": null,
            "GroupAdd": null,
            "IpcMode": "shareable",
            "Cgroup": "",
            "Links": null,
            "OomScoreAdj": 0,
            "PidMode": "",
            "Privileged": false,
            "PublishAllPorts": false,
            "ReadonlyRootfs": false,
            "SecurityOpt": null,
            "UTSMode": "",
            "UsernsMode": "",
            "ShmSize": 67108864,
            "Runtime": "runc",
            "ConsoleSize": [
                0,
                0
            ],
            "Isolation": "",
            "CpuShares": 0,
            "Memory": 0,
            "NanoCpus": 0,
            "CgroupParent": "",
            "BlkioWeight": 0,
            "BlkioWeightDevice": [],
            "BlkioDeviceReadBps": null,
            "BlkioDeviceWriteBps": null,
            "BlkioDeviceReadIOps": null,
            "BlkioDeviceWriteIOps": null,
            "CpuPeriod": 0,
            "CpuQuota": 0,
            "CpuRealtimePeriod": 0,
            "CpuRealtimeRuntime": 0,
            "CpusetCpus": "",
            "CpusetMems": "",
            "Devices": [],
            "DeviceCgroupRules": null,
            "DiskQuota": 0,
            "KernelMemory": 0,
            "MemoryReservation": 0,
            "MemorySwap": 0,
            "MemorySwappiness": null,
            "OomKillDisable": false,
            "PidsLimit": 0,
            "Ulimits": null,
            "CpuCount": 0,
            "CpuPercent": 0,
            "IOMaximumIOps": 0,
            "IOMaximumBandwidth": 0,
            "MaskedPaths": [
                "/proc/asound",
                "/proc/acpi",
                "/proc/kcore",
                "/proc/keys",
                "/proc/latency_stats",
                "/proc/timer_list",
                "/proc/timer_stats",
                "/proc/sched_debug",
                "/proc/scsi",
                "/sys/firmware"
            ],
            "ReadonlyPaths": [
                "/proc/bus",
                "/proc/fs",
                "/proc/irq",
                "/proc/sys",
                "/proc/sysrq-trigger"
            ]
        },
        "GraphDriver": {
            "Data": {
                "LowerDir": "/var/lib/docker/overlay2/4243de581a520d43b180d98d1606a13e620422533cb8b9ba3caa866cc04c878b-init/diff:/var/lib/docker/overlay2/eff55c5cb1f5e2671f86e0e7bd6f510f1455d5fdd65c308145b7e03a73954b4a/diff",
                "MergedDir": "/var/lib/docker/overlay2/4243de581a520d43b180d98d1606a13e620422533cb8b9ba3caa866cc04c878b/merged",
                "UpperDir": "/var/lib/docker/overlay2/4243de581a520d43b180d98d1606a13e620422533cb8b9ba3caa866cc04c878b/diff",
                "WorkDir": "/var/lib/docker/overlay2/4243de581a520d43b180d98d1606a13e620422533cb8b9ba3caa866cc04c878b/work"
            },
            "Name": "overlay2"
        },
        "Mounts": [],
        "Config": {
            "Hostname": "750d9e3dbc63",
            "Domainname": "",
            "User": "",
            "AttachStdin": true,
            "AttachStdout": true,
            "AttachStderr": true,
            "Tty": true,
            "OpenStdin": true,
            "StdinOnce": true,
            "Env": [
                "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"
            ],
            "Cmd": [
                "sh"
            ],
            "ArgsEscaped": true,
            "Image": "busybox",
            "Volumes": null,
            "WorkingDir": "",
            "Entrypoint": null,
            "OnBuild": null,
            "Labels": {}
        },
        "NetworkSettings": {
            "Bridge": "",
            "SandboxID": "99ccdc77b91f7d5df99ecfdddd3a325d29e1dee288b5c9ab3f8c7612a4524c1f",
            "HairpinMode": false,
            "LinkLocalIPv6Address": "",
            "LinkLocalIPv6PrefixLen": 0,
            "Ports": {},
            "SandboxKey": "/var/run/docker/netns/99ccdc77b91f",
            "SecondaryIPAddresses": null,
            "SecondaryIPv6Addresses": null,
            "EndpointID": "9e2f97bf05005f0e6de9ddb35dd8430f46c8288ff3b584d8322b32e333ebf806",
            "Gateway": "172.17.0.1",
            "GlobalIPv6Address": "",
            "GlobalIPv6PrefixLen": 0,
            "IPAddress": "172.17.0.2",
            "IPPrefixLen": 16,
            "IPv6Gateway": "",
            "MacAddress": "02:42:ac:11:00:02",
            "Networks": {
                "bridge": {
                    "IPAMConfig": null,
                    "Links": null,
                    "Aliases": null,
                    "NetworkID": "faa550c2be6d4f7352f0a86f11b78692c91566c4f8fea16d82f7d3dfb4bee150",
                    "EndpointID": "9e2f97bf05005f0e6de9ddb35dd8430f46c8288ff3b584d8322b32e333ebf806",
                    "Gateway": "172.17.0.1",
                    "IPAddress": "172.17.0.2",
                    "IPPrefixLen": 16,
                    "IPv6Gateway": "",
                    "GlobalIPv6Address": "",
                    "GlobalIPv6PrefixLen": 0,
                    "MacAddress": "02:42:ac:11:00:02",
                    "DriverOpts": null
                }
            }
        }
    }
]

在容器中exit，则容器h1退出。
root@us:~# docker ps -a
root@us:~# docker container ls -a
CONTAINER ID        IMAGE               COMMAND             CREATED             STATUS                     PORTS               NAMES
750d9e3dbc63        busybox             "sh"                8 minutes ago       Exited (0) 7 seconds ago                       h1

docker start -i h1   #再次启动
docker kill h1       #强制终止

docker run --name web1 -d nginx:1.14-alpine
curl 172.17.0.2

root@us:~# docker logs web1
172.17.0.1 - - [01/Jul/2019:12:27:19 +0000] "GET / HTTP/1.1" 200 612 "-" "curl/7.47.0" "-"
#日志来自于哪里没有弄明白呀。


基于容器做镜像

docker commit -h
docker commit -p h1
docker ls
docker tag 750d9e3dbc63 xxxregistry/httpd:v0.1-1
docker ls
docker tag xxxregistry/httpd:v0.1-1 xxxregistry/httpd:latest
docker tag xxxregistry/httpd:v0.1-1 xxxregistry/httpd:v0.1-2
docker image rm xxxregistiry/httpd:v0.1-2
docker commit -m="has update" -a="runoob" [image-id] runoob/ubuntu:v2

docker commit -a "yueyemingming<yueyemingming@163.com>" -c 'CMD ["/bin/httpd","-f","-h","/data/html"]' -p h1 xxxregistry/httpd:v0.2
docker run --name http2 xxxregistry/httpd:v0.2
root@us:~# docker container ls -a
CONTAINER ID        IMAGE                    COMMAND                  CREATED             STATUS                         PORTS               NAMES
4125dc74cc94        xxxregistry/httpd:v0.2   "/bin/httpd -f -h /d…"   19 seconds ago      Up 18 seconds                                      h2
e9ce0b2a26ad        nginx:1.14-alpine        "nginx -g 'daemon of…"   About an hour ago   Up About an hour               80/tcp              web1
750d9e3dbc63        busybox                  "sh"                     About an hour ago   Exited (0) About an hour ago                       http1

root@us:~# docker inspect h2
[
    ......
    {
            "EndpointID": "9b99d1975148cf6066526854879706e4efa974a40576c4bb3ab67b66178c8cde",
            "Gateway": "172.17.0.1",
            "GlobalIPv6Address": "",
            "GlobalIPv6PrefixLen": 0,
            "IPAddress": "172.17.0.3",
            "IPPrefixLen": 16,
            "IPv6Gateway": "",
            "MacAddress": "02:42:ac:11:00:03",
            "Networks": {
                "bridge": {
                    "IPAMConfig": null,
                    "Links": null,
                    "Aliases": null,
                    "NetworkID": "faa550c2be6d4f7352f0a86f11b78692c91566c4f8fea16d82f7d3dfb4bee150",
                    "EndpointID": "9b99d1975148cf6066526854879706e4efa974a40576c4bb3ab67b66178c8cde",
                    "Gateway": "172.17.0.1",
                    "IPAddress": "172.17.0.3",
                    "IPPrefixLen": 16,
                    "IPv6Gateway": "",
                    "GlobalIPv6Address": "",
                    "GlobalIPv6PrefixLen": 0,
                    "MacAddress": "02:42:ac:11:00:03",
                    "DriverOpts": null
                }
            }
        }
    }
]
root@us:~# curl 172.17.0.3
<h1>busybox is running .</h1>

推送镜像

本地的镜像标签，一定要与远程仓库的标签保持一致，否则推送不上去。

docker login -u yueyemingming
Password: 
...
Login Succeeded
docker push xxxregistry/httpd  #将当前系统中的所有标签是httpd的都推送上去。
docker push xxxregistry/httpd:v0.2  #推送0.2tag上去。

对于默认的docker.io官方仓库，在对镜像打标签时，不需要带上仓库地址，其它所有仓库打标签时，必须带上仓库地址。

