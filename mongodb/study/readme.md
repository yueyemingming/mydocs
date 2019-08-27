# mongo学习

- [mongo学习](#mongo学习)
  - [1. MongoDB 概念解析](#1-mongodb-概念解析)
  - [2. 连接](#2-连接)
  - [3. 创建数据库](#3-创建数据库)
  - [4. 删除数据库](#4-删除数据库)
  - [5. 创建集合](#5-创建集合)
  - [6. 删除集合](#6-删除集合)
  - [7. 插入文档](#7-插入文档)
  - [8. 更新文档](#8-更新文档)
    - [8.1 update() 方法](#81-update-方法)
    - [8.2 save() 方法](#82-save-方法)
  - [9. 删除文档](#9-删除文档)
  - [10. 查询文档](#10-查询文档)
    - [10.1 MongoDB 与 RDBMS Where 语句比较](#101-mongodb-与-rdbms-where-语句比较)
    - [10.2 MongoDB AND 条件](#102-mongodb-and-条件)
    - [10.3 MongoDB OR 条件](#103-mongodb-or-条件)
    - [10.4 AND 和 OR 联合使用](#104-and-和-or-联合使用)

## 1. MongoDB 概念解析

| QL术语/概念 | MongoDB术语/概念 | 解释/说明                           |
|:------------|:-----------------|:------------------------------------|
| database    | database         | 数据库                              |
| table       | collection       | 数据库表/集合                       |
| row         | document         | 数据记录行/文档                     |
| column      | field            | 数据字段/域                         |
| index       | index            | 索引                                |
| table       | joins            | 表连接,MongoDB不支持                |
| primary key | primary key      | 主键,MongoDB自动将_id字段设置为主键 |

## 2. 连接

```bash
$ ./mongo
MongoDB shell version: 4.0.9
connecting to: test
```

```bash
# 使用用户 admin 使用密码 123456 连接到本地的 MongoDB 服务上。输出结果如下所示：
mongodb://admin:123456@localhost/

#使用用户名和密码连接登陆到指定数据库，格式如下：
mongodb://admin:123456@localhost/test

#连接本地数据库服务器，端口是默认的。
mongodb://localhost

#使用用户名fred，密码foobar登录localhost的admin数据库。
mongodb://fred:foobar@localhost

#使用用户名fred，密码foobar登录localhost的baz数据库。
mongodb://fred:foobar@localhost/baz

#连接 replica pair, 服务器1为example1.com服务器2为example2。
mongodb://example1.com:27017,example2.com:27017

#连接 replica set 三台服务器 (端口 27017, 27018, 和27019):
mongodb://localhost,localhost:27018,localhost:27019

#连接 replica set 三台服务器, 写入操作应用在主服务器 并且分布查询到从服务器。
mongodb://host1,host2,host3/?slaveOk=true

#直接连接第一个服务器，无论是replica set一部分或者主服务器或者从服务器。
mongodb://host1,host2,host3/?connect=direct;slaveOk=true

#当你的连接服务器有优先级，还需要列出所有服务器，你可以使用上述连接方式。
#安全模式连接到localhost:
mongodb://localhost/?safe=true

#以安全模式连接到replica set，并且等待至少两个复制服务器成功写入，超时时间设置为2秒。
mongodb://host1,host2,host3/?safe=true;w=2;wtimeoutMS=2000
```

## 3. 创建数据库

```bash
> use runoob
switched to db runoob
> db
runoob
> show dbs
admin   0.000GB
config  0.000GB
local   0.000GB

#可以看到，我们刚创建的数据库 runoob 并不在数据库的列表中， 要显示它，我们需要向 runoob 数据库插入一些数据。

> db.runoob.insert({"name":"菜鸟教程"})
WriteResult({ "nInserted" : 1 })
> show dbs
admin   0.000GB
config  0.000GB
local   0.000GB
runoob  0.000GB

#MongoDB 中默认的数据库为 test，如果你没有创建新的数据库，集合将存放在 test 数据库中。
```

> 在 MongoDB 中，集合只有在内容插入后才会创建! 就是说，创建集合(数据表)后要再插入一个文档(记录)，集合才会真正创建。

## 4. 删除数据库

```bash
#删除数据库
> use runoob
switched to db runoob
> db.dropDatabase()
{ "dropped" : "runoob", "ok" : 1 }
```

## 5. 创建集合

语法：`db.createCollection(name, options)`

- name: 要创建的集合名称
- options: 可选参数, 指定有关内存大小及索引的选项

options 可以是如下参数：

| 字段        | 类型 | 描述                                                                                                                                                        |
|:------------|:-----|:------------------------------------------------------------------------------------------------------------------------------------------------------------|
| capped      | 布尔 | （可选）如果为 true，则创建固定集合。<br>固定集合是指有着固定大小的集合，当达到最大值时，它会自动覆盖最早的文档。<br>当该值为 true 时，必须指定 size 参数。 |
| autoIndexId | 布尔 | （可选）如为 true，自动在 _id 字段创建索引。<br>默认为 false。                                                                                              |
| size        | 数值 | （可选）为固定集合指定一个最大值（以字节计）。<br>如果 capped 为 true，也需要指定该字段。                                                                   |
| max         | 数值 | （可选）指定固定集合中包含文档的最大数量。                                                                                                                  |

```bash
#创建集合
> use test
switched to db test
> db.createCollection("runoob")
{ "ok" : 1 }

#查看已有集合，可以使用 show collections 或 show tables 命令：
> show collections
runoob
system.indexes

#创建固定集合 mycol，整个集合空间大小 6142800 KB, 文档最大个数为 10000 个。
> db.createCollection("mycol", { capped : true, autoIndexId : true, size : 6142800, max : 10000 } )
{ "ok" : 1 }

#你不需要创建集合。当你插入一些文档时，MongoDB 会自动创建集合。
> db.mycol2.insert({"name" : "菜鸟教程"})
> show collections
mycol2
...
```

## 6. 删除集合

语法格式：

```bash
db.collection.drop()
```

```bash
> use runoob
switched to db runoob
> db.createCollection("runoob")     # 先创建集合，类似数据库中的表
> show tables
runoob
> db.runoob.drop()
true
```

## 7. 插入文档

使用 insert() 或 save() 方法向集合中插入文档，语法如下：

```bash
db.COLLECTION_NAME.insert(document)
```

```bash
#以下文档可以存储在 MongoDB 的 runoob 数据库 的 col 集合中：
> db.col.insert({title: 'MongoDB 教程',
    description: 'MongoDB 是一个 Nosql 数据库',
    by: '菜鸟教程',
    url: 'http://www.runoob.com',
    tags: ['mongodb', 'database', 'NoSQL'],
    likes: 100
})

#以上实例中 col 是我们的集合名，如果该集合不在该数据库中， MongoDB 会自动创建该集合并插入文档。

#查看已插入文档：
> db.col.find()
{ "_id" : ObjectId("56064886ade2f21f36b03134"), "title" : "MongoDB 教程", "description" : "MongoDB 是一个 Nosql 数据库", "by" : "菜鸟教程", "url" : "http://www.runoob.com", "tags" : [ "mongodb", "database", "NoSQL" ], "likes" : 100 }


#我们也可以将数据定义为一个变量，如下所示：

> document=({title: 'MongoDB 教程',
    description: 'MongoDB 是一个 Nosql 数据库',
    by: '菜鸟教程',
    url: 'http://www.runoob.com',
    tags: ['mongodb', 'database', 'NoSQL'],
    likes: 100
});

#执行后显示结果如下：
{
    "title" : "MongoDB 教程",
    "description" : "MongoDB 是一个 Nosql 数据库",
    "by" : "菜鸟教程",
    "url" : "http://www.runoob.com",
    tags: ['mongodb', 'database', 'NoSQL'],
    "likes" : 100
}

#执行插入操作：
> db.col.insert(document)
WriteResult({ "nInserted" : 1 })

#插入单条数据
> var document = db.col.insertOne({"a": 3})
> document
{
    "acknowledged" : true,
    "insertedId" : ObjectId("571a218011a82a1d94c02333")
}

#插入多条数据
> var res = db.col.insertMany([{"b": 3}, {'c': 4}])
> res
{
    "acknowledged" : true,
    "insertedIds" : [
            ObjectId("571a22a911a82a1d94c02337"),
            ObjectId("571a22a911a82a1d94c02338")
    ]
}
```

## 8. 更新文档

### 8.1 update() 方法

update() 方法用于更新已存在的文档。语法格式如下：

```bash
db.collection.update(
   <query>,
   <update>,
   {
     upsert: <boolean>,
     multi: <boolean>,
     writeConcern: <document>
   }
)
```

参数说明：

- query : update的查询条件，类似sql update查询内where后面的。
- update : update的对象和一些更新的操作符（如$,$inc...）等，也可以理解为sql update查询内set后面的
- upsert : 可选，这个参数的意思是，如果不存在update的记录，是否插入objNew,true为插入，默认是false，不插入。
- multi : 可选，mongodb 默认是false,只更新找到的第一条记录，如果这个参数为true,就把按条件查出来多条记录全部更新。
- writeConcern :可选，抛出异常的级别。

```bash
#我们在集合 col 中插入如下数据：
> db.col.insert({
    title: 'MongoDB 教程',
    description: 'MongoDB 是一个 Nosql 数据库',
    by: '菜鸟教程',
    url: 'http://www.runoob.com',
    tags: ['mongodb', 'database', 'NoSQL'],
    likes: 100
})

#接着我们通过 update() 方法来更新标题(title):
> db.col.update({'title':'MongoDB 教程'},{$set:{'title':'MongoDB'}})
WriteResult({ "nMatched" : 1, "nUpserted" : 0, "nModified" : 1 })   # 输出信息

> db.col.find().pretty()
{
        "_id" : ObjectId("56064f89ade2f21f36b03136"),
        "title" : "MongoDB",
        "description" : "MongoDB 是一个 Nosql 数据库",
        "by" : "菜鸟教程",
        "url" : "http://www.runoob.com",
        "tags" : ['mongodb', 'database', 'NoSQL'],
        "likes" : 100
}

#以上语句只会修改第一条发现的文档，如果你要修改多条相同的文档，则需要设置 multi 参数为 true。
> db.col.update({'title':'MongoDB 教程'},{$set:{'title':'MongoDB'}},{multi:true})
```

### 8.2 save() 方法

save() 方法通过传入的文档来替换已有文档。语法格式如下：

```bash
db.collection.save(
   <document>,
   {
     writeConcern: <document>
   }
)
```

参数说明：

- document : 文档数据。
- writeConcern :可选，抛出异常的级别。

```bash

#以下实例中我们替换了 _id 为 56064f89ade2f21f36b03136 的文档数据：

> db.col.save({
    "_id" : ObjectId("56064f89ade2f21f36b03136"),
    "title" : "MongoDB",
    "description" : "MongoDB 是一个 Nosql 数据库",
    "by" : "Runoob",
    "url" : "http://www.runoob.com",
    "tags" : ["mongodb","NoSQL"],
    "likes" : 110
})

#替换成功后，我们可以通过 find() 命令来查看替换后的数据
> db.col.find().pretty()
{
    "_id" : ObjectId("56064f89ade2f21f36b03136"),
    "title" : "MongoDB",
    "description" : "MongoDB 是一个 Nosql 数据库",
    "by" : "Runoob",
    "url" : "http://www.runoob.com",
    "tags" : ["mongodb","NoSQL"],
    "likes" : 110
}

#只更新第一条记录：
> db.col.update( { "count" : { $gt : 1 } } , { $set : { "test2" : "OK"} } );

#全部更新：
> db.col.update( { "count" : { $gt : 3 } } , { $set : { "test2" : "OK"} },false,true );

#只添加第一条：
> db.col.update( { "count" : { $gt : 4 } } , { $set : { "test5" : "OK"} },true,false );

#全部添加进去:
> db.col.update( { "count" : { $gt : 5 } } , { $set : { "test5" : "OK"} },true,true );

#全部更新：
> db.col.update( { "count" : { $gt : 15 } } , { $inc : { "count" : 1} },false,true );

#只更新第一条记录：
> db.col.update( { "count" : { $gt : 10 } } , { $inc : { "count" : 1} },false,false );
```

## 9. 删除文档

remove() 方法的基本语法格式如下所示：

```bash
db.collection.remove(
   <query>,
   <justOne>
)

#MongoDB 是 2.6 版本以后的，语法格式如下：
db.collection.remove(
   <query>,
   {
     justOne: <boolean>,
     writeConcern: <document>
   }
)
```

参数说明：

- query :（可选）删除的文档的条件。
- justOne : （可选）如果设为 true 或 1，则只删除一个文档，如果不设置该参数，或使用默认值 false，则删除所有匹配条件的文档。
- writeConcern :（可选）抛出异常的级别。

```bash
#以下文档我们执行两次插入操作：
> db.col.insert({title: 'MongoDB 教程',
    description: 'MongoDB 是一个 Nosql 数据库',
    by: '菜鸟教程',
    url: 'http://www.runoob.com',
    tags: ['mongodb', 'database', 'NoSQL'],
    likes: 100
})

#使用 find() 函数查询数据：
> db.col.find()
{ "_id" : ObjectId("56066169ade2f21f36b03137"), "title" : "MongoDB 教程", "description" : "MongoDB 是一个 Nosql 数据库", "by" : "菜鸟教程", "url" : "http://www.runoob.com", "tags" : [ "mongodb", "database", "NoSQL" ], "likes" : 100 }
{ "_id" : ObjectId("5606616dade2f21f36b03138"), "title" : "MongoDB 教程", "description" : "MongoDB 是一个 Nosql 数据库", "by" : "菜鸟教程", "url" : "http://www.runoob.com", "tags" : [ "mongodb", "database", "NoSQL" ], "likes" : 100 }

#接下来我们移除 title 为 'MongoDB 教程' 的文档：
> db.col.remove({'title':'MongoDB 教程'})
WriteResult({ "nRemoved" : 2 })           # 删除了两条数据
> db.col.find()
……                                        # 没有数据

#如果你只想删除第一条找到的记录可以设置 justOne 为 1，如下所示：
> db.COLLECTION_NAME.remove(DELETION_CRITERIA,1)

#如果你想删除所有数据，可以使用以下方式（类似常规 SQL 的 truncate 命令）：
> db.col.remove({})
> db.col.find()
```

remove() 方法已经过时了，现在官方推荐使用 deleteOne() 和 deleteMany() 方法。

```bash
#删除集合下全部文档：
db.inventory.deleteMany({})

#删除 status 等于 A 的全部文档：
db.inventory.deleteMany({ status : "A" })

#删除 status 等于 D 的一个文档：
db.inventory.deleteOne( { status: "D" } )
```

remove() 方法 并不会真正释放空间。

需要继续执行 db.repairDatabase() 来回收磁盘空间。

```bash
> db.repairDatabase()
或者
> db.runCommand({ repairDatabase: 1 })
```

## 10. 查询文档

find() 方法以非结构化的方式来显示所有文档。

```bash
db.collection.find(query, projection)
```

- query ：可选，使用查询操作符指定查询条件
- projection ：可选，使用投影操作符指定返回的键。查询时返回文档中所有键值， 只需省略该参数即可（默认省略）。

如果你需要以易读的方式来读取数据，可以使用 pretty() 方法，语法格式如下：

```bash
>db.col.find().pretty()
```

```bash
以下实例我们查询了集合 col 中的数据：
> db.col.find().pretty()
{
    "_id" : ObjectId("56063f17ade2f21f36b03133"),
    "title" : "MongoDB 教程",
    "description" : "MongoDB 是一个 Nosql 数据库",
    "by" : "菜鸟教程",
    "url" : "http://www.runoob.com",
    tags: ['mongodb', 'database', 'NoSQL'],
    "likes" : 100
}
```

除了 find() 方法之外，还有一个 findOne() 方法，它只返回一个文档。

### 10.1 MongoDB 与 RDBMS Where 语句比较

| 操作       | 格式                       | 范例                                      | RDBMS中的类似语句     |
|:-----------|:---------------------------|:------------------------------------------|:----------------------|
| 等于       | {\<key\>:\<value\>}        | db.col.find({"by":"菜鸟教程"}).pretty()   | where by = '菜鸟教程' |
| 小于       | {\<key\>:{$lt:\<value\>}}  | db.col.find({"likes":{$lt:50}}).pretty()  | where likes < 50      |
| 小于或等于 | {\<key\>:{$lte:\<value\>}} | db.col.find({"likes":{$lte:50}}).pretty() | where likes <= 50     |
| 大于       | {\<key\>:{$gt:\<value\>}}  | db.col.find({"likes":{$gt:50}}).pretty()  | where likes > 50      |
| 大于或等于 | {\<key\>:{$gte:\<value\>}} | db.col.find({"likes":{$gte:50}}).pretty() | where likes >= 50     |
| 不等于     | {\<key\>:{$ne:\<value\>}}  | db.col.find({"likes":{$ne:50}}).pretty()  | where likes != 50     |

### 10.2 MongoDB AND 条件

MongoDB 的 find() 方法可以传入多个键(key)，每个键(key)以逗号隔开，即常规 SQL 的 AND 条件。

语法格式如下：

```bash
>db.col.find({key1:value1, key2:value2}).pretty()
```

```bash
以下实例通过 by 和 title 键来查询 菜鸟教程 中 MongoDB 教程 的数据
> db.col.find({"by":"菜鸟教程", "title":"MongoDB 教程"}).pretty()
{
    "_id" : ObjectId("56063f17ade2f21f36b03133"),
    "title" : "MongoDB 教程",
    "description" : "MongoDB 是一个 Nosql 数据库",
    "by" : "菜鸟教程",
    "url" : "http://www.runoob.com",
    tags: ['mongodb', 'database', 'NoSQL'],
    "likes" : 100
}
#以上实例中类似于 WHERE 语句：WHERE by='菜鸟教程' AND title='MongoDB 教程'
```

### 10.3 MongoDB OR 条件

MongoDB OR 条件语句使用了关键字 $or,语法格式如下：

```bash
>db.col.find(
   {
      $or: [
         {key1: value1}, {key2:value2}
      ]
   }
).pretty()
```

```bash
#以下实例中，我们演示了查询键 by 值为 菜鸟教程 或键 title 值为 MongoDB 教程 的文档。
>db.col.find({$or:[{"by":"菜鸟教程"},{"title": "MongoDB 教程"}]}).pretty()
{
    "_id" : ObjectId("56063f17ade2f21f36b03133"),
    "title" : "MongoDB 教程",
    "description" : "MongoDB 是一个 Nosql 数据库",
    "by" : "菜鸟教程",
    "url" : "http://www.runoob.com",
    tags: ['mongodb', 'database', 'NoSQL'],
    "likes" : 100
}
```

### 10.4 AND 和 OR 联合使用

```bash
以下实例演示了 AND 和 OR 联合使用，类似常规 SQL 语句为： 'where likes>50 AND (by = '菜鸟教程' OR title = 'MongoDB 教程')'

>db.col.find({"likes": {$gt:50}, $or: [{"by": "菜鸟教程"},{"title": "MongoDB 教程"}]}).pretty()
{
    "_id" : ObjectId("56063f17ade2f21f36b03133"),
    "title" : "MongoDB 教程",
    "description" : "MongoDB 是一个 Nosql 数据库",
    "by" : "菜鸟教程",
    "url" : "http://www.runoob.com",
    tags: ['mongodb', 'database', 'NoSQL'],
    "likes" : 100
}
```
