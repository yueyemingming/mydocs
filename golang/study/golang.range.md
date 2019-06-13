# range

range 用于 for 循环中迭代数组(array)、切片(slice)、通道(channel)或集合(map)的元素。

* 数组或切片

```golang
nums := []int{2, 3, 4}
for i, num := range nums {
    fmt.Println("index:", i, " num:", num)
}
```

* map

```golang
kvs := map[string]string{"a": "apple", "b": "banana"}
for k, v := range kvs {
    fmt.Printf("%s -> %s\n", k, v)
}
```

* unicode字符串

```golang
//range也可以用来枚举Unicode字符串
//第一个参数是字符的索引
//第二个是字符（Unicode的值）本身。
for i, c := range "go中国" {
    fmt.Println(i, c)
}
```

```text
0 103
1 111
2 20013
5 22269
```
