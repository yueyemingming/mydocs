# 在Ubuntu 16.04 下打包成Debian包的过程

## 1. 建立好如下文件（夹）结构

```t
└── mydeb  #目录 名字自取
    |
    ├── application #目录 名字确定
    │   ├── myapp   #目录 自己应用程序的名字
    │   │   ├── myapp   #程序或或，或其他可运行程序
    │   │   └── data    #程序或文件
    │   └── lib
    │       ├── myapp.desktop  #文件 需要复制到/usr/share/applications/目录，dash中可搜索
    │       ├── myapp.png      #文件 应用程序在unity中显示的图标 需要复制到/usr/share/pixmaps/目录
    │
    │
    └── DEBIAN        #目录 名字确定
        ├── control   #文件 名字不可改
        ├── postinst  #脚本文件 名字不可改 ，运行sudo dpkg -i xx.deb命令时，会运行这个脚本
        ├── postrm    #脚本文件 名字不可改 ，运行sudo dpkg -r app 命令时，会运行这个脚本
```

> 上面结构中, mydeb目录下面的两个目录 application 和 DEBIAN 名字不可改变的。

mydeb/application/lib目录下面的 应用程序图标文件就不用说了，主要是.desktop文件 里面应该写些什么 内容

mydeb/DEBIAN/目录下面的三个文件的内容，是我们需要自己动手写的

## 2. myapp.desktop文件的基本内容

```ini
[Desktop Entry]
Version=0.1                                 #应用程序版本
Name=myapp                                  #应用程序名
Comment=Back up your data with one click    #应用程序描述
Exec=/usr/bin/myapp                         #可运行应用程序最终的绝对路径
keywords=google,myapp,myapp                 #在dasn串搜索时，可用的关键字
StartupNotify=true                          #这个不知道是什么意思
Terminal=false                              #运行时不需要打开终端
Type=Application                            #应用程序类型，在dash和分类中会有显示，还有其它的类型
Categories=Utility;Application;             #应用程序的分类，工具/应用程序
Hidden=false                                #是否隐藏（否），不知道是什么意思
Icon=/usr/share/pixmaps/myapp.png           #应用程序图标名的绝对路径
```

## 3. control文件的基本内容

有一点需要说一下，很多人都不知道Depends后面的依赖库名字是如何得到的，实际上非常容易

使用ldd命令，后面加你程序的名字，就可以得到你的程序需要哪些动态链接库（配合grep qt命令使用）

```js
Package:myapp
Version:0.1
Section:utils
Priority:extra
Maintainer:Me You(QQ:397916230)
Depends:libqt5widgets5(>=5.0),libqt5gui5(>=5.0),libqt5core5a(>=5.0),libqt5x11extras5(>=5.0),libqt5x11extras5-dev(>=5.0)
Architecture:amd64
Description: The software is an opensource package from  397916230@qq.com
```

## 4。 postinst —— 安装用脚本

这个脚本需要可运行的权限

```bash
touch postinst && chmod 755 postinst  
```

```bash
echo "Start to install"                                         #echo显示
mv -f /application/lib/myapp.desktop /usr/share/applications/   #拷贝桌面快捷方式
mv -f /application/lib/myapp.png /usr/share/pixmaps/            #拷贝需要的文件
mv -f /application/myapp/* /usr/bin/                            #拷贝可执行文件等

echo "set QT_PLUGIN_PATH in file .profile"
echo "QT_PLUGIN_PATH=/usr/lib/x86_64-linux-gnu/qt5/plugins" >> ~/.profile   #设置环境变量
echo "Install ok"
```

## 5. postrm —— 卸载用脚本

这个脚本需要可运行的权限

```bash
touch postrm &&  chmod 755 postrm
```

```bash
echo "Start to remove"
rm -rf  /usr/share/applications/myapp.desktop
rm -rf  /usr/share/pixmaps/myapp.png
rm -rf  /usr/bin/myapp
rm -rf  /usr/bin/pymyapp
echo "Remove finished!
```

## 6. 总结

### 6.1 打包

```bash
dpkg -b mydeb/ myapp_0.1_amd64.deb
```

打包完成后，进行安装，删除测试

### 6.2 安装

```bash
dpkg -i  myapp_0.1_amd64.deb
apt-get install -f              #自动 安装程序需要的依赖库
```

### 6.3 删除

```bash
dpkg -r app_name
```
