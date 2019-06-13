# ubuntu protobuf 安装

## 1. 安装

```bash
apt -y install dos2unix

tar xzvf protobuf-3.5.0.tar.gz

cd protobuf-3.5.0/
dos2unix autogen.sh
./autogen.sh
./configure --prefix=/usr/local/protobuf
make -j4
make check  # 时间很长
make install

echo "" >> /etc/profile
echo "export PATH=$PATH:/usr/local/protobuf/bin/" >> /etc/profile
echo "export PKG_CONFIG_PATH=/usr/local/protobuf/lib/pkgconfig/" >> /etc/profile
source /etc/profile
```

## 2. 查看

```bash
protoc --version
```

```text
libprotoc 3.5.0
```

## 3. 配置动态链接库路径

```bash
echo "/usr/local/protobuf/lib" >> /etc/ld.so.conf.d/libprotobuf.conf
ldconfig
```
