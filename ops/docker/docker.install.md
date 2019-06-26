# docker 安装

## 安装docker

```bash
apt update
apt -y install apt-transport-https ca-certificates curl software-properties-common
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
apt-key fingerprint 0EBFCD88
add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable"
apt update
apt -y install docker-ce
```

## 安装docker-compose

```bash
curl -L https://github.com/docker/compose/releases/download/1.22.0/docker-compose-`uname -s`-`uname -m` -o /usr/local/bin/docker-compose
chmod +x /usr/local/bin/docker-compose
docker-compose --version
```

## 参照阿里云安装

<https://help.aliyun.com/document_detail/60742.html?spm=a2c4g.11186623.6.547.71101e66DioUw>
