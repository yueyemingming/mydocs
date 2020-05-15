# c系

- [升级GLIBC](glibc.update.md)

- [boost](boost/readme.md)
  - [官方网址](https://dl.bintray.com/boostorg/release/) —— <https://dl.bintray.com/boostorg/release/>
  - [手动编译boost库](boost/compile.md)
  - [cmakelists.txt使用boost](boost/cmake.md)
  - [boost代码例子及编译](boost/eg.md)
  - [调用外部程序Boost.Process](boost/process.md)
  - [boost.websocket](boost/websocket/readme.md)
  
- [asio](asio/readme.md)

  Asio 是一个跨平台的 C++ 库，常用于网络编程、底层的 I/O 编程等 (low-level I/O)。

  Asio 库分为 Boost 版和 non-Boost 版。

  - **non-Boost版本官方网站** —— **[http://think-async.com/](http://think-async.com/)**

  - **安装**

    ```bash
    ./configure --prefix=/usr/local/
    make -j4
    make
    ```

  - [asio读写串口示例](asio/com.eg.md)

  - [Boost.Asio C++ 网络编程](asio/boost/network.programming.md)

  - [用asio开发过的软件——弗锐思名人堂测试软件(私有仓库需rui-space登录)](https://github.com/rui-space/myfrs.halloffame/tree/master/halloffame.test)

  - [线程池](asio/thread_pool.md)

- [libcurl](curl/readme.md)

- [protobuf](https://github.com/yueyemingming/mydocs/blob/message/protobuf/readme.md)

- [mongo-cxx-driver](mongo-cxx-driver/readme.md)

- [librdkafka](kafka/readme.md)
