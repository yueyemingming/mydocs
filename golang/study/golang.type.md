# 自定义类型

## 1. 强制类型转换

### 1.1 go中所有类型转换都需要强制转换

```golang
typename(value)
```

```golang
type integer int

var i int = 3
var intg integer = integer(i)
```

### 1.2 inteface 类型转换

* [接口类型转换](golang.interface.md#2-空接口及与普通类型的转换)

```golang
y := a.(int)        //转换为int
y, ok := a.(int)    //转换为int，带检测
```

* [反射](golang.reflect.md)

## 2. 类型判断

`.(type)` 获取类型信息

```golang
func test(items ...interface{}) {
    for _, x := range items {
        switch x.(type) {
        case bool:
            fmt.Println("bool")
        case int:
            fmt.Println("int")
        case *int:
            fmt.Println("*int")
        case nil:
            fmt.Println("nil")
        case string:
            fmt.Println("string")
        case float32:
            fmt.Println("float32")
        case Student:
            fmt.Println("Student")
        case *Student:
            fmt.Println("*Student")
        default:
            fmt.Println("default")
        }
    }
}

type Student struct {
    name string
}

func main() {
    s := Student{"xiaoming"}
    test(s)
    test(&s)

    a := 3
    test(a)
    test(&a)
}
```

## 3. 为新类型添加成员函数

```golang
type integer int

func (i integer) print() {  //方法名首字母大写才能被外部访问
    fmt.Println(i)
}

func main() {
    var i integer = 3
    i.print()       //3
}
```