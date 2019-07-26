# 系统智能合约部署

- 下载合约代码

```bash
git clone https://github.com/EOSIO/eosio.contracts
```

- 创建合约账户

```bash
cd eosio.contracts/contracts/eosio.token
cleos create account eosio eosio.token EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
```

- 编译合约

```bash
eosio-cpp -I include -o eosio.token.wasm src/eosio.token.cpp --abigen
# --abigen 生成abi文件

# 坑：/usr/lib/x86_64-linux-gnu/libstdc++.so.6: version `GLIBCXX_3.4.22' not found
# 解决办法

  # 查看glibcxx版本
  strings /usr/lib/x86_64-linux-gnu/libstdc++.so.6 | grep GLIBCXX

  # 安装add-apt-repository, 这里针对ubuntu16.04的docker，如果是ubuntu16.04主机不需要
  apt-get install python-software-properties software-properties-common

  add-apt-repository -y ppa:ubuntu-toolchain-r/test
  apt update
  apt -y install gcc-4.9
  apt -y upgrade libstdc++6
  apt -y dist-upgrade
```

- 部署合约

```bash
cleos set contract eosio.token . --abi eosio.token.abi -p eosio.token@active

  Reading WASM from /root/eosio.contracts/contracts/eosio.token/eosio.token.wasm...
  Publishing contract...
  executed transaction: 03a9717f9a8b3565df71c70cdfe64042428fdda2f33146837a5f93c6da0ab9cf  6984 bytes  5495 us
  #         eosio <= eosio::setcode               {"account":"eosio.token","vmtype":0,"vmversion":0,"code":"0061736d0100000001a0011b60000060017e006002...
  #         eosio <= eosio::setabi                {"account":"eosio.token","abi":"0e656f73696f3a3a6162692f312e310008076163636f756e7400010762616c616e63...
  warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

- 创建资产

`cleos push acction` 这个动作也叫做 **调用合约**

以下两种格式都行

```bash
cleos push action eosio.token create '["eosio", "1000000000.0000 SYS"]' -p eosio.token@active
cleos push action eosio.token create '{"issuer":"eosio", "maximum_supply":"1000000000.0000 SYS"}' -p eosio.token@active

  executed transaction: 8ce7a4b6c0aac82753632014acb8ed74c323cacd1c3b492d7f0d740c1bb06dca  120 bytes  1107 us
  #   eosio.token <= eosio.token::create          {"issuer":"eosio","maximum_supply":"1000000000.0000 SYS"}
  warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

- 发型资产

> 这里可以操作增发。

把100个SYS发型到eosio账户中, 注意，只能发行给资产的创建者，即上面是由系统账户eosio创建的，这里也只能发型给eosio.

```bash
cleos push action eosio.token issue '[ "eosio", "100.0000 SYS", "memo" ]' -p eosio@active

  executed transaction: 36d21513b48f883413d295128a6caad81657baf824eecbb1158bd76c738d8c2f  128 bytes  184 us
  #   eosio.token <= eosio.token::issue           {"to":"eosio","quantity":"100.0000 SYS","memo":"memo"}
  warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

- 转账bob

```bash
cleos push action eosio.token transfer '[ "eosio", "bob", "25.0000 SYS", "m" ]' -p eosio@active

  executed transaction: 745a5431bb49aa0bc20438e590c284445f85b7ad70225cfd71fee1dbef0dd4b3  128 bytes  177 us
  #   eosio.token <= eosio.token::transfer        {"from":"eosio","to":"bob","quantity":"25.0000 SYS","memo":"m"}
  #         eosio <= eosio.token::transfer        {"from":"eosio","to":"bob","quantity":"25.0000 SYS","memo":"m"}
  #           bob <= eosio.token::transfer        {"from":"eosio","to":"bob","quantity":"25.0000 SYS","memo":"m"}
  warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

- 转账alice

```bash
cleos push action eosio.token transfer '[ "bob", "alice", "10.0000 SYS", "m" ]' -p bob@active

  executed transaction: ac154f7c28cf0657aa962e0c6831ce4795069a3f4941b5b4fa38f5c0ea0cdf9b  128 bytes  194 us
  #   eosio.token <= eosio.token::transfer        {"from":"bob","to":"alice","quantity":"10.0000 SYS","memo":"m"}
  #           bob <= eosio.token::transfer        {"from":"bob","to":"alice","quantity":"10.0000 SYS","memo":"m"}
  #         alice <= eosio.token::transfer        {"from":"bob","to":"alice","quantity":"10.0000 SYS","memo":"m"}
  warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

- 查看alice,bob资产

```bash
cleos get currency balance eosio.token eosio SYS
 75.0000 SYS

cleos get currency balance eosio.token bob SYS
 15.0000 SYS

cleos get currency balance eosio.token alice SYS
 10.0000 SYS
```
