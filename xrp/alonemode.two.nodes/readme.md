# 独立模式.双节点.搭建

### 1. 生成验证器密钥对

```bash
validator-keys create_keys --keyfile node1.validator-keys.json
validator-keys create_keys --keyfile node2.validator-keys.json
```

> 不使用 `--keyfile` 指定路径位置的话，默认位置为: `~/.ripple/validator-keys.json`

- 查看 **node1.validator-keys.json** 内容

```bash
cat node1.validator-keys.json
```

显示如下：
```json
{
   "key_type" : "ed25519",
   "public_key" : "nHB1c9HgLVFD1sBTUGDRUWgUYZdYh3XYacnPXKTpSy87rRmqHuiG",
   "revoked" : false,
   "secret_key" : "paGosU3zG3qVEt7c1oimW7XtpyMzYSDFjEzo2b83cQPJ9gB2UeJ",
   "token_sequence" : 1
}
```

- 查看 **node2.validator-keys.json** 内容
```bash
cat node2.validator-keys.json
```

显示如下：

```json
{
   "key_type" : "ed25519",
   "public_key" : "nHBoSFY7MTDdDPL3GDduGnkCj89vtgF4cZyhciCCPaMeYijFbJnR",
   "revoked" : false,
   "secret_key" : "paaSjLUDeTENkNcJu71KmVPkKpTavb8b5BySNjXYRY2LeiMsqzi",
   "token_sequence" : 0
}
```



### 2. 生成验证器令牌

根据 `node1.validator-keys.json` 文件生成验证器令牌

```
validator-keys create_token --keyfile node1.validator-keys.json
```

显示如下:

```text
Update rippled.cfg file with these values and restart rippled:

# validator public key: nHB1c9HgLVFD1sBTUGDRUWgUYZdYh3XYacnPXKTpSy87rRmqHuiG

[validator_token]
eyJtYW5pZmVzdCI6IkpBQUFBQUp4SWUxRWdMM05kVTNkZWMxemZoaXJlK0VNcFBPZWNZMW1J
c1BxblJhenNnNmlhbk1oQXpSakZxekg5MnRVYVpVRS9NcHJIS0lPUzJndmE1NnhFelVHQVB6
c09JalFka2N3UlFJaEFJK001VHJLRnFGRUxqYlllWXU2K05GeXZEY2Nsd3V4V1lIZ0pXVVd3
ZEFSQWlCRVdWRHlVMjJJRnBxQkJOTHAzTk94bUhGQmc5enN4czZOT2JaSlQ3dmh0SEFTUU0w
NDZsUUcvWThLQ1N6RkZjSldoS0VrZjZzMnFwV2lDbEthVCsxNGtySzk3amYzdlgvbVlTMnUw
cllXbmxaaVJqZmttRmxOYzBuWWxqUWVxTDdBQmdBPSIsInZhbGlkYXRpb25fc2VjcmV0X2tl
eSI6IkMyQzg5NTU2ODQ5QjYxNjNGRjFFQTcwMkQwQ0JDRTgwNjg5QUI1MTM0NjdGOEMyNEVC
RThGMEU0NEExNkFENzEifQ==
```



## 3. 编辑node1.cfg

```ini
[server]
port_peer
port_rpc_admin_local
port_ws_admin_local
#port_ws_public

[port_peer]
port = 5001
ip = 0.0.0.0
protocol = peer

[port_rpc_admin_local]
port = 5002
ip = 0.0.0.0
admin = 0.0.0.0
protocol = http

[port_ws_admin_local]
port = 5003
ip = 0.0.0.0
admin = 0.0.0.0
protocol = ws

[port_grpc]
port = 5004
ip = 0.0.0.0

[port_ws_public]
port = 5005
ip = 0.0.0.0
protocol = ws

[node_size]
small

[node_db]
#type=NuDB
#path=/data/rippled/node1/db/nudb
#online_delete=512
#advisory_delete=0

type=RocksDB
path=/data/rippled/node1/db/rocksdb
open_files=512
filter_bits=12
cache_mb=512
file_size_mb=64
file_size_mult=2
online_delete=2000
advisory_delete=0

[database_path]
/data/rippled/node1/db

[debug_logfile]
/data/rippled/node1/debug.log

[sntp_servers]
time.windows.com
time.apple.com
time.nist.gov
pool.ntp.org

[ips_fixed]
10.254.1.150 5001
10.254.1.150 6001

[validators]       #对应上面两个节点key文件中的公钥
nHBoSFY7MTDdDPL3GDduGnkCj89vtgF4cZyhciCCPaMeYijFbJnR
nHB1c9HgLVFD1sBTUGDRUWgUYZdYh3XYacnPXKTpSy87rRmqHuiG

#[validators_file]	#使用这个也行，但要与[validators]中的结构相同
#validators.txt

[rpc_startup]
{ "command": "log_level", "severity": "debug" }

[ssl_verify]
0

[peer_private]
1

# validator public key: nHB1c9HgLVFD1sBTUGDRUWgUYZdYh3XYacnPXKTpSy87rRmqHuiG
# 这个公钥也对应上面创建的令牌的公钥

[validator_token]				# 对应上面创建的令牌
eyJtYW5pZmVzdCI6IkpBQUFBQUp4SWUxRWdMM05kVTNkZWMxemZoaXJlK0VNcFBPZWNZMW1J
c1BxblJhenNnNmlhbk1oQXpSakZxekg5MnRVYVpVRS9NcHJIS0lPUzJndmE1NnhFelVHQVB6
c09JalFka2N3UlFJaEFJK001VHJLRnFGRUxqYlllWXU2K05GeXZEY2Nsd3V4V1lIZ0pXVVd3
ZEFSQWlCRVdWRHlVMjJJRnBxQkJOTHAzTk94bUhGQmc5enN4czZOT2JaSlQ3dmh0SEFTUU0w
NDZsUUcvWThLQ1N6RkZjSldoS0VrZjZzMnFwV2lDbEthVCsxNGtySzk3amYzdlgvbVlTMnUw
cllXbmxaaVJqZmttRmxOYzBuWWxqUWVxTDdBQmdBPSIsInZhbGlkYXRpb25fc2VjcmV0X2tl
eSI6IkMyQzg5NTU2ODQ5QjYxNjNGRjFFQTcwMkQwQ0JDRTgwNjg5QUI1MTM0NjdGOEMyNEVC
RThGMEU0NEExNkFENzEifQ==
```

