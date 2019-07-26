# 合约开发工具包 —— eosio.cdt

## 1. 安装

- 二进制安装方式

```bash
# 下载安装包
wget https://github.com/EOSIO/eosio.cdt/releases/download/v1.6.1/eosio.cdt_1.6.1-1_amd64.deb

# 安装包
apt -y install ./eosio.cdt_1.6.1-1_amd64.deb
apt -y install -f
```

- 二进制安装方式

```bash
git clone --recursive https://github.com/eosio/eosio.cdt --branch v1.6.1 --single-branch
cd eosio.cdt
./build.sh
./install.sh
```
