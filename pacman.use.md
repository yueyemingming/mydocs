# pacman基本使用

```bash
pacman -Q --explicit        # 查看已安装软件包
pacman -Q -e                # 查看已安装软件包
pacman -S <package_names|package_groups>        # 安装新软件包或组
pacman -U <package_name>    # 直接安装软件包文件
pacman -R <package_names|package_groups>        # 删除软件包或组
pacman -Ss <name_pattern>   # 搜索软件包
pacman -Q --groups          # 列出所有软件组
```
