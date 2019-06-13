# fabric各个组件的作用

文件(或目录) | 说明
:--- |:---
cryptogen | 生成用于识别和验证网络中各种组件的x509证书
configtxgen | 生成用于通道和区块所需要配置文件
configtx.yaml | 生成创世区块和通道相关信息的容器配置文件，并保存在channel-artifacts文件夹
crypto-config.yaml | 生成公私钥和证书信息的容器配置文件，并保存在crypto-config文件夹中
channel-artifacts | 存放生成的通道和创世纪块等文件，包括有channel.tx、genesis.block、Org1MSPanchors.tx和Org2MSPanchors.tx
crypto-config | 存放生成的公私钥和证书等文件
