# Hello World 智能合约开发示例

Fabric智能合约又叫做链码

## 1. Hello World 源码

```golang
package main

import "fmt"
import "github.com/hyperledger/fabric/core/chaincode/shim"
import "github.com/hyperledger/fabric/protos/peer"

type Hello struct{}

func (t *Hello) Init( stub shim.ChaincodeStubInterface) peer.Response {
    args := stub.GetStringArgs()
    fmt.Println("----------------------------------init-------------------------")
    fmt.Println(stub)
    err := stub.PutState(args[0], []byte(args[1]))
    if err != nil {
        shim.Error(err.Error())
    }

    return shim.Success(nil)
}

func (t *Hello) Invoke( stub shim.ChaincodeStubInterface ) peer.Response {
    fmt.Println("----------------------------------init-------------------------")
    fn, args := stub.GetFunctionAndParameters()
    if fn == "set" {
        return t.set( stub, args )
    } else if fn == "get" {
        return t.get( stub, args )
    }

    return shim.Error("Invoke fn error")
}

func (t *Hello) set(stub shim.ChaincodeStubInterface, args []string ) peer.Response {
    fmt.Println("----------------------------------set-------------------------")
    err := stub.PutState(args[0], []byte(args[1]))
    if err != nil {
        return shim.Error(err.Error())
    }

    return shim.Success(nil)
}

func (t *Hello) get( stub shim.ChaincodeStubInterface, args []string) peer.Response {
    fmt.Println("----------------------------------get-------------------------")
    value, err := stub.GetState(args[0])
    if err != nil {
        return shim.Error(err.Error())
    }

    return shim.Success(value)
}

func main() {
    err := shim.Start(new(Hello))
    if err != nil {
        fmt.Println("start error")
    }

    fmt.Println("fuck")
}
```

## 2. 测试智能合约

```bash
peer chaincode install -n hello -p github.com/hyperledger/fabric/singlepeer/chaincode/go/ -v 1.0
```

### 5.2 调用智能合约

* 初始化数据为str为"hello world"。

```bash
peer chaincode instantiate -o orderer.example.com:7050 -C mychannel -n hello -v 1.0 -c '{"Args":["str","hello world"]}' -P "AND ('Org1MSP.peer')"
```

* 查询str

```bash
peer chaincode query -C mychannel -n hello -c '{"Args":["get","str"]}'
```

* 设置str

```bash
peer chaincode invoke -C mychannel -n hello -c '{"Args":["set", "str", "fuck"]}'
```

* 设置xxx

```bash
peer chaincode invoke -C mychannel -n hello -c '{"Args":["set", "xxx", "fuck"]}'
```
