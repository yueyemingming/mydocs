# 带缓冲的终端读写

带缓冲的读写操作，即将数据放在缓冲区中，根据需要再一次性写入硬盘，提高了IO操作。

```golang
func main() {
    var inputReader *bufio.Reader = bufio.NewReader(os.Stdin)
    fmt.Println("enter :")
    input, err := inputReader.ReadString('\n') //结尾的分隔符
    if err != nil {
        fmt.Println(err)
        return
    }

    fmt.Println(input)
}
```