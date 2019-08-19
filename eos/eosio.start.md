# ubuntu16.04 eosio 安装,运行,测试helloworld智能合约

## 1. eosio安装

- ubuntu二进制安装方式

```bash
wget https://github.com/eosio/eos/releases/download/v1.8.1/eosio_1.8.1-1-ubuntu-16.04_amd64.deb
dpkg -i eosio_1.8.1-1-ubuntu-16.04_amd64.deb
apt -y install -f
```

- 源码编译安装方式

```bash
git clone https://github.com/EOSIO/eos
cd eos
git checkout v1.8.1
git submodule update --init --recursive
cd scripts
./eosio_build.sh
./eosio_install.sh

echo "" >> ~/.bashrc
echo "export PATH=$PATH:/root/eosio/1.8/bin" >> ~/.bashrc
source ~/.bashrc
```

## 2. eosio 运行单个节点

```bash
keosd &
nodeos -e -p eosio \
--plugin eosio::producer_plugin \
--plugin eosio::chain_api_plugin \
--plugin eosio::http_plugin \
--access-control-allow-origin='*' \
--contracts-console \
--http-validate-host=false \
--verbose-http-errors >> nodeos.log 2>&1 &

tail -f nodeos.log

cleos wallet list

curl http://localhost:8888/v1/chain/get_info
```

```json
{
    "server_version":"14185431",
    "chain_id":"cf057bbfb72640471fd910bcb67639c22df9f92470936cddc1ade0e2f2e7dc4f",
    "head_block_num":160,
    "last_irreversible_block_num":159,
    "last_irreversible_block_id":"0000009fba72bb5d7685821e6a9904bcee0604dd5c6f1b7e5e8adbf4a9912066",
    "head_block_id":"000000a09a19f3f8369052475485db3fe8b0864e0852ce0ff7fdd24229d61012",
    "head_block_time":"2019-07-24T02:21:39.500",
    "head_block_producer":"eosio",
    "virtual_block_cpu_limit":234402,
    "virtual_block_net_limit":1229296,
    "block_cpu_limit":199900,
    "block_net_limit":1048576,
    "server_version_string":"v1.8.1",
    "fork_db_head_block_num":160,
    "fork_db_head_block_id":"000000a09a19f3f8369052475485db3fe8b0864e0852ce0ff7fdd24229d61012"
}
```
