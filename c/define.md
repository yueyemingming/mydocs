# define

## 1. 简单的define定义 

```c
#define MAXTIME 1000
```

这样的定义看起来类似于普通的常量定义CONST，但也有着不同，因为define的定义更像是简单的文本替换，而不是作为一个量来使用，这个问题在下面反映的尤为突出。

## 2. define的“函数定义”

define可以像函数那样接受一些参数，如下

```c
#define max(x,y) (x)>(y)?(x):(y); 
```

定义就将返回两个数中较大的那个。因为这个“函数”没有类型检查，就好像一个函数模板似的，当然，它绝对没有模板那么安全就是了。可以作为一个简单的模板来使用而已。

但是这样做的话存在隐患，例子如下：

```c
#define Add(a,b) a+b; 
```

如果遇到如：c * Add(a,b) * d的时候就会出现问题，代数式的本意是a+b然后去和c，d相乘，但是因为使用了define，所以式子实际上变成了c*a + b*d

## 3. 宏的单行定义

```c
#define A(x)  T_##x
#define B(x)  #@x
#define C(x)  #x
```

我们假设：x=1，则有：

A(1)------〉T_1

B(1)------〉'1'

C(1)------〉"1"

## 4. define的多行定义

define可以替代多行的代码，例如MFC中的宏定义（非常的经典，虽然让人看了恶心）

```c
#define MACRO(arg1, arg2) do { /
/* declarations */ /
stmt1; /
stmt2; /
/* ... */ /
} while(0) /* (no trailing ; ) */
```

关键是要在每一个换行的时候加上一个"/"

## 5. 夸平台条件编译

在大规模的开发过程中，特别是跨平台和系统的软件里，define 最重要的功能是条件编译。

```c
#ifdef WINDOWS
......
#endif
#ifdef LINUX
......
#endif
```

可以在编译的时候通过#define设置编译环境

## 6. 如何定义宏、取消宏

```c
//定义宏
#define [MacroName] [MacroValue]
//取消宏
#undef  [MacroName]
//普通宏
#define PI (3.1415926)
//带参数的宏
#define max(a,b) ((a)>(b)? (a),(b))
```

## 7. 条件编译

```c
#ifdef XXX…
(#else) …
#endif

#ifdef DV22_AUX_INPUT
	#define AUX_MODE 3
#else
	#define AUY_MODE 3
＃endif

#ifndef XXX …
(#else) … 
#endif
```

## 8. 头文件(.h)可以被头文件或C文件包含

重复包含（重复定义）

由于头文件包含可以嵌套，那么C文件就有可能包含多次同一个头文件，就可能出现重复定义的问题的。

通过条件编译开关来避免重复包含（重复定义）

例如

```c
#ifndef __headerfileXXX__
＃define __headerfileXXX__
…
//文件内容
…
#endif
```

## 9. #define中的#、## && #@ 

前些一段时间在看WinCE的Code时发现在宏定义中有用到##，如下所示

```c
#define GPEBLT_FUNCNAME(basename) (SCODE (GPE::*)(struct GPEBltParms *))&GPE::##basename
```

在#define中，标准只定义了#和##两种操作。

#用来把参数转换成字符串，##则用来连接两个前后两个参数，把它们变成一个字符串。 

```c
#define ToString(a) #a
ToString( A b Cd ); //A b Cd
ToString( A/n b Cd ); //A
// b Cd
ToString( A/ n b Cd ); //A n b Cd
```

另外，在网上搜到还有一种用法：#@，把参数转换成字符

```c
#define ToChar(a) #@a
ToChar(a); // a
ToChar(ab); // b
ToChar(abc); // c
ToChar(abcd); // d
//ToChar(abcde); // too many characters in constant
ToChar(1.); // .
```
