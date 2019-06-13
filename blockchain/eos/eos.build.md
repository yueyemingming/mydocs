# eos build

```bash
git clone https://github.com/EOSIO/eos --recursive
cd eos
./eosio_build.sh    # 输入1确认
cd build
make install
```

## 1. 安装环境

* Clang 4.0.0
* CMake 3.5.1
* Boost 1.67
* OpenSSL
* LLVM 4.0
* secp256k1-zkp (Cryptonomex branch)

### 1.1 Install the development toolkit

```bash
apt update
apt -y install vim  wget curl
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key|apt-key add -
apt -y install clang-4.0 lldb-4.0 libclang-4.0-dev cmake make \
                     libbz2-dev libssl-dev libgmp3-dev \
                     autotools-dev build-essential \
                     libbz2-dev libicu-dev python-dev \
                     autoconf libtool git mongodb \
                     python3-dev doxygen graphviz
```

### 1.2 Install Boost 1.67
```
cd ~
wget -c 'https://sourceforge.net/projects/boost/files/boost/1.67.0/boost_1_66_0.tar.bz2/download' -O boost_1.67.0.tar.bz2
tar xjf boost_1.67.0.tar.bz2
cd boost_1_66_0/
echo "export BOOST_ROOT=$HOME/boost_1_66_0" >> ~/.bash_profile
source ~/.bash_profile
./bootstrap.sh "--prefix=$BOOST_ROOT"
./b2 install
source ~/.bash_profile
```

### 1.3 Install mongo-cxx-driver (release/stable):

```
cd ~
curl -LO https://github.com/mongodb/mongo-c-driver/releases/download/1.10.0/mongo-c-driver-1.10.0.tar.gz
tar xf mongo-c-driver-1.10.0.tar.gz
cd mongo-c-driver-1.10.0
cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF -DCMAKE_INSTALL_PREFIX=/usr/local ..
make -j4 install
```

```
cd ~
git clone https://github.com/mongodb/mongo-cxx-driver.git --branch releases/stable --depth 1
cd mongo-cxx-driver/build
cmake -DBUILD_SHARED_LIBS=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local ..
make -j4 install
``` 

### 1.4 Install secp256k1-zkp (Cryptonomex branch):
```
cd ~
git clone https://github.com/cryptonomex/secp256k1-zkp.git
cd secp256k1-zkp
./autogen.sh
./configure
make
make install
```

### 1.5 build LLVM and clang and WASM
```
mkdir  ~/wasm-compiler
cd ~/wasm-compiler
git clone --depth 1 --single-branch --branch release_40 https://github.com/llvm-mirror/llvm.git
cd llvm/tools
git clone --depth 1 --single-branch --branch release_40 https://github.com/llvm-mirror/clang.git
cd ..
mkdir build
cd build
cmake -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX=.. -DLLVM_TARGETS_TO_BUILD= -DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD=WebAssembly -DCMAKE_BUILD_TYPE=Release ../
make -j4 install
```


## 2 build

```
git clone https://github.com/EOSIO/eos
dawn-v4.2.0
git submodule update --init --recursive
./eosio_build.sh

```