## 4. 编辑node2.cfg

```ini
[server]
port_peer
port_rpc_admin_local
port_ws_admin_local
#port_ws_public

[port_peer]
port = 6001
ip = 0.0.0.0
protocol = peer

[port_rpc_admin_local]
port = 6002
ip = 0.0.0.0
admin = 0.0.0.0
protocol = http

[port_ws_admin_local]
port = 6003
ip = 0.0.0.0
admin = 0.0.0.0
protocol = ws

[port_grpc]
port = 6004
ip = 0.0.0.0

[port_ws_public]
port = 6005
ip = 0.0.0.0
protocol = ws

[node_size]
small

[node_db]
#type=NuDB
#path=/data/rippled/node1/db/nudb
#online_delete=512
#advisory_delete=0

type=RocksDB
path=/data/rippled/node1/db/rocksdb
open_files=512
filter_bits=12
cache_mb=512
file_size_mb=64
file_size_mult=2
online_delete=2000
advisory_delete=0

[database_path]
/data/rippled/node1/db

[debug_logfile]
/data/rippled/node1/debug.log

[sntp_servers]
time.windows.com
time.apple.com
time.nist.gov
pool.ntp.org

[ips_fixed]
10.254.1.150 5001
10.254.1.150 6001

[validators]       #对应上面两个节点key文件中的公钥
nHBoSFY7MTDdDPL3GDduGnkCj89vtgF4cZyhciCCPaMeYijFbJnR
nHB1c9HgLVFD1sBTUGDRUWgUYZdYh3XYacnPXKTpSy87rRmqHuiG

#[validators_file]	#使用这个也行，但要与[validators]中的结构相同
#validators.txt


[rpc_startup]
{ "command": "log_level", "severity": "debug" }

[ssl_verify]
0

[peer_private]
1

# validator public key: nHB1c9HgLVFD1sBTUGDRUWgUYZdYh3XYacnPXKTpSy87rRmqHuiG
# 这个公钥也对应上面创建的令牌的公钥

[validator_token]				# 对应上面创建的令牌
eyJtYW5pZmVzdCI6IkpBQUFBQUp4SWUxRWdMM05kVTNkZWMxemZoaXJlK0VNcFBPZWNZMW1J
c1BxblJhenNnNmlhbk1oQXpSakZxekg5MnRVYVpVRS9NcHJIS0lPUzJndmE1NnhFelVHQVB6
c09JalFka2N3UlFJaEFJK001VHJLRnFGRUxqYlllWXU2K05GeXZEY2Nsd3V4V1lIZ0pXVVd3
ZEFSQWlCRVdWRHlVMjJJRnBxQkJOTHAzTk94bUhGQmc5enN4czZOT2JaSlQ3dmh0SEFTUU0w
NDZsUUcvWThLQ1N6RkZjSldoS0VrZjZzMnFwV2lDbEthVCsxNGtySzk3amYzdlgvbVlTMnUw
cllXbmxaaVJqZmttRmxOYzBuWWxqUWVxTDdBQmdBPSIsInZhbGlkYXRpb25fc2VjcmV0X2tl
eSI6IkMyQzg5NTU2ODQ5QjYxNjNGRjFFQTcwMkQwQ0JDRTgwNjg5QUI1MTM0NjdGOEMyNEVC
RThGMEU0NEExNkFENzEifQ==
```



