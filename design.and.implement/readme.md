# linux内核设计与实现

此书基于linux 2.6.34

## 1. linux内核简介

### 1.1 unix历史

贝尔实验室 Dennis Ritchie， Ken Thompson,1969年,1973年用C语言重写

加州大学伯克利分校 BSD

### 1.2 linux历史

芬兰赫尔辛基大学 Linus Torvalds 1991年

- linux设计理念同unix，但源代码完全重写，实现不同。
- linux是自由软件，但也不是无限自由，遵循GNU GPL(General Public License)2.

### 1.3 内核简介
linux系统 用户空间+内核空间

**上下文** 指内核活动范畴

任何时间点上，处理器活动于以下三种情况：

- 运行于用户空间，执行用户进程。
- 运行于内核空间，处于进程上下文，代表某个特定的进程执行。
- 运行于内核空间，处于中断上下文，与任何进程无关，处理某个特定的中断。

### 1.4 内核类型

- 单内核

  单一的静态二进制文件形式存储于磁盘中，内核是一个大的过程。内核中所有服务集成于内核态中，运行于同一地址空间，因此它们之间的通信直接函数调用实现。简单且效率高，扩展性差。UNIX为典型代表。

- 微内核

  所有服务被切分为不同的小的过程，过程运行于用户空间。它们之间的通信通过消息传递实现。消息传递机制开销远大于函数调用，需要频繁上下文切换。早期的windows NT(XP,Vista,win7)和MAC OS，近期的不再将各个服务过程放在用户空间中了。

linux属于**单内核**形式，但也汲取了微内核精华，模块化设计、抢占式、支持内核线程以及动态装载模块等。（由此可以看出linux的系统调用虽然很unix基本相同，但底层实现已经大不相同了）

### 1.5 linux内核版本
主版本号.从版本号.修订版本号.稳定版本号

稳定版本号为偶数，其为稳定版本；否则为开发版。

2.6.26.1是开发版本，非稳定版本.

### 1.6 linux内核开发者社区
lkml		linux kernel mailing list		http://vegr.kernel.org

[注]. 本书大多代码以intel x86/x86_64系统架构为例

## 2. 从内核出发

### 2.1 获取源码
http://www.kernel.org

| 操作  | 说明                                                         |
| ----- | ------------------------------------------------------------ |
| bzip2 | `tar xjvf linux-x.y.z.tar.bz2`                               |
| gzip  | `tar xzvf linux-x.y.z.tar.gz`                                |
| git   | `git clone git://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux-2.6.git` |
| patch | `$ patch -p1 < ../patch-x.y.z`                               |


### 2.2 内核源码树
| 文件或目录    | 说明                                         |
| ------------- | -------------------------------------------- |
| arch          | 特定体系结构的源码                           |
| block         | 块设备I/O层                                  |
| crypto        | 加密API                                      |
| Documentation | 内核源码文档                                 |
| drivers       | 设备驱动程序                                 |
| firmware      | 使用某些驱动程序而需要的设备固件             |
| fs            | VFS和各种文件文件系统                        |
| include       | 内核头文件                                   |
| init          | 内核引导和初始化                             |
| ipc           | 进程间通信                                   |
| kernel        | 像调度程序这样的核心子系统                   |
| lib           | 通用内核函数                                 |
| mm            | 内核管理子系统和VMnet网络子系统              |
| samples       | 示例代码                                     |
| scripts       | 编译内核所用的脚本                           |
| security      | Linux安全模块                                |
| sound         | 语音子系统                                   |
| usr           | 早期用户空间代码(所谓的initramfs)            |
| tools         | 在Linux开发中用的工具                        |
| virt          | 虚拟化基础结构                               |
| COPYING       | 内核许可证(GNU GPL v2)                       |
| CREDITS       | 具有贡献的开发者列表                         |
| MAINTAINERS   | 维护者列表，他们负责维护内核子系统和驱动程序 |
| Makefile      | 编译内核的Makefile                           |

### 2.3 内核构建
#### 2.3.1 配置

| 操作            | 说明           |
| --------------- | -------------- |
| make config     | 命令交互方式   |
| make menuconfig | ncursue方式    |
| make gconfig    | gnome图形方式  |
| make defconfig  | 缺省值内核配置 |
| make oldconfig  | 使用老配置文件 |

以上几种方式任选其一，最终在内核源码根目录下生成  .config 文件，这就是最终的配置文件。
#### 2.3.2 编译

| 操作                 | 说明                                                         |
| -------------------- | ------------------------------------------------------------ |
| make	[-jn]        | n个线程同时编译                                              |
| make bzImage         | 也不再需要，arch/<arch>/boot/bzImage就是最终编译出的内核文件 |
| make modules_install | 模块编译安装，编译后的模块文件为*.ko文件，它们会自动被安装到正确的目录下/lib/modules/linux-x.y.z/ |
| System.map           | 内核符号与内存地址的对照表                                   |

### 2.4 内核开发特点
- 完全脱离用户态C库
  - 内核自己实现了完备的C库,其头文件都在include目录中.
  - linux/string.h	lib/string.c
  - 体系结构相关的头文件位于	**arch/<architecture>/include/asm**	目录下
  - printk取代printf
- 必须使用GNU C 
  - 内核开发者使用的C语言涵盖了ISO C99标准和GNU C扩展。
  - 编译器建议版本为4.4往上.
  - 支持内联
  - 支持内嵌汇编
  - 分支声明(目的为提高效率)
    - unlikely(error)	错误较少发生时使用, error绝大多数为0.
    - likely(success)	正确较多发生时使用, success绝大多数不为0.
- 内核编程本身缺乏像用户空间那样的内存保护机制
  - 用户空间进程访问非法地址空间时，内核对其保护，会发送SIGSEGV信号，结束进程。
  - 而内核自己非法访问内存时，没有谁能对其提供保护，导致oops系统崩溃。
- 难以执行浮点运算
- 内核给每个进程只有一个很小的定长堆栈
- 必须时刻注意同步和并发
- 需要注意可移植性

## 3. 进程管理

### 3.1 进程
进程	处于执行期的程序以及相关的资源的总称

进程资源包括打开的文件，挂起的信号，内核内部数据，处理器状态，内存地址，执行线程等诸多方面的内容。

内核调度的是线程，而不是进程。linux中，线程进程并不区分，线程是特殊的进程。

现代操作系统的两中虚拟机制：虚拟处理器和虚拟内存

- **虚拟处理器**：给各个进程造成假象貌似独占处理器。

- **虚拟内存**：给各个进程造成假象貌似独占所有内存。

同一进程中的各个线程可以共享虚拟内存，但每个都拥有各自的虚拟处理器。

### 3.2 进程描述符及任务结构
#### 3.2.1 进程描述符

##### 3.2.1.1 进程描述符

内核中用进程描述符表示一个具体进程的所有信息(进程资源).

结构体为 **struct task_struct	<linux/sched.h>**，此结构体较大，在32位机上大小约1.7KB.

```c
struct task_struct {
    volatile long state;	/* -1 unrunnable, 0 runnable, >0 stopped */
    /* 当前进程的状态。系统中的每个进程比处于以下五种状态之一：
	 * TASK_RUNNING		    可执行的，它或在执行，或处于运行队列中等待执行。
	 * TASK_INTERRUPTIBLE	正在睡眠，可被唤醒。
	 * TASK_UNINTERRUPTIBLE	正在睡眠，不可被唤醒。很少用。
	 * __TASK_TRACED		被其他进程跟踪。例如通过ptrace对调试程序进行跟踪。
	 * __TASK_STOPPED		应该说是暂停执行。比如收到SIGSTOP，SIGTSTP，SIGTTIN，SIGTTOU等信号时。另外在调试期间接受到任何信号，都会使进程进入这种状态。
	 *
	 * 状态设置函数	
	 * 	set_task_state( task, state ) 和 set_current_state( task, state ) 
	 *	二者相同，功能类似于 task->state=state , 但尽量使用函数, 函数在有必要的时候，会设置内存屏障来强制其他处理器做重新排序(SMP有效时)
	 */

    void *stack;	// 指向当前进程的内核栈空间首地址
    ...
    pid_t	pid ;	// 进程id，最大个数32768, root用户也可以通过修改/proc/sys/kernel/pid_max来提高上限.
    ...
    struct task_struct *parent; /* recipient of SIGCHLD, wait4() reports */	// 父进程指针
    struct list_head children;	/* list of my children */ // 子进程链表
    struct list_head sibling;	/* linkage in my parent's children list */	// 兄弟进程链表
    ...
} ;
```

slab分配器对task_struct结构体对象分配内存空间(预先分配且重复使用task_struct内存空间，而非动态分配和释放.)

##### 3.2.1.2 进程内核栈

进程在 **用户态** 和 **内核态** 都有各自独立的 栈空间。

进程内核栈结构体

```c
union thread_union {
    struct thread_info thread_info;
    unsigned long stack[THREAD_SIZE/sizeof(long)];		// 此处可以看到这个栈空间是静态分配的，且大小固定有限(4k或8k，依据处理器不同而不同).
};
```

```c
struct thread_info {
    struct task_struct *task;	/* main task structure */	// 这个这个task是有slab分配器分配的空间，前有叙述。
    struct exec_domain *exec_domain;/* execution domain */
    unsigned long flags;		/* thread_info flags (see TIF_*) */
    mm_segment_t addr_limit;	/* user-level address space limit */
    __u32 cpu;			/* current CPU */
    int preempt_count;		/* 0=premptable, <0=BUG; will also serve as bh-counter */
    struct restart_block restart_block;
};
```

由上述内容，可以看出，在内核栈空间中的有个成员是thread_info。而thread_info有个成员是task。 而task有个成员stack最终又指向了其内核栈空间手地址.即相当于如下:

`thread_union.thread_info->task->stack == &thread_union ;`

##### 3.2.1.3 任务队列

内核中用双向循环链表任务队列来管理所有的进程，其节点就是进程描述符。

任务队列名称	`task_list	<linux/wait.h>`

**进程上下文**	指“代表进程执行”的内核部分。即current. 

宏current用于查找当前正在运行的进程的"进程描述符"。

 current的查找速度尤为重要。依据硬件体系结构不同。有的处理器提供了专门的寄存器用于存储current值，而有的系统没有专门的寄存器，以内存指针代替,速度效率较低。

```c
#define get_current() (current_thread_info()->task)
#define current get_current()
```

**current_thread_info()**

- 对于专门寄存器存储的方式：直接获取相应寄存器的值，此值就是内存上当前进程task_struct结构体对象的首地址.
- 对于无专门寄存器存储方式：正在运行的当前这个进程的内核栈首地址是被存储在某个寄存器中的(x86_32是esp寄存器中), 因为内核栈跟thread_info共用空间，也就是说内核栈首地址就是thread_info首地址. 因此thread_info->task就是要查找的进程描述符。

##### 3.2.1.4 进程家族树

​    内核在系统启动的最后阶段启动init进程。

    系统中每个进程都必须有一个父进程，有0个或多个子进程。同一个父进程的所有进程称作兄弟.
    
    所有进程都是pid为1的init进程的后代。init是祖先进程。
    祖先进程描述符为 init_task 。
        struct task_struct init_task = INIT_TASK(init_task);	// 此处语法妙
        #define INIT_TASK(tsk)	\
        {									\
            .state		= 0,						\
            .stack		= &init_thread_info,				\
            ...						\
        }
        可以看出init祖先进程是静态初始化的.
    
    struct task_struct中parent指针和children子进程链表表明了所有进程间的关系。
    struct task_struct *parent;	// 父进程指针,父进程只有一个
    struct list_head children;	// 子进程链表
    struct list_head sibling;	// 兄弟进程链表

##### 3.2.1.5 进程描述符各种访问方式

- 直接队列访问方式
  - 相邻后一个节点
  ```c
  struct task_struct*	the_next = list_entry( some_task->tasks.next. struct task_struct, tasks ) ;
  内核中已提供next_task宏
  #define next_task(p)  list_entry_rcu((p)->tasks.next, struct task_struct, tasks)
  ```
  - 相邻前一个节点
  ```c
  struct task_struct*	the_prev = list_entry( some_task->tasks.prev. struct task_struct, tasks ) ;
  内核中已提供prev_task宏
  #define prev_task(p) list_entry_rcu((p)->tasks.prev, struct task_struct, tasks)
  ```
  - 遍历所有节点
  ```c
  内核中已提供for_each_process(p)宏
  #define for_each_process(p) \
  for (p = &init_task ; (p = next_task(p)) != &init_task ; )
  ```
- 兄弟访问方式
```c
struct task_struct* the_sibling = list_entry( some_task->sibling.next, struct task_struct, sibling ) ;
```
- 父子访问方式
  - 访问父进程
  ```c
  struct task_struct*	my_parent = current->parent ;
  ```
  - 上溯直到祖先
  ```c
  struct task_struct*	task ;
  for( task = current ; task != &init_task ; task = task->parent ) ;
  ```
- 所有子进程访问方式
```c
struct task_struct*	task ;
struct list_head* list = ... ; // 任务队列的某个节点
list_for_each( list, ¤t->children ) {
    for( list = current->children->next; list != current->children; list = list->next )	{
        task = list_entry( list, struct task_struct, sibling ) ;
    /* 此时task指向某个子进程描述符 */
    }
}
```

#### 3.2.2 进程创建

##### 3.2.2.1 unix进程创建方式与众不同
- 其他系统	首先在新的地址空间里创建进程，读入可执行文件，最后开始执行。
- unix系统	分解为两个单独的函数去执行：fork()和exec()
  - fork()拷贝当前进程创建出一个新子进程。父子之间的区别仅在于pid/ppid以及某些其他资源。 
  - exec()负责读取可执行文件，将其载入子进程的地址空间中开始执行。

##### 3.2.2.2 写时拷贝
linux的fork()应用了写时拷贝技术。

fork()创建子进程时，并没有完全拷贝父进程所有内容。对于子进程，只有需要写入资源的时刻，才从父进程复制过来。而对于读的资源来说，子进程完全没有复制父进程的资源。

这种优化可以避免拷贝大量根本就不会被使用到的数据所带来的时间及空间的开销.

##### 3.2.2.3 线程

内核中没有线程概念。线程仅仅被视为与其他进程共享某些资源的进程。线程有自己独立的task_struct。

对于linux来说，线程只是进程间共享资源的手段。因此linux中没有"轻量级进程"的说法。

##### 3.2.2.4 fork()

fork(),vfork(),__clone(),线程都可以创建新的进程，行为以及底层各自的参数标志不同。

- 调用过程
```c
fork(),vfork(),__clone(),线程  库函数
  -->  clone()  系统调用
    -->  do_fork()  内核函数<kernel/fork.c>
      -->  copy_process()  内核函数<kernel/fork.c>
```
- copy_process()实际完成子进程的创建工作.

- 参数传递

| 函数 | 方法 |
| ------- | ------------------- |
| fork()  | clone( SIGCHLD, 0 ) |
| vfork() |clone( CLONE_VFORK \| CLONE_VM \| SIGCHLD, 0 )|
| 线程|clone( CLONE_VM \| CLONE_FS \| CLONE_FILES \| CLONE_SIGHAND, 0 )|



| 参数                 | 说明                                               |
| -------------------- | -------------------------------------------------- |
| CLONE_FILES          | 共享打开的文件                                     |
| CLONE_FS             | 共享文件系统信息                                   |
| CLONE_IDLETASK       | 将pid设置为0(只供idle进程使用)                     |
| CSIGNAL              | 0x000000ff	/* signal mask to be sent at exit */ |
| CLONE_VM             | 共享地址空间                                       |
| CLONE_SIGHAND        | 父子进程共享信号处理函数及被阻断的信号             |
| CLONE_PTRACE         | 继续调试子进程                                     |
| CLONE_VFORK          | vfork调用，即父进程准备睡眠等待子进程将其唤醒      |
| CLONE_PARENT         | 指定父子进程拥有同一个父进程                       |
| CLONE_THREAD         | 父子进程放入相同的线程组                           |
| CLONE_NEWNS          | 为子进程创建新的命名空间                           |
| CLONE_SYSVSEM        | 共享system v SEM_UNDO语义                          |
| CLONE_SETTLS         | 为子进程创建新的TLS                                |
| CLONE_PARENT_SETTID  | 设置父进程TID                                      |
| CLONE_CHILD_CLEARTID | 清除子进程TID                                      |
| CLONE_DETACHED       | 未使用                                             |
| CLONE_UNTRACED       | 防止跟踪进程在子进程上强制执行CLONE_PTRACE         |
| CLONE_CHILD_SETTID   | 设置子进程TID                                      |
| CLONE_STOPPED        | 以TASK_STOPPED状态开始进程                         |
| CLONE_NEWUTS         | 0x04000000	/* New utsname group? */             |
| CLONE_NEWIPC         | 0x08000000	/* New ipcs */                       |
| CLONE_NEWUSER        | 0x10000000	/* New user namespace */             |
| CLONE_NEWPID         | 0x20000000	/* New pid namespace */              |
| CLONE_NEWNET         | 0x40000000	/* New network namespace */          |
| CLONE_IO             | 拷贝IO上下文                                       |



