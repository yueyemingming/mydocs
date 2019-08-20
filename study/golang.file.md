# 文件读写

* 查看文档 <https://go-zh.org/pkg/os/>

## 1. 普通读写方式

```golang
func main() {
    file, err := os.OpenFile("test.txt", os.O_CREATE|os.O_WRONLY, 0664)
    if err != nil {
        fmt.Println("open file error : ", err)
        return
    }

    defer file.Close()

    fmt.Fprintln(file, "fuck")
}
```

## 2. ioutil包读写方式

```golang
func main() {
    buf, _ := ioutil.ReadFile("test.txt")
    fmt.Println(buf)
    fmt.Println(string(buf))
}
```

## 3. 压缩文件读写

```golang
func main() {
    file, err := os.Open("test.txt.gz")
    if err != nil {
        fmt.Println("os open file error")
        return
    }
    defer file.Close()

    fz, _ := gzip.NewReader(file)
    if err != nil {
        fmt.Println("gzip open file error")
        return
    }

    r := bufio.NewReader(fz)
    for {
        line, err := r.ReadString('\n')
        if err == io.EOF { //判断文件结尾
            return
        }
        if err != nil {
            fmt.Println("read file error")
            return
        }
        fmt.Println(line)
    }
}
```