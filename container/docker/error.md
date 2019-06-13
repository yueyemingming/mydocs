# Docker删除images,重新部署镜像时,ERROR,Unable to enable SKIP DNAT rule

重新部署images时,出现服务端口被占用,类似的下列错误:

> ERROR: Failed to Setup IP tables: Unable to enable SKIP DNAT rule:  (iptables failed: iptables --wait -t nat -I DOCKER -i br-6111be397ec5 -j RETURN: iptables: No chain/target/match by that name. (exit status 1))

* 重启docker服务即解决

```bash
service docker restart
```