##### 3.2.2.5	内核线程

它运行于内核中，用于执行某写后台操作。跟普通进程一样，可以被调度和抢占。

ps -ef	可以看到内核线程,[]代表内核线程

```bash
$ ps -ef
UID        PID  PPID  C STIME TTY          TIME CMD
root         2     0  0 Oct31 ?        00:00:00 [kthreadd]	
root         3     2  0 Oct31 ?        00:00:02 [migration/0]
root         4     2  0 Oct31 ?        00:00:07 [ksoftirqd/0]
... ...
root     10906     2  0 08:41 ?        00:00:00 [flush-8:48]
... ...
```

第一个内核线程为 kthreadd, 它创建其他的内核线程。
​

- 创建但不运行  

  ```c
  struct task_struct *kthread_create(int (*threadfn)(void *data),   //threadfn执行路径
                                     void *data,
                                     const char namefmt[], ...) ;
  ```

- 唤醒使其运行

  ```c
  wake_up_process()   //内核线程创建后不会自主运行，需要唤醒.
  ```

- 创建同时运行

  ```c
  #define kthread_run(threadfn, data, namefmt, ...)			   \
      ({									   \
          struct task_struct *__k						   \
  	        = kthread_create(threadfn, data, namefmt, ## __VA_ARGS__); \
          if (!IS_ERR(__k))						   \
            wake_up_process(__k);					   \
           __k;								   \
      })
  ```

- 终止

  ```c
  int kthread_stop(struct task_struct *k);  或 do_exit() ;
  ```

#### 3.2.3 进程终结

do_exit() <kernel/exit.c>完成资源释放后，调用schedule()切换到新的进程，并使自己处于EXIT_ZOMBIE(僵尸)状态。
父进程用wait4()负责收尸，最后释放子进程的task_struct和thread_info以及内核栈空间等。

孤儿进程
    子进程为孤儿进程时，需要做的是寻找一个新的父进程。
    调用过程
        do_exit()
            -->  exit_notify()
                -->  非孤儿进程情况
                    -->  收尸
                -->  孤儿进程情况 forget_original_parent()
                    -->  find_new_reaper() (完成查找父进程)
                    -->  将当前孤儿进程的兄弟进程的父进程统统设置为找到的这个新的养父
                        -->  ptrace寻父 ...
                        -->  收尸


## 4. 进程调度
调度程序的任务是在一组处于可运行状态的进程中选择一个来执行。
多任务分类	非抢占式任务 和 抢占式任务
让步	非抢占模式中，进程主动挂起自己

抢占	抢占模式中，调度程序强制挂起某个正在执行的任务。
    这不同于中断。这说的是进程上下文的事。指的是，当前一个进程在执行，突然来了一个高优先级的进程介入，使当前进程强制挂起。
    注意抢占跟中断完全是两码事。
    
非抢占模式缺点	调度程序无法对每个进程执行时间作出同一规定，所以进程独占的处理器时间可能超出预料；更糟糕的是一个决不让步的的悬挂程序能是系统崩溃。
Unix从一开是就是抢占模式设计的。
时间片	分配给可执行进程的处理器时间段
linux 2.6.23之前采用O(1)调度器算法；之后采用CFS算法，“完全公平调度算法”。
进程资源消耗类型	I/O消耗型和处理器消耗型
linux进程优先级
    linux采用两种不同的优先级类型
        nice值	范围-20到+19	默认值0	数值越大优先级越低
                linux中代表时间片比例；有些则不同，mac os x(unix变种)代表分配给进程的时间片绝对值
                ps -el 的NI列为nice值
                    $ ps -el
                    F S   UID   PID  PPID  C PRI  NI ADDR SZ WCHAN  TTY          TIME CMD
                    4 S     0     1     0  0  80   0 -  4839 poll_s ?        00:00:04 init
                    1 S     0     2     0  0  80   0 -     0 kthrea ?        00:00:00 kthreadd

        实时优先级	范围0-99	数值越大优先级越高
                ps -eo state,uid,pid,ppid,rtprio,time,comm可用来查看实时优先级
                    $ ps -eo state,uid,pid,ppid,rtprio,time,comm
                    S   UID   PID  PPID RTPRIO     TIME COMMAND
                    S     0     1     0      - 00:00:04 init
                    S     0     2     0      - 00:00:00 kthreadd
                    S     0     3     2     99 00:00:02 migration/0
                    S     0     4     2      - 00:00:07 ksoftirqd/0
                    S     0     5     2     99 00:00:00 migration/0



5. 系统调用
在linux中，系统调用是用户空间访问内核的唯一手段；除异常和陷入外，它们是内核唯一的合法入口。
标准C或posix   -->  系统调用  --> 内核
在Unix世界里，最流行的应用编程接口是基于Posix标准的。
不同的体系结构具有不同的系统调用.以下多以x86或x86_64为例.

### 4.1 内核中系统调用函数名称
<linux/syscalls.h>
    #define SYSCALL_DEFINEx(x, sname, ...)	__SYSCALL_DEFINEx(x, sname, __VA_ARGS__)
    #define SYSCALL_DEFINE(name)	asmlinkage long sys_##name
    #define SYSCALL_DEFINE0(name)	asmlinkage long sys_##name(void)
    #define SYSCALL_DEFINE1(name, ...) SYSCALL_DEFINEx(1, _##name, __VA_ARGS__)
    #define SYSCALL_DEFINE2(name, ...) SYSCALL_DEFINEx(2, _##name, __VA_ARGS__)
    #define SYSCALL_DEFINE3(name, ...) SYSCALL_DEFINEx(3, _##name, __VA_ARGS__)
    #define SYSCALL_DEFINE4(name, ...) SYSCALL_DEFINEx(4, _##name, __VA_ARGS__)
    #define SYSCALL_DEFINE5(name, ...) SYSCALL_DEFINEx(5, _##name, __VA_ARGS__)
    #define SYSCALL_DEFINE6(name, ...) SYSCALL_DEFINEx(6, _##name, __VA_ARGS__)

其中的x代表参数个数，0代表无参；同时可以知道最多6个参数. 
asmlinkage限定词通知编译器仅从栈中提取参数.
返回值通常为long值，0通常表示正确, 负数通常表示错误.

eg.	系统调用函数getpid()
    SYSCALL_DEFINE0(getpid)		// <kernel/timer.c>
    // asmlinkage long sys_getpid(void)	// 展开后
    {
        return task_tgid_vnr(current);
    }
    由上可知，用户态系统调用名称为getpid，内核中称为sys_getpid。

### 4.2 系统调用表
每个系统调用唯一对应一个系统调用号。其对应关系存储在相应体系架构的数组系统调用表sys_call_table中.

系统调用表定义
    <arch/x86/kernel/syscall_64.c>
        const sys_call_ptr_t sys_call_table[__NR_syscall_max+1] = {
            [0 ... __NR_syscall_max] = &sys_ni_syscall,	// 这种初始化方式很特别。成员均始化为sys_in_syscall()，即"未实现"的系统调用，它只返回-ENOSYS.
        };
    
    <kernel/sys_ni.c>
        asmlinkage long sys_ni_syscall(void)	{	return -ENOSYS;		}

系统调用表初始化
    <arch/x86/syscall_table_32.s>
        ENTRY(sys_call_table)
        .long sys_restart_syscall	/* 0 - old "setup()" system call, used for restarting */
        .long sys_exit
        .long ptregs_fork
        .long sys_read
        .long sys_write
        ... ...
        .long sys_pwritev
        .long sys_rt_tgsigqueueinfo	/* 335 */
        .long sys_perf_event_open
        .long sys_recvmmsg

系统调用号即数组下标. 
    <asm/unistd.h>
        #define __NR_restart_syscall      0
        #define __NR_exit		  1
        #define __NR_fork		  2
        ......
        #define __NR_perf_event_open	336
        #define __NR_recvmmsg		337

通过系统调用号，获取到相应的系统调用的函数的首地址，而不是获取函数名称.


### 4.3 切入内核
系统调用靠软中断切入内核。
x86系统调用过程
    a) 系统调用		系统调用号存于寄存器eax,参数存于寄存器ebx/ecx/edx/esi/edi中或进程上下文栈中。
    b) 软中断切入内核	老的int $0x80指令 或 新的 sysenter指令(效率更高)
    c) system_call()	取出系统调用号及参数，根据系统调用号返回系统调用处理程序首地址.
    d) 相应函数


### 4.4 内核中加入新的系统调用	
1) 系统调用表末尾增加一个表项，即最大个数 __NR_syscall_max+=1
2) 添加系统调用号. <asm/unistd.h> 在此头文件中添加，代表所有体系结构都支持此体系结构。
3) 实现系统调用函数. <kernel/sys.c>


eg.	实现系统调用long foo() ;

    核心态
    1) <arch/x86/syscall_table_32.s>
        ENTRY(sys_call_table)
            ... ...
            .long sys_foo
    2) <asm/unistd.h>
        ... ...
        #define __NR_recvmmsg		337
        #define __NR_foo		338
    3 实现
        函数定义体在<kernel/sched.c>文件中实现。
            asmlinkage long sys_foo(void)
            {
                prink( "sys_foo system call test .\n" ) ;
                return	0 ;
            }
    
    用户态
    系统调用是以宏形式存在.以open为例.
        <asm/unistd.h>
            #define	NR_open	5
            _syscall3( long, open, const char* filename, int, flags, int, mode )
    foo()实现
        <asm/unistd.h>
            #define	NR_foo	283
            _syscall0( long, foo )
    
    调用
        printf( "foo : %ld\n", foo() ) ;


​	

## 6. 内核数据结构
链表类型
    单向链表
    单向循环链表
    双向链表
    双向循环链表

### 1. 内核链表
内核链表是双向循环链表。
linux内核链表不是将数据结构嵌入链表，而是将链表节点嵌入数据结构。

<linux/list.h>

声明链表
    struct list_head {
        struct list_head *prev;
        struct list_head *next;
    };


​	
​	配合示例
​		struct Test {
​			int	a ;
​			int	b ;
​			struct list_head	list ;		// 嵌入到数据结构中
​		} ;


​	
​	结构成员偏移
​		#define offsetof(type, member) \
​			((size_t)(&((type *)0)->member))
​	
​		eg.	int	offset = offsetof( struct Test, list ) ;		// offset其结果为8


​	
​	根据结构体中成员偏移获取结构体对象地址
​		#define container_of(ptr, type, member) \
​			( (type *)((size_t)(ptr) - offsetof(type, member)) )
​		#define list_entry(ptr, type, member)	container_of(ptr, type, member)
​		返回节点指针所嵌入的结构地对象的首地址
​	
​		eg.	struct list_head*	p = .... ;
​			struct Test*	test = list_entry( p, struct Test, list ) ;


​		
​	两种方式定义链表
​		#define LIST_HEAD_INIT(name) { &(name), &(name) }
​		static inline void INIT_LIST_HEAD(struct list_head *list)


​	
​	链表头
​		#define LIST_HEAD(name) \
​			struct list_head name = LIST_HEAD_INIT(name)


​	
​	在链表头部增加一个节点
​		static inline void list_add(struct list_head *new, struct list_head *head) ;


​	
​	在链表尾部增加一个节点
​		static inline void list_add_tail(struct list_head *new, struct list_head *head) ;


​	
​	从链表中删除一个节点
​		static inline void list_del(struct list_head *entry) ;


​	
​	删除的此节点被重新初始化，因此还可以继续被使用.
​		static inline void list_del_init(struct list_head *entry)
​		{
​			__list_del(entry->prev, entry->next);
​			INIT_LIST_HEAD(entry);
​		}


​	
​	把节点从一个链表移动到另一个链表
​		static inline void list_move(struct list_head *list, struct list_head *head)
​		从一个链表中移除list节点，加入到另一链表的head节点后。


​	
​	把节点从一个链表移动到另一个链表末尾
​		static inline void list_move_tail(struct list_head *list, struct list_head *head)
​		从一个链表中移除list节点，加入到另一链表的head节点前。


判断链表是否为空
    static int list_is_empty(struct list_head *head) ;


​	
​	链表合并
​		static void list_splice(const struct list_head *list, struct list_head *head)
​		将list链表插入到head元素后面
​		
​		static void list_splice_tail(const struct list_head *list, struct list_head *head)
​		将list链表插入到head元素前面


​	
​	遍历列表
​		#define list_for_each(ptr, head) \
​			for (ptr = (head)->next; \
​			ptr != head; \
​			ptr = (ptr)->next)
​		用临时链表节点指针ptr遍历列表head
​		
​		#define list_for_each_safe(ptr, tmp, head) \
​			for (ptr = (head)->next, tmp = (ptr)->next; \
​			ptr != head; \
​			ptr = tmp, tmp = (tmp)->next)
​		用临时节点指针ptr和tmp遍历列表head, 遍历过程中找到匹配的节点可以进行节点删除操作，但这个遍历本身没有删除功能.
​		
​    #define list_for_each_entry(ptr, head, member) \
​        for (ptr = list_entry((head)->next, typeof(*ptr), member); \
​        &(ptr)->member != head; \
​        ptr = list_entry((ptr)->member.next, typeof(*ptr), member))
​    用临时结构体对象指针ptr遍历列表head

    #define list_for_each_entry_safe(ptr, tmp, head, member) \
        for (ptr = list_entry((head)->next, typeof(*ptr), member), \
        tmp = (ptr)->member.next; \
        &(ptr)->member != head; \
        ptr = list_entry(tmp, typeof(*ptr), member), tmp = (tmp)->next)
    用临时节点指针ptr和tmp遍历列表head, 遍历过程中找到匹配的节点可以进行节点删除操作，但这个遍历本身没有删除功能.
    
    #define list_for_each_entry_reverse(ptr, head, member)			\
        for (ptr = list_entry((head)->prev, typeof(*ptr), member);	\
        &ptr->member != (head); 	\
        ptr = list_entry(ptr->member.prev, typeof(*ptr), member))
    用临时结构体对象指针cur反向遍历列表head
    
    ... ... 此处省略其他函数


### 2. 队列
FIFO	先进先出
kfifo	<linux/kfifo.h>	<linux/kfifo.c>

kfifo结构体
    struct kfifo {
        unsigned char *buffer;	/* the buffer holding the data */
        unsigned int size;	/* the size of the allocated buffer */
        unsigned int in;	/* data is added at offset (in % size) */
        unsigned int out;	/* data is extracted from off. (out % size) */
    };


静态创建队列
    DECLARE_KFIFO(name, size) 
    创建大小为size，名称为name的队列，size必须为2的幂。
    
    INIT_KFIFO(name)
    创建name队列

动态创建队列
    void kfifo_init(struct kfifo *fifo, void *buffer, unsigned int size);
    创建队列fifo, 使用buffer空间为fifo队列空间,size必须为2的幂

    int kfifo_alloc(struct kfifo *fifo, unsigned int size, gfp_t gfp_mask);
    创建队列fifo,同时分配内存空间, 空间大小为size, size必须为2的幂, gfp_mask内存分配特性.

清空数据但不销毁空间
    
清空数据同时销毁空间
    void kfifo_free(struct kfifo *fifo);
    fifo_alloc的反函数


