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

## 启动节点

```bash
rippled --start --conf=/root/ripple/alonemode.two.nodes/node1.cfg
rippled --start --conf=/root/ripple/alonemode.two.nodes/node2.cfg
```

## 查看服务器状态

```bash
rippled  --conf=/root/ripple/alonemode.two.nodes/node2.cfg server_info

```

显示如下：

```json
Loading: "/root/ripple/alonemode.two.nodes/node2.cfg"
2021-Jan-14 10:16:58.995027390 UTC HTTPClient:NFO Connecting to 127.0.0.1:6002

{
   "result" : {
      "info" : {
         "build_version" : "1.6.0",
         "closed_ledger" : {
            "age" : 5,
            "base_fee_xrp" : 1e-05,
            "hash" : "0E2FF12B65551F107B8862DFAD72A6A59F8003E36749F7996BA1C6BFDAD28243",
            "reserve_base_xrp" : 200,
            "reserve_inc_xrp" : 50,
            "seq" : 673
         },
         "complete_ledgers" : "empty",
         "hostid" : "develop",
         "io_latency_ms" : 1,
         "jq_trans_overflow" : "0",
         "last_close" : {
            "converge_time_s" : 2,
            "proposers" : 0
         },
         "load" : {
            "job_types" : [
               {
                  "in_progress" : 1,
                  "job_type" : "clientCommand"
               },
               {
                  "job_type" : "peerCommand",
                  "per_second" : 1
               }
            ],
            "threads" : 6
         },
         "load_factor" : 1,
         "peer_disconnects" : "0",
         "peer_disconnects_resources" : "0",
         "peers" : 1,
         "pubkey_node" : "n9KtAhiNf7TYbfTvMDt1BWjzpGmQMRQAPwDBxHBrwJVQmcbMqDYq",
         "pubkey_validator" : "nHB1c9HgLVFD1sBTUGDRUWgUYZdYh3XYacnPXKTpSy87rRmqHuiG",
         "published_ledger" : "none",
         "server_state" : "proposing",
         "server_state_duration_us" : "2551277",
         "state_accounting" : {
            "connected" : {
               "duration_us" : "97385790",
               "transitions" : 670
            },
            "disconnected" : {
               "duration_us" : "2271225",
               "transitions" : 1
            },
            "full" : {
               "duration_us" : "2066581269",
               "transitions" : 670
            },
            "syncing" : {
               "duration_us" : "0",
               "transitions" : 0
            },
            "tracking" : {
               "duration_us" : "4209",
               "transitions" : 670
            }
         },
         "time" : "2021-Jan-14 10:16:58.995462 UTC",
         "uptime" : 2166,
         "validation_quorum" : 2,
         "validator_list" : {
            "count" : 1,
            "expiration" : "never",
            "status" : "active"
         }
      },
      "status" : "success"
   }
}
```



## 查看peers链接情况

```bash
rippled  --conf=/root/ripple/alonemode.two.nodes/node1.cfg peers
```

显示如下：

```json
Loading: "/root/ripple/alonemode.two.nodes/node1.cfg"
2021-Jan-14 10:14:08.090435163 UTC HTTPClient:NFO Connecting to 127.0.0.1:5002

{
   "result" : {
      "cluster" : {},
      "peers" : [
         {
            "address" : "10.254.1.150:42896",
            "inbound" : true,
            "latency" : 0,
            "ledger" : "51A1C34CB4A64868CEB1042C7F0FEA9332D44BD60B07B1C99151FBA9E12D7AE7",
            "load" : 51,
            "metrics" : {
               "avg_bps_recv" : "4533",
               "avg_bps_sent" : "373",
               "total_bytes_recv" : "4794804",
               "total_bytes_sent" : "465266"
            },
            "protocol" : "XRPL/2.1",
            "public_key" : "n9KtAhiNf7TYbfTvMDt1BWjzpGmQMRQAPwDBxHBrwJVQmcbMqDYq",
            "sanity" : "unknown",
            "uptime" : 1994,
            "version" : "rippled-1.6.0"
         }
      ],
      "status" : "success"
   }
}
```

