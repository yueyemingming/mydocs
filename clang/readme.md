# clang

## ubuntu安装clang

```bash
apt update
apt -y install software-properties-common
apt-add-repository -y "deb http://apt.llvm.org/trusty/ llvm-toolchain-trusty-4.0 main"
apt update
apt -y install clang-4.0 lldb-4.0 libclang-4.0-dev
```

- [结构化编译器前端 Clang 介绍](clang.introduce.md)
