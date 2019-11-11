# mongo driver安装

## mongo c++ driver 编译安装

```bash
#curl -L https://gitlab.cindywj.cn/liruigang/eosio.depends/raw/master/mongo-cxx-driver-r3.4.0.tar.gz -o mongo-cxx-driver-r3.4.0.tar.gz
tar -xzf mongo-cxx-driver-r3.4.0.tar.gz
cd mongo-cxx-driver-r3.4.0 && mkdir -p build && cd build
cmake -DBUILD_SHARED_LIBS=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local/ ..
make -j8
make install
```

## mongo c driver 编译安装

```bash
curl -LO https://gitlab.cindywj.cn/liruigang/eosio.depends/raw/master/mongo-c-driver-1.13.0.tar.gz
tar -xzf mongo-c-driver-1.13.0.tar.gz
cd mongo-c-driver-1.13.0 && mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_BSON=ON -DENABLE_SSL=OPENSSL -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF -DENABLE_STATIC=ON ..
make -j8
make install
```
