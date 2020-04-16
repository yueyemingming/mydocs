# golang语法

[TOC]

## 1. 基本类型

- [变量](golang.datatype.md#1-变量)
- [常量](golang.datatype.md#2-常量)
- [布尔](golang.datatype.md#3-布尔)
- [数字](golang.datatype.md#4-数字)
- [字符串](golang.datatype.md#5-字符串)

## 2. 派生类型

> **切片, 字典, 通道 是<引用类型>，即指针, 其他的都是<值类型>**
>
> **引用类型在传参时都是引用传递**
>
> **引用类型需要用make创建空间**

- [数组 (array)](array.md)
- [切片 (slice) —— 引用类型](slice.md)
- [字典 (map) —— 引用类型](map.md)
- [指针 (pointer)](pointer.md)
- [结构 (struct)](struct.md)
- [函数 (function)](function.md)
- [接口 (interface)](interface.md)
- [通道 (channel) —— 引用类型](goroute.channel.md)

## 3. 基本扩展

- [panic,recorver,defer](panic.recorver.defer.md)
- [锁](lock.md)
- [range](range.md)
- [自定义类型](type.md)
- [反射](reflect.md)

## 4. 应用扩展

- [带缓冲的终端读写](bufio.md)
- [文件读写](file.md)
- [命令行参数](cmd.args.md)
- [json序列化](json.md)
- [定时器](timer.md)
- [http](http.md)
