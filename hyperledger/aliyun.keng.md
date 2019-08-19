# 在运行go程序过程中，阿里云存在巨大的坑，哪怕是在docker中

## 现象

* go程序崩溃的现象，哪怕是在docker中也存在

```text
fatal error: unexpected signal during runtime execution
[signal SIGSEGV: segmentation violation code=0x1 addr=0x63 pc=0x7f77885ea259]

runtime stack:
runtime.throw(0xe183a7, 0x2a)
    /opt/go/src/runtime/panic.go:616 +0x81
runtime.sigpanic()
    /opt/go/src/runtime/signal_unix.go:372 +0x28e

goroutine 22 [syscall]:
runtime.cgocall(0xb04b30, 0xc4205465f8, 0x29)
    /opt/go/src/runtime/cgocall.go:128 +0x64 fp=0xc4205465b8 sp=0xc420546580 pc=0x402124
net._C2func_getaddrinfo(0xc42003c834, 0x0, 0xc42017f080, 0xc42018a118, 0x0, 0x0, 0x0)
    _cgo_gotypes.go:92 +0x55 fp=0xc4205465f8 sp=0xc4205465b8 pc=0x51fa65
net.cgoLookupIPCNAME.func1(0xc42003c834, 0x0, 0xc42017f080, 0xc42018a118, 0x7, 0x7, 0xc420d30000)
    /opt/go/src/net/cgo_unix.go:149 +0x13b fp=0xc420546640 sp=0xc4205465f8 pc=0x5267cb
net.cgoLookupIPCNAME(0xc42018d0f0, 0x6, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0)
    /opt/go/src/net/cgo_unix.go:149 +0x174 fp=0xc420546738 sp=0xc420546640 pc=0x5210d4
net.cgoIPLookup(0xc420526300, 0xc42018d0f0, 0x6)
    /opt/go/src/net/cgo_unix.go:201 +0x4d fp=0xc4205467c8 sp=0xc420546738 pc=0x52179d
runtime.goexit()
    /opt/go/src/runtime/asm_amd64.s:2361 +0x1 fp=0xc4205467d0 sp=0xc4205467c8 pc=0x45b6d1
created by net.cgoLookupIP
    /opt/go/src/net/cgo_unix.go:211 +0xaf
```

* 对于peer来说，上面的现象导致了如下的上层现象

After 10 attempts,peer0.org1 has failed to join channel ‘mychannel’

    Error: error getting endorser client for channel: endorser client failed to connect to peer0.org1.example.com:7051: failed to create new connection: context deadline exceeded
    peer0.org1 failed to join the channel, Retry after 3 seconds

* 对于orderer来说，上面的现象导致了orderer节点根本无法启动，直接退出docker.

## 解决办法

修改 `/etc/resolv.conf` 文件，将里面 `options timeout:2 attempts:3 rotate single-request-reopen` 注释掉.