# 定时器

定时器对象：`Ticker`和`Timer`

```golang
type Ticker struct {
        C <-chan Time // The channel on which the ticks are delivered.
        // contains filtered or unexported fields
}
```

```golang
type Timer struct {
        C <-chan Time
        // contains filtered or unexported fields
}
```

* `func time.NewTicker(d Duration) *Ticker`  循环触发的定时器
* `func time.NewTimer(d Duration) *Timer`    单次触发的定时器
* `func time.After(d Duration) <-chan Time`  单次触发的定时器, 注意这里返回的是channel。这个函数本身存在内存泄漏的问题

* 定时器必须要停止，否则会产生内存泄漏

```golang
func main() {
    t := time.NewTicker(time.Second)    //每秒产生一个事件, 请参照 <https://golang.org/pkg/time/#Ticker>
    for v := range t.C {                //这是一个time.Time时间类型的channel，因此它的输出是时间内容
        fmt.Println("hello, ", v)
    }
    t.Stop()                            //必须停止，否则会存在内存泄漏
}
```

结果如下：

```result
hello,  2019-01-31 15:58:46.4981272 +0800 CST m=+1.008459401
hello,  2019-01-31 15:58:47.4983417 +0800 CST m=+2.008654501
hello,  2019-01-31 15:58:48.4978365 +0800 CST m=+3.008130001
hello,  2019-01-31 15:58:49.4989561 +0800 CST m=+4.009230301
```

## 内存泄漏

```golang
func main() {
    for {
        select {
        case <-time.After(time.Second): //这种方式每秒执行一次，但存在内存泄漏，不要采用
            fmt.Println("-------")      //按秒循环打印------
        }
    }
}
```

修改如下：

```golang
func main() {
    for {
        t := time.NewTicker(time.Second)
        select {
        case <-t.C :                    //这种方式每秒执行一次，但存在内存泄漏，不要采用
            fmt.Println("-------")      //按秒循环打印------
        }
        t.Stop()                        //停止，防止内存泄漏
    }
}
```
