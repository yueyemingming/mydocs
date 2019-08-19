# eos 9大系统账户名称及作用

名称 | 作用
:--- | :---
eosio.token | 发行和管理token的账户;
eosio.msig | 多重签名管理的账户;
eosio.names | 靓号账户拍卖管理的账户;
eosio.ram | 内存买卖管理的账户;
eosio.ramfee | 内存买卖收取手续费的账户，按照每笔交易千分之5的费率收取手续费;
eosio.saving | 增发EOS临时存放账户，增发总量 5%，其中80%放在此账户，另外 20%再分成25%和75%，分别给eosio.bpay和eosio.vpay;
eosio.stake | 管理EOS抵押的账户;
eosio.bpay | 矿工获取出块奖励的临时代管账户，增发EOS的1%的25%会先转到这个账户;
eosio.vpay | 矿工按照获得投票多少比例获取奖励的临时代管账户，增发EOS的1%的75%会先转到这个账户.
