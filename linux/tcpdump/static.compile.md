# 静态交叉编译tcpdump

## 交叉编译器

```bash
export PATH=$PATH:/xxx/yyy
```

## 解压

```bash
tar xzvf libpcap-1.7.2.tar.gz
tar xzvf tcpdump-4.7.3
```

## 编译安装libcap

```bash
ac_cv_linux_vers=2.6.30.1 ./configure --prefix=/home/src/tcpdump/install --host=arm-linux --with-pcap=linux
make -j4
make install
```

## 编译安装tcpdump

```bash
ac_cv_linux_vers=2.6.30.1 ac_cv_lib_crypto_DES_cbc_encrypt=no ./configure --prefix=/home/src/tcpdump/install --disable-FEATURE --host=arm-linux

vim makefile
    CFLAGS = -DINET6 -g -O2 -static #(-static添加的内容)

make
make install
```
