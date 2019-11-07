# msys2安装

- [msys2官方网站 - <http://www.msys2.org/>](http://www.msys2.org/)
- [安装(略)]

## 更新

### 修改仓库配置文件

修改为清华和中科大的仓库源

- %MSYS2_HOME%\etc\pacman.d\mirrorlist.mingw32

```ini
Server = https://mirrors.tuna.tsinghua.edu.cn/msys2/mingw/i686
Server = http://mirrors.ustc.edu.cn/msys2/mingw/i686
```

- %MSYS2_HOME%\etc\pacman.d\mirrorlist.mingw64

```ini
Server = https://mirrors.tuna.tsinghua.edu.cn/msys2/mingw/x86_64
Server = http://mirrors.ustc.edu.cn/msys2/mingw/x86_64
```

- %MSYS2_HOME%\etc\pacman.d\mirrorlist.msys

```ini
Server = https://mirrors.tuna.tsinghua.edu.cn/msys2/msys/$arch
Server = http://mirrors.ustc.edu.cn/msys2/msys/$arch
```

### 更新软件源

```bash
pacman -Syu
```

### 更新msys2核心程序包

```bash
pacman -S --needed filesystem msys2-runtime bash libreadline libiconv libarchive libgpgme libcurl pacman ncurses libintl
```

## 安装开发工具包

```bash
pacman -S mingw-w64-x86_64-toolchain    #这里面gcc，gdb，make等都有了。
pacman -S mingw-w64-x86_64-cmake
pacman -S mingw-w64-x86_64-clang    #可以单独去仓库中下载指定版本安装
```
