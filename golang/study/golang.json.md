# json序列化反序列化

- [1. 序列化int](#1-序列化int)
- [2. 序列化map](#2-序列化map)
- [3. 序列化结构体](#3-序列化结构体)

操作 | 介绍
:--- | :---
`json.Marshal()` | 序列化
`json.UnMarshal()` | 反序列化

## 1. 序列化int

```golang
func main() {
    data, _ := json.Marshal(10)
    fmt.Println(string(data))       //10

    var i int
    //var i interface{}             //万能接口, 默认被识别成map
    json.Unmarshal([]byte(data), &i)
    fmt.Println(i)                  //10
}
```

## 2. 序列化map

```golang
func main() {
    v := make(map[string]map[string]string)
    v["key1"] = make(map[string]string)
    v["key1"]["k1"] = "v1"
    v["key1"]["k2"] = "v2"
    v["key2"] = make(map[string]string)
    v["key2"]["k1"] = "v1"
    v["key2"]["k2"] = "v2"

    data, _ := json.Marshal(v)
    fmt.Println(string(data))   //{"key1":{"k1":"v1","k2":"v2"},"key2":{"k1":"v1","k2":"v2"}}

    var uv interface{}          //万能接口, 默认被识别成map
    json.Unmarshal([]byte(data), &vm)
    fmt.Println(vm)             //map[key2:map[k1:v1 k2:v2] key1:map[k1:v1 k2:v2]]
}
```

## 3. 序列化结构体

```golang
type Student struct {
    Name string                 //首字母必须大写才能被json导出
    Age  int
}

func main() {
    v := Student{"xiaoming", 10}
    data, _ := json.Marshal(v)
    fmt.Println(string(data))   //{"Name":"xiaoming","Age":10}

    uv := Student{}
    //var uv interface{}        //万能接口, 默认被识别成map
    json.Unmarshal([]byte(data), &uv)
    fmt.Println(uv)             //{xiaoming 10}
}
```

- 利用tag修改json字段名称

```golang
type Student struct {
    Name string `json:"student_name"`
    Age  int    `json:"student_age"`
}

func main() {
    v := Student{"xiaoming", 10}
    data, _ := json.Marshal(v)
    fmt.Println(string(data))       //{"student_name":"xiaoming","student_age":10}

    //uv := Student{}
    //json.Unmarshal([]byte(data), &uv)
    //fmt.Println(uv)               //{xiaoming 10}

    var uv interface{}              //万能接口, 默认被识别成map
    json.Unmarshal([]byte(data), &uv)
    fmt.Println(uv)                 //map[student_name:xiaoming student_age:10], 完成接口这里被识别成map，而不是结构体
}
```
