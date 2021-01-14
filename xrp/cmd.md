# 命令使用



### 生成验证器密钥对

```
$ /opt/ripple/bin/validator-keys create_keys
```

警告：将生成的`validator-keys.json`密钥文件存储在安全但可恢复的位置，如加密的USB闪存驱动器。不要修改其内容。



### 生成验证器令牌并编辑您的`rippled.cfg`文件以添加`[validator_token]`值。

```
$ /opt/ripple/bin/validator-keys create_token --keyfile /path/to/your/validator-keys.json
```

如果您以前没有使用该`validator-keys`工具配置验证器，则还必须`[validation_seed]`从`rippled.cfg`文件中删除该验证器。这会更改您的验证器公钥。