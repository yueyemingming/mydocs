# 基础

```go
//main.go

//定义包名
package main	//只有名称为main包，可以有main函数，只有非main包才可以被导出

//包路径为 $GOPATH/src/
import "fmt"	//导入非main包
import (		//导入多个包
    "net/http"
    "json"
)
import "fmt"	//导入非main包

//常量
const (
    PI = 3.14				//定义，类型自动推到
    Num = 2					//首字母大写才能被导出
    name string = "fuck"	//指定类型
)

//变量
var (
    Pi float64				//只声明，未定义
    Num int64 = 2
)

///////////////////////////// 结构体 ////////////////////////////
// type 结构名 struct {}
type Gohper struct {    
}

//结构结构体函数成员
// func (对象名 类型名) 函数名() {}
type (gohper Gopher) Go() {    
}

///////////////////////////// 接口 ////////////////////////////
//type 名称 interface {}
type Writer interface {}

//主程序
func main() {
    fmt.Println("Hello world .")
}
```

```bash
go build .
go build main.go
go build -o main main.go
```

## 1. 引入外部包

```go
import "fmt"	//导入fmt
import f "fmt"	//别名，f.Println(...)
import . "fmt"	//省略包名， Println(...)
```

## 2. 可见性规则

- 首字母是 **小写**，表示私有
- 首字母是 **小写**，表示公有，可被外部调用。
- 以上规则，适合**所有的**go语言对象