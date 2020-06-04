# asio

Asio 是一个跨平台的 C++ 库，常用于网络编程、底层的 I/O 编程等 (low-level I/O)。

Asio 库分为 Boost 版和 non-Boost 版。

- **non-Boost版本官方网站** —— **[http://think-async.com/](http://think-async.com/)**

- **安装**

```bash
./configure --prefix=/usr/local/
make -j4
make
```

- [asio读写串口示例](com.eg.md)
- [Boost.Asio C++ 网络编程](boost/network.programming.md)
- [用asio开发过的软件——弗锐思名人堂测试软件(私有仓库需rui-space登录)](https://github.com/rui-space/myfrs.halloffame/tree/master/halloffame.test)
- [线程池](thread_pool.md)