# defer, panic, recorver

```go
func initConfig() error {
    return errors.New("init config error")    //手动产生两个一个错误
}

func test() {
    defer func() {
        if err := recover(); err != nil {    //捕捉panic
            fmt.Println(err)
        }
    }()

    err := initConfig()
    if err != nil {
        panic(err)                          //触发panic
    }
}

func main() {
    for {
        test()
        time.Sleep(time.Second)
    }

    fmt.Println("end")
}
```

```text
init config error
init config error
init config error
init config error
...
```