​	
​	填充数据
​		int kfifo_in(struct kfifo *fifo, const void *from, unsigned int len);
​		拷贝from缓冲区len个字节数据到fifo队列中.返回实际拷贝个数，包括fifo空间不足的情况。
​	
​	剪切提取数据
​		unsigned int kfifo_out(struct kfifo *fifo,void *to, unsigned int len);
​		剪切fifo中len个字节数据到to缓冲区中，返回实际拷贝个数，包括len大与fifo大小的情况。
​		
提取数据
​        unsigned int kfifo_out_peek(struct kfifo *fifo,void *to, unsigned int len, unsigned offset);
​        拷贝fifo中从offset开始的len个字节数据到to缓冲区中，返回实际拷贝个数，包括len大与fifo大小的情况。
​        
长度
​    static inline unsigned int kfifo_size(struct kfifo *fifo) ;
​    获取fifo容量
​    
​    static inline unsigned int kfifo_len(struct kfifo *fifo) ;
​    获取fifo中数据大小
​    
​    static inline unsigned int kfifo_avail(struct kfifo *fifo) ;
​    获取fifo中剩余空间
​    
​    static inline int kfifo_is_empty(struct kfifo *fifo) ;
​    非0值代表空，0代表非空。
​    
​    static inline int kfifo_is_full(struct kfifo *fifo) ;
​    非0值代表满，0代表不满。


### 3. 映射			 
### 4. 二叉树
这两节实在搞不定，回头看。
            


## 7. 中断和中断处理

### 1. 中断			 
中断控制器是个简单的电子芯片，其作用是将多路中断管线，采用复用技术只通过一个和处理器连接的管线与处理器通信。当接收到一个中断后，中断控制器会给处理器发送一个电信号。
中断号——中断请求线(IRQ)，每个设备中断都有一个唯一的数字标志。

异常	它与中断不同，它在产生时必须考虑处理器时钟同步，它也常常成为同步中断。在处理器执行到由于编程失误而导致的错误指令(如被0除)

多个不同设备，或者说不同的中断服务程序可以共享同一个中断线。
    $ cat /proc/interrupts
        CPU0       CPU1       CPU2       CPU3       
    0:       2130          0          0          0   IO-APIC-edge      timer
    1:     297678          0          0          0   IO-APIC-edge      i8042
    3:          1          0          0          0   IO-APIC-edge    
    4:          1          0          0          0   IO-APIC-edge    
    7:          0          0          0          0   IO-APIC-edge      parport0
    8:         23          0          0          0   IO-APIC-edge      rtc0
    9:          0          0          0          0   IO-APIC-fasteoi   acpi
    12:    170767          0          0          0   IO-APIC-edge      i8042
    14:         0          0          0          0   IO-APIC-edge      ata_piix
    15:   4587257          0          0          0   IO-APIC-edge      ata_piix
    16:    213836          0          0          0   IO-APIC-fasteoi   Ensoniq AudioPCI
    17:    318506          0          0          0   IO-APIC-fasteoi   ehci_hcd:usb1, ioc0
    18:       187          0          0          0   IO-APIC-fasteoi   uhci_hcd:usb2
    19:    541649          0          0          0   IO-APIC-fasteoi   eth0
    24:         0          0          0          0   PCI-MSI-edge      pciehp
    ... ...
第一列为中断号，二三四五列代表cpu为4核的，数字代表设备在某个核上产生的中断次数，第六列为中断控制器类型,第七列为产生中断的设备。
XT-PIC为PC上标准的可编程控制器，在具有I/O APIC的系统上，大多数会使用IO-APIC-level或IO-APIC-edge作为自己的中断控制器。
上面红色部分 ehci_hcd:usb1, ioc0 表示有两个设备在公用同一中断线17.


​		
### 2. 中断服务程序
中断处理程序——中断服务例程——ISR——中断上下文
中断上下文中不可以阻塞和睡眠
中断上下文被切割伤上半部和下半部两部分
    上半部执行执行要求严格时限的工作
    下半部执行允许稍后完成的工作
    
注册中断处理函数
    int request_irq(unsigned int irq, irq_handler_t handler, unsigned long flags,const char *name, void *dev);
        irq	要分配的中断号
        handler	要注册的中断处理程序入口，其函数原型为
            typedef irqreturn_t (*irq_handler_t)(int, void *);
        flags	可为0或以下标志的位掩码
            IRQF_DISABLED	指定内核在处理中断处理程序期间，禁止一切其他中断发生。这个比较狠。
            IRQF_SAMPLE_RANDOM	
            IRQF_TIMER	内核为系统定时器的中断处理而准备的。
            IRQF_SHARED	多个中断处理程序可共享中断线，用参数dev来区分不同的中断处理程序路径。
        name	描述设备，一般为设备名称。
        dev	共享同一个irq时使用，用于区分不同的设备，一般为设备自己特殊结构地对象指针。当然也可以为NULL指针。
    此函数本身能够引起睡眠，因此中断上下文中禁用。

释放中断处理函数
    void free_irq(unsigned int irq, void *dev);
        注销 匹配的 符和irq中断号和dev设备的中断处理程序，并禁用中断线。

中断处理函数
    typedef irqreturn_t (*irq_handler_t)(int irq, void *dev);
        此函数即request_irq函数的第二个参数。
        dev通过request_irq第5个参数传递过来的参数，用于区分共享同一中断处理程序的多个不同设备。它通常是设备的数据结构对象指针。
        返回值	irqreturn_t
            成功返回IRQ_HANDLED，失败返回IRQ_NONE,如响应中断时发现设备类型不匹配，或设备没有注册中断处理函数.
        此函数一般声明为static，防止文件外访问。
        linux中断处理程序为无须重入的，函数进入第一步一般为屏蔽中断线中断响应。
        对于共享的中断类型，中断到来时，会依次调用在该中断线上注册的每一个处理程序。

### 3. 中断响应	
中断上下文中没有current当前进程的概念,current宏只专属于进程上下文。
目前的内核中，每个处理器核有一个内核栈，大小为1页，即4KB.所有中断处理程序共享使用这些(多核时)内核栈空间。中断处理程序运行在某个核上时，独享该核上的内核栈空间.

内核中存在全局中断线描述符表irq_desc[irq]，

    struct irq_desc {
        ... ...
        struct irqaction	*action;	/* IRQ action list */
        ... ...
    } ;
    
    struct irqaction {
        irq_handler_t handler;	// 即前文注册后的中断处理函数
        ... ...
        struct irqaction *next;	// 共享中断线的处理程序是链表形式存在
        int irq;
        ... ...
    };


​	
​	当中断到来时，处理器立即停止正在做的事，关闭中断系统，切换为中断模式，然后跳到预定义的内存地址运行响应代码。这个位置早起是由汇编实现(<arch\x86\kernel\entry_32.S>或<arch\x86\kernelengry_64.S>)的，现在用C来实现，即函数do_IRQ  .
​		<arch\x86\include\asm\irq.h>
​		unsigned int do_IRQ(struct pt_regs *regs);
​	参数regs保存的是当前处理器现场,irq也是其中之一。因此可以通过此参数获得irq号。
​	
​	中断响应过程
​		do_IRQ
​		   -->  handle_irq
​		      -->  generic_handle_irq_desc
​		         -->  __do_IRQ
​		            -->  ...
​		               -->  handle_IRQ_event 
​	
​    <kernel/irq/handler.c>	
​    irqreturn_t handle_IRQ_event(unsigned int irq, struct irqaction *action)
​    {
​        ... ...		
​        do {
​            trace_irq_handler_entry(irq, action);
​            ret = action->handler(irq, action->dev_id);
​            trace_irq_handler_exit(irq, action, ret);
​    
​            switch (ret) {
​            case IRQ_WAKE_THREAD:
​                ... ...
​            case IRQ_HANDLED:
​                ... ...
​            default:
​                break;
​            }
​    
​            retval |= ret;
​            action = action->next; 
​        } while (action);
​        ... ... 
​        return retval;
​    }
由上可知，中断响应将当前中断线上注册的所有共享的中断处理程序都遍历一遍，找到匹配的中断处理程序，进而运行。

### 4. 中断控制

控制处理器核中断
    local_irq_disable()		禁止当前处理器核中断, cli汇编指令。
    local_irq_enable()		使能当前处理器核中断, set汇编指令。
    
    unsigned long flags ;
    local_irq_save(flags)		禁止当前处理器核中断，同时保存处理器现场。（原来的设置也有可能是禁止的）
    local_irq_restore(flags)	恢复原来当前处理器核中断设置.（原来的设置是禁止的，恢复后依旧是禁止的；原来激活的，恢复后依旧是激活的）

控制中断线中断
    void disable_irq(unsigned int irq);			禁止irq中断线中断，等待此中断线上的所有其他处理程序完成后才返回。
    void disable_irq_nosync(unsigned int irq);		禁止irq中断线中断，不等待此中断线上的处理是否完成，直接返回。
    void synchronize_irq(unsigned int irq);		用于等待irq中断线上所有处理程序完成。
    此三个函数可在中断和进程上下文使用，不会引起睡眠。
    
    void enable_irq(unsigned int irq);			使能irq中断线中断。
    以上函数可以嵌套，但禁止和使能要成对。比如禁止两次要求使能两次。

中断状态
    irqs_diabled()	本地中断传递被禁止，返回非0值。
    in_interrupt()	执行处于中断上下文(包括下半部在内)返回非0值，处于进程上下文返回0值。
    in_softirq()	执行处于软中断上下文返回非0值。
    in_irq()	当前正在执行中断处理程序返回非0值。


## 8. 下半部和推后执行的工作

### 1. 概述
不仅仅是linux，许多操作系统也把处理硬件中断的过程分为两个部分。上半部简单快速，执行的时候禁止一些或者全部中断；下半部稍后执行，执行期间可以响应所有中断。 这种设计科使系统处于中断屏蔽的时间尽可能短，以提高系统的响应能力。
下半部：BH(bottom half)，任务队列(task queue)， 软中断(softirq), tasklet, 工作队列(work queue)
linux2.6内核中BH和任务队列已经摒弃。目前的内核只有 软中断, tasklet, 工作队列 三种.

### 2. 软中断

a) 软中断处理函数
函数类型为
    void	(*action)(struct softirq_action *);		// 函数指针成员参数为当前结构体对象指针。用法新鲜

b) 软中断处理对象
软中断处理函数被包含在叫做softirq_action的结构体中，此结构体的实例对象我们暂且称之为软中断处理对象.(我自己起的名词)
    
    <linux/interrupt.h>
    struct softirq_action
    {
        void	(*action)(struct softirq_action *);		// 函数指针成员参数为当前结构体对象指针。用法新鲜
    };
action所指向的函数就是软中断处理函数.

eg. 使用示例
    void	read_action(struct softirq_action *)	{	printk( "xxxxxxxxxx\n" ) ;	}
    struct softirq_action	my_softirq = { real_action } ;
    
    最后调用
    my_softirq.action( &my_softirq ) ;

内核中的这种用法很新鲜，这个小技巧可以保证将来在结构体中加入新的域时，无须对所有的软中断处理程序都进行变动。

c) 软中断处理对象数组
内核软中断类型
    <linux/interrupt.h>
    enum
    {
        HI_SOFTIRQ=0,		// 优先级高的tasklets
        TIMER_SOFTIRQ,		// 定时器软中断相关
        NET_TX_SOFTIRQ,	// 网络发送软中断相关
        NET_RX_SOFTIRQ,	// 网络接收软中断相关
        BLOCK_SOFTIRQ,		// block装置
        BLOCK_IOPOLL_SOFTIRQ,	// block输入输出池
        TASKLET_SOFTIRQ,	// 正常优先级的tasklets
        SCHED_SOFTIRQ,		// 调度程序相关
        HRTIMER_SOFTIRQ,	// 搞分辨率定时器相关
        RCU_SOFTIRQ,		// RCU锁定
    
        NR_SOFTIRQS
    };

各种软中断类型的处理对象都被包含在一个全局数组 softirq_vec中.
    <kernel/softirq.c>
    static struct softirq_action softirq_vec[NR_SOFTIRQS];
由上可知所有软中断处理对象都是内核在编译期间就已经静态分配注册的。

此数组最大能承载32个元素,但目前实际只有枚举中的了了几个类型而已。

d) 新软中断处理对象添加及函数注册
我们可以手动添加新的软中断类型到softirq_vec数组中。步骤如下:
    1) 在枚举中的BLOCK_SOFTIRQ和TASKLET_SOFTIRQ之间添加新的软中断类型。
        eg.
            enum
            {
                ...
                BLOCK_SOFTIRQ,
                MY_TEST_SOFTIRQ,	// 添加的新的软中断类型
                TASKLET_SOFTIRQ,
                ...
                NR_SOFTIRQS
            };
    
    2) 实现新的软中断处理函数.
        eg.		void	my_test_softirq_action(struct softirq_action *)	{	printk( "my test softirq action ./n" ) ;	// 只打印一句话 }
        
    3) 用函数open_softirq注册新的软中断处理函数。
    
        <kernel/softirq.c>
        void open_softirq(int nr, void (*action)(struct softirq_action *))
        {
            softirq_vec[nr].action = action;
        }
        
        open_softirq其实就是将数组的新的类型与实际处理函数进行关联。
        
        eg.	open_softirq( MY_TEST_SOFTIRQ, my_test_softirq_action ) ;
    
    4) 软中断是静态实现的，因此还需要重新编译内核, 完成。

下面代码是内核已经实现的软中断类型的注册情况。
    Softirq.c (...\kernel):	open_softirq(HI_SOFTIRQ, tasklet_hi_action);
    Timer.c (...\kernel):	open_softirq(TIMER_SOFTIRQ, run_timer_softirq);
    Dev.c (...\net\core):	open_softirq(NET_TX_SOFTIRQ, net_tx_action);
    Dev.c (...\net\core):	open_softirq(NET_RX_SOFTIRQ, net_rx_action);
    Blk-softirq.c (...\block):	open_softirq(BLOCK_SOFTIRQ, blk_done_softirq);
    Blk-iopoll.c (...\block):	open_softirq(BLOCK_IOPOLL_SOFTIRQ, blk_iopoll_softirq);
    Softirq.c (...\kernel):	open_softirq(TASKLET_SOFTIRQ, tasklet_action);
    Sched.c (...\kernel):	open_softirq(SCHED_SOFTIRQ, run_rebalance_domains);
    Hrtimer.c (...\kernel):	open_softirq(HRTIMER_SOFTIRQ, run_hrtimer_softirq);
    Rcutiny.c (...\kernel):	open_softirq(RCU_SOFTIRQ, rcu_process_callbacks);


​	
e) 软中断处理对象触发标记
​	软中断处理对象必须先被标记(即触发)，然后再在某个恰当的时机去运行。
​	
​		<arch/x86/include/asm>	
​		typedef struct {
​			unsigned int __softirq_pending;
​			... ...
​		} irq_cpustat_t;
​		
​		struct irq_cpustat_t	irq_stat ;	// 它是全局的
​	
softirq_vec数组最大32个元素, 全局变量irq_stat的__softirq_pending成员记录着所有软中断处理对象触发标记情况.
它是32位的数据，某位为1，代表某位被触发标记过, 软中断执行时响应的软中断处理函数可以得到执行.
下面我们称之为pending。
​    
​    <arch/x86/include/asm/hardirq.h>	
​    #define set_softirq_pending(x)	percpu_write(irq_stat.__softirq_pending, (x))	// irq_stat.__softirq_pending = x ; 
​    设置位图pending取值为x，为0时代表清除所有触发标记。
​        
​    #define or_softirq_pending(x)	percpu_or(irq_stat.__softirq_pending, (x))		// irq_stat.__softirq_pending |= x ; 
​    再原在的位图pending基础上或上x，即添加新的触发标记.
​        
​    #define local_softirq_pending()	percpu_read(irq_stat.__softirq_pending)		// return irq_stat.__softirq_pending ;
​    获取所有软中断处理对象标记位图pending。

软中断触发接口
    <include/linux/interrupt.h>
    void raise_softirq(unsigned int nr);
    eg. raise_softirq( MY_TEST_SOFTIRQ ) ;
    
    触发调用过程
        raise_softirq(nr)
            -->  raise_softirq_irqoff(nr)
                -->  __raise_softirq_irqoff(nr)
                    -->  do { or_softirq_pending(1UL << (nr)); } while (0)
                    -->  irq_stat.__softirq_pending << (nr)


