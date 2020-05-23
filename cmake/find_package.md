# CMake如何查找链接库 —— find_package的使用方法
[TOC]

如果编译软件使用了外部库，事先并不知道它的头文件和链接库的位置。得在编译命令中加上包含它们的查找路径。

CMake使用 find_package() 命令来解决这个问题。本文讨论了find_package()的工作原理。

## 1. find_package()的查找 *.cmake 的顺序

find_package(<Name>)命令首先会在模块路径中寻找 Find<name>.cmake，这是查找库的一个典型方式。具体查找路径依次为： 

### 1.1 模块模式

- ${CMAKE_MODULE_PATH}路径
- ${CMAKE_ROOT}路径   针对不同的系统，不同的编译器，这个路径都是不一样的，可以通过message进行输出查看。

### 1.2 配置模式

寻找<库名字的大写>Config.cmake 或者 <库名字的小写>-config.cmake

- ~/.cmake/packages/

- /usr/share/cmake-x.y/Modules

  > 不同的cmake版本，都有自己不同的支持的*.cmake文件。这个路径就是存放这堆 *.cmake 文件的路径。
  >
  > 通过命令 `cmake --help-module-list` 得到你的CMake支持的模块的列表，或者直接查看这个模块路径。

## 2. *.cmake中包含的变量

*.cmake文件中，包含CMakeLists.txt所需要的诸多变量。一般都会定义下面这些变量：

```js
<NAME>_FOUND
<NAME>_INCLUDE_DIRS 或 <NAME>_INCLUDES
<NAME>_LIBRARIES 或 <NAME>_LIBRARIES 或 <NAME>_LIBS
<NAME>_DEFINITIONS
```

> 大部分包的这些变量中的包名是全大写的，如 LIBFOO_FOUND ，有些包则使用包的实际大小写，如 LibFoo_FOUND

| 使用       |                                                |
| ---------- | ---------------------------------------------- |
| 加载头文件 | include_directories(<NAME>_INCLUDE_DIRS)       |
| 链接库文件 | target_link_libraries(源文件 <NAME>_LIBRARIES) |

## 3. 示例

让我们以bzip2库为例。

```cmake
cmake_minimum_required(VERSION 2.8)
project(helloworld)
add_executable(helloworld hello.c)
find_package(BZip2)
if (BZIP2_FOUND)
  include_directories(${BZIP_INCLUDE_DIRS})
  target_link_libraries (helloworld ${BZIP2_LIBRARIES})
endif (BZIP2_FOUND)
```

可以用 cmake 和 make VERBOSE=1 来验证传给编译器和链接器的flag是否正确。也可以用ldd或者dependency walker之类的工具在编译后验证 helloworld 链接的文件。