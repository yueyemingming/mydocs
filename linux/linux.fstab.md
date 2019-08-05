# 解决存储设备自动挂载及挂载是umask为0000的问题

```bash
lsblk
  /dev/sda5 8C645D6C645D5A52
  ...

vim /etc/fstab
```

```ini
# /etc/fstab
# Created by anaconda on Sat Dec 20 14:15:05 2014
#
# Accessible filesystems, by reference, are maintained under '/dev/disk'
# See man pages fstab(5), findfs(8), mount(8) and/or blkid(8) for more info
#
UUID=4ea0ab97-7e99-4c9f-87a2-04de0080a0df /                       ext4    defaults        1 1
UUID=0704fc4a-718f-4a9f-a40f-da794893eba5 /boot                   ext4    defaults        1 2
UUID=cdf1abc1-25f4-40f8-8e61-062c1be2e575 /workspace              ext4    defaults        1 2
UUID=703295b1-56ca-4bc6-bec9-d3aa809a6e6b swap                    swap    defaults        0 0
tmpfs                   /dev/shm                tmpfs   defaults        0 0
devpts                  /dev/pts                devpts  gid=5,mode=620  0 0
sysfs                   /sys                    sysfs   defaults        0 0
proc                    /proc                   proc    defaults        0 0
UUID=8C645D6C645D5A52 /media/c                       ntfs-3g    umask=0002        0 0
UUID=080C30170C2FFE6E /media/d                       ntfs-3g    umask=0002        0 0
UUID=B800A0AC00A07354 /media/rui-goflex                       ntfs-3g    umask=0002        0 0
UUID=0C00AD2200AD13AC /media/rui-u                       ntfs-3g    umask=0002        0 0
```

> 最后的几句保证了在插入ntfs设备时，自动挂载到某一个已经创建好的目录上。并且制定挂载时的umask为0002.