f) 软中断执行
软中断处理函数最终会在函数do_softirq中得到执行。

    <kernel/softirq.c>
    
    asmlinkage void do_softirq(void)
    {
        __u32 pending;
        unsigned long flags;
    
        if (in_interrupt())
            return;
    
        local_irq_save(flags);		// 禁止当前处理器核中断响应
    
        pending = local_softirq_pending();	// 获取触发标记位图pending
    
        if (pending)				// 判断是否存在触发标记过的软中断
            __do_softirq();
    
        local_irq_restore(flags);		// 开启当前处理器核中断响应
    }
    
    asmlinkage void __do_softirq(void)
    {
        struct softirq_action *h;
        __u32 pending;
        
        ... ...
    
        local_irq_enable();			// 前面屏蔽了当前处理器核中断，此处又开启中断响应，这是为何？？？？？
        
        pending = local_softirq_pending();	// 获取触发标记位图pending
        
        set_softirq_pending(0);		// 将软中断所有元素触发标记重置为0，为下次再次触发做准备。
        
        h = softirq_vec;			// 指向软中断素组首地址
    
        // 此处一一遍历数组个元素，已经触发标记过的软中断，此处将被调用执行。
        do {
            if (pending & 1) {		// 判断最低位是否为1, 为1代表触发标记过。
                ... ...
                h->action(h);	// 执行软中断处理函数
                ... ...
            }
            h++;				// 指向下一个元素首地址
            pending >>= 1;			// 右移一位，获取下一个元素的触发标记
        } while (pending);
    
        local_irq_disable();			// 禁止当前处理器核中断响应
        ... ...
    }

由上可知:
    1) 软中断在实际执行时的处理器核是可以中断响应的。
    2) 软中断在实际执行时完全没有锁机制存在，因此其他处理器也有同时执行别的软中断的机会。因此两个核访问共享数据时存在一定的问题。
        此种情况，tasklet是更好的选择，它本质也是软中断，但它实现了只能在一个处理器核上运行，其他核上不能运行。


g) 软中断执行时机
1) 从一个硬件中断代码返回后

2) 在ksoftirqd内核线程中
    每个处理器核上都运行辅助处理软中断的内核线程，称为ksoftirqd。
    ksoftirqd/n 代表在第n个核上运行的这样的线程。
    
    源码解析
        <kernel/softirq.c>
        static int run_ksoftirqd(void * __bind_cpu)
        {
            ... ...
    
            while (local_softirq_pending()) {
                ... ...
                do_softirq();
                ... ...
            }
            ... ...
        }

3) 显式检查和调用执行


### 3. tasklet
tasklet本质就是软中断。只有两种，即前面软中断类型枚举中的 HI_SOFTIRQ 和 TASKLET_SOFTIRQ 类型。

a) tasklet对象
<include/linux/interrupt.h>
struct tasklet_struct
{
    struct tasklet_struct *next;	// tasklet对象以链表形式存在
    unsigned long state ;
        // 0 创建对象时使用
        // TASKLET_STATE_SCHED	代表已被触发标记，等待运行
        // TASKLET_STATE_RUN	代表该对象正在某核上运行。
        // 软中断在每个和核上都运行着ksoftirqd内核线程，此时其他核用此值来判断是否需要运行该对象，如果此值是TASKLET_STATE_RUN，则说明此对象正被某个核执行中，直接跳过去执行其他对象。
    atomic_t count;			// count为0时，代表此对象激活状态, 即允许其运行，当然需要先设置触发标记TASKLET_STATE_SCHED。
    void (*func)(unsigned long);	// tasklet处理函数
    unsigned long data;			// tasklet处理函数参数
};

静态创建
    创建激活的
        #define DECLARE_TASKLET(name, func, data) \
        struct tasklet_struct name = { NULL, 0, ATOMIC_INIT(0), func, data }
    
    创建未激活的
        #define DECLARE_TASKLET_DISABLED(name, func, data) \
        struct tasklet_struct name = { NULL, 0, ATOMIC_INIT(1), func, data }

动态创建
    void tasklet_init(struct tasklet_struct *t, void (*func)(unsigned long), unsigned long data)
    {
        t->next = NULL;
        t->state = 0;
        atomic_set(&t->count, 0);
        t->func = func;
        t->data = data;
    }
    由上可知，对象空间需要手动分配和释放.

b) tasklet对象链
内核使用全局tasklet对象链 tasklet_vec(普通) 和 tasklet_hi_vec(高优先级) 来管理tasklet.

    <kernel/softirq.c>
    struct tasklet_head
    {
        struct tasklet_struct *head;
        struct tasklet_struct **tail;
    };
    
    static struct tasklet_head	tasklet_vec ;
    static struct tasklet_head	tasklet_hi_vec ;

c) tasklet调度
tasklet调度	= 添加新tasklet对象入链 + 触发标记tasklet软中断。

    <include/linux/interrupt.h>

1) 调度tasklet_vec链
    static inline void tasklet_schedule(struct tasklet_struct *t) ;
    
    源码
        static inline void tasklet_schedule(struct tasklet_struct *t)
        {
            if (!test_and_set_bit(TASKLET_STATE_SCHED, &t->state))
                __tasklet_schedule(t);
        }
        
        void __tasklet_schedule(struct tasklet_struct *t)
        {
            unsigned long flags;
        
            local_irq_save(flags);
            t->next = NULL;
            *__get_cpu_var(tasklet_vec).tail = t;
            __get_cpu_var(tasklet_vec).tail = &(t->next);	// 将新的对象加入链尾部
            raise_softirq_irqoff(TASKLET_SOFTIRQ);			// 触发标记，以便下次软中断运行时其得到运行
            local_irq_restore(flags);
        }


​	
​	2) 调度tasklet_hi_vec链
​		static inline void tasklet_hi_schedule(struct tasklet_struct *t) ;
​		
​		源码
​			static inline void tasklet_hi_schedule(struct tasklet_struct *t)
​			{
​				if (!test_and_set_bit(TASKLET_STATE_SCHED, &t->state))
​					__tasklet_hi_schedule(t);
​			}
​			
​        void __tasklet_hi_schedule(struct tasklet_struct *t)
​        {
​            unsigned long flags;
​        
​            local_irq_save(flags);
​            t->next = NULL;
​            *__get_cpu_var(tasklet_hi_vec).tail = t;
​            __get_cpu_var(tasklet_hi_vec).tail = &(t->next);		// 将新的对象加入链尾部
​            raise_softirq_irqoff(HI_SOFTIRQ);						// 触发标记，以便下次软中断运行时其得到运行
​            local_irq_restore(flags);
​        }

3) 删除对象
    void tasklet_kill(struct tasklet_struct *t) ;
    等待对象运行完成后，在挂起的队列中删除对象t。
    它会休眠等待，中断上下文禁用。
    
4) 激活和禁止
    不等待对象执行完毕，立即禁止对象。
        void tasklet_disable_nosync(struct tasklet_struct *t) ;
    
    等待对象执行完毕后禁止对象。
        void tasklet_disable(struct tasklet_struct *t) ;
    
    激活对象
        void tasklet_enable(struct tasklet_struct *t)

d) tasklet处理函数注册
前面软中断处理对象添加及函数注册一节已经提到
    Softirq.c (...\kernel):	open_softirq(HI_SOFTIRQ, tasklet_hi_action);
    Softirq.c (...\kernel):	open_softirq(TASKLET_SOFTIRQ, tasklet_action);

源码解析

    <kernel/softirq.c>
    
    static void tasklet_action(struct softirq_action *a)
    {
        struct tasklet_struct *list;
    
        // 操作tasklet对象链时是中断禁止的
        local_irq_disable();
        list = __get_cpu_var(tasklet_vec).head;	// 获取全局对象链头指针
        __get_cpu_var(tasklet_vec).head = NULL;
        __get_cpu_var(tasklet_vec).tail = &__get_cpu_var(tasklet_vec).head;
        local_irq_enable();
    
        while (list) {
            struct tasklet_struct *t = list;
    
            list = list->next;
    
            if (tasklet_trylock(t)) {
                if (!atomic_read(&t->count)) {
                    if (!test_and_clear_bit(TASKLET_STATE_SCHED, &t->state))
                        BUG();
                    t->func(t->data);
                    tasklet_unlock(t);
                    continue;
                }
                tasklet_unlock(t);
            }
    
            // 操作tasklet对象链时是中断禁止的
            local_irq_disable();
            t->next = NULL;
            *__get_cpu_var(tasklet_vec).tail = t;
            __get_cpu_var(tasklet_vec).tail = &(t->next);
            __raise_softirq_irqoff(TASKLET_SOFTIRQ);
            local_irq_enable();
        }
    }
    
    static void tasklet_hi_action(struct softirq_action *a)
    {
        struct tasklet_struct *list;
    
        // 操作tasklet对象链时是中断禁止的
        local_irq_disable();
        list = __get_cpu_var(tasklet_hi_vec).head;	// 获取全局对象链头指针
        __get_cpu_var(tasklet_hi_vec).head = NULL;
        __get_cpu_var(tasklet_hi_vec).tail = &__get_cpu_var(tasklet_hi_vec).head;
        local_irq_enable();
    
        while (list) {
            struct tasklet_struct *t = list;
    
            list = list->next;
    
            if (tasklet_trylock(t)) {
                if (!atomic_read(&t->count)) {
                    if (!test_and_clear_bit(TASKLET_STATE_SCHED, &t->state))
                        BUG();
                    t->func(t->data);
                    tasklet_unlock(t);
                    continue;
                }
                tasklet_unlock(t);
            }
    
            // 操作tasklet对象链时是中断禁止的
            local_irq_disable();
            t->next = NULL;
            *__get_cpu_var(tasklet_hi_vec).tail = t;
            __get_cpu_var(tasklet_hi_vec).tail = &(t->next);
            __raise_softirq_irqoff(HI_SOFTIRQ);
            local_irq_enable();
        }
    }

由上可知，tasklet只不过是两个全局对象链表(对象中含有功能函数指针)而已。使用 tasklet__schedule或tasklet_hi_schedule函数向链表中添加新对象和触发标记。执行时依旧是在注册的tasklet_action和tasklet_hi_action中, 只是此时操作的是这个全局tasklet对象链而已。


### 4. 工作队列
内核开发者们非常反对为满足某些应用而创建新的内核线程。

工作队列子系统
    内核中的一个子系统, 它提供了用于创建内核线程的接口。

工作者线程
    工作队列子系统创建的内核线程。
    工作者线程处于进程上下文空间，包括进程上下文的各种特性，甚至休眠，而软中断和tasklet的内核线程不能休眠。

缺省的工作者线程
    工作队列子系统提供了缺省的工作者线程，称为events/n, n是处理器核编号。因此每个处理器核对应一个线程。
    
工作队列
    队列里面存放着由内核其他部分安插进来的各种任务。

a) 工作队列子系统

<kernel/workqueue.c>
struct workqueue_struct 
{
    struct cpu_workqueue_struct *cpu_wq;	// 缺省的工作者线程数组，数组大小为处理器核心数. 即有几个核就有几个缺省的工作者线程。 
    struct list_head list;
    const char *name;
    int singlethread;
    int freezeable;		/* Freeze threads during suspend */
    int rt;
};

b) 工作者线程

<kernel/workqueue.c>
struct cpu_workqueue_struct {
    spinlock_t lock;					// 保护锁
    struct list_head worklist;			// 工作队列
    wait_queue_head_t more_work;
    struct work_struct *current_work;
    struct workqueue_struct *wq;		// 关联工作队列子系统对象
    struct task_struct *thread;			// 关联的线程
} ;

c) 工作

<kernel/workqueue.h>
struct work_struct {
    atomic_long_t data;
    ... ...
    struct list_head entry;
    work_func_t func;
    ... ...
};

工作处理函数原型
    typedef void (*work_func_t)(struct work_struct *work);

静态创建工作对象
    DECLARE_WORK(n, f)
    n为工作名称，f为工作处理函数
    
动态创建工作对象
    INIT_WORK(_work, _func)
    _work为预先分配好的工作对象指针，_func为工作处理函数。
    此工作对象需要手动分配空间和释放空间。

调度工作对象
    int schedule_work(struct work_struct *work);
    标记




为什么要定义这样的每cpu变量？这其实和linux内部的同步有关，因为如果我们把变量定义成所有cpu都可以访问的，那么就必须用同步机制来保证cpu对这个变量的互斥访问，很明显这是要花费时间的，linux内核为了能够减少这种时间开销，就在每个cpu都定义了一个一模一样的变量，这样每个cpu都使用自己的变量，而不会去访问其它cpu上的变量，也就没有了同步的开销。不过在使用每cpu变量时，必须保证禁用内核抢占。因为内核抢占还是会使每cpu变量产生竞争条件，例如一个内核控制路径获得了它的每cpu变量本地副本的地址，然后它又被抢占跑到另一个cpu上去了，但仍然使用原来cpu上的每cpu变量。


详细请查看 Linux workqueue工作原理.htm

### 5 下半部机制的选择
软中断提供的执行序列化保障最少。也就是说同一个软中断的不同任务，可能并行运行在不同的处理器核上，因此数据共享安全操作要求较高。但单处理器核上，它效率还是最高。
tasklet通过结构体的state成员的TASKLET_STATE_RUN状态，保证了任务执行的序列化。
工作队列，在进程上下文中运行，具有进程上下文的特性。但它牵扯到内核线程甚至上下文切换所带来的开销。

软中断和tasklet都在中断上下文中运行，工作队列在进程上下文中运行。如果你的任务需要休眠，那就选择工作队列。

### 6. 禁止下半部
这里的禁止，只针对在中断上下文中运行的软中断和tasklet，对进程上下文运行的工作队列不起作用。

<include/linux/bottom_half.h>
void local_bh_disable(void) ;	禁止本地处理器核软中断和tasklet
void local_bh_enable(void) ;	激活本地处理器核软中断和tasklet


## 9. 内核同步介绍

临界区	访问和操作共享数据的代码段
竞争条件	两个线程处于同一临界区执行
同步	避免并发和防止竞争条件
伪并发	单处理器上多进程

i++
    得到变量i的值并拷贝到寄存器中
    将寄存器中的值加1
    把i的新值写回到内存中

内核造成并发的原因
    中断
    软中断和tasklet
    抢占
    睡眠及与用户空间的同步
    对称多处理器

死锁	线程相互等待已经相互上锁的资源，导致一直等待。

加锁粒度	包括 精细化保护数据的规模变小 和 全部处理器加锁到各处理器核加锁，用以提高效率。


## 10. 内核同步方法

### 1. 原子操作
原子操作可以保证指令以原子方式执行——执行过程不被打断。

内核提供了两组原子操作接口—— 1） 针对整数；	2) 针对单独的位

a) 针对整数

1) 32位
    <include/linux/types.h>
    typedef struct {
        volatile int counter;
    } atomic_t;

    由上可知，volatile在起关键性作用，意义为 确保不对响应的值进行优化。这点使得原子操作最终接收到正确的内存地址，而不只是一个别名。
    
    尽管Linux支持的所有机器上的整型数据都是32位的，但是使用atomic_t的代码只能将该类型的数据当作24位来用。这是因为在SPARC体系结构上对原子操作缺乏指令级的支持，所以32位int类型的低8位被嵌入一个锁中，利用该锁来避免对原子类型数据的并发访问。
    
    <include/asm-generic/atomic.h>
    静态初始化
        ATOMIC_INIT(int i)
    
    读v值
        int atomic_read(atomic_t *v) ;			
    
    写v值
        void atomic_set(atomic_t *v, int i);
    
    v+=i
        void atomic_add(int i, atomic_t *v);
    
    v-=i
        void atomic_sub(int i, atomic_t *v);
    
    v++
        void atomic_inc(atomic_t *v);
    
    v--
        void atomic_dec(atomic_t *v);
    
    v+=i，return v
        int atomic_add_return(int i, atomic_t *v);
    
    v-=i, return v
        int atomic_sub_return(int i, atomic_t *v);
    
    v++, return v
        int atomic_inc_return(atomic_t *v);
    
    v--, return v
        int atomic_dec_return(atomic_t *v);
    
    return ( v-i == 0 )
        int atomic_sub_and_test(int i, atomic_t *v);
    
    return ( v+i < 0 )
        int atomic_add_negative(int i, atomic_t *v);
    
    return ( v-1 == 0 )
        int atomic_dec_and_test(atomic_t *v);
    
    return ( v+1 == 0 )
        int atomic_inc_and_test(atomic_t *v);


