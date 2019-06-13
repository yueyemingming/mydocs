# Linux epoll介绍和程序实例

## 1. epoll是何方神圣？

epoll可是当前在Linux下开发大规模并发网络程序的热门人选。

epoll 在Linux2.6内核中正式引入, 和select相似, 其实都I/O多路复用技术而已, 并没有什么神秘的。

## 2. 常用模型的缺点

如果不摆出来其他模型的缺点, 怎么能对比出epoll的优点呢。

### 2.1 PPC/TPC模型

其实在Linux下设计并发网络程序, 向来不缺少方法, 比如典型的Apache模型（Process Per Connection, 简称PPC）, TPC（Thread PerConnection）模型, 以及select模型和poll模型, 那为何还要再引入epoll这个东东呢？那还是有得说说的…

这两种模型思想类似, 就是让每一个到来的连接一边自己做事去, 别再来烦我。只是PPC是为它开了一个进程, 而TPC开了一个线程。可是别烦我是有代价的, 它要时间和空间啊, 连接多了之后, 那么多的进程/线程切换, 这开销就上来了；因此这类模型能接受的最大连接数都不会高, 一般在几百个左右。

### 2.2 select模型

1. 最大并发数限制, 因为一个进程所打开的FD（文件描述符）是有限制的, 由FD_SETSIZE设置, 默认值是1024/2048, 因此Select模型的最大并发数就被相应限制了。自己改改这个FD_SETSIZE？想法虽好, 可是先看看下面吧…
2. 效率问题, select每次调用都会线性扫描全部的FD集合, 这样效率就会呈现线性下降, 把FD_SETSIZE改大的后果就是, 大家都慢慢来, 什么？都超时了？？！！
3. 内核/用户空间 内存拷贝问题, 如何让内核把FD消息通知给用户空间呢？在这个问题上select采取了内存拷贝方法。

### 2.3 poll模型

基本上效率和select是相同的, select缺点的2和3它都没有改掉。

## 3. epoll的提升

把其他模型逐个批判了一下, 再来看看epoll的改进之处吧, 其实把select的缺点反过来那就是epoll的优点了。

### 3.1. epoll没有最大并发连接的限制

上限是最大可以打开文件的数目, 这个数字一般远大于2048, 一般来说这个数目和系统内存关系很大, 具体数目可以 `cat /proc/sys/fs/file-max` 察看。

### 3.2 效率提升

epoll最大的优点就在于它只管你“活跃”的连接, 而跟连接总数无关, 因此在实际的网络环境中, epoll的效率就会远远高于select和poll。

### 3.3. 内存拷贝

epoll在这点上使用了 **共享内存**, 这个内存拷贝也省略了。

## 4. epoll为什么高效

epoll的高效和其数据结构的设计是密不可分的, 这个下面就会提到。

首先回忆一下select模型。当有I/O事件到来时, select通知应用程序有事件到了快去处理, 而应用程序必须轮询所有的FD集合, 测试每个FD是否有事件发生, 并处理事件；代码像下面这样：

```c
int res = select(maxfd+1, &readfds, NULL, NULL, 120);
if(res > 0) {
    for(int i = 0; i < MAX_CONNECTION; i++) {
        if(FD_ISSET(allConnection[i],&readfds))
            handleEvent(allConnection[i]);
    }
}
// if(res == 0) handle timeout, res < 0 handle error
```

epoll不仅会告诉应用程序有I/0事件到来, 还会告诉应用程序相关的信息, 这些信息是应用程序填充的, 因此根据这些信息应用程序就能直接定位到事件, 而不必遍历整个FD集合。

```c
intres = epoll_wait(epfd, events, 20, 120);
for(int i = 0; i < res;i++)
    handleEvent(events[n]);
```

## 5. epoll关键数据结构

前面提到epoll速度快和其数据结构密不可分, 其关键数据结构就是：

```c
typedef union epoll_data
{
    void *ptr;
    int fd;
    uint32_t u32;
    uint64_t u64;
} epoll_data_t;

struct epoll_event
{
    uint32_t events;    /* Epoll events */
    epoll_data_t data;  /* User data variable */
} __attribute__ ((__packed__));
```

可见epoll_data是一个union结构体,借助于它应用程序可以保存很多类型的信息: fd、指针等等。有了它, 应用程序就可以直接定位目标了。
