# http使用

## 1. 服务端

```go
package main

import "fmt"
import "net/http"
import "io/ioutil"
import "encoding/json"

func handler(writer http.ResponseWriter, request *http.Request) {
    fmt.Fprintf(writer, "Hello World !")
    body, _ := ioutil.ReadAll(request.Body)
    fmt.Println(string(body))

    var args interface{}
    json.Unmarshal([]byte(body), &args)
    fmt.Println(args)
}

func main() {
    http.HandleFunc("/", handler)
    http.ListenAndServe(":8080", nil)
}
```

## 2. 客户端

```go
package main

import "fmt"
import "encoding/json"
import "net/http"
import "io/ioutil"
import "bytes"

type MyArgs struct {
    Consensus string `json:"consensus"`
    Token string `json:"token"`
    Interval int `json:"interval"`
    NodeCount int `json:"node_count"`
}

func main()  {
    myargs := MyArgs{"dpos", "TST", 3000, 1}
    data, _ := json.Marshal(myargs)
    fmt.Println(string(data))

    send_body := bytes.NewReader(data)
    response, _ := http.Post("http://192.168.1.79:8080","application/json;charset=utf-8", send_body)
    body, _ := ioutil.ReadAll(response.Body)
    fmt.Println(string(body))
}
```
