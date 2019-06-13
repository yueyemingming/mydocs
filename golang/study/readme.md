# golang语法

## 基本类型

* [变量常量](golang.variable.md)
* [布尔](golang.datatype.md#1-布尔)
* [数字](golang.datatype.md#2-数字)
* [字符串](golang.datatype.md#3-字符串)

## 派生类型

* **引用类型说明**
  * **切片, 字典, 通道 是<引用类型>，即指针, 其他的都是<值类型>**
  * **引用类型在传参时都是引用传递**
  * **引用类型需要用make创建空间**

* [数组 (array)](golang.array.md)
* [切片 (slice)](golang.slice.md)
* [字典 (map)](golang.map.md)
* [指针 (pointer)](golang.pointer.md)
* [结构 (struct)](golang.struct.md)
* [函数 (function)](golang.function.md)
* [接口 (interface)](golang.interface.md)
* [通道 (channel)](golang.goroute.channel.md)

---

* [panic,recorver,defer](panic.recorver.defer.md)
* [锁](golang.lock.md)
* [range](golang.range.md)
* [自定义类型](golang.type.md)
* [自定义类型方法 (type)](golang.type.md#3-为新类型添加成员函数)
* [反射](golang.reflect.md)

---

* [带缓冲的终端读写](golang.bufio.md)
* [文件读写](golang.file.md)
* [命令行参数](golang.cmd.args.md)
* [json序列化](golang.json.md)
* [定时器](golang.timer.md)