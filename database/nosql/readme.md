# nosql

NoSQL 数据库分类

类型 | 部分代表 | 特点
:--- | :--- | :---
列存储 | Hbase<br>Cassandra<br>Hypertable | 顾名思义，是按列存储数据的。<br>最大的特点是方便存储结构化和半结构化数据，方便做数据压缩，对针对某一列或者某几列的查询有非常大的IO优势。
文档存储 | MongoDB<br>CouchDB | 文档存储一般用类似json的格式存储，存储的内容是文档型的。<br>这样也就有机会对某些字段建立索引，实现关系数据库的某些功能。
key-value存储 | Tokyo Cabinet / Tyrant<br>Berkeley DB<br>MemcacheDB<br>Redis | 可以通过key快速查询到其value。<br>一般来说，存储不管value的格式，照单全收。（Redis包含了其他功能）
图存储 | Neo4J<br>FlockDB | 图形关系的最佳存储。<br>使用传统关系数据库来解决的话性能低下，而且设计使用不方便。
对象存储 | db4o<br>Versant | 通过类似面向对象语言的语法操作数据库，通过对象的方式存取数据。
xml数据库 | Berkeley DB XML<br>BaseX | 高效的存储XML数据，并支持XML的内部查询语法，比如XQuery,Xpath。