2) 64位
    <include/asm-generic/types.h>
    typedef struct {
        volatile long counter;		// 此处，不同的cpu架构，有的地方类型是longlong，此处以long为例子
    } atomic64_t;
    
    <include/asm-generic/atomic64.h>
    静态初始化
        ATOMIC_INIT(long i)
    
    读v值
        long atomic64_read(atomic64_t *v) ;			
    
    写v值
        void atomic64_set(atomic64_t *v, long i);
    
    v+=i
        void atomic64_add(long i, atomic64_t *v);
    
    v-=i
        void atomic64_sub(long i, atomic64_t *v);
    
    v++
        void atomic64_inc(atomic64_t *v);
    
    v--
        void atomic64_dec(atomic64_t *v);
    
    v+=i，return v
        long atomic64_add_return(long i, atomic64_t *v);
    
    v-=i, return v
        long atomic64_sub_return(long i, atomic64_t *v);
    
    v++, return v
        long atomic64_inc_return(atomic64_t *v);
    
    v--, return v
        int atomic64_dec_return(atomic64_t *v);
    
    return ( v-i == 0 )
        long atomic64_sub_and_test(long i, atomic64_t *v);
    
    return ( v+i < 0 )
        long atomic64_add_negative(long i, atomic64_t *v);
    
    return ( v-1 == 0 )
        long atomic64_dec_and_test(atomic64_t *v);
    
    return ( v+1 == 0 )
        long atomic64_inc_and_test(atomic64_t *v);


a) 针对位
<asm/bitop.h>

设置addr数据的第nr位为1
    void set_bit(int nr, void *addr);

清空addr数据的第nr位
    void clear_bit(int nr, void *addr) ;
    
翻转addr数据的第nr位
    void change_bit(int nr, void *addr);

设置addr数据的第nr位，并返回原先的值
    int test_and_set_bit(int nr, void *addr);

清空addr数据的第nr位，并返回原先的值
    int test_and_clear_bit(int nr, void *addr);

翻转addr数据的第nr位，并返回原先的值
    int test_and_change_bit(int nr, void *addr);

返回addr数据的第nr位
    int test_bit(int nr, void *addr);

返回addr数据中第一个1的位置，搜索范围为size
    int find_first_bit(unsigned long *addr,unsigned int size);

返回addr数据中第一个0的位置，搜索范围为size
    int find_first_zero_bit(unsigned long *addr,unsigned int size);
    
非原子位函数	功能上述操作对应，但其名字前缀多两个下划线。
    unsigned long __ffs(unsigned long word) ;	find first set bit in word
    unsigned long ffz(unsigned long word) ;		find first zero bit in word
    unsigned long __fls(unsigned long word) ;	find last set bit in word

令人感到奇怪的是位操作的数据是以普通的内存地址指针形式存在。

### 2. 自旋锁
自旋锁	试图上锁的线程会自旋(自循环)等待已经加锁的线程释放锁。
eg. 自旋锁原理
    某人拿到钥匙进入房间。另一个来到门前，无法拿到钥匙，间隔一段时间来查看一下是否第一个人已经出来放下钥匙，循环往复。第一个出来，放下钥匙后，此人才能拿到钥匙进入房间。
自旋锁尽量应用在短时间任务。

自旋锁会禁止内核抢占，甚至有些函数会禁止中断。
自旋锁可以使用在中断处理程序中。在中断处理程序中使用自旋锁时，一定要在获取锁之前，首先禁止本地中断。

选项CONFIG_DEBUG_SPINLOCK可用来调试自旋锁。
自旋锁不可递归使用

    <include/linux/spinlock.h>
    
    struct spinlock_t;


    静态初始化
        DEFINE_SPINLOCK(x)
    
    动态初始化		
        spin_lock_init(x)
    
    获取指定的自旋锁
        void spin_lock(spinlock_t *lock) ;
        底层调用了preempt_disable();
    
    释放指定的锁	
        void spin_unlock(spinlock_t *lock) ;
        底层调用了preempt_enable();
    
    禁止本地中断并获取指定的锁
        void spin_lock_irq(spinlock_t *lock) ;
        底层调用了local_irq_enable()和preempt_enable().
    
    释放指定的锁，并激活本地中断
        void spin_unlock_irq(spinlock_t *lock)
        底层调用了local_irq_disable()和preempt_disable().


​		
​		保存本地中断的当前状态，禁止本地中断，并获取指定的锁   
​			#define spin_lock_irqsave(lock, flags) ...
​			底层调用了local_irq_save(flags)和preempt_disable();
​		
​		释放指定的锁，并让本地中断恢复到以前的状态
​			void spin_unlock_irqrestore(spinlock_t *lock, unsigned long flags)
​			底层调用了local_irq_restore(flags)和preempt_enable();
​		
​    试图获取指定的锁
​        int spin_trylock(spinlock_t *lock)
​        如果未获取则返回非0
​    
​    判断是否上锁
​        int spin_is_locked(spinlock_t *lock) ;
​        如果指定的锁当前正在被获取则返回非0，否则返回0
​    
​    禁止所有下半部的执行，并获取指定的锁.
​        void spin_lock_bh(spinlock_t *lock) ;
​        函数底层调用了preempt_disable();
​    
​    释放指定的锁，允许下半部的执行
​        void spin_unlock_bh(spinlock_t *lock) ;

当下半部和进程上下文共享数据时，需要加锁的同时还要禁止下半部执行；当中断处理程序和下半部共享数据时，需要加锁的同时还要禁止中断；当数据被两个不同种类的tasklet共享或软中断共享时，没有必要禁止下半部。


### 3. 读-写自旋锁
读写锁	自旋锁的一种。读操作多于写操作的环境中，效率极高。
读锁	共享锁，多读锁可同时存在
写锁	排斥锁，写锁唯一。
读锁和写锁不共存。


​	
​	<include/linux/rwlock.h>
​	struct rwlock_t;	
​	
​	动态初始化
​		rwlock_init(lock)
​	
获取指定的读锁	
​    read_lock(lock) ;

禁止本地中断并获取指定的读锁
    read_lock_irq(lock);		

保存本地中断的当前状态，禁止本地中断并获取指定的读锁
        read_lock_irqsave(lock,flags);

释放指定的读锁
    read_unlock(lock);

释放指定的读锁，并激活本地中断
    read_unlock_irq(lock)

释放指定的读锁，并让本地中断恢复到以前的状态
    read_unlock_irqrestore(lock,flags);

获取指定的写锁
    write_lock(lock);

禁止本地中断并获取指定的写锁
    write_lock_irq(lock);

保存本地中断的当前状态，禁止本地中断并获取指定的写锁
    write_lock_irqsave(lock,flags);

释放指定的写锁
    write_unlock(lock);

释放指定的写锁，并激活本地中断
    write_unlock_irq(lock);

释放指定的写锁，并让本地中断恢复到以前的状态
    write_unlock_irqrestore(lock,flags);

试图获得指定的写锁；如果写锁不可用，返回非0值
    write_trylock(lock);

如果指定的锁当前已被持有，该函数返回非0值，否则返回0
    rw_is_locked(lock);

... ...


### 4. 信号量
信号量是一种睡眠锁，如果一个任务试图获得一个不可用(已被占用)的信号量时，信号量会将其推进一个等待队列，然后让其睡眠。这时处理器能重获自由，从而去执行其他代码。当持有信号量的执行路径释放信号量后，处于等待队列中的任务将被唤醒，并获得信号量，得以执行。
eg. 信号量原理
    某人拿到钥匙进入房间。另一个来到门前，无法拿到钥匙，门前排队列表上记下自己的名字，然后就去睡觉了。第一个人出来后，睡觉的人被唤醒，而得以拿到钥匙进入房间。

信号量好比允许睡眠的自旋锁。

信号量不会禁止内核抢占。

数量	也称为使用者数量，即同一时刻，允许的信号量持有者个数。
二值信号量	也称为互斥信号量。信号量使用者数量为1，即在同一时刻，仅允许一个信号量持有者。
计数信号量	信号量使用者数量大与1.
P()	-1	down()	用于获取锁	-1后结果>=0，可以获得信号量锁，进入临界区。
V()	+1	up() 	用于释放锁	+1后结果<0，任务依旧等待队列中等待。


<include/linux/semaphore.h>
struct semaphore {
    spinlock_t		lock;
    unsigned int		count;
    struct list_head	wait_list;
};

静态初始化
    DECLARE_MUTEX(name)

动态初始化
    void sema_init(struct semaphore *sem, int val) ;

    动态初始化没锁定的信号量
    #define init_MUTEX(sem)		sema_init(sem, 1)
    动态初始化已锁定的信号量
    #define init_MUTEX_LOCKED(sem)	sema_init(sem, 0)

试图获得信号量
void down(struct semaphore *sem);
    如果信号已被争用，则进入不可中断睡眠状态，即不可被信号唤醒.此函数不常用。
int down_interruptible(struct semaphore *sem);
    如果信号已被争用，则进入可中断睡眠状态，即可被信号唤醒，返回-EINTR。最常用。
int down_killable(struct semaphore *sem);
    如果信号已被争用，则进入睡眠状态，但可被致命信号唤醒，且被杀死，返回-EINTR.
int down_timeout(struct semaphore *sem, long jiffies);
    如果信号已被争用，在指定的时间jiffies内获取信号量，若超时未获取，返回-ETIME。

尝试获得信号量
int down_trylock(struct semaphore *sem);
    如果信号已被争用，则不进入睡眠状态，而是立刻返回非0值.

释放指定的信号量
extern void up(struct semaphore *sem);
    如果睡眠队列不空，则唤醒其中的一个任务.

... ...


### 5. 读-写信号量
读-写信号量	逻辑和概念和读-写自旋锁基本相同，只是它会引起睡眠，不能用于中断上下文。


​	
​	<include/linux/rwsem.h>
​	struct rw_semaphore {
​		rwsem_count_t		count;
​		spinlock_t		wait_lock;
​		struct list_head	wait_list;
​		... ...
​	};
​	
​	静态初始化
​		DECLARE_RWSEM(name) ;
​	
动态初始化
​    init_rwsem(sem) ;

加读锁
    void down_read(struct rw_semaphore *sem);

试图加读锁
    int down_read_trylock(struct rw_semaphore *sem);

加写锁
    void down_write(struct rw_semaphore *sem);

试图加写锁
    int down_write_trylock(struct rw_semaphore *sem);

释放读锁
    void up_read(struct rw_semaphore *sem);

释放写锁
    void up_write(struct rw_semaphore *sem);

转换读锁为写锁
    void downgrade_write(struct rw_semaphore *sem);

... ...


​	
### 6. 互斥体
互斥体	好比计数为1的信号量，它是实现互斥的特定睡眠锁。

<include/linux/mutex.h>
struct mutex {
    /* 1: unlocked, 0: locked, negative: locked, possible waiters */
    atomic_t		count;
    spinlock_t		wait_lock;
    struct list_head	wait_list;
    ... ...
};

静态初始化
    DEFINE_MUTEX(mutexname)
    
上锁
    void mutex_lock(struct mutex *lock);
    锁不可用时睡眠

解锁
    void mutex_unlock(struct mutex *lock);

试图上锁
    int mutex_trylock(struct mutex *lock);

是否上锁
    int mutex_is_locked(struct mutex *lock) ;
    上锁返回1，否则返回0.

... ...


​	
​	
### 7. 完成变量
完成变量是使两个任务得以同步的简单方法，利用它，内核中一个任务可以发出信号通知另一个任务发生了某个特定事件。如果一个任务要执行一些工作时，另一个任务就会在完成变量上等待，当这个任务完成后，会使用完成变量去唤醒在等待的任务。
eg. 当子进程执行或退出时，vfork系统调用使用完成变量唤醒父进程。

完成变量提供了代替信号量的一个简单解决办法。	
    
<include/linux/completion.h>
struct completion {
    unsigned int done;
    wait_queue_head_t wait;
};	

静态初始化
    DECLARE_COMPLETION(work)

动态初始化
    void init_completion(struct completion *x) ;

等待指定的完成变量接收信号
    void wait_for_completion(struct completion *);

发信号唤醒等待任务
    void complete(struct completion *);

... ...


### 8. 顺序锁	
Seq锁是在2.6内核版本中才引入的一种新型锁。这种锁提供了一种很简单的机制，用于读写共享数据。实现这种锁主要依靠一个序列计数器。当有疑义的数据被写入时，会得到一个锁，并且序列值会增加。在读取数据之前和之后，序列号都被读取。如果读取的序列号相同，说明在读操作进行的过程中没有被写操作打断过。如果读的值是偶数，那么就表明写操作没有发生。Seq锁对写者更有利，只要没有其它写者，写锁总是能被成功获得。另外，挂起的写者会不断地使得读操作循环，直到不再有任何写者持有锁为止。

<include/linux/seqlock.h>
typedef struct {
    unsigned sequence;
    spinlock_t lock;
} seqlock_t;

静态初始化
    DEFINE_SEQLOCK(x)
    
写锁
    void write_seqlock(seqlock_t *sl) ;

释放锁
    void write_sequnlock(seqlock_t *sl) ;

读锁逻辑
    do {
        seq = read_seqbegin(&foo);
        // 要读取的数据
    } while (read_seqretry(&foo, seq));	// 读到释放seq锁后，跳出循环

eg.
    写逻辑
        write_seqlock( &xtime_lock ) ;
        jiffes_64 += 1 ;
        write_sequnlock( &xtime_lock ) ;
    
    读逻辑
        do {
            seq = read_seqbegin( &xtime_lock );
            ret = jiffes_64 ;	// 要读取的数据
        } while (read_seqretry( &xtime_lock, seq));	// 读到释放seq锁后，跳出循环


### 9. 禁止抢占
如果数据对每个处理器是唯一的，这样的数据可能就不需要使用锁来保护，但如果内核是抢占式的，为了防止数据被多个进程以伪并发的方式访问，需要禁止内核抢占.

<include/linux/preempt.h>

禁止内核抢占
    Preempt_disable( )

激活内核抢占并检查和执行被挂起的需要调度的任务
    Preempt_enable( )

激活内核抢占但不再进行调度
    Preempt_enable_no_resched( )

返回抢占计数
    Preempt_count( )

### 10. 顺序和屏障
当处理多处理器之间或硬件设备之间的同步问题时，有时需要在程序代码中以指定的顺序发出读内存和写内存指令。在和硬件交互时，时常需要确保一个给定的读操作发生在其它读或写操作之前。另外，在多处理器上，可能需要按写数据的顺序读数据。但编译器和处理器为了提高效率，可能对读和写重新排序。Linux提供了确保顺序的指令称做屏障。

<arch/x86/include/asm/system.h>

rmb( )		阻止跨跃屏障的载入动作发生重排序
read_barrier_depends( )		阻止跨跃屏障的具有数据依赖关系的载入动作重排序
wmb( )		阻止跨跃屏障的存储动作发生重排序
mb( )		阻止跨跃屏障的载入和存储动作重新排序
smp_rmb( )	在SMP上提供rmb( )功能，在UP上提供barrier( )功能
smp_read_barrier_depends( )	在SMP上提供read_barrier_depends( )功能，在UP上提供barrier( )功能
smp_wmb( )	在SMP上提供wmb( )功能，在UP上提供barrier( )功能
smp_mb( )	在SMP上提供mb( )功能，在UP上提供barrier( )功能
barrier( )	阻止编译器跨屏障对载入或存储操作进行优化


## 11. 定时器和时间管理

### 1. 基本概念

定时器中断	系统定时器(可编程硬件芯片)以固定频率产生的中断。其中断处理程序负责 更新系统时间 和 执行需要周期性运行的任务。

节拍	系统定时器的一次振荡。

节拍率	HZ	系统定时器一秒内震荡次数.
        它可通过编程预置，在系统启动过程中按照HZ值对硬件进行设置。
        x86中默认为100.	
            <asm/param.h>
            #define HZ 100

节拍时间	1/HZ秒	系统定时器一次振荡的时间.这也是定时器中断一次间隔的时间。
        1/100秒	x86一次振荡的时间，即10ms。即x86中断一次的最短时间就是10ms。
        
提高节拍率
    优势	提高了中断抢占等的解析度和准确度。
    劣势	中断明显增多，处理其他工作的时间减少，同时增加了耗电。

jiffies	系统启动以来，所有节拍总数，即系统定时器的总振荡次数。
    <include/linux/jiffies.h>
    extern unsigned long volatile jiffies ;

