# 如何解决Mongodb内存过大问题

- [1. MongoDB的数据库文件主要有3种](#1-mongodb的数据库文件主要有3种)
  - [1.1 journal 日志文件](#11-journal-日志文件)
  - [1.2 namespace 表名文件dbname.ns](#12-namespace-表名文件dbnamens)
  - [1.3 data 数据及索引文件dbname.0, dbname.1,… dbname.n](#13-data-数据及索引文件dbname0-dbname1-dbnamen)
- [2. 在执行db.stats()命令后，得到数据库大小，主要参数为dataSize、storageSize以及fileSize](#2-在执行dbstats命令后得到数据库大小主要参数为datasizestoragesize以及filesize)
  - [2.1 dataSize](#21-datasize)
  - [2.2 storageSize](#22-storagesize)
  - [2.3 fileSize](#23-filesize)
- [3. 内存过大解决方案](#3-内存过大解决方案)
  - [3.1 减小预分配文件的大小](#31-减小预分配文件的大小)
  - [3.2 将数据库导入导出](#32-将数据库导入导出)
  - [3.3 禁用预分配机制（可能会导致性能降低）](#33-禁用预分配机制可能会导致性能降低)

## 1. MongoDB的数据库文件主要有3种

### 1.1 journal 日志文件

跟一些传统数据库不同，MongoDB的日志文件只是用来在系统出现宕机时候恢复尚未来得及同步到硬盘的内存数据。日志文件会存放在一个分开的目录下面。启动时候MongoDB会自动预先创建3个每个为1G的日志文件（初始为空）。除非你真的有持续海量数据并发写入，一般来说3个G已经足够。

### 1.2 namespace 表名文件dbname.ns

这个文件用来存储整个数据库的集合以及索引的名字。这个文件不大，默认16M，可以存储24000个集合或者索引名以及那些集合和索引在数据文件中得具体位置。通过这个文件MongoDB可以知道从哪里去开始寻找或插入集合的数据或者索引数据。

### 1.3 data 数据及索引文件dbname.0, dbname.1,… dbname.n

MongoDB的数据以及索引都存放在一个或者多个MongoDB数据文件里。第一个数据文件会以“数据库名.0”命名，如 my-db.0。这个文件默认大小是64M，在接近用完这个64M之前，MongoDB 会提前生成下一个数据文件如my-db.1。数据文件的大小会2倍递增。第二个数据文件的大小为128M，第三个为256M。一直到了2G以后就会停止，一直按这个2G这个大小增加新的文件。

## 2. 在执行db.stats()命令后，得到数据库大小，主要参数为dataSize、storageSize以及fileSize

### 2.1 dataSize

dataSize是最接近真实数据大小的一个参数。你可以用来检查你的数据有多少。这个大小包括了数据库（或者集合）的每条记录的总和。注意每条记录除了BSON文档外还有header及padding这些额外开销。所以实际大小会比真正数据所占空间会稍大。

### 2.2 storageSize

这个参数等于数据库或者某个集合所有用到的Data Extents的总和。注意这个数字会大于dataSize因为Extent里面会有一些删除文档之后留下来的碎片（deleted）。如果有新插入的文档小于或等于碎片的大小，MongoDB会重新利用这个碎片来存储新的文档。不过在这之前这些碎片将一直会被保留在那里占用空间。由于这个原因，你删除文档的时候这个参数不会变小。

### 2.3 fileSize

这个参数只在数据库上有效，指的是实际文件系统中用到的文件的大小。它包括所有的数据Extents的总和，索引Extent的总和，以及一些未被分配的空间。之前提到MongoDB会对数据库文件创建时候进行预分配，例如最小就是64M，哪怕你只有几百个KB的数据。所以这个参数可能会比实际的数据大小会大不少。 这些额外未用空间是用来保证MongoDB可以在新的数据写入时候快速的分配新的Extent，避免引起磁盘空间分配引起的延迟。

## 3. 内存过大解决方案

### 3.1 减小预分配文件的大小

```js
# mongo
  > use admin
  > db.shutdownServer()
Ctrl+c
# mongod --config /etc/mongodb.conf –smallfiles
# mongo
  > use dbname
  > db.repairDatabase()
```

### 3.2 将数据库导入导出

```js
# mongodump -d databasename
# echo 'db.dropDatabase()' | mongo databasename
# mongorestore dump/databasename
```

### 3.3 禁用预分配机制（可能会导致性能降低）

```js
# mongo
  >use admin
  >db.shutdownServer()
Ctrl+c

# vi /etc/mongodb.conf
    在最后添加「noprealloc = true」

# mongod --config /etc/mongodb.conf
# mongo
  >use dbname
  >db.repairDatabase()
```
