# 切片

[TOC]

* 引用类型,数组的引用,不需要取地址就可以更改切片内容
* 动态数组，长度可变
* 切片没有长度，[]必须为空

## 1. 定义

* 声明

```go
var slice [] int   //[]切片，必须为空
var array [5] int  //[]数组，要指定长度
```

* 创建

```go
var slice []type = make([]type, len)    //声明并创建, 通过make创建
slice := make([]type, length)           //创建, 通过make创建
slice := make([]type, length, capacity) //创建, 通过make创建
```

* 声明, 创建, 同时通过数组初始化

```go
slice := [] int {1,2,3 }
slice := arr[start:end]
slice := arr[start:]
slice := arr[:end]
```

## 2. len()和cap()

* len() 实际数据空间大小
* cap() 容量最大大小
* 0 <= len(slice) <= cap(slice)

```go
func main() {
    array := [...]int{1, 2, 3, 4, 5}

    fmt.Println(array)

    var slice []int
    slice = array[2:4]
    slice = slice[1:2]
    fmt.Println(cap(slice))   //这个地方的cap依旧是2,注意
    fmt.Println(len(slice))
}
```

```result
[1 2 3 4 5]
5
5
2
1
```

## 3. 判断空

```go
if(slice == nil)            //与nil比较
```

## 4. append() 和 copy() 函数

如果想增加切片的容量，我们必须创建一个新的更大的切片并把原分片的内容都拷贝过来。

```go
//允许追加空切片
slice = append(slice, 0)

//向切片添加一个元素
slice = append(slice, 1)

//同时添加多个元素
slice = append(slice, 2,3,4)

//创建切片 slice1 是之前切片的两倍容量
slice1 := make([]int, len(slice), (cap(slice))*2)

//拷贝 slice 的内容到 slice1
copy(slice1,slice)
```

## 5. 切片的实现

```go
type myslice struct {
    ptr *[10]int
    len int
    cap int
}

func mymake(s myslice, cap int) myslice {
    s.ptr = new([10]int)
    s.cap = cap
    s.len = 0
    return s
}

func main() {
    var s1 myslice
    s1 = mymake(s1, 10)
    s1.ptr[0] = 100
    fmt.Println(s1.ptr)
}
```

## 6. string

```go
func main() {
    s := "中aaa"
    s1 := []byte(s)    //此处应该是rune
    s1[0] = 'b'
    fmt.Println(s)
    fmt.Println(s1)
}
```

```result
中aaa
[98 184 173 97 97 97]     //这不是我们要的结果, 修改如下
```

```go
func main() {
    s := "中aaa"
    s1 := []rune(s)

    s1[0] = 'b'

    fmt.Println(s)
    fmt.Println(s1)
}
```

```result
中aaa
[98 97 97 97]    //这是我们想要的结果
```