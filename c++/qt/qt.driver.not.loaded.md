# Ubuntu16.04 64位 Qt5.12.3中使用mysql

## 问题

连接mysql数据库时，出现如下 提示

```text
QSqlDatabase: QMYSQL driver not loaded
QSqlDatabase: available drivers: QSQLITE QMYSQL QMYSQL3 QPSQL QPSQL7
db open err: “Driver not loaded Driver not loaded”
```

## 原因

qt自带的Qt5Sql默认连接是sqlite，需要通过编译源码的方式，将Qt5Sql连接成mysql。

## 解决步骤

### 1. 安装mysql客户端依赖

```bash
apt -y install libmysqlclient-dev
```

### 2. 编译及安装

#### 2.1 进入源码目录

进入QT安装目录的源码目录，找到MySql驱动源码目录：

```bash
cd  /opt/Qt5.12.3/5.12.3/Src/qtbase/src/plugins/sqldrivers/mysql
```

#### 2.2 执行qmake，生成Makefile

```bash
qmake "INCLUDEPATH+=/usr/include/mysql" "LIBS+=-L/usr/lib/x86_64-linux-gnu/ -lmysqlclient" mysql.pro
```

> 出错：**Project ERROR: Library ‘mysql’ is not defined** . 则注释掉`mysql.pro`文件中的 `QMAKE_USE +=mysql` 。  
> 出错：**qtsqldrivers-config.pri 没找到**, 直接用configure.prj进行替换。

```bash
cp ../configure.pri ../qtsqldrivers-config.pri
```

#### 2.3 编译安装

```bash
make
make install    # 生成的库所在的目录： ../plugins/sqldrivers/

# 把生成的libqsqlmysql.so拷贝到安装目录.
cd ../plugins/sqldrivers/
cp libqsqlmysql.so /opt/Qt5.12.3/5.12.3/gcc_64/plugins/sqldrivers
```
