# C++11 多线程初探

## 与 C++11 多线程相关的头文件

C++11 新标准中引入了四个头文件来支持多线程编程，他们分别是<atomic> ,<thread>,<mutex>,<condition_variable>和<future>。

| 头文件               | 介绍                                                         |
| -------------------- | ------------------------------------------------------------ |
| <atomic>             | 主要声明了两个类, std::atomic 和 std::atomic_flag，另外还声明了一套 C 风格的原子类型和与 C 兼容的原子操作的函数 |
| <thread>             | 主要声明了 std::thread 类，另外 std::this_thread 命名空间也在该头文件中 |
| <mutex>              | 主要声明了与互斥量(mutex)相关的类，包括 std::mutex 系列类，std::lock_guard, std::unique_lock, 以及其他的类型和函数 |
| <condition_variable> | 该头文件主要声明了与条件变量相关的类，包括 std::condition_variable 和 std::condition_variable_any |
| <future>             | 该头文件主要声明了 std::promise, std::package_task 两个 Provider 类，以及 std::future 和 std::shared_future 两个 Future 类，另外还有一些与之相关的类型和函数，std::async() 函数就声明在此头文件中。 |

### **std::thread "Hello world"**

下面是一个最简单的使用 std::thread 类的例子：

```c++
#include <stdio.h>
#include <stdlib.h>

#include <iostream> // std::cout
#include <thread>   // std::thread

void thread_task() {
    std::cout << "hello thread" << std::endl;
}

int main(int argc, const char *argv[])
{
    std::thread t(thread_task);
    t.join();

    return EXIT_SUCCESS;
}  
```

Makefile 如下：

```cmake
all:Thread

CC=g++
CPPFLAGS=-Wall -std=c++11 -ggdb
LDFLAGS=-pthread

Thread:Thread.o
    $(CC) $(LDFLAGS) -o $@ $^

Thread.o:Thread.cc
    $(CC) $(CPPFLAGS) -o $@ -c $^

.PHONY:
    clean

clean:
    rm Thread.o Thread
```

注意在 Linux GCC4.6 环境下，编译时需要加 -pthread，否则执行时会出现：

```bash
$ ./Thread
terminate called after throwing an instance of 'std::system_error'
  what():  Operation not permitted
Aborted (core dumped)
```

原因是 GCC 默认没有加载 pthread 库，据说在后续的版本中可以不用在编译时添加 -pthread 选项。

更多的有关 C++11 Concurrency 的介绍将在后续的一系列博客中写出，希望自己勤快一点吧 ;-)