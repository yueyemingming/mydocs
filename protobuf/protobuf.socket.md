# LINUX下QT+googleprotobuf+socket

make

sudo make install

/sbin/ldconfig -v

最后一条命令是为了避免在使用protoc命令时出现库的问题。

## QTPeople.proto

```proto
syntax="proto3";
package QTpeople;

messagePeople
{
  required string name=1;
  required int32 ID=2;
  required string passwd=3;
}
```

## 编译生成 QTPeople.pb.cc 和 QTPeople.pb.h

```bash
protoc-I=. --cpp_out=./ QTPeople.proto
```

## 接收端 QtprotoC

在pro文件里面添加LIBS +=-L /usr/lib-lprotobuf

```cpp
#include <QCoreApplication>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <iostream>
#include <QDebug>
#include "QTPeople.pb.h"

#define MAXSIZE 1024
#define SERVERIP "127.0.0.1"
#define SERVERPORT 2236
#define DATA "this is guest"

using namespace std;

void Client()
{
    int Mysock,recvbytes;
    string data;
    char buf[MAXSIZE];
    struct sockaddr_in ServerAddr;
    QTpeople::People peo;

    peo.set_id(1002);
    peo.set_name("def");
    peo.set_passwd("1234567");
    peo.SerializeToString(&data);

    char sendmsg[MAXSIZE];
    strcpy(sendmsg,data.c_str());
    if((Mysock=socket(AF_INET,SOCK_STREAM,0))==-1){
        qDebug()<<"Sockt error";
        exit(-1);
    }

    bzero(&ServerAddr,sizeof(ServerAddr));
    ServerAddr.sin_family=AF_INET;
    ServerAddr.sin_port=htons(SERVERPORT);
    ServerAddr.sin_addr.s_addr=inet_addr(SERVERIP);

    if (connect(Mysock, (struct sockaddr *)&ServerAddr,sizeof(struct sockaddr)) == -1){
        qDebug()<<"Connect error";
        exit(-1);
    }

    write(Mysock,sendmsg,sizeof(sendmsg));

    if((recvbytes=recv(Mysock,buf,MAXSIZE,0))==-1) {
        qDebug()<<"received error";
        exit(-1);
    }

    qDebug()<<"sucess";

    //这里必须这样带着长度来构造string，因为buf中可能含有\0。
    std::string getmsg(buf, recvbytes);

    bool isGet=peo.ParseFromString(getmsg);
    qDebug()<<isGet;
    cout<<"id="<<peo.id()<<endl;
    cout<<"name="<<peo.name()<<endl;
    cout<<"passwd="<<peo.passwd()<<endl;
    close(Mysock);
    // google::protobuf::ShutdownProtobufLibrary();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Client();
    return a.exec();
}
```

## 发送端 QtprotoS

```cpp
#include <QCoreApplication>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <iostream>
#include <QDebug>

#include "QTPeople.pb.h"

#define SERVPORT 2236
#define BACKLOG 10
#define MAXSIZE 1024

using namespace std;

void server() {
    int sockfd, client_fd;
    string data;
    struct sockaddr_in my_addr;
    struct sockaddr_in remote_addr;
    QTpeople::People peo;

    peo.set_id(1001);
    peo.set_name("abc");
    peo.set_passwd("7654321");
    peo.SerializeToString(&data);

    char sendmsg[MAXSIZE];
    strcpy(sendmsg,data.c_str());

    //创建套接字
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) ==-1) {
        perror("socket create failed!");
        exit(1);
    }

    //绑定端口地址
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(SERVPORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(my_addr.sin_zero),8);

    if (bind(sockfd, (struct sockaddr*) &my_addr, sizeof(my_addr))== -1) {
        perror("bind error!");
        exit(1);
    }

    //监听端口
    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen error");
        exit(1);
    }

    qDebug()<<"before while ok";

    while (1) {
        qDebug()<<"while first ok";
        socklen_t sin_size = sizeof(remote_addr);

        qDebug()<<"before accept ok";

        /*     if ((client_fd = accept(sockfd, (struct sockaddr *)&remote_addr,&sin_size)) ==-1){
                perror("accept error!");
                continue;
                }
            */

        client_fd=accept(sockfd,(struct sockaddr *)&remote_addr,&sin_size);

        qDebug()<<client_fd;
        if(client_fd==-1)
        {
            perror("accept error");
            continue;
        }

        qDebug()<<"accept ok";

        // client_fd = accept(sockfd, (struct sockaddr *)&remote_addr,&sin_size);
        printf("Received a connection from %s\n", (char*)inet_ntoa(remote_addr.sin_addr));
        qDebug()<<"before fork ok";

        //子进程段
        if (!fork()) {
            qDebug()<<"fork ok";
            //接受client发送的请示信息
            int rval;
            char buf[MAXSIZE];
            if ((rval = read(client_fd, buf, MAXSIZE)) < 0) {
                perror("reading stream error!");
                continue;
            }

            qDebug()<<"read ok";
            buf[MAXSIZE]='\0';

            string getmsg=buf;
            bool isGet=peo.ParseFromString(getmsg);
            qDebug()<<isGet;

            cout<<"id="<<peo.id()<<endl;
            cout<<"name="<<peo.name()<<endl;
            cout<<"passwd="<<peo.passwd()<<endl;

            //向client发送信息
            if (send(client_fd,sendmsg, strlen(sendmsg), 0) == -1)
                perror("send error!");

            close(client_fd);
            exit(0);
        }
        close(client_fd);
    }

    close(sockfd);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    server();
    return a.exec();
}
```
