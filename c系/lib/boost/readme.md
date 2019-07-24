# boost

- [官方网址](https://dl.bintray.com/boostorg/release/) —— <https://dl.bintray.com/boostorg/release/>

## 手动编译安装boost

```bash
mkdir -p ~/software && cd ~/software
wget https://dl.bintray.com/boostorg/release/1.67.0/source/boost_1_67_0.tar.gz -O  boost_1_67_0.tar.gz
tar -zxvf boost_1_67_0.tar.gz && cd boost_1_67_0 && chmod +x bootstrap.sh
./bootstrap.sh --prefix=/usr
./b2 --buildtype=complete install
```
