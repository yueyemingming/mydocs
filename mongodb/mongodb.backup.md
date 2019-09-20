# MongoDB 数据导入导出和备份恢复

- [1. MongoDB数据导入与导出](#1-mongodb数据导入与导出)
  - [1.1 导出工具：mongoexport](#11-导出工具mongoexport)
    - [1.1.1 概念](#111-概念)
    - [1.1.2 语法](#112-语法)
    - [1.1.3 示例](#113-示例)
  - [1.2 数据导入：mongoimport](#12-数据导入mongoimport)
    - [1.2.1 语法](#121-语法)
    - [1.2.2 示例](#122-示例)
- [2. MongoDB备份与恢复](#2-mongodb备份与恢复)
  - [2.1 MongoDB数据库备份](#21-mongodb数据库备份)
    - [2.1.1 语法](#211-语法)
    - [2.1.2 实例](#212-实例)
  - [2.2 MongoDB数据库恢复](#22-mongodb数据库恢复)
    - [2.2.1 语法](#221-语法)
    - [2.2.2 实例](#222-实例)

## 1. MongoDB数据导入与导出

### 1.1 导出工具：mongoexport

#### 1.1.1 概念

mongoDB中的mongoexport工具可以把一个collection导出成JSON格式或CSV格式的文件。可以通过参数指定导出的数据项，也可以根据指定的条件导出数据。

#### 1.1.2 语法

```bash
mongoexport -d dbname -c collectionname -o file --type json/csv -f field

参数说明：
-d ：数据库名
-c ：collection名
-o ：输出的文件名
--type ： 输出的格式，默认为json
-f ：输出的字段，如果-type为csv，则需要加上-f "字段名"
```

#### 1.1.3 示例

```bash
mongoexport -d mongotest -c users -o /home/python/Desktop/mongoDB/users.json --type json -f  "_id,user_id,user_name,age,status"
```

### 1.2 数据导入：mongoimport

#### 1.2.1 语法

```bash
mongoimport -d dbname -c collectionname --file filename --headerline --type json/csv -f field

参数说明：

-d ：数据库名
-c ：collection名
--type ：导入的格式默认json
-f ：导入的字段名
--headerline ：如果导入的格式是csv，则可以使用第一行的标题作为导入的字段
--file ：要导入的文件
```

#### 1.2.2 示例

```bash
mongoimport -d mongotest -c users --file /home/mongodump/articles.json --type json
```

## 2. MongoDB备份与恢复

### 2.1 MongoDB数据库备份

### 2.1.1 语法

```bash
mongodump -h dbhost -d dbname -o dbdirectory

参数说明：
-h ：MongDB所在服务器地址，例如：127.0.0.1，当然也可以指定端口号：127.0.0.1:27017
-d ：需要备份的数据库实例，例如：test
- o：备份的数据存放位置，例如：/home/mongodump/，当然该目录需要提前建立，这个目录里面存放该数据库实例的备份数据。
```

#### 2.1.2 实例

```bash
rm -rf /home/momgodump/
mkdir -p /home/momgodump
mongodump -h 192.168.17.129:27017 -d itcast -o /home/mongodump/
```

### 2.2 MongoDB数据库恢复

#### 2.2.1 语法

```bash
mongorestore -h dbhost -d dbname --dir dbdirectory

参数说明：

-h： MongoDB所在服务器地址
-d： 需要恢复的数据库实例，例如：test，当然这个名称也可以和备份时候的不一样，比如test2
--dir： 备份数据所在位置，例如：/home/mongodump/itcast/
--drop： 恢复的时候，先删除当前数据，然后恢复备份的数据。就是说，恢复后，备份后添加修改的数据都会被删除，慎用！
```

#### 2.2.2 实例

```bash
mongorestore -h 192.168.17.129:27017 -d itcast_restore --dir /home/mongodump/itcast/
```
