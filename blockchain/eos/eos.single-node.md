# eos 创建单一节点

`nodeos`

`vim /root/.local/share/eosio/nodeos/config`

```
enable-stale-production = true
producer-name = eosio
plugin = eosio::producer_plugin
plugin = eosio::wallet_api_plugin
plugin = eosio::chain_api_plugin
plugin = eosio::http_plugin
#plugin = eosio::account_history_api_plugin

```

`nodeos`

创建钱包
`cleos wallet create`

为超级账户eosio部署eosio.bios合约
`cleos set contract eosio ~/eos/build/contracts/eosio.bios -p eosio`

创建公私钥对
`cleos create key`

导入私钥
`cleos wallet import <privkey>`

创建合约账户currency
`cleos create account eosio currency <pubkey1> <pubkey2>`
eosio父账户, currency子帐户

查看账户currency
`cleos get account currency`

查看合约currency
`cleos get code currency`
0代表此账户还未部署合约
