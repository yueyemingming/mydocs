# 反射

- [1. 相互转换](#1-相互转换)
- [2. 获取变量某种类型的值](#2-获取变量某种类型的值)
- [3. 设置变量某种类型的值](#3-设置变量某种类型的值)
- [4. 结构体的解析及调用](#4-结构体的解析及调用)

- 在运行时获取变了的相关信息

## 1. 相互转换

> 变量  <-->  interface{}  <-->  reflect.Value

- `reflect.TypeOf`  返回变量类型, 返回类型为：`reflect.Type`
- `reflect.ValueOf`  返回变量值, 返回类型为：`reflect.Value`
- `reflect.Value.Kind`  获取变量类别, 返回类型为：`一个常量`
- `reflect.Value.Interface()`    转换为接口, 返回类型为：`interface{}`

> Kind类型请查看 <https://go-zh.org/pkg/reflect/#Kind>

```golang
type Student struct{}

func main() {
    a := 10
    fmt.Println(reflect.ValueOf(a))             //10
    fmt.Println(reflect.TypeOf(a))              //int
    fmt.Println(reflect.ValueOf(a).Kind())      //int

    pa := &a
    fmt.Println(reflect.ValueOf(pa))            //0xc00005a080
    fmt.Println(reflect.TypeOf(pa))             //*int  指针类型
    fmt.Println(reflect.ValueOf(pa).Kind())     //ptr   只能是记住

    ia := reflect.ValueOf(a).Interface()        //接口转换：int --> interface{}
    if b, ok := ia.(int); ok {                  //接口转换：interface{} --> int
        fmt.Println(b)                          //10
    }

    s := Student{"xiaoming"}
    fmt.Println(reflect.ValueOf(s))             //{xiaoming}
    fmt.Println(reflect.TypeOf(s))              //main.Student, 注意这里Type跟Kind的区别
    fmt.Println(reflect.ValueOf(s).Kind())      //struct, 注意这里Type跟Kind的区别

    is := reflect.ValueOf(s).Interface()        //接口转换：Student --> interface{}
    if b, ok := is.(Student); ok {              //接口转换：interface{} --> Student
        fmt.Println(b)                          //{xiaoming}
    }
}
```

- 这里又有个接口转换的例子
- 这里需要注意type和kind的区别。

## 2. 获取变量某种类型的值

```golang
reflect.ValueOf(x).Float()
reflect.ValueOf(x).Int()
reflect.ValueOf(x).String()
reflect.ValueOf(x).Bool()
...
```

## 3. 设置变量某种类型的值

```golang
reflect.ValueOf(x).SetFloat(..)
reflect.ValueOf(x).SetInt(..)
reflect.ValueOf(x).SetString(..)
reflect.ValueOf(x).SetBool(..)
...
```

- `指针Value` 转换为 `值Value` 用Elem()
- 设置值时，Value类型必须是**指针**Value

```golang
func main() {
    a := 10
    pv := reflect.ValueOf(&a)       //必须获取指针Value, 才能调用SetXXX方法进行修改
    pv.Elem().SetInt(100)           //指针Value需要用Elem()进行转换为值类型
    fmt.Println(pv.Elem().Int())    //100
}
```

## 4. 结构体的解析及调用

```golang
type Student struct {
    Name string `json:"student_name"` //要通过反射修改结构体对象的值，成员要求必须是导出的，即首字母要大写
    Age  int
}

func (s Student) Print() {
    fmt.Println("this is print function")
}

func main() {
    s := Student{}
    pv := reflect.ValueOf(&s)

    if pv.Kind() != reflect.Ptr { //判断是否是指针, 如果是结构体, 用 refelect.Struct : <https://go-zh.org/pkg/reflect/#Kind>
        fmt.Println("not struct pointer")
        return
    }

    //--------------成员的获取及修改-------------------
    fmt.Println(pv.Elem().NumField())        //2, 获取成员个数
    pv.Elem().Field(0).SetString("xiaoming") //修改s中的name字段
    fmt.Println(s)                           //{xiaoming,0}

    //--------------tag的获取------------------------
    fmt.Println(reflect.TypeOf(s).Field(0).Tag.Get("json")) //student_name

    //--------------方法的获取及调用-------------------
    fmt.Println(pv.Elem().NumMethod())  //1, 获取方法个数

    //pv.Elem().Method(0).Call(nil)     //方法调用1：fuck
    var params []reflect.Value          //调用方法是要传个slice参数
    pv.Elem().Method(0).Call(params)    //方法调用2：fuck
}
```
