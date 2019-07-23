# golang语法

- [1. 基本类型](#1-基本类型)
- [2. 派生类型](#2-派生类型)
- [3. 基本扩展](#3-基本扩展)
- [4. 应用扩展](#4-应用扩展)

## [1. 基本类型](golang.datatype.md)

- [变量常量](golang.datatype.md#1-变量常量)
- [布尔](golang.datatype.md#2-布尔)
- [数字](golang.datatype.md#3-数字)
- [字符串](golang.datatype.md#4-字符串)

## 2. 派生类型

1. **切片, 字典, 通道 是<引用类型>，即指针, 其他的都是<值类型>**
2. **引用类型在传参时都是引用传递**
3. **引用类型需要用make创建空间**

- [数组 (array)](golang.array.md)
- [切片 (slice) —— 引用类型](golang.slice.md)
- [字典 (map) —— 引用类型](golang.map.md)
- [指针 (pointer)](golang.pointer.md)
- [结构 (struct)](golang.struct.md)
- [函数 (function)](golang.function.md)
- [接口 (interface)](golang.interface.md)
- [通道 (channel) —— 引用类型](golang.goroute.channel.md)

## 3. 基本扩展

- [panic,recorver,defer](panic.recorver.defer.md)
- [锁](golang.lock.md)
- [range](golang.range.md)
- [自定义类型](golang.type.md)
- [反射](golang.reflect.md)

## 4. 应用扩展

- [带缓冲的终端读写](golang.bufio.md)
- [文件读写](golang.file.md)
- [命令行参数](golang.cmd.args.md)
- [json序列化](golang.json.md)
- [定时器](golang.timer.md)
- [http](golang.http.md)
