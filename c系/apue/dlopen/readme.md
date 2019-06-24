# dlopen、dlsym、dlclose加载动态链接库

## 1. 前言

为了使程序方便扩展, 具备通用性, 可以采用插件形式。采用异步事件驱动模型, 保证主程序逻辑不变, 将各个业务已动态链接库的形式加载进来, 这就是所谓的插件。

linux提供了加载和处理动态链接库的系统调用, 非常方便。本文先从使用上进行总结, 涉及到基本的操作方法, 关于动态链接库的本质及如何加载进来, 需要进一步学习, 后续继续补充。如何将程序设计为插件形式, 挖掘出主题和业务之间的关系, 需要进一步去学习。

## 2. 生产动态链接库

将如下程序编译为动态链接库libfun.so, 程序如下：

```c
#include <stdio.h>

void function( void )
{
    printf( "hello dlopen...\n" ) ;
}
```

编译

```bash
gcc -fPIC -shared fun.c -o libfun.so
```

## 3. dlopen调用动态链接库

* 头文件

```c
#include <dlfcn.h>
```

* **void \*dlopen(const char \*filename, int flag);**

dlopen以指定模式打开指定的动态连接库文件, 并返回一个句柄给调用进程, dlopen打开模式如下：

mode | desc
:--- | :---
RTLD_LAZY | 暂缓决定, 等有需要时再解出符号
RTLD_NOW | 立即决定, 返回前解除所有未决定的符号。

* **char \*dlerror(void);**

dlerror返回出现的错误.

* **void *dlsym(void \*handle, const char \*symbol);**

dlsym通过句柄和连接符名称获取函数名或者变量名。

* **int dlclose(void \*handle);**

dlclose来卸载打开的库。

* 调用上面生成的libfun.so, 测试程序如下：

```c
#include <stdio.h>
#include <errno.h>
#include <dlfcn.h>

int main( void )
{
    // 打开动态链接库
    void* handle = dlopen( "./libfun.so", RTLD_LAZY ) ;
    if ( !handle )
    {
        perror( "dlopen " ) ;
        return -1 ;
    }

    // 清除之前存在的错误
    dlerror() ;

    // 获取一个函数function
    void(*function)(void) = dlsym( handle, "function" ) ;
    if ( dlerror() != NULL )
    {
        perror( "dlsym " ) ;
        return -1 ;
    }

    // 调用库函数
    function() ;

    // 关闭动态链接库
    dlclose( handle ) ;

    return 0 ;
}
```

* 编译

```bash
gcc -rdynamic -o main main.c -ldl
```

* 运行

```bash
./main
hello dlopen...
```
