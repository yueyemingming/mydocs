# 命令

- 显示帮助

  ```go
  go
  ```

- 编译

  ```go
  go build
  go build .
  go build src/
  go build main.go
  go build -o main main.go
  go build -o main src/main.go
  go build -o main src/
  ```

- 安装

  ```go
  go install .
  go install main.go
  go install -o main main.o
  ```

- 直接运行

  ```go
  go run main.go
  ```
  
- 清理

  ```go
  go clean
  ```

- 显示环境变量

  ```go
  go env
  ```

- 格式化代码

  ```go
  go fmt
  ```

- 安装第三方包

  ```go
  go get github.com/go-sql-driver/mysql
  ```

- 测试

  ```go
  go test
  ```