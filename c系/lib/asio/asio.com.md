# 通过asio库读写串口

Asio 是一个跨平台的 C++ 库，常用于网络编程、底层的 I/O 编程等 (low-level I/O)，其结构框架如下：

## 1. 使用 Asio

### 1.1 下载

Asio 库分为 Boost 版和 non-Boost 版，后者的下载地址为： http://think-async.com/ ，下载完成后，直接解压到合适位置即可。

### 1.2 安装

```bash
./configure --prefix=/usr/local/
make -j4
make
```

## 2. 使用

### 2.1 Qt工程配置

使用 Qt 5.9.1，其 .pro 工程文件中，添加如下配置：

```pro
INCLUDEPATH += /usr/local/include
DEFINES += ASIO_STANDALONE
```

### 2.2 代码示例

下面是一个简单的串口通讯例子，主要步骤为： 创建串口 --> 配置参数 --> 读写数据 --> 开启事件循环

```c++
#include <functional>
#include "asio.hpp"
#include <QDebug>

using namespace asio;
// 存储接收到的数据
char kBuf[16];
// 声明回调函数, 打印接收的数据
void PrintBuf()
{
    qDebug() << kBuf;
}

int main()
{
    // 串口 COM1
    io_service iosev;
    serial_port port(iosev, "COM1");

    // 参数设置：波特率、流控、奇偶校验、停止位、数据位
    port.set_option(serial_port::baud_rate(115200));
    port.set_option(serial_port::flow_control(serial_port::flow_control::none));
    port.set_option(serial_port::parity(serial_port::parity::none));
    port.set_option(serial_port::stop_bits(serial_port::stop_bits::one));
    port.set_option(serial_port::character_size(8));

    // 向串口写数据
    write(port, buffer("Hello Asio", 16));

    // 从串口读数据（异步）
    port.async_read_some(buffer(kBuf),std::bind(PrintBuf));

    // 开启事件循环
    iosev.run();
}
```
