# 变量常量

## 变量

```golang
var i int   //声明不赋初值
var i = 3   //根据初值自动判定变量类型
i := 3      //省略var形式, 因为会自动推到，所以带var多余。这种形式只能出现在函数体内

var i1, i2, i3 int
i1, i2, i3 = 1, 2, 3

var i1, i2, i3 = 1, 2, 3
i1, i2, i3 := 1, 2, 3

// 这种因式分解关键字的写法一般用于声明全局变量
var (
    i int
    s string
)
```

## 常量

const   常量
iota    自动累增计数器

```golang
const i = 3

const (
        a = iota   //0
        b          //1
        c          //2
        d = "ha"   //独立值，iota += 1
        e          //"ha"   iota += 1
        f = 100    //iota +=1
        g          //100  iota +=1
        h = iota   //7,恢复计数
        i          //8
)

const (
    i=1<<iota      //iota = 0
    j=3<<iota      //iota = 1
    k              //iota = 2, j= 3<<iota
    l              //iota = 3, j= 3<<iota
)
```