# bitcoin 节点操作

## 同步数据

```bash
bitcoind -daemon -datadir=/opt/chain_data/.btc -server -txindex -rpcuser=root -rpcpassword=123456
bitcoind -daemon -datadir=/alidata1/btc -server -txindex -rpcuser=root -rpcpassword=123456
bitcoind -daemon -server -reindex -txindex -rpcuser=root -rpcpassword=123456
```

## 客户端命令

```bash
bitcoin-cli   -rpcuser=root -rpcpassword=123456 getblockhash 170
bitcoin-cli   -rpcuser=root -rpcpassword=123456 getblock 000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f 2
```

## rpc

```bash
curl -u root -X POST -i 'http://127.0.0.1:8332/' --data '{"method":"getinfo","params":[],"id":1,"jsonrpc":"1.0"}'
```