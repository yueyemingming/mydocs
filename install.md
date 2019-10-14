# golang安装

```bash
wget https://studygolang.com/dl/golang/go1.11.linux-amd64.tar.gz

tar xzvf go1.11.linux-amd64.tar.gz -C /opt


vim ~/.bashrc

echo "" >> ~/.bashrc
echo "export GOPATH=/opt/gopath" >> ~/.bashrc
echo "export GOROOT=/opt/go" >> ~/.bashrc
echo "#export GOARCH=amd64" >> ~/.bashrc
echo "#export GOOS=linux" >> ~/.bashrc
echo "export GOTOOLS=$GOROOT/pkg/tool" >> ~/.bashrc
echo "export PATH=$PATH:$GOROOT/bin:$GOPATH/bin" >> ~/.bashrc

source ~/.bashrc
```
