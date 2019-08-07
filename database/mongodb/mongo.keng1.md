# mongodb 坑 "generic server error"

某些客户端在连接某些mongo服务端时，出现"generic server error"这个错误。

- 原因
  客户端驱动版本过高，而服务端mongodb版本过低。

- 解决办法

  1. 降低客户端驱动版本
  2. 提升服务器端mongodb版本。具体请查看[mongodb安装](mongo.install.md)。
