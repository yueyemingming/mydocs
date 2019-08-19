# 系统智能合约部署

## 1. hello合约账号向任意人打招呼

### 1.1 编辑代码

```bash
vim hello.cpp
```

```cpp
#include <eosio/eosio.hpp>

using namespace eosio;

class [[eosio::contract]] hello : public contract {
  public:
      using contract::contract;

      [[eosio::action]]
      void hi( name user ) {
         print( "Hello, ", user);
      }
};
```

### 1.2 编译

```bash
eosio-cpp hello.cpp -o hello.wasm
```

### 1.3 创建合约账户，合约账户名称hello

```bash
cleos wallet create_key
  Created new private key with a public key of: "EOS8WaPmdy8dNS3QUGmCtQbGjLb2jAgKThuJGrwTgj1ezFFvWxeMT"

cleos create account eosio hello EOS8WaPmdy8dNS3QUGmCtQbGjLb2jAgKThuJGrwTgj1ezFFvWxeMT -p eosio@active
  executed transaction: 91d94ce8c53036b5c29ca8dd0009e457850cabd75aa72fdf889fe8b3001776f5  200 bytes  138 us
  #         eosio <= eosio::newaccount            {"creator":"eosio","name":"hello","owner":{"threshold":1,"keys":[{"key":"EOS8WaPmdy8dNS3QUGmCtQbGjLb...
  warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

### 1.4 部署合约

```bash
# 这是通过abi文件部署合约
#cleos set contract hello . --abi eosio.token.abi -p eosio.token@active

# 这是直接通过 wasm 文件部署合约
cleos set contract hello . -p hello@active
  Reading WASM from /root/hello/hello.wasm...
  Publishing contract...
  executed transaction: a6f543fda601324c78fca5fa4dd3756eb877e38619b7c0d41307f2ba9be19649  688 bytes  203 us
  #         eosio <= eosio::setcode               {"account":"hello","vmtype":0,"vmversion":0,"code":"0061736d0100000001370b6000017f60027f7f0060037f7f...
  #         eosio <= eosio::setabi                {"account":"hello","abi":"0e656f73696f3a3a6162692f312e31000102686900010475736572046e616d650100000000...
  warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

### 1.5 调用合约

```bash
cleos push action hello hi '["bob"]' -p bob@active
```

## 2. hello合约账号向任意人打招呼，但要经过对方许可

### 2.1 编辑代码

```bash
vim hello.cpp
```

```cpp
#include <eosio/eosio.hpp>

using namespace eosio;

class [[eosio::contract]] hello : public contract {
  public:
      using contract::contract;

      [[eosio::action]]
      void hi( name user ) {
         require_auth( user );
         print( "Hello, ", name{user} );
      }
};
```

### 2.2 编译

```bash
eosio-cpp hello.cpp -o hello.wasm
```

### 2.3 部署合约

```bash
cleos set contract hello . -p hello@active
  Reading WASM from /root/hello/hello.wasm...
  Skipping set abi because the new abi is the same as the existing abi
  Publishing contract...
  executed transaction: 38d3dbc7960ac3269447cad2f630402a101ad9ea3aceea62ffc1ecee5a32a6be  656 bytes  182 us
  #         eosio <= eosio::setcode               {"account":"hello","vmtype":0,"vmversion":0,"code":"0061736d0100000001370b6000017f60027f7f0060037f7f...
  warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

> 卧槽，这里这能部署同名函数合约呀。

### 2.4 调用合约

#### 2.4.1 向alice打招呼，经过 -p alice@active 允许

```bash
cleos push action hello hi '["alice"]' -p alice@active
  executed transaction: c599d5c4d45a100013767b010f38823d5ee88b9dcbe3bda22d461c7ee51b119e  104 bytes  157 us
  #         hello <= hello::hi                    {"user":"alice"}
  >> Hello, alice
  warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

#### 2.4.2 向bob打招呼，经过 -p bob@active 允许

```bash
cleos push action hello hi '["bob"]' -p bob@active
  executed transaction: 3f1509b62c1c0063e2460bd3aabb37960739b833395a16685c414c7077fa9a83  104 bytes  152 us
  #         hello <= hello::hi                    {"user":"bob"}
  >> Hello, bob
  warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

#### 2.4.3 向bob打招呼，经过 -p alice@actiove 允许,出错！！！！！！！！！-

```bash
cleos push action hello hi '["bob"]' -p alice@active
  Error 3090004: Missing required authority
  Ensure that you have the related authority inside your transaction!;
  If you are currently using 'cleos push action' command, try to add the relevant authority using -p option.
  Error Details:
  missing authority of bob
  pending console output:
```