jiffies/HZ	系统启动以来的总秒数。
    unsigned long time_stamp = jiffies ;	// 现在
    unsigned long next_tick = jiffies + 1 ;	// 从现在开始的第一个节拍
    unsigned long later = jiffies+5*HZ ;	// 从现在开始的5秒
    unsigned long fraction = jiffies+HZ/10 ;	// 从现在开始的1/5秒。

32位的jiffies变量，在始终频率为100HZ的情况下，497天就会溢出。如果是1000HZ，49.7天就会溢出。而如果是64位的jiffies变量的话，任何人都别指望看到它溢出。

jiffies在内存中的表示
    <include/linux/jiffies.h>
    #define __jiffy_data  __attribute__((section(".data")))
    extern u64 __jiffy_data jiffies_64;
    extern unsigned long volatile __jiffy_data jiffies;
    上面意思是，在内存中 jiffies和jiffies_64指向同一个内存地址，并且jiffies是jiffies_64的低32位。
    
    jiffies变量在32位机上是32位的，在64位机上时64位的。
    在32位机上，jiffies只会读取到jiffies_64的低32位。通过 u64 get_jiffies_64(void); 可以读取jiffies_64整个数据的值。
    在64位机上,jiffies和jiffies_64完全相同，都是64位的。

jiffies回绕
    溢出的缘故，会产生回绕出现。解决办法如下：
    
    <include/linux/jiffies.h>
    time_after(a,b)		当a超过b时，返回真。
    time_before(a,b)		当a没超过b时，返回真。
    time_after_eq(a,b)		当a超过和等于b时，返回真。
    time_before_eq(a,b)		当a没超过和等于b时，返回真。
    
    a通常是jiffies。b是要比较的值。
    eg.
        usigned long timeout = jiffies + HZ/2 ;
        if( time_before( jiffies, timeout ) ) {
            /* 没有超时 */
        } else {
            /* 超时 */
        }


​	
​	系统时钟资源
​		系统时间	rtc(硬件实时时钟) --> xtime(内核时间变量)	
​		定时器		x86中定时器中断源	PIT(可编程中断时钟)
​		其他		本地APIC时钟
​				时间戳技术(TSC)

### 2. 时钟中断处理程序
分为两部分 系统结构相关部分 和 体系结构无关部分 。

    相关部分最底线工作
        获得xtime_lock锁，以便对访问jiffies_64和墙上时间xtime进行保护。
        需要时应答或重新设置系统时钟。
        周期性的使用墙上时间更新实时时钟。
        调用体系结构无关的例程：tick_periodic。
    
    无关部分最底线工作
        给jiffies_64变量增加1
        更新资源消耗的统计值，比如当前进程所消耗的系统时间和用户时间。
        执行已经到期的动态定时器。
        执行scheduler_tick()函数。
        更新墙上时间，该时间存放在xtime变量中。
        计算平均负载值。
    
        流程
        static void tick_periodic(int cpu)
        {
            if (tick_do_timer_cpu == cpu) {
                write_seqlock(&xtime_lock);		/* 获取写锁 xtime_lock */
        
                /* Keep track of the next tick event */
                tick_next_period = ktime_add(tick_next_period, tick_period);
        
                do_timer(1);
                /*	void do_timer(unsigned long ticks)
                    *	{
                    *		jiffies_64 += ticks;	/* 给jiffies_64变量增加1 */
                    *		update_wall_time();	/* 更新墙上时钟 */
                    *		calc_global_load();	/* 计算平均负载值 */
                    *	}
                    */
    
                write_sequnlock(&xtime_lock);		/* 释放写锁 xtime_lock */
            }
        
            update_process_times(user_mode(get_irq_regs()));
            /* user_mode(get_irq_regs()) ;  根据cpu寄存器判断cpu当前处于内核空间还是用户空间 */
            -->	void update_process_times(int user_tick)
                {
                    struct task_struct *p = current;
                    int cpu = smp_processor_id();
                
                    account_process_tick(p, user_tick);	/* 记账当前进程在用户空间和内核空间所花费的时间，节拍期间可能还有进程调度，所以这种颗粒细度记账节拍并不是十分准确 */
                    run_local_timers();
                    /*	void run_local_timers(void)
                        *	{
                        *		hrtimer_run_queues();
                        *		raise_softirq(TIMER_SOFTIRQ);	/* 触发标记定时器软中断 */
                        *		softlockup_tick();
                        *	}
                        */
                    
                    rcu_check_callbacks(cpu, user_tick);
                    printk_tick();
                    perf_event_do_pending();
                    scheduler_tick();					/* 负责减少进程时间片计算，并在需要的时候设置 need_resched()标志。
                    run_posix_cpu_timers(p);
                }				
            profile_tick(CPU_PROFILING);
        }






## 12. 内存管理

### 1. 页
MMU	内存管理单元	把虚拟地址转换为物理地址的硬件
页	物理页是内存管理单元的基本管理单位，MMU以页大小为单位来管理系统中的页表，即从虚拟内存的角度看，页也是最小单位。

物理页大小
    32位系统：页大小4KB
    64位系统：页大小8KB

内核用 struct page 结构表示系统中的每个物理页。
    <include/linux/mm_types.h>
    struct page {
        page_flags_t flags;		/* 表示页的状态，32位数据.每一位表示一种状态, 脏的，空的等 */
        atomic_t _count;		/* 正数 页的引用计数，-1 未曾使用， 0 空闲页 */	
        atomic_t _mapcount;
        unsigned long private;	/* 指向私有数据 */
        strcut address_space *mapping;	/* 关联的address_space 对象 */
        pgoff_t index;
        struct list_head lru;
        void *virtual;			/* 页在虚拟内存中的地址, 即页的虚拟地址，动态映射物理页 */
    } ;

### 2. 区		
由于硬件的限制，内核并不能对所有的页一视同仁。Linux必须处理如下两种由于硬件存在缺陷而引起的内存寻址问题：
    1）一些硬件只能用某些特定的内存地址来执行DMA（直接内存访问）。
    2）一些体系结构其内存的物理寻址范围比虚拟寻址范围大得多。这样，就有一些内存不能永久地映射到内核空间上。

由于存在这种限制，内核把具有相似特性的页划分为不同的区（ZONE）
    1）ZONE_DMA——这个区包含的页能用来执行DMA操作。
    2）ZONE_DMA32——同上，不过只能被32位设备访问
    3）ZONE_NORMAL——这个区包含的都是能正常地映射网页。
    4）ZONE_HIGHMEM——这个区包含“高端内存”，其中的页并能不永久地映射到内核地址空间。
    ... ...
    用于DMA的内存必须从ZONE_DMA中进行分配，但是一般用途的内存却既能从ZONE_DMA分配，也能从ZONE_NORMAL分配。
        
Linux把系统的页划分为区，形成不同的内存池，这样就可以根据用途进行分配。
注意，区的划分没有任何物理意义，这只是内核为了管理页而采取的一种逻辑上的分组。

    x86-32上的区		描述					物理内存
    ZONE_DMA		DMA使用的页				<16MB
    ZONE_NORMAL		正常寻址的页				16 ~ 896MB
    ZONE_HIGNMEN		高端内存，动态映射的页		>896MB
    
    x86-64中没有ZONE_HIGHMEN区。


### 3. 以低级物理页为单位分配释放内存的接口

<include/linux/gfp.h>

分配单页
    struct page *alloc_page( unsigned int gfp_mask );
        只分配一页，返回页结构体指针
        
    unsigned long _get_free_page( unsigned int gfp_mask );
        只分配一页，返回页逻辑地址指针
        
    unsigned long get_zeroed_page(unsigned int gfp_mask );
        只分配一页，内容填充为0，返回页逻辑地址指针	

释放单页
    void free_page( unsigned long addr );
        通过逻辑地址进行释放

分配多页
    struct page *alloc_pages( unsigned int gfp_mask, unsigned int order );
        分配 2^order 个连续的物理页，返回第一页结构体指针，出错返回NULL。

    unsigned long __get_free_pages( unsigned int gfp_mask, unsigned int order );
        分配 2^order 个连续的物理页，返回第一页逻辑地址指针。

释放多页	
    void __free_pages( struct page *page, unsigned int order );
        通过页结构体指针进行释放2^order个页
        
    void free_pages( unsigned long addr, unsigned int order );
        通过逻辑地址指针进行释放2^order个页

页和逻辑地址的转换
    void* page_address( struct page *page );
        把页转换成它的逻辑地址。

以后上函数
    返回page结构体指针的函数
        可以在高端内存分配内存，包括 alloc_page 和 alloc_pages .
    返回逻辑地址的函数
        不可以在高端内存分配内存，包括 _get_free_page, get_zeroed_page, __get_free_pages .

### 4. 以字节为单位分配释放内存的接口
a) kmalloc和kfree
<include/linux/slab.h>

分配
    void* kmalloc( size_t size, int gfp_mask );
        返回一个指向内存块的指针，其内存块至少要有 size 大小。
        所分配的内存正在物理上是连续的。
        除非没有足够的内存可用，否则内核总能分配成功。
        在出错时，它返回 NULL。
        
释放
    void kfree( const void *ptr );
        kfree 函数释放由 kmalloc分配出来的内存块。调用 kfree( NULL ) 是安全的。		



b) vmalloc和vfree
分配
    void *vmalloc(unsigned long size) ;
        该函数返回一个指针，指向逻辑上连续的一块内存，其大小至少为size。
        在发生错误时，函数返回NULL。
        函数可能睡眠，因此么不能从中断上下文中进行调用，也不能从其他不允许阻塞的情况下进行调用。
        
    vmalloc 的工作方式是类似于 kmalloc，只不过前者分配的内存虚拟地址是连续的，而物理地址则无需连续。这也是用户空间分配函数的工作方式：由malloc()返回的页在进程的虚拟地址空间内是连续的，但是这并不保证他们在物理RAM中也是连续的。kmalloc()函数确保页在物理地址上是连续。vmalloc函数值确保在虚拟地址空间内是连续的。它通过分配非连续的物理内存块，在修订页表，把内存映射到逻辑地址空间的连续区域中，就能做到这点。
    
    大多数情况下，只有硬件设备需要得到物理地址连续的内存，因为硬件设备存在内存管理单元以外，它根本不理解什么是虚拟地址。尽管仅仅在某些情况下才需要物理上连续的内存块，但是很多内核都有kmalloc()来获取内存，而不是vmalloc()。这主要出于性能方面的考虑。vmalloc()函数为了把物理上不连续的页转换成虚拟地址空间上连续的页，必须专门建立页表项。糟糕的是，通过vmalloc()获得的页必须一个一个地进行映射。因为这些原因，一般是在为了获得大块内存时，例如当模块被动态插入内核时，就把模块装载到由vmalloc()分配的内存上。


​	
​	释放
​		void vfree(const void *addr) ;


### 5. gfp_mask（分配器标志）
用到了gfp_mask分配器标志的函数有 低级页分配函数 和 kmalloc 。
标志分类：行为修饰符、区修饰符及类型。
1）行为修饰符表示内核应当如何分配所需的内存。
    __GFP_WAIT		分配器可以睡眠
    __GFP_HIGH		分配器可以访问紧急事件缓冲池
    __GFP_IO		分配器可以启动磁盘I/O
    __GFP_FS		分配器可以启动文件系统I/O
    __GFP_COLD		分配器可以使用高速缓存中快要淘汰出去的页
    __GFP_NOWARN		分配器将不打印失败警告
    __GFP_REPEAT		分配器在分配失败时重复进行分配，但是这次分配还存在失败的可能
    __GFP_NOFALL		分配器将无限地重复进行分配，分配不能失败
    __GFP_NORETRY	分配器在分配失败时绝不会重新分配
    __GFP_NO_GROW	由slab层内部使用
    __GFP_COMP		添加混合页数据，在hugetlb的代码内部使用

2）区修饰符指明到底从哪一区中进行分配。
    __GFP_DMA		从ZONE_DMA分配
    __GFP_DMA32		从ZONE_DMA32分配
    __GFP_HIGHMEM	从ZONE_HIGHMEM或ZONE_NORMAL分配
    未指定			从ZONE_NORMAL或ZONE_DMA分配
    
3）类型标志 = 行为修饰符+区修饰符的组合.
    类型标志	实际标志		描述
    GFP_ATOMIC	__GFP_HIGH		这个标志用在中断处理程序，下半部，持有自旋锁以及其他不能睡眠的地方
    GFP_NOWAIT	0			与 GFP_ATOMIC 类似，不同之处在于，调用不会退给紧急内存池。这就增加了内存分配失败的可能性
    GFP_NOIO	__GFP_WAIT		这种分配可以阻塞，但不会启动磁盘I/O。这个标志在不能引发更多磁盘I/O时能阻塞I/O代码，可能会导致递归
    GFP_NOFS	__GFP_WAIT｜__GFP_IO	这种分配在必要时可能阻塞，也可能启动磁盘I/O，但不会启动文件系统操作。这个标志在你不能再启动另一个文件系统的操作时，用在文件系统部分的代码中
    GFP_KERNEL	__GFP_WAIT｜__GFP_IO｜__GFP_FS	这是常规的分配方式，可能会阻塞。这个标志在睡眠安全时用在进程上下文代码中。为了获得调用者所需的内存，内核会尽力而为。这个标志应当为首选标志
    GFP_USER	__GFP_WAIT｜__GFP_IO｜__GFP_FS	这是常规的分配方式，可能会阻塞。用于为用户空间进程分配内存时
    GFP_HIGHUSER	__GFP_WAIT｜__GFP_IO｜__GFP_FS｜__GFP_HIGHMEM	从 ZONE_HIGHMEM 进行分配，可能会阻塞。用于为用户空间进程分配内存.
    GFP_DMA	__GFP_DMA		从 ZONE_DMA 进行分配。需要获取能供DMA使用的内存的设备驱动程序使用这个标志

以上各种类型标志的使用场景总结：
    进程上下文，可以睡眠 		使用 GFP_KERNEL
    进程上下文，不可以睡眠 		使用 GFP_ATOMIC，在睡眠之前或之后以 GFP_KERNEL 执行内存分配
    中断处理程序				使用 GFP_ATOMIC
    软中断					使用 GFP_ATOMIC
    tasklet				使用 GFP_ATOMIC
    需要用于DMA的内存，可以睡眠	使用 (GFP_DMA｜GFP_KERNEL)
    需要用于DMA的内存，不可以睡眠	使用 (GFP_DMA｜GFP_ATOMIC)，或者在睡眠之前执行内存分配


​	
### 6. slab层和高速缓存	
linux高速缓存通过slab层来实现的。
对象高速缓存
    高速缓存针对操作的对象的不同，被分为不同的类型，但统称对象高速缓存。
    比如针对进程task_struct的对象高速缓存，针对硬盘inode节点的对象高速缓存等。

高速缓存原理
    结构体对象存储空间的分配和释放是内核中普遍的操作之一。为了便于数据的频繁快速的分配和回收，编程者常常会用到一个空闲链表。该链表包含可供使用的、已经分配好的数据结构块。当代码需要这个数据结构的实例时，可从空闲链表中抓取一个，而不需要分配内存。当不再需要这个数据结构的实例时，就把它放回空闲链表，而不是释放掉。这个意义上说，空闲链表相当于对象高速缓存以便快速存储释放频繁使用的对象类型。

空闲链表即slab链表。slab由一个或多个物理上连续的页组成。每个slab中都承载被缓存的结构体对象。
每个对象高速缓存都有三个slab链表，他们分别是 ： 满slab链表、部分满slab链表 或 空slab链表。当内核需要一个对象时，首先考虑的是部分满的slab，如果不存在部分满的slab则去空的slab分配，如果也不存在空的slab，则内核需要申请页重新分配高速缓存。

图 1. slab 分配器的主要结构

slab数据结构：
    每个高速缓存用kmem_cache结构来表示：
        <include/linux/slab_def.h>
        struct kmem_cache {
            struct kmem_list3 **nodelists;
            ……
        } ;

    缓存区包含三种slab：满，未满，空闲
        <mm/slab.c>
        struct kmem_list3 {
                struct list_head slabs_partial; /* partial list first, better asm code */
                struct list_head slabs_full;
                struct list_head slabs_free;
                ……
        };
    
    每一个slab包含多个对象：
        <mm/slab.c>
        struct slab {
            struct list_head list;
            unsigned long colouroff;
            void *s_mem;            /* including colour offset */
            unsigned int inuse;     /* num of objs active in slab */
            kmem_bufctl_t free;
            unsigned short nodeid;
        };






