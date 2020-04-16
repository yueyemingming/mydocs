# 数组

[TOC]

***同种数据类型，固定长度***

## 1. 声明定义

数组默认初始化为0

### 1.1 一维数组

```go
var a [10] int          //声明直接创建空间，同时初始化为0
var a = [5] float32 {1000.0, 2.0, 3.4, 7.0, 50.0}
var a = [...] float32 {1000.0, 2.0, 3.4, 7.0, 50.0}  //自动计算数组长度
var str = [5] string { 3: "hello", 4: " world" }     //通过下标赋值
a := make([]int, 10)    //用make创建空间，同时初始化为0
```

### 1.2 二维数组

```go
a := [3][4] int {
    {0, 1, 2, 3} ,
    {4, 5, 6, 7} ,
    {8, 9, 10, 11},			//这么使用时，这个逗号不能省
}
```

### 1.3 多维数组

```go
var a [5][10][4] int		//声明直接创建空间，同时初始化为0
```

## 2. 遍历

```go
for i := 0 ; i < len(a) ; i++ {
}
```

```go
for index, value := range a {
}
```

二维数组的遍历

```go
func main() {
    a := [...][5]int{
        {1, 2, 3, 4, 5},
        {6, 7, 8, 9, 10},
    }

    for row, row_value := range a {
        for col, col_value := range row_value {
            fmt.Printf("%d, %d, %d\n", row, col, col_value)
        }
        fmt.Println()
    }
}
```

## 3. 地址越界panic

```go
func main() {
    var a [10]int
    a[0] = 10
    fmt.Println(a)   //[10 0 0 0 0 0 0 0 0 0]

    //a[10] = 10 //编译时就判断除了地址越界，invalid array index 10 (out of bounds for 10-element array)
    //fmt.Println(a)

    j := 10
    a[j] = 10 //编译阶段没发现，运行时崩溃，runtime error: index out of range
    fmt.Println(a)
}
```

## 4. 传地址

golang数组本身是**值传递**，而C中默认是首地址，是指针传递，这是跟C语言特别不一样的地方。

```go
func test(arr *[5]int) {
    (*arr)[0] = 10
}

func main() {
    var a [5]int
    a[0] = 5
    fmt.Println(a)

    test(&a)      //这是go的数组地址指针传递，果然不一样
    fmt.Println(a)
}
```