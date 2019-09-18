# 在 C++ 裡面调用外部程序 Boost.Process

- [1. 简易使用](#1-简易使用)
  - [1.1 child](#11-child)
  - [1.2 system()](#12-system)
  - [1.3 spawn()](#13-spawn)
- [2. 执行属性](#2-执行属性)
- [3. 执行参数（cmd、exe、args）](#3-执行参数cmdexeargs)
- [4. 使用系统壳层执行（shell）](#4-使用系统壳层执行shell)
- [4. 指定起始位置（start_dir）](#4-指定起始位置start_dir)
- [5. 输入输出（std_in、std_out、std_err）](#5-输入输出std_instd_outstd_err)
- [6. 其他](#6-其他)

如果想在 C++ 的程序裡面、调用外部的程序的话，一般传统的做法，应该有使用 system()（[参考](http://en.cppreference.com/w/cpp/utility/program/system)）或 C 风格的 popen() 这两种方法（[参考](http://pubs.opengroup.org/onlinepubs/9699919799/functions/popen.html)）。

不过，以 system() 来说，他并不能处理外部程序的输出与输入，在某些情况下不算实用。

popen() 的话，则是不存在于 MSVC 的环境中，需要使用替代的 _popen()（MSDN）才行；同时，popen() 也仅有提供单向、不能双向（只能写或读、不能同时读写）；真要双向的使用，其实相当地麻烦（[参考](http://stackoverflow.com/questions/6171552/popen-simultaneous-read-and-write)）…

而 Boost.Process 则是一个还在开发中、也还没正式进到 Boost C++ Libraries 的函式库，他基本上就是希望可以提供一个跨平台、同时有更多功能的外部程序处理函式库。

它的原始码在：

<https://github.com/klemens-morgenstern/boost-process>

## 1. 简易使用

要使用 Boost.Process 需要引入 boost/process.hpp 这个头文件，之后的东西大多会在 boost::process 这个 namespace 下。

由于 Boost.Process 本身是 header-only 的，所以不需要编译，不过因为他有用到 Boost.System，所以还是要建置 Boost C++ Libraries 才行。

至于要怎麽透过 Boost.Process 调用外部的程序呢？基本上有三种方法。

### 1.1 child

在使用上，他主要是提供了 child 这个类别，来处理外部的程序；他最简单的写法，大致上可以写成：

```cpp
#include <iostream>
#include <boost/process.hpp>

int main(int argc, char** argv)
{
    boost::process::child c("ping 127.0.0.1");
    c.wait();

    std::cout << c.exit_code() << std::endl;
}
```

在上面的例子裡面，他会去建立一个 child 的对象 c 去执行「ping 127.0.0.1」这个外部命令；之后则是透过 wait() 来确定他结束了，才会输出这个外部程序回传的结果（c.exit_code()）、并输出。

### 1.2 system()

而如果中间没有打算对这个外部程序做特殊处理的话，其实也可以用 Boost.Process 提供的 system() 来简化，其写法如下：

```cpp
#include <iostream>
#include <boost/process.hpp>

int main(int argc, char** argv)
{
    std::cout << boost::process::system("ping 127.0.0.1") << std::endl;
}
```

至于和标准函式库的 system() 看起来似乎一样？不过实际上 Boost.Process 提供的 system() 还有许多其他的功能，这点就之后再提了。

### 1.3 spawn()

上面的程序写法，基本上都是会等到外部的指令执行完成后，在结束程序。而如果希望把外部的程序执行起来后、就不管它的话，则就可以使用 spawn() 这个函式。

```cpp
#include <iostream>
#include <boost/process.hpp>

int main(int argc, char** argv)
{
    boost::process::spawn("notepad");
}
```

像是以上面的程序来说，在他把 notepad 开启后，自己就会先结束了。

而如果把 spawn() 改成 system() 的话，则会等到 notepad 被使用者关闭、程序才会结束。

## 2. 执行属性

上面基本上都是最基本的用法。不过如果只能这样用的话，那其实用 Boost.Process 的用处就不大了。

在小编来看，Boost.Process 最大的好处，就是提供了许多执行时的属性（properties），可以用来做进一步的控制～这些属性包括了：cmd、args、exe、shell、env、std_in 等等…完整的列表可以参考官方文件（[链接](http://klemens-morgenstern.github.io/process/boost_process/handbook.html#boost_process.handbook.properties)），下面小编就选一些做说明。

## 3. 执行参数（cmd、exe、args）

在前面的例子裡面，不管是 child、system() 或是 spawn()，都是把指令以单一字串的形式传进去。

而实际上，Boost.Process 在执行外部程序的时候，在参数的传递上给了相当大的自由度。

比如说，以「ping 127.0.0.1」这个指令，直接以 system() 来调用的话，可以写成：

```cpp
namespace bp = boost::process;
bp::system( "ping 127.0.0.1" );
```

而这边其实可以把执行指令「ping」和执行参数「127.0.0.1」分开、写成：

```cpp
namespace bp = boost::process;
bp::system("ping", "127.0.0.1");
```

但是小编自己在测试的时候，却发现这样的写法会出现找不到指定程序文件的错误…而要让他可以运作的话，则需要使用完整的路径，也就是：

```cpp
namespace bp = boost::process;
bp::system("C:\\Windows\\System32\\PING.EXE", "127.0.0.1");
```

而除了直接以字串传入指令之外，他其实也还有提供许多属性（properties），在执行时可以做设定，包括了：cmd、args、exe、shell、env 等等…完整的列表可以参考官方文件（[链接](http://klemens-morgenstern.github.io/process/boost_process/handbook.html#boost_process.handbook.properties)）。

比如说，上面的例子实际上就是：

```cpp
bp::system(bp::exe="C:\\Windows\\System32\\PING.EXE", bp::args="127.0.0.1");
```

而最初的写法，也等同于：

```cpp
bp::system(bp::cmd="ping 127.0.0.1");
```

如果要执行的参数不只一个的话，也可以有几种写法；下面几个写法的效果基本上都是相同的：

```cpp
bp::system("ping 127.0.0.1 -n 1");
bp::system(bp::cmd="ping 127.0.0.1 -n 1");

bp::system("C:\\Windows\\System32\\PING.EXE", "127.0.0.1", "-n", "1");
bp::system(bp::exe = "C:\\Windows\\System32\\PING.EXE",
        bp::args = { "127.0.0.1", "-n", "1" });
```

不过，个人觉得比较讨厌的，是「-n 1」这种用空格隔开的参数不能写成一个字串、而得分成两个，算是比较不好用的点了。

## 4. 使用系统壳层执行（shell）

另外，如果想直接使用 DOS 内建的命令（例如「dir」）的话，直接写

```cpp
bp::system("dir");
```

应该也会出现错误。

而要解决的话，就是再加上 shell，让 Boost.Process 透过系统的 shell 去执行，这样就可以了～他的写法就是：

```cpp
bp::system("dir", bp::shell);
```

而使用系统 shell 的另一个好处，就是前面有的必须要写出可制成程序文件完整路径的情况，也可以不用了！例如：

```cpp
bp::system("ping", "127.0.0.1", "-n", "1", bp::shell);
bp::system(bp::exe="ping", bp::shell, bp::args = {"127.0.0.1","-n","1"});
```

所以目前看来，在使用 Boost.Process 的时候，全部都加上 shell 应该会是个比较直觉的做法。

## 4. 指定起始位置（start_dir）

再来，如果想要指定程序运作时的路径的话，则可以透过 start_dir 这个属性来指定，比如说：

```cpp
bp::system("dir", bp::shell, bp::start_dir="d:\\");
```

就可以透过 dir 的指令、列出 d:\ 下的程序文件了。

## 5. 输入输出（std_in、std_out、std_err）

前面最初就有提到，小编之所以不用 popen()，一个主要的原因就是他要同时操作 std input 和 std output 时会很麻烦…

而 Boost.Process 在这部分，就算是相对方便了！他提供了 std_in、std_out、std_err 这三个属性，来对应标准输入输出、以及错误的处理。

在使用时，std_out、std_err 都是透过「>」来指定重新导向的目标，而 std_in 则是透过「<」来指定来源；这边的目标或来源，可以直接给程序文件名称、或是使用 Boost.Process 的 pipe、pstream 来做处理。

最简单的例子，就是如果不希望外部程序的输出显示在 console 上的话，可以透过把 std_out 导到 null 来做；这样的程序会是：

```cpp
bp::system("dir", bp::shell, bp::std_out > bp::null);
```

如此一来，程序执行时就看不到 dir 的结果了。

而官方也提供了一个范例：

```cpp
boost::filesystem::path log = "my_log_file.txt";
boost::filesystem::path input = "input.txt";
boost::filesystem::path output = "output.txt";
bp::system("my_prog", bp::std_out>output, bp::std_in<input, bp::std_err>log);
```

在这个范例裡，会把 input.txt 这个程序文件的内容、透过标准输入（std::cin）的方式送给程序（my_prog），而程序输出到标准输出（std::cout）的内容、则是会被写到 output.txt 这个程序文件；另外，输出到标准错误（std::cerr）的内容则是会被写到 my_log_file.txt 裡。

而如果希望把输出都以字串的形式记录下来的话，也可以写成：

```cpp
std::string line;
std::vector<std::string> outline;

namespace bp = boost::process;
bp::ipstream ps;
bp::child c("ping 127.0.0.1", bp::std_out > ps);

while (c.running() && std::getline(ps, line) && !line.empty()) {
    outline.push_back(line);
}
```

如此一来，ping 的输出结果都会逐行储存在 outline 裡面，也就可以拿来做后续的分析了。

而如果同时要输出输入的话，则可以写成类似下面的样子：

```cpp
namespace bp = boost::process;
bp::opstream ops;
bp::ipstream ips;
bp::child c("prog", bp::std_out > ips, bp::std_in < ops);

ops << "1";
std::string str;
ips >> str;

c.wait();
```

不过如果要和外部程序互动的话，实际上应该会有很多繁琐的东西要处理就是了。

一般比较可能会用到的功能，大概就是这些了吧？

## 6. 其他

除了上面的功能之外，其实 child 本身也还有一些其他的函式可以使用；像是他实际上有提供 terminate() 这个函式，理论上应该可以用来中断所建立的外部程序。但是很遗憾的是，在小编这边测试都是失败的…看来在 Windows 10 的环境下，他并没有办法把外部程序真的中断掉。

另外，他实际上也支援 asynchronous 的操作，或是透过设定 on_eixt、来指定当外部程序结束后、要做什麽动作；在某些其况下，这样的架构应该也算很实用的。

最后，他也有提供一个 boost::this_process，可以用来取得一些目前 process 的资讯；其中，也可以透过 environment() 这个函式、来取得环境变数；不过，由于 Windows 和 Linux 的环境变数定义差很多，所以如果是写跨平台的程序，那这边应该还是得分开处理了。
