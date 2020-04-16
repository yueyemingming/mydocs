# 命令行参数

## 1. 第一种方式

`os.Args` 是string切片，存储所有的命令行参数

```go
func main() {
    fmt.Println(len(os.Args))
    for i, v := range os.Args {
        fmt.Println(i, " ", v)
    }
}
```

```result
./go.exe test.txt
2
0   D:\study\go\go.exe
1   test.txt
```

## 2. 第二种方式

```go
flag.BoolVar(&b, "b", false, "get b arg")
flag.StringVar(&s, "s", "", "get s arg")
flag.IntVar(&i, "i", 0, "get i arg")
...
```

解释：获取参数"-b"的子参数，如果没有，变量b的默认值false; "get b arg"缺少这个b参数时的提示

```go
func main() {
    var s string
    flag.StringVar(&s, "s", "fuck", "enter a string .")
    flag.Parse()
    fmt.Println(s)
}
```

结果如下:

```result
$ ./go.exe      //s的默认参数时fuck
fuck

$ ./go.exe -s string
string          //输入了-s的参数是string
```