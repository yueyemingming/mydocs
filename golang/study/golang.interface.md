# 接口

## 1. 基础概念

* 只定义方法，且不需要实现
* 不含任意变量

```golang
/* 定义接口 */
type 接口名称 interface {
   方法名1 [返回值列表]
   ...
}

/* 定义结构体 */
type 结构体名称 struct {}

/* 实现接口方法 */
func (变量名 结构体名) 方法名1() [返回值列表] {}
func (变量名 *结构体名) 方法名1() [返回值列表] {}       //接口类型赋值时需要取结构体对象地址
```

* eg01 .

```golang
type Test interface {
    Print()
}

type Student struct {
    name string
    age  int
}

//func (p Student) Print() {            //普通对象形式
func (p *Student) Print() {             //指针对象形式
    fmt.Println("name : ", p.name)
    fmt.Println("age : ", p.age)
}

func main() {
    stu := Student{"stu1",20}

    //进行接口对象赋值
    //var t Test = stu                  //普通对象形式
    var t Test = &stu                   //指针对象形式
    t.Print()
}
```

* eg02 .

```golang
type Phone interface {
    call()
}

type NokiaPhone struct {
}

func (nokiaPhone NokiaPhone) call() {
    fmt.Println("I am Nokia, I can call you!")
}

type IPhone struct {
}

func (iPhone IPhone) call() {
    fmt.Println("I am iPhone, I can call you!")
}

func main() {
    var phone Phone

    phone = new(NokiaPhone)
    phone.call()

    phone = new(IPhone)
    phone.call()
}
```

* 错误接口

```golang
func Sqrt(f float64) (float64, error) {
    if f < 0 {
        return 0, errors.New("math: square root of negative number")
    }
    // 实现
}

result, err:= Sqrt(-1)
if err != nil {            //与nil进行比较
   fmt.Println(err)
}
```

## 2. 空接口及与普通类型的转换

`var i interface{}`     空接口可以被任何对象赋值

```golang
func main() {
    a := 10
    var i interface{} = a       //空接口赋值
    fmt.Printf("%T\n", i)       //int，%T是代表打印变量类型
    fmt.Println(i.(int))        //10, 转换为int

    test(i)
}

func test(i interface{}) {
    if y, ok := i.(int); ok {   //转换为int，带检测
        fmt.Println(y)          //10
    }
}
```

## 3. 接口嵌套

```golang
type Interface1 interface { fun1() }
type Interface2 interface { fun2() }
type Interface3 interface {
    Interface1      //Interface3继承了1和2，一旦继承的接口中有同名函数就会出错。因此此处是 fun1, fun2, fun3
    Interface2
    fun3()
}
```

```golang
type Reader interface {
    Read()
}

type Writer interface {
    Write()
}

type ReaderWriter interface {
    Reader
    Writer
}

type File struct{}

func (f File) Read() {
    fmt.Println("read")
}

func (f File) Write() {
    fmt.Println("write")
}

func main() {
    var f File
    var rw ReaderWriter = f
    rw.Read()
    rw.Write()
}
```
