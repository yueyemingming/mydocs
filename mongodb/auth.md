# MongoDB用户角色配置

## 1. 基本知识介绍

### 1.1 MongoDB基本的角色

1. 超级用户角色：root 
2. 数据库管理角色：dbAdmin、dbOwner、userAdmin；
3. 集群管理角色：clusterAdmin、clusterManager、clusterMonitor、hostManager；
4. 备份恢复角色：backup、restore；
5. 所有数据库角色：readAnyDatabase、readWriteAnyDatabase、userAdminAnyDatabase、dbAdminAnyDatabase
6. 数据库用户角色：read、readWrite;

> 这里还有几个角色间接或直接提供了系统超级用户的访问（dbOwner 、userAdmin、userAdminAnyDatabase）

其中MongoDB默认是没有开启用户认证的，也就是说游客也拥有超级管理员的权限。

userAdminAnyDatabase：有分配角色和用户的权限，但没有查写的权限

## 2. 带授权启动

修改mongod.conf文件

```yaml
security:
  authorization: enabled    //启用授权
```

```bash
service mongod start      #启动
service mongod restart    #重启
```

## 3. 客户端操作

```bash
/mongo   #连接到MongoDB服务器
```

### 3.1 创建root/admin用户

- 创建系统管理员

```js
use admin
db.createUser({user:"root",pwd:"password",roles:["root"]})
```

- 创建eos的reader账户

```js
use admin   //注意这里是在admin库中，创建一个针对eos库操作的账户，这个账户只能读eos库
db.createUser({user:"root",pwd:"password",roles:["root"]})

use eos     //注意这里是在eos库中，创建一个读自身eos库操作的账户，这个账户只能读eos库
db.createUser({
    user: "reader",
    pwd: "password",
    roles: [{role: "read", db: "eos"}]
})
```

### 3.2 登陆密码认证

```js
use admin
db.auth("admin","password");
```

```js
use eos
db.auth("reader","password");
```

### 3.2 修改用户密码

```js
use admin
db.updateUser( "admin",{pwd:"password"});
```

```js
use eos
db.updateUser( "reader",{pwd:"chain"});
```

### 3.3 删除用户

```js
use admin
db.system.users.remove({user:"reader"});    //需要root权限，会将所有数据库中的reader用户删除

use eos
db.dropUser("reader");                      //删除用户(权限要求没有那么高，只删除本数据中的reader用户)
```

```bash
mongo admin --eval 'db.createUser({user:"root", pwd:"123456", roles:["root"]})'
mongo admin --eval 'db.auth("root", "123456") ; db.createUser({user:"eoser", pwd:"123456", roles:[{role:"dbAdmin", db:"eos"}]})'
mongo admin --eval 'db.auth("root", "123456") ; db.createUser({user:"aaa", pwd:"123456", roles:[{role:"dbOwnew", db:"eos"}]})'
```