slab接口
    <mm/slab.c>
    
    创建缓存
        内核函数 kmem_cache_create 用来创建一个新缓存。这通常是在内核初始化时执行的，或者在首次加载内核模块时执行。
        struct kmem_cache *kmem_cache_create (
            const char *name,		/* 缓存名称，proc 文件系统（在 /proc/slabinfo 中）使用它标识这个缓存。*/
            size_t size,			/* 为这个缓存创建的对象的大小 */
            size_t align,		/* 每个对象必需的对齐 */
            unsigned long flags,	/* 为缓存启用的选项 */
            void (*ctor)(void *)) ;	/* 可选的构造函数，用于初始化，它是用户提供的回调函数。 */
            
            flags 参数
                SLAB_RED_ZONE	在对象头、尾插入标志，用来支持对缓冲区溢出的检查。
                SLAB_POISON		使用一种己知模式填充 slab，允许对缓存中的对象进行监视（对象属对象所有，不过可以在外部进行修改）。
                SLAB_HWCACHE_ALIGN	指定缓存对象必须与硬件缓存行对齐。
                ctor 和 dtor 	可选的对象构造器和析构器。构造器和析构器是。
    
    销毁缓存
        void kmem_cache_destroy(struct kmem_cache *cachep) ;


    从缓存中分配一个对象
        void* kmem_cache_alloc( struct kmem_cache *cachep, gfp_t flags );
            从缓存中返回一个对象。注意如果缓存目前为空，那么这个函数就会调用 cache_alloc_refill 向缓存中增加内存。
            cachep	所建立的缓存
            flags
                GFP_USER	为用户分配内存（这个调用可能会睡眠）。
                GFP_KERNEL	从内核 RAM 中分配内存（这个调用可能会睡眠）。
                GFP_ATOMIC	使该调用强制处于非睡眠状态（对中断处理程序非常有用）。
                GFP_HIGHUSER	从高端内存中分配内存。
    
    释放对象
        void kmem_cache_free(struct kmem_cache *cachep, void *objp) ;
            只是将对象objp空间清空，并没有释放空间。


​	
​	slab分配器使用实例
​		此处以task_struct为例子。
​		
​		全局存放task_struct对象的高速缓存指针
​			struct kmem_cache* task_struct_cachep = NULL ;
​		
​    初始化时创建高速缓存
​        <kernel/fork.c>	fork_init()
​        task_struct_cachep = kmem_cache_create("task_struct", sizeof(struct task_struct),
​                ARCH_MIN_TASKALIGN, SLAB_PANIC | SLAB_NOTRACK, NULL);
​        
​    fork()时创建新的进程描述符
​        do_fork()
​            --> dup_task_struct() 
​            struct task_struct *tsk;
​            tsk = alloc_task_struct();
​            if (!tsk)
​                return NULL;
​    
​    进程执行完成后释放
​        free_task_struct()
​        # define free_task_struct(tsk)	kmem_cache_free(task_struct_cachep, (tsk))	
​    
​    销毁进程高速缓存
​        int	err = kmem_cache_destory( task_struct_cachep ) ;
​        if ( err ) ...


​	
### 7. 高端内存的映射

高端内存映射有三种方式：

a) 映射到“内核动态映射空间”
这种方式很简单，因为通过 vmalloc() ，在”内核动态映射空间“申请内存的时候，就可能从高端内存获得页面（参看 vmalloc 的实现），因此说高端内存有可能映射到”内核动态映射空间“ 中。

b) 永久内核映射
如果是通过 alloc_page() 获得了高端内存对应的 page，如何给它找个线性空间？
内核专门为此留出一块线性空间，从 PKMAP_BASE 到 FIXADDR_START ，用于映射高端内存。在 2.4 内核上，这个地址范围是 4G-8M 到 4G-4M 之间。这个空间起叫“内核永久映射空间”或者“永久内核映射空间”。通过 kmap()， 可以把一个 page 映射到这个空间来。由于这个空间是 4M 大小，最多能同时映射 1024 个 page。因此，对于不使用的的 page，及应该时从这个空间释放掉（也就是解除映射关系），通过 kunmap() ，可以把一个 page 对应的线性地址从这个空间释放出来。
内核逻辑地址空间通过线性映射就可以访问到物理低端内存；而内核虚拟地址空间必须通过动态映射才能访问物理高端内存。

映射
    void *kmap(struct page *page) ;
        映射一个给定的page到内核地址空间。说白了就是将实际物理页映射称为虚拟地址。
        这个函数在高端内存和低端内存上都能用。如果page是低端的，它只会单纯的返回该页的虚拟地址；如果page是高端的，则会建立一个永久映射，再返回地址。
        这个函数可以睡眠，因此只能在进程上下文中使用。


​			
​	解除映射
　　	void kunmap(struct page *page) ;

c) 临时映射
内核在 FIXADDR_START 到 FIXADDR_TOP 之间保留了一些线性空间用于特殊需求。这个空间称为“固定映射空间”。在这个空间中，有一部分用于高端内存的临时映射。
这块空间具有如下特点：
    1、 每个CPU 占用一块空间
    2、 在每个CPU 占用的那块空间中，又分为多个小空间，每个小空间大小是 1 个 page，每个小空间用于一个目的，这些目的定义在 kmap_types.h 中的km_type 中。
当要进行一次临时映射的时候，需要指定映射的目的，根据映射目的，可以找到对应的小空间，然后把这个空间的地址作为映射地址。这意味着一次临时映射会导致以前的映射被覆盖。

void *kmap_atomic(struct page *page) ;


### 8. 分配函数的选择
连续的物理页：kmalloc或者低级页分配器
高端内存分配：alloc_pages 指向page结构指针，不是逻辑地址指针。再通过kmap()把高端地址内存映射到内核的逻辑地址空间。
无需连续物理地址：vmalloc 虚拟地址连续物理地址可能不连续，相对存在性能损失
频繁创建和销毁很多较大数据结构：建立slab缓存区，提高对象分配和回收性能。


### 9. 每cpu变量
a) 早期2.4版本时的接口
每cpu变量本身是数组。每cpu变量就是 按照当前处理器号 确定这个数组中的当前元素。
这种方式目前的2.6中依旧在使用。
    unsigned long my_percpu[NR_CPUS] ;	/* 每cpu变量数组 */
    int cpu = get_cpu() ;	/* 获取当前处理器号，同时禁止内核抢占 */
    my_percpu[cpu]++ ;	/* 每cpu变量操作 */
    put_cpu() ;		/* 激活内核抢占 */

b) 新的接口
1. 静态声明，即编译时确定的每cpu变量。
        <include/linux/percpu-defs.h>
        DEFINE_PER_CPU(type, name)
    
    重新编译源码后，即可运行投入使用；同时它不需要释放。
    
    这种方式的每cpu变量，连接程序实际上将它们创建在一个唯一的可执行段 .data.percpu 中，因此它们不能放在模块中定义。
    
2. 动态
    分配
        <include/linux/percpu.h>
        #define alloc_percpu(type)	\
            (typeof(type) __percpu *)__alloc_percpu(sizeof(type), __alignof__(type))
            sizeof(type)		分配空间的实际大小
            __alignof__(type)	按多大空间大小进行对其， __alignof__ 其实就等于 sizeof .

    释放
        <include/linux/percpu.h>
        void free_percpu(void __percpu *__pdata);


​	3. 操作
​		每cpu变量无论是静态声明的，还是动态申请的，操作方式都是一样。
​		
​		操作
​			<include/linux/percpu.h>
​			#define get_cpu_var(var) (*({ \
​				preempt_disable(); \		/* 此处同样禁止内核抢占 */
​				&__get_cpu_var(var); }))
​			
​            eg. get_cpu_var(name)++ ;
​    
​    操作完成
​        <include/linux/percpu.h>
​        #define put_cpu_var(var) do { \
​            (void)&(var); \
​            preempt_enable(); \		/* 激活内核抢占 */
​            } while (0)


​	
## 13. 虚拟文件系统

### 1. 文件系统抽象层


### 2. unix文件系统
unix系统将 文件的相关信息 和 文件内容 这两个概念加以区分。
    文件的相关信息
        也称为文件的元数据，对应的内核结构体为inode(index node)。
    文件内容
        对应的内核结构体 file 。

unix中存在文件、目录项、索引节点、安装点等传统的概念。

为了在不同的文件系统上实现相同的操作方式，内核提供了vfs(虚拟文件系统)概念。

vfs核心概念包括超级块、索引节点、目录项、文件，以及在这些核心概念上响应的操作内容。
    超级块		代表具体的已安装的文件系统
    索引节点	代表一个具体文件
    目录项		代表路径的一个组成部分。
    文件		代表由进程打开的文件

结构体对应
            数据结构体		操作结构体
    超级块		super_block		super_operations
    索引节点	inode			inode_operations
    目录项		dentry			dentry_operations
    文件		file			file_operations

a) 超级块

<linux/fs.h>

struct super_block {
    struct list_head        s_list;            /* 指向超级块链表的指针 */
    dev_t                   s_dev;             /* 设备标识符 */
    unsigned long           s_blocksize;       /* 以字节为单位的块大小 */
    unsigned long           s_old_blocksize;   /* 以位为单位的旧的块大小 */
    unsigned char           s_blocksize_bits;  /* 以位为单位的块大小 */
    unsigned char           s_dirt;            /* 修改脏标志 */
    unsigned long long      s_maxbytes;        /* 文件大小上限 */
    struct file_system_type s_type;            /* 文件系统类型 */
    struct super_operations s_op;              /* 超级块方法 */
    struct dquot_operations *dq_op;            /* 磁盘限额方法 */
    struct quotactl_ops     *s_qcop;           /* 限额控制方法 */
    struct export_operations *s_export_op;     /* 到处方法 */
    unsigned long            s_flags;          /* 挂载标志 */
    unsigned long            s_magic;          /* 文件系统魔数 */
    struct dentry            *s_root;          /* 目录挂载点 */
    struct rw_semaphore      s_umount;         /* 卸载信号量 */
    struct semaphore         s_lock;           /* 超级块信号量 */
    int                      s_count;          /* 引用计数 */
    int                      s_syncing;        /* 文件系统同步标志 */
    int                      s_need_sync_fs;   /* 尚未同步标志 */
    atomic_t                 s_active;         /* 活动引用计数 */
    void                     *s_security;      /* 安全模块 */
    struct list_head         s_dirty;          /* 脏节点链表 */
    struct list_head         s_io;             /* 回写链表 */
    struct hlist_head        s_anon;           /* 匿名目录项 */
    struct list_head         s_files;          /* 被分配文件链表 */
    struct block_device      *s_bdev;          /* 相关块设备 */
    struct list_head         s_instances;      /* 该类型文件系统 */
    struct quota_info        s_dquot;          /* 限额相关选项 */
    char                     s_id[32];         /* 文本名字 */
    void                     *s_fs_info;       /* 文件系统特殊信号 */
    struct semaphore         s_vfs_rename_sem; /* 重命名信号量 */
};

超级块对象中的s_op指向超级块的操作函数表。
struct super_operations {
    struct inode *(*alloc_inode) (struct super_block *sb);	/* 在给定的超级块下创建并初始化一个新的索引节点对象 */
    void (*destroy_inode) (struct inode *);			/* 释放跟定的索引节点 */
    void (*read_inode) (struct inode *);			/* 以inode->i_ino为索引，从磁盘上读取索引节点，并填充内存中对应的索引节点结构的剩余部分 */
    void (*dirty_inode) (struct inode *);			/* VFS在索引节点脏时(被修改)会调用此函数，日志文件系统(ect3等)执行该函数进行日志更新 */
    void (*write_inode) (struct inode *, int);			/* 将给定的索引节点写入磁盘，wait参数知名写操作是否需要同步 */
    void (*put_inode) (struct inode *);				/* 释放给定索引节点 */
    void (*drop_inode) (struct inode *);			/* 在最后一个索引节点被释放后，VFS调用此函数 */
    void (*delete_inode) (struct inode *);			/* 从磁盘上删除给点的索引节点 */
    void (*put_super) (struct super_block *);			/* 在卸载文件系统时由VFS调用，用来释放超级块 */
    void (*write_super) (struct super_block *);		/* 使文件系统的数据元与磁盘上的文件系统同步 */
    int (*sync_fs) (struct super_block *, int);		/* 是文件系统数据源与磁盘上的文件系统同步 */
    void (*write_super_lockfs) (struct super_block *);	/* 此函数首先禁止对文件系统作改变，再使用给定的超级块更新磁盘上的超级块。LVM会调用此函数 */
    void (*unlockfs) (struct super_block *);			/* 对文件系统解除锁定 */
    int (*statfs) (struct super_block *, struct statfs *);	/* VFS通过调用该函数获取文件系统状态，文件系统信息放置在statfs中 */
    int (*remount_fs) (struct super_block *, int *, char *);	/* 当制定新的安装选项重新安装文件系统时，vfs会调用该函数 */
    void (*clear_inode) (struct inode *);			/* 释放索引节点 */
    void (*umount_begin) (struct super_block *);		/* 调用该函数释放索引节点，并清空包含相关数据的所有页面 */
    int (*show_options) (struct seq_file *, struct vfsmount *);
};


b) 索引节点

<include/linux/fs.h>

struct inode {
    struct hlist_node       i_hash;              /* 哈希表 */
    struct list_head        i_list;              /* 索引节点链表 */
    struct list_head        i_dentry;            /* 目录项链表 */
    unsigned long           i_ino;               /* 节点号 */
    atomic_t                i_count;             /* 引用记数 */
    umode_t                 i_mode;              /* 访问权限控制 */
    unsigned int            i_nlink;             /* 硬链接数 */
    uid_t                   i_uid;               /* 使用者id */
    gid_t                   i_gid;               /* 使用者id组 */
    kdev_t                  i_rdev;              /* 实设备标识符 */
    loff_t                  i_size;              /* 以字节为单位的文件大小 */
    struct timespec         i_atime;             /* 最后访问时间 */
    struct timespec         i_mtime;             /* 最后修改(modify)时间 */
    struct timespec         i_ctime;             /* 最后改变(change)时间 */
    unsigned int            i_blkbits;           /* 以位为单位的块大小 */
    unsigned long           i_blksize;           /* 以字节为单位的块大小 */
    unsigned long           i_version;           /* 版本号 */
    unsigned long           i_blocks;            /* 文件的块数 */
    unsigned short          i_bytes;             /* 使用的字节数 */
    spinlock_t              i_lock;              /* 自旋锁 */
    struct rw_semaphore     i_alloc_sem;         /* 索引节点信号量 */
    struct inode_operations *i_op;               /* 索引节点操作表 */
    struct file_operations  *i_fop;              /* 默认的索引节点操作 */
    struct super_block      *i_sb;               /* 相关的超级块 */
    struct file_lock        *i_flock;            /* 文件锁链表 */
    struct address_space    *i_mapping;          /* 相关的地址映射 */
    struct address_space    i_data;              /* 设备地址映射 */
    struct dquot            *i_dquot[MAXQUOTAS]; /* 节点的磁盘限额 */
    struct list_head        i_devices;           /* 块设备链表 */
    union {
        struct pipe_inode_info *i_pipe;       /* 管道信息 */
        struct block_device *i_bdev;          /* 块设备驱动 */
        struct cdev      *i_cdev;             /* 字符设备驱动 */
    };
    unsigned long           i_dnotify_mask;      /* 目录通知掩码 */
    struct dnotify_struct   *i_dnotify;          /* 目录通知 */
    unsigned long           i_state;             /* 状态标志 */
    unsigned long           dirtied_when;        /* 首次修改时间 */
    unsigned int            i_flags;             /* 文件系统标志 */
    unsigned char           i_sock;              /* 可能是个套接字吧 */
    atomic_t                i_writecount;        /* 写者记数 */
    void                    *i_security;         /* 安全模块 */
    __u32                   i_generation;        /* 索引节点版本号 */
    union {
        void            *generic_ip;          /* 文件特殊信息 */
    } u;
};


