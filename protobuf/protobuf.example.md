# 在C++中使用protobuf

## 1. 简介

假如我们要实现write将把一个结构化数据写入磁盘，以便其他人来读取。如果我们不使用 Protobuf，一个可能的方法是将数据转换为字符串，然后将字符串写入磁盘。转换为字符串的方法可以使用 sprintf()，这非常简单。数字 123 可以变成字符串”123”。

这样做似乎没有什么不妥，但是仔细考虑一下就会发现，这样的做法对写 read 的那个人的要求比较高。比如”123”可以是单个数字 123，但也可以是三个数字 1,2 和 3，等等。这么说来，我们还必须让 write 定义一种分隔符一样的字符，以便 read可以正确读取。但分隔符也许还会引起其他的什么问题。最后我们发现一个简单的 Helloworld 也需要写许多处理消息格式的代码。

如果使用 Protobuf，那么这些细节就可以不需要应用程序来考虑了。

使用 Protobuf，write的工作很简单，需要处理的结构化数据由 .proto 文件描述，并定义在 lm.helloworld.pb.h 中。对于本例，类名为 lm::helloworld。Writer 需要 include 该头文件，然后便可以使用这个类了。

当我们需要将该结构化数据保存到磁盘上时，类 lm::helloworld 已经提供相应的方法来把一个复杂的数据变成一个字节序列，我们可以将这个字节序列写入磁盘。

对于想要读取这个数据的程序来说，也只需要使用类 lm::helloworld 的相应反序列化方法来将这个字节序列重新转换会结构化数据。这同我们开始时那个“123”的想法类似，不过 Protobuf 想的远远比我们那个粗糙的字符串转换要全面，因此，我们不如放心将这类事情交给 Protobuf 吧。

## 2. msg.proto

```proto
syntax="proto3";

package lm;

message helloworld {
    int32   id=1;
    string  str=2;
    int32   opt=3;
}
```

### 编译生成 msg.pb.cc 和 msg.pb.h

```bash
protoc --cpp_out=. ./msg.proto
```

## 3. write.cc

```cpp
#include "msg.pb.h"
#include <fstream>
#include <iostream>

using namespace std ;

int main(void) {
    lm::helloworld msg1;
    msg1.set_id(101);
    msg1.set_str("hello");
    fstream output("./log", ios::out | ios::trunc | ios::binary );

    if ( !msg1.SerializeToOstream(&output)) {
        cerr << "Failed to write msg ." << endl ;
        return -1 ;
    }

    return 0 ;
}
```

## 4. read.cc

```cpp
#include "msg.pb.h"
#include <fstream>
#include <iostream>

using namespace std;

void ListMsg(const lm::helloworld& msg ) {
    cout << msg.id() << endl ;
    cout << msg.str() << endl ;
}

int main(int argc, char* argv[]) {
    lm::helloworld msg1;

    {
        fstream input("./log", ios::in|ios::binary);
        if ( !msg1.ParseFromIstream(&input) ) {
            cerr << "Failed to parse address book ." << endl ;
            return -1 ;
        }
    }

    ListMsg(msg1);
}
```

## 5. makefile

```makefile
all: write read

clean:
    @rm -f write read msg.*.cc msg.*.h *.o  log

write: msg.pb.cc write.cc
    g++ -std=c++11  msg.pb.cc write.cc -o write  `pkg-config --cflags --libs protobuf`

read: msg.pb.cc read.cc
    g++ -std=c++11  msg.pb.cc read.cc -o read  `pkg-config --cflags --libs protobuf`

msg.pb.cc msg.pb.h :msg.proto
    protoc --cpp_out=. ./msg.proto
```

## 6. 编译运行

```bash
make
./write
./read
```

> 101
> hello

## 7. 常见错误

> #error This file requires compiler and library support for the ISO C++ 2011 standard. This support is currently experimental, and must be enabled with the -std=c++11 or -std=gnu++11 compiler options.

这个错误出现的话需要在Makefile中指定=std=c++11