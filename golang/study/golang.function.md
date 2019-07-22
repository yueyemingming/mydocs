# 函数

## 1. 函数基本用法

```golang
func 函数名( [参数列表] ) [`返回值列表`] { ... }
```

```golang
func test(a, b int) (int, int) {    //两个返回值
    return a, b
}

func test(a string, b int) string { //一个返回值
    return a
}
```

* [接口作为参数](golang.interface.md#2-空接口及与普通类型的转换)

## 2. 函数类型

```golang
type 函数类型名 func(int, int) int     //func(int, int) int, 这是函数签名
```

```golang
func add(a, b int) int {
    return a + b
}

func operator(op func(int, int) int, a, b int) int {   //函数类型的变量
    return op(a, b)
}

func main() {
    fun := add
    fmt.Println(operator(fun, 3, 4))
}
```

使用 `type` 定义如下

```golang

type fun_type func(int, int) int     //函数类型声明

func add(a, b int) int {
    return a + b
}

func operator(op fun_type, a, b int) int {   //函数类型的变量
    return op(a, b)
}

func main() {
    fun := add
    fmt.Println(operator(fun, 3, 4))
}
```

## 3. 可变参函数

```golang
func add(arg ...int) int {   //...代表可变参，事实上这个arg相当于是个数组
    sum := 0

    for i := 0; i < len(arg); i++ {
        sum += arg[i]
    }

    return sum
}

func main() {
    fmt.Println(add(1, 2, 3))
}
```

## 4. 匿名函数

```golang
func main() {
    fun := func(a, b int) int {     //定义一个匿名函数,赋值给fun，fun就是事实上函数名，但没有调用
        return a + b
    } //这里没有调用这个匿名函数

    fmt.Println(fun(3, 4))          //调用函数

    res := func(a, b int) int {     //这里直接定义匿名函数，同时直接调用，res是函数调用的结果
        return a + b
    }(3, 5) //这里直接调用 (3,5) 这个括号代表调用加传递参数

    fmt.Println(res)
}
```

## 5. 闭包

一个函数和与其相关的应用环境组成的实体

### eg.01

```golang
func Adder() func(int) int {        //把闭包看成是个类,  'func(int) int'代表返回值是个函数
    var x int                       //把闭包内变量看成是类成员变量
    fmt.Println("Adder.x = ", x)    //只调用一次

    return func(d int) int {        //把闭包内函数看成是类成员函数, 'func(int) int'这个函数就是返回值
        x += d
        fmt.Println("Adder.func.x = ", x)
        return x
    }
}

func main() {
    f := Adder()        //创建闭包对象,生成了Adder.x这个闭包内变量

    fmt.Println()

    fmt.Println(f(1))   //闭包内函数传值并计算
    fmt.Println(f(10))  //闭包内函数传值并计算
    fmt.Println(f(100)) //闭包内函数传值并计算
}
```

```text
Adder.x =  0

Adder.func.x =  1
1
Adder.func.x =  11
11
Adder.func.x =  111
111
```

### eg.02

```golang
func makeSuffixFunc(suffix string) func(string) string {   //相当于类, ‘suffic’是类成员变量, "func(string) string"是成员函数

    return func(name string) string {
        if !strings.HasSuffix(name, suffix) {
            return name + suffix
        }
        return name
    }
}

func main() {
    func1 := makeSuffixFunc(".bmp")     //创建闭包对象，".bmp"绑定了闭包的 suffix 成员变量
    func2 := makeSuffixFunc(".jpg")     //创建闭包对象，".jpg"绑定了闭包的 suffix 成员变量

    fmt.Println(func1("test"))          //test.bmp
    fmt.Println(func2("test"))          //test.jpg
}
```
