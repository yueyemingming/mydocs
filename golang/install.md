# golang安装

```bash
wget https://studygolang.com/dl/golang/go1.11.linux-amd64.tar.gz

tar xzvf go1.11.linux-amd64.tar.gz -C /opt


vim ~/.bashrc

export GOPATH=/opt/gopath
export GOROOT=/opt/go
#export GOARCH=amd64
#export GOOS=linux
export GOTOOLS=$GOROOT/pkg/tool
export PATH=$PATH:$GOROOT/bin:$GOPATH/bin

source ~/.bashrc
```