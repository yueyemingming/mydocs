# C++

- 学习c++极好的网站 —— <https://zh.cppreference.com/w/首页>

- [易忘重点知识点](important/readme.md)

- **STL**
  - [底层存储结构](stl/storage.struct/readme.md)
  - [string](stl/string/readme.md)
  - [vector\<char\>与字符串的相互转换](stl/vecotr.char.convert.md)
  - [map](stl/map.md)
  
- [C++11](c++11/readme.md)

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

- [Qt](qt/readme.md)

- [C++11 并发指南系列](https://www.cnblogs.com/haippy/p/3284540.html)
  
  - [多线程初探](stl/concurrency/01.md)
  - [std::thread 详解](stl/concurrency/01.md)
  - [std::mutex 详解](stl/concurrency/01.md)
  - [std::lock 详解](stl/concurrency/01.md)
  - [future.01.std::promise](stl/concurrency/01.md)
  - [future.02.std::packaged_task](stl/concurrency/01.md)
  - [future.03.std::future & std::shared_future](stl/concurrency/01.md)
  - [std::condition_variable](stl/concurrency/01.md)
  - [atomic.01.atomic_flag](stl/concurrency/01.md)
  - [atomic.02.std::atomic](stl/concurrency/01.md)
  - [atomic.03.std::atomic](stl/concurrency/01.md)
  - [atomic.04.c风格原子操作](stl/concurrency/01.md)
  - [内存模型.01.介绍](stl/concurrency/01.md)
  - [综合运行:生产者消费者模型详解](stl/concurrency/01.md)

