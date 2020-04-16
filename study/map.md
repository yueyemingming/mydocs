# Map

Map 是使用 hash 表来实现的, 因此它是无序的。

## 1. 定义

### 1.1 声明, 创建空间, 赋值

***声明是不会分配内存的，初始化需要make***

```go
var a map[string]string     //声明, 默认nil
a = make(map[string]string) //创建空间
a["key"] = "value"
```

```go
a := make(map[string]string)    //自动推导类型
a["key"] = "value"
```

### 1.2 直接初始化

```go
var a map[string]string = map[string]string{
    "key" : "value",
}
```

### 1.3 二维map

```go
func main() {
    a := make(map[string]map[string]string)
    a["key1"] = make(map[string]string)
    a["key1"]["k1"] = "v1"
    a["key1"]["k2"] = "v2"
    a["key1"]["k3"] = "v3"

    fmt.Println(a)
}
```

结果如下：

```result
map[key1:map[k1:v1 k2:v2 k3:v3]]
```

## 2. 操作

### 2.1 插入和更新

```go
a [ "aaa" ] = "AAA"
```

### 2.2 查找

```go
value, ok := a [ "xxx" ]
if (ok) {
    fmt.Println("exist")
} else {
    fmt.Println("not exist")
}
```

### 2.3 遍历

```go
for key := range a {
    fmt.Println(key, "value is ", a[key])
}
```

### 2.4 删除

```go
delete(a, "aaa")
```

### 2.5 长度

```go
len(a)
```
