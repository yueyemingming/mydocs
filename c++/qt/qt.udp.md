# 浅谈 Qt 中 UDP 之 UdpSocket通信实例

UDP（User Datagram Protocol即用户数据报协议）是一个轻量级的，不可靠的，面向数据报的无连接协议。我们现在几乎每个人都使用的QQ，其聊天时就是使用UDP协议进行消息发送的。就像QQ那样，当有很多用户，发送的大部分都是短消息，要求能及时响应，并且对安全性要求不是很高的情况下使用UDP协议。在选择使用协议的时候，选择UDP必须要谨慎。在网络质量令人十分不满意的环境下，UDP协议数据包丢失会比较严重。但是由于UDP的特性：它不属于连接型协议，因而具有资源消耗小，处理速度快的优点，所以通常音频、视频和普通数据在传送时使用UDP较多，因为它们即使偶尔丢失一两个数据包，也不会对接收结果产生太大影响。所以qq这种对保密要求并不太高的聊天程序就是使用的UDP协议。

在Qt中提供了QUdpSocket 类来进行UDP数据报（datagrams）的发送和接收。这里我们还要了解一个名词Socket，也就是常说的“套接字”。Socket简单地说，就是一个IP地址加一个port端口  。因为我们要传输数据，就要知道往哪个机子上传送，而IP地址确定了一台主机，但是这台机子上可能运行着各种各样的网络程序，我们要往哪个程序中发送呢？这时就要使用一个端口来指定UDP程序。所以说，Socket指明了数据报传输的路径。这些和Java中的情况基本一致。不再继续介绍了。

## 发送端

```cpp
#include <QByteArray>
#include <QCoreApplication>
#include <QHostAddress>
#include <QUdpSocket>

const quint16 PORT = 2333;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QUdpSocket qus;
//  qus.bind(QHostAddress("127.0.0.1"), PORT+1);
    QByteArray msg = "Hello world!";
    std::cout << "--- Sender ---" << std::endl;
    for(int i=0; i<100; ++i)
        qus.writeDatagram(msg, QHostAddress("127.0.0.1"), PORT);

    return a.exec();
}
```

## 接收端

```cpp
#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

#include <QObject>
#include <QUdpSocket>

class UdpReceiver : public QObject
{
    Q_OBJECT

public:
    UdpReceiver(QObject *p = 0);
    ~UdpReceiver();

public slots:
    void receive();

private:
    QUdpSocket *uSocket;
};

#endif // UDPRECEIVER_H
```

UdpReceiver.cpp

```cpp
#include <QByteArray>
#include <iostream>
#include "UdpReceiver.h"

const quint16 PORT = 2333;

UdpReceiver::UdpReceiver(QObject *p) :
    QObject(p)
{
    uSocket = new QUdpSocket;
    uSocket->bind(QHostAddress("127.0.0.1"), PORT);
    connect(uSocket, SIGNAL(readyRead()), this, SLOT(receive()));
}

UdpReceiver::~UdpReceiver()
{
    delete uSocket;
}

void UdpReceiver::receive()
{
    QByteArray ba;
    while(uSocket->hasPendingDatagrams())
    {
        ba.resize(uSocket->pendingDatagramSize());
        uSocket->readDatagram(ba.data(), ba.size());
        std::cout << ba.data() << std::endl;
    }
}
```

mian.cpp

```cpp
#include <QCoreApplication>
#include "udpreceiver.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    UdpReceiver ur;
    std::cout << "--- Recevier ---" << std::endl;
    return a.exec();
}
```
