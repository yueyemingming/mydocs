# mongodb监控的关键命令

进入mongo的shell命令模式下，db.stats() 与 db.serverStatus() 是查看当前Mongodb运行状态的两个重要命令。下面简单解释一下这两个命令。

## 1.0 db.stats()

```json
{
    "db":"test",                //库名
    "collections":3,            //集合个数
    "objects":13,               //所有集合的总记录条数
    "avgObjSize":36,            //平均记录长度,bytes。
    "dataSize":468,             //所有数据总大小，bytes，不是指占有磁盘大小。
    "storageSize":13312,        //占有磁盘大小，bytes
    "numExtents":3,             //不知道
    "indexes":1,                //总索引个数
    "indexSize":8192,           //总索引大小
    "fileSize":201326592,       //当前数据库预分配的文件大小,例如test.0,test.1，不包括test.ns。(并没有找到类似的文件)
    "ok":1
}
```

## 2.0 db.serverStatus()

```json
{
    "Host": "te.rzw.com:27018",             //运行实例所监听的IP与端口
    "version": "1.8.3",                     //当前实例所使用的版本
    "Process": "mongod",                    //当前实例是mongod，还是mongos
    "Uptime": 231905,                       //实例连续正常运行时间,单位秒
    "uptimeEstimate": 223083,               //基于mongodb内部粗粒定时器的连续正常运行时间
    "localTime": ISODate("2011-10-13T01:47:38.773Z"),   //本机时间，以UTC时间为计算标准。
    "globalLock.totalTime": 231905155987,               //自实例启动全局锁创建以来到现在多长时间，单位微秒.
    "globalLock.lockTime": 9053050,         //自全局锁创建以来锁定总时间，单位微秒
    "globalLock.ratio": 0.0000390377262,    //锁定的时间所占的比例(lockTime/ totalTime)
    "globalLock.currentQueue.total": 0,     //当前等待全局锁的数量
    "globalLock.currentQueue.readers": 0,   //当前等待读锁的数量
    "globalLock.currentQueue.writers": 0,   //当前等待写锁的数量
    "globalLock.activeClients.total": 1,    //连接到当前实例处于活动状态的客户端数量。
    "globalLock.activeClients.readers": 1,  //处于活动状态的客户端中有多少是在执行read操作
    "globalLock.activeClients.writers": 0,  //处于活动状态的客户端中有多少是在执行write操作
    "Mem.resident": 6,                      //到现在总共使用的物理内存,单位是MB
    "Mem.virtual": 17307,                   //当前Mongodb实例使用的虚拟内存大小，单位MB，一般情况下比mem.map的值要超大一点，如果大很多很有可能发生内存泄露，如果使用journal,大约是2倍的map值
    "Mem.maped": 8556,                      //Mongodb使所有数据都映射到内存中，所以这个值可以看似整个数据量的值。
    "Mem.bits": 64,                         //机器位数,32位或64位
    "Mem.supported": true,                  //本机是否支持内存扩展
    "Connections.current": 2,               //当前连接到本机处于活动状态的连接数
    "Connections. available": 19998,        //剩余多少可供连接
    "extra_info.heap_usage_bytes": 521152,  //当前实例堆大小，单位bytes
    "extra_info. page_faults": 569,         //加载磁盘内容时发生页错误的次数
    "indexCounters. Btree.accesses": 1,     //访问索引次数
    "indexCounters. Btree.hits": 1,         //访问索引时，索引在内存中被命中的次数
    "indexCounters. Btree.misses": 0,       //访问索引时，索引不是在内存中被命中的次数
    "indexCounters. Btree.resets": 0,       //索引计数器被重置的次数
    "indexCounters. Btree.missRatio": 0,    //索引非命中率
    "backgroundFlushing.flushes": 3864,     //实例刷新数据到磁盘的数次
    "backgroundFlushing.total_ms": 15473,   //刷新到磁盘总共花费的时间,单位毫秒
    "backgroundFlushing.average_ms": 4.004399585921325,             //平均每次刷新执行时间
    "backgroundFlushing.last_ms": 3,        //最后一次刷新执行时间
    "backgroundFlushing.last_finished": ISODate("2011-10-13T01:46:51.184Z"),             //最后一次刷新完成的时间点
    "Cursors.totalOpen": 1,                 //当前游标数量
    "Cursors.timeOut": 2,                   //从实例启动到现在游标超时的总数量
    "Network.bytesIn": 4680778,             //发送到实例的字节大小
    "Network.bytesOut": 4759827,            //发送出去的字节大小
    "Repl.setName": "myset",                //replSet结构定义的名称
    "Repl.isMaster": true,                  //当前实例是否是replSet结构中的Master节点
    "Repl.secondary": false,                //当前实例是否是replSet结构中的secondary节点
    "Repl.hosts": ["te.rzw.com:27018","te.rzw.com:27019"],          //replSet结构中每个节点情况
    "Opcounters.insert": 4,                 //自实例启动以来，执行insert次数
    "Opcounters.query": 181,                //自实例启动以来，执行query次数
    "Opcounters.update": 92084,             //自实例启动以来，执行update次数
    "Opcounters.delete": 0,                 //自实例启动以来，执行delete次数
    "Opcounters.getmore": 92816,            //自实例启动以来，在游标执行getMore次数
    "Opcounters.command": 316,              //自实例启动以来，执行其他操作的次数
    "Asserts.regular": 0,                   //自实例启动以来,断言正常的次数
    "Asserts.warn": 0,                      //自实例启动以来,断言警告的次数
    "Asserts.msg": 0,                       //自实例启动以来，断言内部错误的次数
    "Asserts.user": 134,                    //自实例启动以来,因用户使用造成的错误而被断言次数
    "Asserts.rollovers": 0,                 //断言被翻转的次数
    "writeBacksQueued": false,              //在mongos中，操作被重做的次数
    "Dur.commits": 1,                       //在最近间隔，向journal文件提交的次数
    "Dur.journaledMB": 0.008192,            //在最近间隔,journalr文件写入数据量，单位MB
    "Dur. writeToDataFilesMB": 0,           //在最近间隔,从journal文件向数据文件写入的数据量，单位MB
    "Dur. commitsInWriteLock": 0,           //在最近间隔中，处于写锁状态下向journal提交的次数.这种提交方式是不被推荐的
    "Dur.earlyCommits": 0,                  //在预定时间之前请求提交的次数
    "Dur.timeMs.dt": 3045,                  //Journal状态收集的频率,单位毫秒
    "Dur.timeMs. prepLogBuffer": 0,         //向journal写数据前的准备所花费时间, 单位毫秒
    "Dur.timeMs. writeToJournal": 2,        //向journal写数据所花费时间，单位毫秒
    "Dur.timeMs. writeToDataFiles": 0,      //在写入journal文件后向数据文件写数据花费时间，单位毫秒
    "Dur.timeMs. remapPrivateView": 0,      //重新在内存中映射数据花费的时间
    "Ok": 1                                 //serverStatus操作是否被成功执行
}