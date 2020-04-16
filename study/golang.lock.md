# 锁

## 互斥锁

```go
import "sync"     //引入包

func main() {
    rand.Seed(time.Now().Unix())

    var lock sync.Mutex     //创建锁

    a := 3

    for i := 0; i < 2; i++ {
        go func(p *int) {
            lock.Lock()         //加锁
            *p = rand.Intn(10)
            fmt.Println(*p)
            lock.Unlock()       //解锁
        }(&a)
    }

    fmt.Println(a)

    time.Sleep(time.Second)
}
```

## 读写锁

```go
import "sync"     //引入包

var rwlock sync.RWMutex     //创建锁

rwlock.RLock()
rwlock.RUnlock()

rwlock.WLock()
rwlock.WUnlock()
```

## 原子操作

```go
import "sync/atomic"

var count int32
atomic.AddInt32(&count, 1)  //原子操作+
atomic.LoadInt32(&count)    //原子操作读
```