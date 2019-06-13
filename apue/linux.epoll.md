# linux下epoll编程

## 1. 定义

epoll是Linux内核为处理大批句柄而作改进的poll，是Linux下多路复用IO接口select/poll的增强版本。

它能显著的减少程序在大量并发连接中只有少量活跃的情况下的系统CPU利用率。因为它会复用文件描述符集合来传递结果，而不是迫使开发者每次等待事件之前都必须重新准备要被侦听的文件描述符集合；另一个原因就是获取事件的时候，**它无须遍历整个被侦听的描述符集，只要遍历那些被内核IO事件异步唤醒而加入Ready队列的描述符集合**就行了。

epoll除了提供select\poll那种IO事件的电平触发(Level Triggered)外，还提供了边沿触发(Edge Triggered)，这就使得用户空间程序有可能缓存IO状态，减少epoll_wait/epoll_pwait的调用，提供应用程序的效率。

## 2. 工作方式

### 水平触发 LT(level triggered)

缺省方式，同时支持block和no-block socket。

在这种模式中，内核告诉我们一个文件描述符是否被就绪了。如果就绪了，你就可以对这个就绪的fd进行IO操作。如果你不作任何操作，内核还是会继续通知你的。

所以，这种模式编程出错的可能性较小。传统的select\poll都是这种模型的代表。

### 边沿触发 ET(edge-triggered)

高速工作方式，只支持no-block socket。

在这种模式下，当描述符从未就绪变为就绪状态时，内核通过epoll告诉你。然后它会假设你知道文件描述符已经就绪，不会再为那个描述符发送更多的就绪通知，直到你做了某些操作导致那个文件描述符不再为就绪状态了(比如：你在发送、接受或者接受请求，或者发送接受的数据少于一定量时导致了一个EWOULDBLOCK错误)。

但是请注意，如果一直不对这个fs做IO操作(从而导致它再次变成未就绪状态)，内核不会发送更多的通知。

### 区别

* **LT模式**事件不会丢弃，而是只要读buffer里面有数据可以让用户读取，则不断的通知你。
* **ET模式**只在事件发生之时通知。

## 3. 使用方式

### 3.1 epoll句柄创建

* **int epoll_create(int size);**

创建一个epoll句柄，参数size用来告诉内核监听的数目。

### 3.2 epoll事件注册

* **int epoll_ctl(int epfd, int op, int fd, struct epoll_event \*event);**

注册一个epoll事件函数。

* **epfd**为epoll的句柄；
* **op**表示动作，用3个宏来表示：

    op | desc
    :--- | :---
    EPOLL_CTL_ADD | 注册新的fd到epfd
    EPOLL_CTL_MOD | 修改已经注册的fd的监听事件
    EPOLL_CTL_DEL | 从epfd删除一个fd

* **fd**为需要监听的标示符；
* **event**告诉内核需要监听的事件，event的结构如下：

```c
struct epoll_event {
  __uint32_t events;  /* Epoll events */
  epoll_data_t data;  /* User data variable */
};
```

events可以是以下几个宏的集合:

event | desc
:--- | :---
EPOLLIN | 表示对应的文件描述符可以读（包括对端SOCKET正常关闭）
EPOLLOUT | 表示对应的文件描述符可以写
EPOLLPRI | 表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）
EPOLLERR | 表示对应的文件描述符发生错误
EPOLLHUP | 表示对应的文件描述符被挂断
EPOLLET | 将EPOLL设为边缘触发(Edge Triggered)模式, 这是相对于水平触发(Level Triggered)来说的.
EPOLLONESHOT | 只监听一次事件, 当监听完这次事件之后, 如果还需要继续监听这个socket的话, 需要再次把这个socket加入到EPOLL队列里.

### 3.3 等待事件的产生

* **int epoll_wait(int epfd, struct epoll_event \* events, int maxevents, int timeout);**

等待事件的产生，类似于select()调用。

* **events**用来从内核得到事件的集合。
* **maxevents**告之产生的内核events大小。这个maxevents的值不能大于创建epoll_create()时的size。
* **timeout**是超时时间（毫秒，0会立即返回，-1将不确定，也有说法说是永久阻塞）。

该函数返回需要处理的事件数目，如返回0表示已超时。

## 4. 应用举例

### 4.1 服务器

