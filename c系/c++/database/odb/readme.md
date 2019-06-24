# odb 数据库编程

这个例子展示了ODB创建数据库和访问数据库的方法。

本例中演示的方法包括自动发现合适的访问器/修饰符函数、显式指定访问器/修饰符函数、显式指定更复杂的访问器/修饰符表达式以及使用虚拟数据成员。

## 1. 初始文件

文件 | 含义
:--- | :---
driver.cxx | 本例的主程序
person.hxx | 用户生成数据库支持代码和sql文件的头文件，"person"持久类的头文件
database.hxx | 链接数据库的通用接口，它只有一个函数create_database()，用于链接具体的数据库，可以设定ip和port。丫的为啥不写道libodb中呢。

## 1. 生成数据库持久化源代码和sql文件

ODB编译器通过 "**person.hpp**" 文件编译生成数据库支持的代码和sql文件。这里以mysql为例，命令如下:

```bash
odb -d mysql --generate-query --generate-schema person.hxx
```

生成如下的文件四个：

文件 | 含义
:--- | :---
person.sql | 创建数据库表的sql文件
person-odb.hxx | 访问数据库的头文件
person-odb.cxx | 访问数据库的实现文件
person-odb.ixx | 感觉像是什么东西跟C++的映射文件

## 2. 编译

**driver.cxx** 是本例的主程序文件。

它 #include "person.hxx" 和 #include "person-odb.hxx" 头文件，以访问持久类及其数据库支持代码。

它 #include "database.hxx", 用于调用create_database()函数。

在main()中，首先调用create_database()来获取数据库实例。然后，它在"person"对象上执行许多数据库事务。

编译步骤如下：

```bash
c++ -c person-odb.cxx
c++ -DDATABASE_MYSQL -c driver.cxx
c++ -o driver driver.o person-odb.o -lodb-mysql -lodb   # 链接找不到库路径，手动添加：-L/usr/local/lib/
```

## 3. 运行

### 3.1 创建数据库

通过"root"，在"odb_test"数据库中执行"person.sql"脚本创建数据库。

```bash
mysql --user=root --passwd --database=odb_test < person.sql
```

### 5.2 运行程序

```bash
# export LD_LIBRARY_PATH=/usr/local/lib/    #执行时找不到库，制定库路径
./driver --user root --passwd --database odb_test
```
