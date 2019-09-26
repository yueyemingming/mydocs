# 使用kubeadm在Ubuntu16.04上搭建Kubernetes1.9集群

* **[这个办法需要首先翻墙](../../shadowsocks/)**
* **[安装docker](../../docker/install.md)**

三台Ubuntu 16.04虚拟机，各项参数如下：

节点 | IP地址
:--- | :---
master | 192.168.66.130
node1 | 192.168.66.131
node2 | 192.168.66.132

## 安装docker、kubelet、kubeadm和kubectl

分别在各台机器上以root身份进行安装

```bash
curl -s https://packages.cloud.google.com/apt/doc/apt-key.gpg | apt-key add -
cat <<EOF > /etc/apt/sources.list.d/kubernetes.list
deb http://apt.kubernetes.io/ kubernetes-xenial main
EOF
apt update
apt -y install kubelet kubeadm kubectl kubernetes-cni
```

## 初始化master

在master上以root用户运行：

```bash
kubeadm init --token=yiqian.123456

[kubeadm] WARNING: kubeadm is in alpha, please do not use it for production clusters.
[preflight] Running pre-flight checks
[init] Using Kubernetes version: v1.5.1
[tokens] Validating provided token
[tokens] Accepted provided token
[certificates] Generated Certificate Authority key and certificate.
[certificates] Generated API Server key and certificate
[certificates] Generated Service Account signing keys
[certificates] Created keys and certificates in "/etc/kubernetes/pki"
[kubeconfig] Wrote KubeConfig file to disk: "/etc/kubernetes/kubelet.conf"
[kubeconfig] Wrote KubeConfig file to disk: "/etc/kubernetes/admin.conf"
[apiclient] Created API client, waiting for the control plane to become ready
[apiclient] All control plane components are healthy after 14.534854 seconds
[apiclient] Waiting for at least one node to register and become ready
[apiclient] First node is ready after 0.506558 seconds
[apiclient] Creating a test deployment
[apiclient] Test deployment succeeded
[token-discovery] Created the kube-discovery deployment, waiting for it to become ready
[token-discovery] kube-discovery is ready after 2.005108 seconds
[addons] Created essential addon: kube-proxy
[addons] Created essential addon: kube-dns

Your Kubernetes master has initialized successfully!

You should now deploy a pod network to the cluster.
Run "kubectl apply -f [podnetwork].yaml" with one of the options listed at:
    http://kubernetes.io/docs/admin/addons/

You can now join any number of machines by running the following on each node:

kubeadm join --token=yiqian.123456 192.168.0.158
```

> 官方文档上kubeadm init的时候是不带token的，但我这样运行之后，console也没给我一个默认的token(exo me???)。但其他节点在加入集群的时候是需要指定master的token的，所以我在kubeadm初始化的时候手动给master指定了一个token。

如果初始化失败了，需要手动清理残留目录后，再重新初始化。

```bash
systemctl stop kubelet
docker rm -f $(docker ps -q)
mount | grep "/var/lib/kubelet/*" | awk '{print $3}' | xargs umount 1>/dev/null 2>/dev/null
rm -rf /var/lib/kubelet /etc/kubernetes /var/lib/etcd /etc/cni
systemctl start kubelet
kubeadm init --token=<token>
```

> 如果碰到如下错误：

    I0118 01:43:12.496376   12123 version.go:94] could not fetch a Kubernetes version from the internet: unable to get URL "https://dl.k8s.io/release/stable-1.txt": Get https://dl.k8s.io/release/stable-1.txt: net/http: request canceled while waiting for connection (Client.Timeout exceeded while awaiting headers)
    I0118 01:43:12.496463   12123 version.go:95] falling back to the local client version: v1.13.2
    [init] Using Kubernetes version: v1.13.2
    [preflight] Running pre-flight checks
        [WARNING SystemVerification]: this Docker version is not on the list of validated versions: 18.09.1. Latest validated version: 18.06
    error execution phase preflight: [preflight] Some fatal errors occurred:
        [ERROR Swap]: running with swap on is not supported. Please disable swap
    [preflight] If you know what you are doing, you can make a check non-fatal with `--ignore-preflight-errors=...`

> 进行如下步骤:
> 1. 修改配置文件
>
    vim /etc/default/kubelet

    KUBELET_EXTRA_ARGS="--fail-swap-on=false"

> 2. 加一个启动参数

    kubeadm init --ignore-preflight-errors=Swap

master初始化成功后，通过kubectl看看集群当前的状态：

```bash
kubectl get nodes

NAME             STATUS         AGE
xyq-k8s-master   Ready,master   1m
```

```text
kubectl get pods --all-namespaces

NAMESPACE     NAME                                     READY     STATUS              RESTARTS   AGE
kube-system   dummy-2088944543-gjmv4                   1/1       Running             0          1m
kube-system   etcd-xyq-k8s-master                      0/1       Pending             0          2s
kube-system   kube-apiserver-xyq-k8s-master            1/1       Running             0          20s
kube-system   kube-controller-manager-xyq-k8s-master   1/1       Running             0          22s
kube-system   kube-discovery-1769846148-9dpvt          1/1       Running             0          1m
kube-system   kube-dns-2924299975-lc4rr                0/4       ContainerCreating   0          1m
kube-system   kube-proxy-w5v9m                         1/1       Running             0          1m
kube-system   kube-scheduler-xyq-k8s-master            0/1       Pending             0          4s
```

可以看出来，除了kube-dns之外，其他pods都跑起来了。是因为在没有部署集群网络时，dns是起不来的。

## 3. 初始化dns

官方一共给出了五种网络addon，我选了Weave Net（因为它的安装步骤最方便诶嘿嘿=。=），直接运行：

```bash
kubectl apply -f https://git.io/weave-kube
```

等weave跑起来后，kube-dns自然就跑起来啦。

```bash
kubectl apply -f https://git.io/weave-kube
kubectl get pods --all-namespaces

NAMESPACE     NAME                                     READY     STATUS    RESTARTS   AGE
kube-system   dummy-2088944543-gjmv4                   1/1       Running   0          3m
kube-system   etcd-xyq-k8s-master                      1/1       Running   3          1m
kube-system   kube-apiserver-xyq-k8s-master            1/1       Running   0          2m
kube-system   kube-controller-manager-xyq-k8s-master   1/1       Running   0          2m
kube-system   kube-discovery-1769846148-9dpvt          1/1       Running   0          3m
kube-system   kube-dns-2924299975-lc4rr                4/4       Running   0          3m
kube-system   kube-proxy-w5v9m                         1/1       Running   0          3m
kube-system   kube-scheduler-xyq-k8s-master            1/1       Running   0          1m
kube-system   weave-net-8j1zg                          2/2       Running   0          1m
```

## node加入集群

root登录到各个node节点。

```bash
kubeadm join --token=yiqian.123456 192.168.0.158
```

## 查看集群状态

在master节点上执行:

```bash
kubectl get nodes

NAME             STATUS         AGE
xyq-k8s-master   Ready,master   10m
xyq-k8s-s1       Ready          5m
xyq-k8s-s2       Ready          5m
```