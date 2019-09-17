# 开启swap交换分区

swap交换分区除了在安装系统时，直接创建分区，还可以将文件作为交换分区，供系统使用。

```bash
# 建立一个 10G 的 swap 所需的文件空间。
dd if=/dev/zero of=/var/blockd.swap bs=1M count=10240

# 格式化swap文件
mkswap /var/blockd.swap

echo "swapon /var/blockd.swap" >> /etc/rc.local
echo "/var/blockd.swap swap swap default 0 0" >> /etc/fstab

# 激活交换功能
wapon /var/blockd.swap

# 内存超过60%生效交换功能，临时生效
sysctl vm.swappiness=60

# 内存超过60%生效交换功能，永久生效
echo "vm.swappiness=60" >> /etc/sysctl.conf

# 关闭交换功能
swapoff /var/blockd.swap

# 查看 Swap 利用率
cat /proc/sys/vm/swappiness
```

> **vm.swappiness**  这个参数主要用来表示物理内存还剩多大比例才开始使用内存交换，本文中设置的值为 60 即当物理内存还剩 60% 时开始进行内存交换
