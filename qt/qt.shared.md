# Qt5 库创建及使用

## 1. 创建

![image](D:\mydocs\qt\qt.lib\create.lib\001.png)

![002](D:\mydocs\qt\qt.lib\create.lib\002.png)

![003](D:\mydocs\qt\qt.lib\create.lib\003.png)

![004](D:\mydocs\qt\qt.lib\create.lib\004.png)

![005](D:\mydocs\qt\qt.lib\create.lib\005.png)

![006](D:\mydocs\qt\qt.lib\create.lib\006.png)

![007](D:\mydocs\qt\qt.lib\create.lib\007.png)

## 2. 使用动态库

![](D:\mydocs\qt\qt.lib\use.shared\001.png)

![002](D:\mydocs\qt\qt.lib\use.shared\002.png)

![003](D:\mydocs\qt\qt.lib\use.shared\003.png)

![004](D:\mydocs\qt\qt.lib\use.shared\004.png)

![005](D:\mydocs\qt\qt.lib\use.shared\005.png)

## 3. 使用静态库

静态库跟动态库的使用方式是一样的，唯一不同的就是把 MyLib.dll 换成 libMyLib.a

```ini
LIBS += D:\qt.lib\UseStatic\debug\libMyLib.a
```

或者 

```ini
LIBS += -LD:\qt.lib\UseStatic\debug\ -lMyLib
```

