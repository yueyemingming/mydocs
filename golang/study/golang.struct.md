# 结构

* 结构体中的字段内存中是连续的

## 1. 声明

```golang
type User struct {
    Name string   //首字母大写可被外部打包
    age int       //首字母小写部可被外部打包
}

type U User   //类型别名
```

## 2. 定义

```golang
var u User
var u *User = new (User)    //new的操作，go中没有delete对应，go回自动垃圾回收，new完直接用就行
var u *User = &User{ "fuck", 20 }
var u *User = &User{ Name : "fuck", age : 20 }

// 隐式使用
fmt.Println(User{"username", 20})
fmt.Println(User{title: "username", age: 20})
fmt.Println(User{title: "username"})
```

## 3. 访问成员

```golang
var u User
u.name = "username"
u.age = 20
```

* 指针访问结构体成员, 不同于c, 不用`->`, 而依旧是`.`操作符

```golang
var up *User
up = &u
up.name = "username"
up.age = 20
```

## 4. tag

tag 主要是描述性内容，但是在json打包时起到了json字段的功能

```golang
type Student struct {
  name string
  age  int
}

func main() {
  stu := Student{
    name: "stu01",  //首字母小写，打包时无法打进json
    age:  18,
  }

  data, err := json.Marshal(stu)    //json打包
  if err != nil {
    fmt.Println("json encode stu failed")
    return
  }

  //结构体中的字段小写，因此无法打包进json
  fmt.Println(data)           //输出 [123 125]
  fmt.Println(string(data))   //输出 {}
}
```

结构体成员改为大写后，可以得到以下结果：

```result
[123 34 78 97 109 101 34 58 34 115 116 117 48 49 34 44 34 65 103 101 34 58 49 56 125]
{"Name":"stu01","Age":18}
```

* 利用tag修改json的字段名

```golang
type Student struct {
  Name string `json:"student_name"`
  Age  int    `json:"student_age"`
}

func main() {
  stu := Student{
    Name: "stu01",
    Age:  18,
  }

  data, err := json.Marshal(stu)
  if err != nil {
    fmt.Println("json encode stu failed")
    return
  }

  fmt.Println(data)
  fmt.Println(string(data))
}
```

输出结果如下：

```result
[123 34 115 116 117 100 101 110 116 95 110 97 109 101 34 58 34 115 116 117 48 49 34 44 34 115 116 117 100 101 110 116 95 97 103 101 34 58 49 56 125]
{"student_name":"stu01","student_age":18}   //可以看到这里已经被修改了字段名
```

## 5. 匿名字段

```golang

type Car struct {
  name string
  age  int
}

type Bus struct {
  Car
  int
}

func main() {

  //错误的
  //  bus := Bus{
  //    name: "bus",
  //    age:  2,
  //    int:  3,
  //  }

  //正确的
  var bus Bus
  bus.Car.name = "bus"
  bus.Car.age = 2
  bus.int = 3

  //正确的
  // var bus Bus
  // bus.name = "bus"
  // bus.age = 2
  // bus.int = 3

  fmt.Println(bus)
}
```

结果如下：

```result
{{bus 2} 3}
```

* 若Bus结构体中也有一个age字段，将`覆盖`Car的age字段。
* 若Bus结构体中有 `Car1` 和 `Car2` 两个子类型，那成员访问用 `bus.Car1.name`, `bus.Car1.age`, `bus.Car2.name`, `bus.Car2.age` 。

## 6. 成员函数

```golang
func (对象 结构体) 函数名(参数列表) [`返回值列表`]{ ... }
```

```golang
type Circle struct {
  radius int
}

//修改时用指针
func (p *Circle) init(radius int) {   //方法名首字母大写才能被外部访问
  p.radius = radius
}

//只读时可以不用指针
func (c Circle) getArea() float64 {   //方法名首字母大写才能被外部访问
  return float64(3.14) * float64(c.radius) * float64(c.radius)
}

func main() {
  var c Circle
  c.init(10)
  fmt.Println("圆的面积 = ", c.getArea())
}
```

## 7. 实现String()方法

如果一个变量实现了 `String()` 这个方法，那么 `fmt.Println()` 默认会调用这个变量的 `String()` 方法进行输出。

```golang

type Test struct {
    m int
}

func (t Test) String() string {     //一定要返回string类型，才能被fmt.Println识别
    return "String() " + strconv.Itoa(t.m)
}

func main() {
    t := Test{3}
    fmt.Println(t)      //String() 3
}
```

---

* [反射对结构体的解析及调用](golang.reflect.md#4-结构体的解析及调用)