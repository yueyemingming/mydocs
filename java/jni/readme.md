# JNI技术总结

## 1. 简介

JNI是java语言访问c/c++语言的技术手段。

java native interface

## 2. PC（x86）平台jni

### 2.1 环境

software | desc
:--- | :---
系统 | linux
java | jdk-1.6.0.45 j2sdk开发包+jre(java虚拟机)
gcc | c/c++编译工具套件

### 2.2 过程

* 配置java环境

```bash
./jdk-6u45-linux-x64.bin
export    JAVA_HOME=/opt/jdk1.6.0_45
export    CLASSPATH=.:$JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar
export    PATH=$PATH:$JAVA_HOME/bin
javac -version
```

javac 1.6.0_45              (代表成功)

* gcc 编译套件

```bash
gcc -v
Using built-in specs.
……
```

gcc version 4.8.2           (代表成功)

* 编写编译java程序

编写 JniTest.java

```java
public class    JniTest
{
    static
    {
        System.loadLibrary("JniTest") ;
    }

    public native static void test() ;
    public static void main( String[] args )
    {
        test() ;
    }
}
```

生成 class

```bash
javac  JniTest.java      # JniTest.class这就是最终的java可执行程序
```

生成c头文件 JniTest.h

```bash
javah  JniTest.class
```

JniTest.h内容如下

```c++
/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class JniTest */

#ifndef _Included_JniTest
#define _Included_JniTest
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     JniTest
 * Method:    test
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_JniTest_test
  (JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif
```

* 编写编译c程序

JniTest.c

```bash
cp JniTest.h JniTest.c
```

```c++
/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class JniTest */

#ifndef _Included_JniTest
#define _Included_JniTest
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     JniTest
 * Method:    test
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_JniTest_test
  (JNIEnv *env, jclass obj)
{
    printf( "aaaaaaaaaaaaaaaaa\n" ) ;
}

#ifdef __cplusplus
}
#endif
#endif
```

生成C库 libJniTest.so

```bash
gcc -fPIC -shared JniTest.c -o libJniTest.so -I /opt/jdk1.6.0_45/include/ -I /opt/jdk1.6.0_45/include/linux/
```

* 运行

```bash
export LD_LIBRARY_PATH=/myspace/jni/new/        (指定C库位置)
java JniTest
```

aaaaaaaaaaaaaaaaaa                              (运行成功)

## 3. Android带包java程序jni头文件生成

JniTest.java

```java
package   com.example.jnitest ;         (此处加上包名)
public class    JniTest
{
    static
    {
        System.loadLibrary("JniTest") ;
    }

    public native static void test() ;
    public static void main( String[] args )
    {
        test() ;
    }
}
```

生成 class

eclipse直接编译java文件。生成带包class路径如下：  
  bin->classes->com->example->jnitest : `JniTest.class`

生成c头文件 JniTest.h

```bash
javah -classpath bin/classes -d jni com.example.jnitest.JniTest
```

```text
用法：javah [选项] <类>
其中 [选项] 包括：
    -help                   输出此帮助消息并退出
    -classpath <路径>       用于装入类的路径
    -bootclasspath <路径>   用于装入引导类的路径
    -d <目录>               输出目录
    -o <文件>               输出文件（只能使用 -d 或 -o 中的一个）
    -jni                    生成 JNI样式的头文件（默认）
    -version                输出版本信息
    -verbose                启用详细输出
    -force                  始终写入输出文件
使用全限定名称指定 <类>（例如, java.lang.Object）。
其中需要注意 -d jni , 指定头文件输出的目录必须为jni。
```

## 4. 如何编译此Android平台的动态库, 请参见[NDK技术总结](../ndk/readme.md)。