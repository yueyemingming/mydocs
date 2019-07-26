# 系统智能合约部署

```bash
# 编辑代码
vim hello.cpp

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

# 编译
eosio-cpp hello.cpp -o hello.wasm

# 创建合约账户，合约账户名称hello
cleos create account eosio hello YOUR_PUBLIC_KEY -p eosio@active

# 部署合约
cleos set contract hello CONTRACTS_DIR/hello -p hello@active

# 创建资产，以下两种格式都行
cleos push action hello hi '["bob"]' -p bob@active
cleos push action hello hi '["bob"]' -p alice@active

# 发型资产，这里可以允许增发。
# 把100个SYS发型到eosio账户中, 注意，只能发行给资产的创建者，即上面是由系统账户eosio创建的，这里也只能发型给eosio.
cleos push action eosio.token issue '[ "eosio", "100.0000 SYS", "memo" ]' -p eosio@active

  executed transaction: 36d21513b48f883413d295128a6caad81657baf824eecbb1158bd76c738d8c2f  128 bytes  184 us
  #   eosio.token <= eosio.token::issue           {"to":"eosio","quantity":"100.0000 SYS","memo":"memo"}
  warning: transaction executed locally, but may not be confirmed by the network yet         ]

# 转账bob
cleos push action eosio.token transfer '[ "eosio", "bob", "25.0000 SYS", "m" ]' -p eosio@active

  executed transaction: 745a5431bb49aa0bc20438e590c284445f85b7ad70225cfd71fee1dbef0dd4b3  128 bytes  177 us
  #   eosio.token <= eosio.token::transfer        {"from":"eosio","to":"bob","quantity":"25.0000 SYS","memo":"m"}
  #         eosio <= eosio.token::transfer        {"from":"eosio","to":"bob","quantity":"25.0000 SYS","memo":"m"}
  #           bob <= eosio.token::transfer        {"from":"eosio","to":"bob","quantity":"25.0000 SYS","memo":"m"}
  warning: transaction executed locally, but may not be confirmed by the network yet         ]

# 转账alice
cleos push action eosio.token transfer '[ "bob", "alice", "10.0000 SYS", "m" ]' -p bob@active

  executed transaction: ac154f7c28cf0657aa962e0c6831ce4795069a3f4941b5b4fa38f5c0ea0cdf9b  128 bytes  194 us
  #   eosio.token <= eosio.token::transfer        {"from":"bob","to":"alice","quantity":"10.0000 SYS","memo":"m"}
  #           bob <= eosio.token::transfer        {"from":"bob","to":"alice","quantity":"10.0000 SYS","memo":"m"}
  #         alice <= eosio.token::transfer        {"from":"bob","to":"alice","quantity":"10.0000 SYS","memo":"m"}
  warning: transaction executed locally, but may not be confirmed by the network yet         ]

# 查看alice,bob资产
cleos get currency balance eosio.token bob SYS
15.0000 SYS
root@da6d1f929e2c:~/eosio.contracts/contracts/eosio.token# cleos get currency balance eosio.token alice SYS
10.0000 SYS
cleos get currency balance eosio.token eosio SYS        # 175.0000 SYS
cleos get currency balance eosio.token bob SYS          # 15.0000 SYS
cleos get currency balance eosio.token alice SYS        # 10.0000 SYS
```
