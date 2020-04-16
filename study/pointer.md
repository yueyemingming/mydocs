# 指针

* `&` 返回变量存储地址
* `*` 指针变量。
* `nil` 空指针

## new, make

**new** 用来分配内存，主要用来分配值类型指针，比如int, struct，string等。

**make** 用来分配内存，主要用来分配引用类型变量，比如chan，map，slice

```go
func main() {
    s1 := make([]int, 10)   //生成空间，同时返回引用
    fmt.Println(s1)         //[0 0 0 0 0 0 0 0 0 0]
    s1[0] = 100             //第一空间直接赋值
    fmt.Println(s1)         //[100 0 0 0 0 0 0 0 0 0]

    s2 := new([]int)        //new只产生首地址
    fmt.Println(s2)         //&[]
    *s2 = make([]int, 10)   //为生成空间，将引用返给*s1
    (*s2)[0] = 100          //第一个空间赋值
    fmt.Println(s2)         //&[100 0 0 0 0 0 0 0 0 0]
}
```
