# 创建账户

创建账户bob,alic

## 1. bob的key

```bash
cleos wallet create_key

  warn  2019-07-26T02:15:23.726 keosd     wallet.cpp:223                save_wallet_file     ] saving wallet to file /root/eosio-wallet/./default.wallet
  Created new private key with a public key of: "EOS8NDH9wTwbeKx9n9i3mavbnSVyhEie4NKARZj7Wy1bBxtS1k7q2"
```

## 2. 创建bob账户

通过系统账户eosio创建bob账户, 此处要求eosio账户key必须已经在钱包里

```bash
cleos create account eosio bob EOS8NDH9wTwbeKx9n9i3mavbnSVyhEie4NKARZj7Wy1bBxtS1k7q2

  executed transaction: ea3077c580c503cd01bd73b74598601058021a75362836576483b2c313336d75  200 bytes  139 us
  #         eosio <= eosio::newaccount            {"creator":"eosio","name":"bob","owner":{"threshold":1,"keys":[{"key":"EOS8NDH9wTwbeKx9n9i3mavbnSVyh...
```

## 3. alice的key

```bash
cleos wallet create_key
  warn  2019-07-26T02:15:27.227 keosd     wallet.cpp:223                save_wallet_file     ] saving wallet to file /root/eosio-wallet/./default.wallet
  Created new private key with a public key of: "EOS5BDnLT41xLLCF9rfQAUHZ2pTkPTPbrmz8mBJQKTb2WAYM9Fgqy"
```

## 4. 创建alice账户

通过系统账户eosio创建alice账户, 此处要求eosio账户key必须已经在钱包里

```bash
cleos create account eosio alice EOS5BDnLT41xLLCF9rfQAUHZ2pTkPTPbrmz8mBJQKTb2WAYM9Fgqy

  executed transaction: 54acac7e451e329038287d7363d9e1e6f00464530bdaf201e7f678267de83b5c  200 bytes  316 us
  #         eosio <= eosio::newaccount            {"creator":"eosio","name":"alice","owner":{"threshold":1,"keys":[{"key":"EOS5BDnLT41xLLCF9rfQAUHZ2pT...
```

## 5. 查看账户信息

```bash
cleos get account bob
cleos get account alice
```