​	
​	struct inode_operations {
​		int (*create) (struct inode *, struct dentry *,int);
​			/*VFS通过系统调用create()和open()来调用该函数，从而为dentry对象创建一个新的索引节点。在创建时使用mode制定初始模式*/
​		struct dentry * (*lookup) (struct inode *, struct dentry *);
​			/*该韩式在特定目录中寻找索引节点，该索引节点要对应于dentry中给出的文件名*/
​		int (*link) (struct dentry *, struct inode *, struct dentry *);
​			/*该函数被系统调用link()电泳，用来创建硬连接。硬链接名称由dentry参数指定，连接对象是dir目录中ld_dentry目录想所代表的文件*/
​		int (*unlink) (struct inode *, struct dentry *);
​			/*该函数被系统调用unlink()调用，从目录dir中删除由目录项dentry制动的索引节点对象*/
​		int (*symlink) (struct inode *, struct dentry *, const char *);
​			/*该函数被系统电泳symlik()调用，创建符号连接，该符号连接名称由symname指定，连接对象是dir目录中的dentry目录项*/
​		int (*mkdir) (struct inode *, struct dentry *, int);
​			/*该函数被mkdir()调用，创建一个新鲁姆。创建时使用mode制定的初始模式*/
​		int (*rmdir) (struct inode *, struct dentry *);
​			/*该函数被系统调用rmdir()调用，删除dir目录中的dentry目录项代表的文件*/
​		int (*mknod) (struct inode *, struct dentry *, int, dev_t);
​			/*该函数被系统调用mknod()调用，创建特殊文件(设备文件、命名管道或套接字)。要创建的文件放在dir目录中，其目录项问dentry，关联的设备为rdev，初始权限由mode指定*/
​		int (*rename) (struct inode *, struct dentry *,struct inode *, struct dentry *);
​			/*VFS调用该函数来移动文件。文件源路径在old_dir目录中，源文件由old_dentry目录项所指定，目标路径在new_dir目录中，目标文件由new_dentry指定*/
​		int (*readlink) (struct dentry *, char *, int);
​			/*该函数被系统调用readlink()调用，拷贝数据到特定的缓冲buffer中。拷贝的数据来自dentry指定的符号链接，最大拷贝大小可达到buflen字节*/
​		int (*follow_link) (struct dentry *, struct nameidata *);
​			/*该函数由VFS调用，从一个符号连接查找他指向的索引节点，由dentry指向的连接被解析*/
​		int (*put_link) (struct dentry *, struct nameidata *);
​			/*在follow_link()调用之后，该函数由vfs调用进行清楚工作*/
​		void (*truncate) (struct inode *);
​			/*该函数由VFS调用，修改文件的大小，在调用之前，索引节点的i_size项必须被设置成预期的大小*/
​		int (*permission) (struct inode *, int);
​			/*该函数用来检查给低昂的inode所代表的文件是否允许特定的访问模式，如果允许特定的访问模式，返回0，否则返回负值的错误码。多数文件系统都将此区域设置为null，使用VFS提供的通用方法进行检查，这种检查操作仅仅比较索引及诶但对象中的访问模式位是否和mask一致，比较复杂的系统，比如支持访问控制链(ACL)的文件系统，需要使用特殊的permission()方法*/
​		int (*setattr) (struct dentry *, struct iattr *);
​			/*该函数被notify_change调用，在修改索引节点之后，通知发生了改变事件*/
​		int (*getattr) (struct vfsmount *, struct dentry *, struct kstat *);
​			/*在通知索引节点需要从磁盘中更新时，VFS会调用该函数*/
​		int (*setxattr) (struct dentry *, const char *, const void *, size_t, int);
​			/*该函数由VFS调用，向dentry指定的文件设置扩展属性，属性名为name，值为value*/
​		ssize_t (*getxattr) (struct dentry *, const char *, void *, size_t);
​			/*该函数被VFS调用，向value中拷贝给定文件的扩展属性name对应的数值*/
​		ssize_t (*listxattr) (struct dentry *, char *, size_t);
​			/*该函数将特定文件所有属性别表拷贝到一个缓冲列表中*/
​		int (*removexattr) (struct dentry *, const char *);
​			/*该函数从给定文件中删除指定的属性*/
​	};

c) 目录项

<include/linux/dcache.h>

struct dentry {
    atomic_t                 d_count;      /* 使用记数 */
    unsigned long            d_vfs_flags;  /* 目录缓存标志 */
    spinlock_t               d_lock;       /* 单目录项锁 */
    struct inode             *d_inode;     /* 相关联的索引节点 */
    struct list_head         d_lru;        /* 未使用链表 */
    struct list_head         d_child;      /* 副目录中目录项对象的链表 */
    struct list_head         d_subdirs;    /* 子目录*/
    struct list_head         d_alias;      /* 索引节点的别名链表 */
    unsigned long            d_time;       /* 重新声响的时间 */
    struct dentry_operations *d_op;        /* 目录项操作表 */
    struct super_block       *d_sb;        /* 文件超级块 */
    unsigned int             d_flags;      /* 目录项标识 */
    int                      d_mounted;    /* 可能是登陆点的目录项 */
    void                     *d_fsdata;    /* 文件系统特殊的数据 */
    struct rcu_head          d_rcu;        /* RCU锁 */
    struct dcookie_struct    *d_cookie;    /* cookie */
    struct dentry            *d_parent;    /* 父目录的目录项对象 */
    struct qstr              d_name;       /* 目录项的名字 */
    struct hlist_node        d_hash;       /* 散列表 */
    struct hlist_head        *d_bucket;    /* 散列表头 */
    unsigned char            d_iname[DNAME_INLINE_LEN_MIN]; /* 短文件名 */
};

struct dentry_operations {
    int (*d_revalidate) (struct dentry *, int);		/*判断目录对象是否有效*/
    int (*d_hash) (struct dentry *, struct qstr *);		/*问目录项生成散列表*/
    int (*d_compare) (struct dentry *, struct qstr *, struct qstr *);
        /*VFS调用该函数来比较name1和name2这两个文件名，使用该函数需要家dcache_lock锁。注意fat等不区分大小写的文件系统*/
    int (*d_delete) (struct dentry *);
        /*当目录项对象的d_count计数值等于0时，VFS调用该函数，使用该函数需要家dcache_lock锁*/
    void (*d_release) (struct dentry *);
        /*当目录项对象将要被释放时，VFS调用该函数，默认情况下，什么都不做*/
    void (*d_iput) (struct dentry *, struct inode *);
        /*当目录项对象丢失了其相关的索引节点时，VFS调用该函数。默认情况下VFS会调用iput()函数释放索引节点。如果文件系统重载了该函数，那么除了执行此文件系统的特殊的工作外，还必须调用iput()函数*/
};	



d) 文件

<include/linux/fs.h>

struct file {
    union {
        struct list_head     fu_list ;        /* 文件对象链表 */
        struct rcu_head      fu_rcuhead ;     /* 释放之后的RCU链表 */
    } f_u ;
    struct path              f_path ;            /* 包含目录项 */
    struct file_operations   *f_op ;             /* 文件操作表 */
    spinlock_t               f_lock ;            /* 单个文件结构锁 */
    atomic_t                 f_count ;           /* 文件对象使用计数 */
    unsigned int             f_flags ;           /* 当打开文件时所指定的标志 */
    mode_t                   f_mode ;            /* 文件的访问模式 */
    loff_t                   f_pos ;             /* 文件当前位置偏移(文件位置指针) */
    struct fown_struct       f_owner ;           /* 拥有者通过信号进行异步IO数据的传送 */
    const struct cred        *f_cred ;           /* 文件的信任状态 */
    struct file_ra_stat      f_ra ;              /* 预读状态 */
    u64                      f_version ;         /* 版本号 */
    void                     *f_security ;       /* 安全模式 */
    void                     *private_data ;     /* tty设备驱动钩子 */
    struct list_head         f_ep_links ;        /* 事件池链表 */
    spinlock_t               f_ep_lock ;         /* 事件池锁 */
    struct address_space     *f_mapping ;        /* 页缓存映射 */
    unsigned long            f_mnt_write_state ; /* 调试状态 */
} ;

struct file_operations {
    struct module *owner;
        /* 指向拥有该结构的模块的指针，避免正在操作时被卸载，一般为初始化为THIS_MODULES */
    loff_t (*llseek) (struct file *, loff_t, int);
        /* 用于更新偏移量指针,由系统调用lleek()调用它 */
    ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
        /* 由系统调用read()调用它 */
    ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
        /* 由系统调用write()调用它 */
    ssize_t (*aio_read) (struct kiocb *, const struct iovec *, unsigned long, loff_t);
        /* 由系统调用 aio_read() 调用它 */
    ssize_t (*aio_write) (struct kiocb *, const struct iovec *, unsigned long, loff_t);
        /* 由系统调用 aio_write() 调用它 */
    int (*readdir) (struct file *, void *, filldir_t);
        /* 仅用于读取目录，对于设备文件，该字段为 NULL */
    unsigned int (*poll) (struct file *, struct poll_table_struct *);
        /* 返回一个位掩码，用来指出非阻塞的读取或写入是否可能。将pool定义为 NULL，设备会被认为即可读也可写。 */
    int (*ioctl) (struct inode *, struct file *, unsigned int, unsigned long); 
        /* 提供一种执行设备特殊命令的方法。不设置入口点，返回-ENOTTY */
    long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
        /* 不使用BLK的文件系统，将使用此种函数指针代替ioctl */
    long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
        /* 在64位系统上，32位的ioctl调用，将使用此函数指针代替 */
    int (*mmap) (struct file *, struct vm_area_struct *);
        /* 将给定文件映射到指定的地址空间上,由系统调用 mmap 调用它 */
    int (*open) (struct inode *, struct file *);
        /* 创建一个新的文件对象,并将它和相应的索引节点对象关联起来 */
    int (*flush) (struct file *, fl_owner_t id);
        /* 当已打开文件的引用计数减少时,VFS调用该函数 */
    int (*release) (struct inode *, struct file *);
        /* file结构释放时，将调用此指针函数 */
    int (*fsync) (struct file *, struct dentry *, int datasync);
        /* 刷新待处理的数据，如果驱动程序没有实现，fsync调用将返回-EINVAL */
    int (*aio_fsync) (struct kiocb *, int datasync);
        /* 异步fsync */
    int (*fasync) (int, struct file *, int);
        /* 通知设备FASYNC标志发生变化，如果设备不支持异步通知，该字段可以为NULL */
    int (*lock) (struct file *, int, struct file_lock *);
        /* 实现文件锁，设备驱动常不去实现此lock */
    ssize_t (*sendpage) (struct file *, struct page *, int, size_t, loff_t *, int);
        /* 实现sendfile调用的另一部分，内核调用将其数据发送到对应文件，每次一个数据页，设备驱动通常将其设置为NULL */
    unsigned long (*get_unmapped_area)(struct file *, unsigned long, unsigned long, unsigned long, unsigned long);
        /* 在进程地址空间找到一个合适的位置，以便将底层设备中的内存段映射到该位置。大部分驱动可将其设置为NULL */
    int (*check_flags)(int);
        /* 允许模块检查传递给fcntl（F_SETEL…）调用的标志 */
    int (*flock) (struct file *, int, struct file_lock *);
        /* 实现文件锁 */
    ssize_t (*splice_write)(struct pipe_inode_info *, struct file *, loff_t *, size_t, unsigned int);
    ssize_t (*splice_read)(struct file *, loff_t *, struct pipe_inode_info *, size_t, unsigned int);
    int (*setlease)(struct file *, long, struct file_lock **);
};


### 3. 与文件系统相关的结构体
根据文件系统所在的物理介质和数据在物理介质上的组织方式来区分不同的文件系统类型的。 file_system_type结构用于描述具体的文件系统的类型信息。被Linux支持的文件系统，都有且仅有一个file_system_type结构而不管它有零个或多个实例被安装到系统中。
而与此对应的是每当一个文件系统被实际安装，就有一个vfsmount结构体被创建，这个结构体对应一个安装点.
        
struct file_system_type {
    const char *name;   /* 文件系统名称 */
    int fs_flags;       /* 文件系统类型标志 */
    int (*get_sb) (struct file_system_type *, int, const char *, void *, struct vfsmount *);	/* 从磁盘中读取超级块,并且在文件系统被安装时,在内存中组装超级块对象 */
    void (*kill_sb) (struct super_block *);	/* 终止访问超级块 */
    struct module *owner;           /* 文件系统模块 */
    struct file_system_type * next; /* 链表中下一个文件系统类型 */
    struct list_head fs_supers;     /* 超级块对象链表 */
    
    /* 下面都是运行时的锁 */
    struct lock_class_key s_lock_key;
    struct lock_class_key s_umount_key;
    
    struct lock_class_key i_lock_key;
    struct lock_class_key i_mutex_key;
    struct lock_class_key i_mutex_dir_key;
    struct lock_class_key i_alloc_sem_key;
};

struct vfsmount
{
    struct list_head mnt_hash;        /* 散列表 */
    struct vfsmount *mnt_parent;      /* 父文件系统 */
    struct dentry *mnt_mountpoint;    /* 安装点的目录项对象 */
    struct dentry *mnt_root;          /* 该文件系统的根目录项对象 */
    struct super_block *mnt_sb;       /* 该文件系统的超级块 */
    struct list_head mnt_mounts;      /* 子文件系统链表 */
    struct list_head mnt_child;       /* 子文件系统链表 */
    atomic_t mnt_count;               /* 使用计数 */
    int mnt_flags;                    /* 安装标志 */
    char *mnt_devname;                /* 设备文件名 */
    struct list_head mnt_list;        /* 描述符链表 */
    struct list_head mnt_fslink;      /* 具体文件系统的到期列表 */
    struct namespace *mnt_namespace;  /* 相关的名字空间 */
    struct list_head mnt_expire;      /* 到期链表的入口 */
    struct list_head mnt_share;       /* 共享安装链表的入口 */
    struct list_head mnt_slave_list;  /* 从安装链表 */
    struct list_head mnt_slave;       /* 从安装链表的入口 */
    struct vfsmount *mnt_master;      /* 从安装链表的主人 */
    struct mnt_namespace *mnt_ns;     /* 相关的命名空间 */
    int mnt_id;                       /* 安装标识符 */
    int mnt_group_id;                 /* 组标识符 */
    atomic_t mnt_count;               /* 使用计数 */
    int mnt_expiry_mark;              /* 如果标记为到期，则为 True */
    int mnt_pinned;                   /* "钉住"进程计数 */
    int mnt_ghosts;                   /* "镜像"引用计数 */
    int mnt_writers;                  /* 写者引用计数 */
};


### 4. 与进程相关的结构体
struct files_struct  ：由进程描述符中的 files 目录项指向，所有与单个进程相关的信息(比如打开的文件和文件描述符)都包含在其中。
struct fs_struct     ：由进程描述符中的 fs 域指向，包含文件系统和进程相关的信息。
struct mmt_namespace ：由进程描述符中的 mmt_namespace 域指向。

<linux/fdtable.h>
struct files_struct {
    atomic_t count;      /* 使用计数 */
    struct fdtable *fdt; /* 指向其他fd表的指针 */
    struct fdtable fdtab;/* 基 fd 表 */
    spinlock_t file_lock ____cacheline_aligned_in_smp; /* 单个文件的锁 */
    int next_fd;                                       /* 缓存下一个可用的fd */
    struct embedded_fd_set close_on_exec_init;         /* exec()时关闭的文件描述符链表 */
    struct embedded_fd_set open_fds_init;              /* 打开的文件描述符链表 */
    struct file * fd_array[NR_OPEN_DEFAULT];           /* 缺省的文件对象数组 */
};


​	
​	<linux/fs_struct.h>
​	struct fs_struct {
​		int users;               /* 用户数目 */
​		rwlock_t lock;           /* 保护结构体的读写锁 */
​		int umask;               /* 掩码 */
​		int in_exec;             /* 当前正在执行的文件 */
​		struct path root, pwd;   /* 根目录路径和当前工作目录路径 */
​		struct dentry * altroot；    /*可供选择的根目录的目录项对象*/
​		struct vfsmount * rootmnt;   /*根目录的安装点对象*/
​		struct vfsmount * pwdmnt;    /*pwd的安装点对象*/
​		struct vfsmount * altrootmnt;/*可供选择的根目录的安装点对象*/
​	};
​	
​	<linux/mmt_namespace.h>
​	struct mnt_namespace {
​		atomic_t count;		/* 结构的使用计数 */
​		struct vfsmount *root;	/* 根目录的安装点对象 */
​		struct list_head list;	/* 安装点链表 */
​		wait_queue_head_t poll;	/* 轮询的等待队列 */
​		int event;			/* 事件计数 */
​	};