```cpp
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <iostream>

#define BUF_SIZE 1024               //默认缓冲区
#define SERVER_PORT 8888            //监听端口
#define SERVER_HOST "127.0.0.1"     //服务器IP地址

#define CHK(fun) \
    if(fun < 0) {\
    perror("fun");\
    exit(-1);\
    }
#define CHK2(res, fun) \
    if((res = fun) < 0){\
    perror("fun"); \
    exit(-1);\
    }

using namespace std;

int main(void)
{
    struct sockaddr_in addr;
    addr.sin_family = PF_INET;
    addr.sin_port = htons(SERVER_PORT);
    addr.sin_addr.s_addr = inet_addr(SERVER_HOST);

    int listen_fd;
    CHK2(listen_fd, socket(PF_INET, SOCK_STREAM, 0));    //初始化监听socket
    CHK(fcntl(listen_fd, F_SETFL, fcntl(listen_fd, F_GETFD, 0)|O_NONBLOCK));//设置监听socket为不阻塞
    CHK(bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr)));    //绑定监听socket
    CHK(listen(listen_fd, 1));   //设置监听

    int epoll_fd ;
    CHK2(epoll_fd,epoll_create(1000));    //创建一个epoll描述符，并将监听socket加入epoll
    static struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = listen_fd;
    CHK(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev));

    while(true) {
        int epoll_events_count=0;
        struct epoll_event events[EPOLL_SIZE] = { 0x00 } ;
        CHK2(epoll_events_count,epoll_wait(epoll_fd, events, EPOLL_SIZE, -1));

        cout << "产生了" << epoll_events_count << "个新事件 ." << endl ;

        for(int i = 0; i < epoll_events_count ; i++){

            cout << "处理文件描述符 : " << events[i].data.fd << endl ;

            //新的连接到来，将连接添加到epoll中，并发送欢迎消息
            if(events[i].data.fd == listen_fd) {
                struct sockaddr_in client_addr;
                socklen_t socklen = sizeof(struct sockaddr_in);
                int client;
                CHK2(client,accept(listen_fd, (struct sockaddr *) &client_addr, &socklen));
                cout << "发现新的客户端 : " << client << endl << endl ;

                //设置监听socket为不阻塞
                CHK(fcntl(client, F_SETFL, fcntl(listen_fd, F_GETFD, 0)|O_NONBLOCK));

                // 加入新的等待队列
                ev.data.fd = client;
                CHK(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client, &ev));
                continue;
            }

            cout << "怎么没等待？" << endl ;
            //接受客户端信息
            char buf[BUF_SIZE] = { 0x00 };
            int len;

            CHK2(len,recv(events[i].data.fd, buf, BUF_SIZE, 0));
            cout << buf << endl << endl;
        }
    }

    close(listen_fd);
    close(epoll_fd);

    return 0;
}
```

### 4.2 客户端

模拟单个发送消息。

```cpp
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <iostream>

#define BUF_SIZE 1024               //默认缓冲区
#define SERVER_PORT 8888            //监听端口
#define SERVER_HOST "127.0.0.1"     //服务器IP地址

#define CHK(eval) if(eval < 0){perror("eval"); exit(-1);}
#define CHK2(res, eval) if((res = eval) < 0){perror("eval"); exit(-1);}

using namespace std;

int main(void)
{
    // 创建套接口
    struct sockaddr_in addr;
    addr.sin_family = PF_INET;
    addr.sin_port = htons(SERVER_PORT);
    addr.sin_addr.s_addr = inet_addr(SERVER_HOST);
    int sock;
    CHK2(sock,socket(PF_INET, SOCK_STREAM, 0));
    CHK(connect(sock, (struct sockaddr *)&addr, sizeof(addr)));

    while(true){
        // 循环输入消息，发送的服务
        char    buf[BUF_SIZE] = { 0x00 } ;
        cin >> buf ;
        CHK(send(sock, buf, BUF_SIZE, 0));
    }

    return 0;
}
```

### 4.3 模拟高并发客户端

创建1000个连接到服务器

```cpp
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <iostream>
#include <list>

using namespace std;

#define BUF_SIZE 1024               //默认缓冲区
#define SERVER_PORT 8888            //监听端口
#define SERVER_HOST "127.0.0.1"     //服务器IP地址

#define CHK(eval) if(eval < 0){perror("eval"); exit(-1);}
#define CHK2(res, eval) if((res = eval) < 0){perror("eval"); exit(-1);}

int main(void)
{
    struct sockaddr_in addr;
    addr.sin_family = PF_INET;
    addr.sin_port = htons(SERVER_PORT);
    addr.sin_addr.s_addr = inet_addr(SERVER_HOST);

    list<int> client_list;

    // 生成1000个客户端，模拟高并发
    for(int i=0 ; i<1000; i++) {
        int sock;
        CHK2(sock,socket(PF_INET, SOCK_STREAM, 0));
        CHK(connect(sock, (struct sockaddr *)&addr, sizeof(addr)));

        client_list.push_back(sock) ;

        char buf[BUF_SIZE] = { 0x00 } ;
        sprintf( buf, "client name %d", i ) ;
        CHK(send(sock, buf, BUF_SIZE, 0));
    }

    list<int>::iterator it;          //移除所有客户端
    for(it = client_list.begin(); it != client_list.end() ; it++)
        close(*it);

    return 0;
}
```