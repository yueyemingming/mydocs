# Qt widget程序在ubuntu 16.04 unity界面无法全屏的解决办法

## 全屏方法

### 主要思路

qtwidget窗口默认为子窗口，升级为顶级窗口，而后再全屏。

### 升级顶级窗口

setWindowFlags(Qt::Dialog);
setWindowFlags(Qt::Window);

### 全屏显示

showFullScreen();

## 恢复方法

### 设置为非顶级窗口

setWindowFlags(Qt::subwindow);
setWindowFlags(Qt::Dialog);

### 正常显示

showNormal();
