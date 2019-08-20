# java安装

* [安装文件](/software/develop/java/linux)

## 解压

```bash
tar xzvf jdk-8u151-linux-x64.tar.gz -C /opt
```

## 配置环境

```bash
vim ~/.bashrc   #添加如下内容到.bashrc中
------------------------------------------------------------------------
export JAVA_HOME=/opt/jdk1.8.0_151
export CLASSPATH=.:$JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar
export PATH=$PATH:$JAVA_HOME/bin
------------------------------------------------------------------------

source ~/.bashrc
```