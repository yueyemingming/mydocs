# OpenSSL之编译安装

```bash
apt -y install zlib-devel
tar -zxvf openssl-1.0.1h.tar.gz
./config --prefix=/opt/openssl shared zlib-dynamic enable-camellia
make
make install
/opt/openssl/bin/openssl version
export PATH="/opt/openssl/bin/:${PATH}"
echo 'export PATH="/opt/openssl/bin/:${PATH}"' >>~/.bashrc
```

