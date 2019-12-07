# gdb基本命令

- [1. 启动](#1-启动)
- [2. 基本命令](#2-基本命令)
- [3. 多线程调试](#3-多线程调试)

## 1. 启动

```bash
gdb                  #启动gdb
gdb -tui             #接将屏幕分成两个部分，上面显示源代码，比用list方便
gdb <program>        #调试指定程序
gdb <program> core   #运行加载core dump文件，重现出错现场。
gdb <program> <PID>  #program的某进程
gdb <PID>            #调试指定进程
```

## 2. 基本命令

```t
(gdb) help             #显示帮助信息
(gdb) show language    #可以查看gdb中可以设置的程序语言。
(gdb) set language c++ #手动设置当前的程序语言为c++
(gdb) file app         #载入指定的程序
(gdb) show args        #显示缺省的参数列表
(gdb) run              #重新运行调试的程序
(gdb) backtrace        #显示当前调用函数堆栈中的函数
(gdb) kill             #终止正在调试的程序

(gdb) list file:N      #列出指定的第N行代码，也可以指定函数
(gdb) list n1 n2       #列出指定区域(n1到n2之间)的代码,如果没有n1和n2参数，那么就会默认显示当前行和之后的10行，再执行又下滚10行。一般来说在list后面可以跟以下这们的参数：
        <linenum>             #行号。
        <+offset>             #当前行号的正偏移量。
        <-offset>             #当前行号的负偏移量。
        <filename:linenum>    #哪个文件的哪一行。
        <function>            #函数名。
        <filename:function>   #哪个文件中的哪个函数。
        <*address>            #程序运行时的语句在内存中的地址。
(gdb) next           #执行一行代码，如果行为函数，则跳过函数。
(gdb) next N         #执行N行代码
(gdb) [Enter]        #重复执行上条的命令
(gdb) step           #单步进入：即执行一行代码，如果为函数，则函数
(gdb) continue       #继续运行程序直接运行到下一个断点，如果没有断点就一直运行。
(gdb) finish         #执行完当前函数返回到调用它的函数
(gdb) until          #运行直到退出当前循环
(gdb) jump 5         #跳转执行程序到第5行，如果后面没有断点则继续执行，而并不是停在那里了。
(gdb) return         #强制返回当前函数
(gdb) call <expr>    #强制调用函数

(gdb) print <expr>   #强制调用函数2，数的返回值是void，print依旧打印出函数的返回值并且存放到历史记录中。
    print var        #打印变量的值
    print &var       #打印变量的地址
    print *address   #打印地址的数据值
    print /x var     #变量显示格式：
        x   #按十六进制格式显示变量。
        d   #按十进制格式显示变量。
        u   #按十六进制格式显示无符号整型。
        o   #按八进制格式显示变量。
        t   #按二进制格式显示变量。
        a   #按十六进制格式显示变量。
        c   #按字符格式显示变量。
        f   #按浮点数格式显示变量。
(gdb) print *a@10    #会显示10个元素
(gdb) print x=4      #修改运行时候的变量值
(gdb) whatis var     #显示变量var的类型
(gdb) ptype var      #以更详细的方式显示变量var的类型，这里，会打印出var的结构定义。

(gdb) watch i != 10  #检测表达式变化则停住，这里，i != 10这个表达式一旦变化，则停住。watch <expr> 为表达式（变量）expr设置一个观察点。一量表达式值有变化时，马上停住程序(也是一种断点)。

(gdb) break 6             #在当前的文件中某行设定断点
(gdb) break 46 if testsize==100            #设置条件断点，如果testsize==100就在46行处断点。
(gdb) break func          #在当前文件的某函数设定断点
(gdb) break fileName:N    #给指定文件（fileName）的某个行（N）处设置断点

(gdb) delete N    #删除N号断点
(gdb) delete      #删除所有断点
(gdb) clear N     #清除行N上面的所有断点

(gdb) info breakpoints    #显示所有断点信息
(gdb) info break          #显示当前所有的断点，断点号，断点位置等等。
(gdb) info frame          #查看当前函数的程序语言
(gdb) info source         #显示当前所在的源代码文件信息,例如文件名称，程序语言等
```

## 3. 多线程调试

| cmd | desc |
| :--- |:--- |
| `info threads` | 显示当前可调试的所有线程。每个线程会有gdb为其分配的ID，后面的操作会用到这个ID。前面带'*'号的是当前正在调试的线程。|
| `thread <id>` | 切换当前调试的线程为指定ID的线程 |
| `break thread_test.c:123 thread all` | 在所有线程中相应的行上设置断点 |
| `thread apply <id1> <id2> command` | 让一个或者多个线程执行gdb命令command。 |
| `thread apply all command` | 让所有被调试线程执行gdb命令command |
| `set scheduler-locking [off\|on\|step]` | 在使用step或者continue命令调试当前被调试线程的时候，其他线程也是同时执行的，怎么只让被调试程序执行呢？通过这个命令就可以实现这个需求。<br> `off`：不锁定任何线程，也就是所有线程都执行，这是默认值。<br> `on`：只有当前被调试程序会执行。 <br> `step`：在单步的时候，除了next过一个函数的情况(熟悉情况的人可能知道，这其实是一个设置断点然后continue的行为)以外，只有当前线程会执行。|
| `show scheduler-locking` | 查看当前锁定线程的模式 |
