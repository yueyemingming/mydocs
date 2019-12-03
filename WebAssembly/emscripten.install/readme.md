# emscripten编译器安装

```bash
apt -y install python2.7 nodejs cmake default-jre git-core

git clone https://github.com/juj/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest

source ./emsdk_env.sh --build=Release
```
