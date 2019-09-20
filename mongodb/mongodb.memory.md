# MongoDB 如何使用内存？为什么内存满了

- [1. MongoDB 内存用在哪](#1-mongodb-内存用在哪)
  - [1.1 存储引擎 Cache](#11-存储引擎-cache)
  - [1.2 TCP 连接及请求处理](#12-tcp-连接及请求处理)
- [2. 如何控制内存使用](#2-如何控制内存使用)
  - [2.1 合理配置 WiredTiger cacheSizeGB](#21-合理配置-wiredtiger-cachesizegb)
  - [2.2 控制并发连接数](#22-控制并发连接数)
  - [2.3 是否应该配置 SWAP](#23-是否应该配置-swap)
  - [2.4 其他](#24-其他)
- [3. 作者简介](#3-作者简介)

一月 8, 2019张友东的博客mongodb、swap、tcmalloc、tcp socket、内存zydcom
最近接到多个MongoDB内存方面的线上case及社区问题咨询，主要集中在:

- 为什么我的 MongoDB 使用了 XX GB 内存？
- 一个机器上部署多个 Mongod 实例/进程，WiredTiger cache 应该如何配置？
- MongoDB 是否应该使用 SWAP 空间来降低内存压力？

## 1. MongoDB 内存用在哪

Mongod 进程启动后，除了跟普通进程一样，加载 binary、依赖的各种library 到内存，其作为一个DBMS，还需要负责客户端连接管理，请求处理，数据库元数据、存储引擎等很多工作，这些工作都涉及内存的分配与释放，默认情况下，MongoDB 使用 Google tcmalloc 作为内存分配器，内存占用的大头主要是「存储引擎」与 「客户端连接及请求的处理」。

### 1.1 存储引擎 Cache

MongoDB 3.2 及以后，默认使用 WiredTiger 存储引擎，可通过 cacheSizeGB 选项配置 WiredTiger 引擎使用内存的上限，一般建议配置在系统可用内存的60%左右（默认配置）。

举个例子，如果 cacheSizeGB 配置为 10GB，可以认为 WiredTiger 引擎通过tcmalloc分配的内存总量不会超过10GB。为了控制内存的使用，WiredTiger 在内存使用接近一定阈值就会开始做淘汰，避免内存使用满了阻塞用户请求。

目前有4个可配置的参数来支持 wiredtiger 存储引擎的 eviction 策略（一般不需要修改），其含义是：

| 参数 | 默认值 | 含义 |
| :--- | :--- | :--- |
| eviction_target | 80 | 当 cache used 超过 eviction_target，后台evict线程开始淘汰 CLEAN PAGE |
| eviction_trigger | 95 | 当 cache used 超过 eviction_trigger，用户线程也开始淘汰 CLEAN PAGE |
| eviction_dirty_target | 5 | 当 cache dirty 超过 eviction_dirty_target，后台evict线程开始淘汰 DIRTY PAGE |
| eviction_dirty_trigger | 20 | 当 cache dirty 超过 eviction_dirty_trigger, 用户线程也开始淘汰 DIRTY PAGE |

在这个规则下，一个正常运行的 MongoDB 实例，cache used 一般会在 0.8 * cacheSizeGB 及以下，偶尔超出问题不大；如果出现 used>=95% 或者 dirty>=20%，并一直持续，说明内存淘汰压力很大，用户的请求线程会阻塞参与page淘汰，请求延时就会增加，这时可以考虑「扩大内存」或者 「换更快的磁盘提升IO能力」。

### 1.2 TCP 连接及请求处理

MongoDB Driver 会跟 mongod 进程建立 tcp 连接，并在连接上发送数据库请求，接受应答，tcp 协议栈除了为连接维护socket元数据为，每个连接会有一个read buffer及write buffer，用户收发网络包，buffer的大小通过如下sysctl系统参数配置，分别是buffer的最小值、默认值以及最大值，详细解读可以google。

```text
net.ipv4.tcp_wmem = 8192  65536  16777216
net.ipv4.tcp_rmem = 8192  87380  16777216
```

redhat7(redhat6上并没有导出这么详细的信息） 上通过 ss -m 可以查看每个连接的buffer的信息，如下是一个示例，读写 buffer 分别占了 2357478bytes、2626560bytes，即均在2MB左右；500个类似的连接就会占用掉 1GB 的内存；buffer 占到多大，取决于连接上发送/应答的数据包的大小、网络质量等，如果请求应答包都很小，这个buffer也不会涨到很大；如果包比较大，这个buffer就更容易涨的很大。

```text
tcp    ESTAB      0      0                       127.0.0.1:51601                                 127.0.0.1:personal-agent
   skmem:(r0,rb2357478,t0,tb2626560,f0,w0,o0,bl0)
```t

除了协议栈上的内存开销，针对每个连接，Mongod 会起一个单独的线程，专门负责处理这条连接上的请求，mongod 为处理连接请求的线程配置了最大1MB的线程栈，通常实际使用在几十KB左右，通过 proc 文件系统看到这些线程栈的实际开销。 除了处理请求的线程，mongod 还有一系列的后台线程，比如主备同步、定期刷新 Journal、TTL、evict 等线程，默认每个线程最大ulimit -s（一般10MB）的线程栈，由于这批线程数量比较固定，占的内存也比较可控。

```text
# cat /proc/$pid/smaps

7f563a6b2000-7f563b0b2000 rw-p 00000000 00:00 0
Size:              10240 kB
Rss:                  12 kB
Pss:                  12 kB
Shared_Clean:          0 kB
Shared_Dirty:          0 kB
Private_Clean:         0 kB
Private_Dirty:        12 kB
Referenced:           12 kB
Anonymous:            12 kB
AnonHugePages:         0 kB
Swap:                  0 kB
KernelPageSize:        4 kB
MMUPageSize:           4 kB
```

线程在处理请求时，需要分配临时buffer存储接受到的数据包，为请求建立上下文（OperationContext），存储中间的处理结果（如排序、aggration等）以及最终的应答结果等。

当有大量请求并发时，可能会观察到 mongod 使用内存上涨，等请求降下来后又慢慢释放的行为，这个主要是 tcmalloc 内存管理策略导致的，tcmalloc 为性能考虑，每个线程会有自己的 local free page cache，还有 central free page cache；内存申请时，按 local thread free page cache ==> central free page cache 查找可用内存，找不到可用内存时才会从堆上申请；当释放内存时，也会归还到 cache 里，tcmalloc 后台慢慢再归还给 OS， 默认情况下，tcmalloc 最多会 cache min（1GB，1/8 * system_memory） 的内存， 通过 setParameter.tcmallocMaxTotalThreadCacheBytesParameter 参数可以配置这个值，不过一般不建议修改，尽量在访问层面做调优）

tcmalloc cache的管理策略，MongoDB 层暴露了几个参数来调整，一般不需要调整，如果能清楚的理解tcmalloc原理及参数含义，可做针对性的调优；MongoDB tcmalloc 的内存状态可以通过 db.serverStatus().tcmalloc 查看，具体含义可以看 tcmalloc 的文档。重点可以关注下 total_free_bytes，这个值告诉你有多少内存是 tcmalloc 自己缓存着，没有归还给 OS 的。

```json
mymongo:PRIMARY&gt; db.serverStatus().tcmalloc
{
    "generic" : {
        "current_allocated_bytes" : NumberLong("2545084352"),
        "heap_size" : NumberLong("2687029248")
    },
    "tcmalloc" : {
        "pageheap_free_bytes" : 34529280,
        "pageheap_unmapped_bytes" : 21135360,
        "max_total_thread_cache_bytes" : NumberLong(1073741824),
        "current_total_thread_cache_bytes" : 1057800,
        "total_free_bytes" : 86280256,
        "central_cache_free_bytes" : 84363448,
        "transfer_cache_free_bytes" : 859008,
        "thread_cache_free_bytes" : 1057800,
        "aggressive_memory_decommit" : 0,
        ...
    }
}
```

## 2. 如何控制内存使用

### 2.1 合理配置 WiredTiger cacheSizeGB

- 如果一个机器上只部署 Mongod，mongod 可以使用所有可用内存，则是用默认配置即可。
- 如果机器上多个mongod混部，或者mongod跟其他的一些进程一起部署，则需要根据分给mongod的内存配额来配置 cacheSizeGB，按配额的60%左右配置即可。

### 2.2 控制并发连接数

TCP连接对 mongod 的内存开销上面已经详细分析了，很多同学对并发有一定误解，认为「并发连接数越高，数据库的QPS就越高」，实际上在大部分数据库的网络模型里，连接数过高都会使得后端内存压力变大、上下文切换开销变大，从而导致性能下降。

MongoDB driver 在连接 mongod 时，会维护一个连接池（通常默认100），当有大量的客户端同时访问同一个mongod时，就需要考虑减小每个客户端连接池的大小。mongod 可以通过配置 net.maxIncomingConnections 配置项来限制最大的并发连接数量，防止数据库压力过载。

### 2.3 是否应该配置 SWAP

官方文档上的建议如下，意思是配置一下swap，避免mongod因为内存使用太多而OOM。

```text
For the WiredTiger storage engine, given sufficient memory pressure, WiredTiger may store data in swap space.

Assign swap space for your systems. Allocating swap space can avoid issues with memory contention and can prevent the OOM Killer on Linux systems from killing mongod.
```

开启 SWAP 与否各有优劣，SWAP开启，在内存压力大的时候，会利用SWAP磁盘空间来缓解内存压力，此时整个数据库服务会变慢，但具体变慢到什么程度是不可控的。不开启SWAP，当整体内存超过机器内存上线时就会触发OOM killer把进程干掉，实际上是在告诉你，可能需要扩展一下内存资源或是优化对数据库的访问了。

是否开启SWAP，实际上是在「好死」与「赖活着」的选择，个人觉得，对于一些重要的业务场景来说，首先应该为数据库规划足够的内存，当内存不足时，「及时调整扩容」比「不可控的慢」更好。

### 2.4 其他

- 尽量减少内存排序的场景，内存排序一般需要更多的临时内存
- 主备节点配置差距不要过大，备节点会维护一个buffer（默认最大256MB）用于存储拉取到oplog，后台从buffer里取oplog不断重放，当备同步慢的时候，这个buffer会持续使用最大内存。
- 控制集合及索引的数量，减少databse管理元数据的内存开销；集合、索引太多，元数据内存开销是一方面的影响，更多的会影响启动加载的效率、以及运行时的性能。

## 3. 作者简介

张友东，阿里云高级技术专家，主要关注分布式存储与数据库等技术领域，先后参与淘宝分布式文件系统TFS、阿里云数据库（PolarDB、MySQL、MongoDB、Redis）等项目的开发工作，致力于让开发者用上最好的云数据库服务。
