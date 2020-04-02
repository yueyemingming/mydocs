# typedef

## 1. 定义一种类型的别名，而不只是简单的宏替换。可以用作同时声明指针型的多个对象。

比如：

```c
char* pa, pb; // 这多数不符合我们的意图，它只声明了一个指向字符变量的指针，和一个字符变量；
typedef char* PCHAR;
PCHAR pa, pb;  
```

## 2. 用在旧的C代码中，帮助struct。

以前的代码中，声明struct新对象时，必须要带上struct，即形式为： struct 结构名对象名，如：

```c
struct tagPOINT1
{
    int x;
    int y; 
};
struct tagPOINT1 p1;

typedef TAGP struct tagPOINT1;
TAGP p2;
```

## 3. 用typedef来定义与平台无关的类型。

比如定义一个叫 REAL 的浮点类型，在目标平台一上，让它表示最高精度的类型为：

```c
typedef long double REAL;
```

在不支持 long double 的平台二上，改为：

```c
typedef double REAL;
```

在连 double 都不支持的平台三上，改为：

```c
typedef float REAL;
```

也就是说，当跨平台时，只要改下 typedef 本身就行，不用对其他源码做任何修改。

标准库就广泛使用了这个技巧，比如size_t。另外，因为typedef是定义了一种类型的新别名，不是简单的字符串替换，所以它比宏来得稳健。

这个优点在我们写代码的过程中可以减少不少代码量哦！

## 4. 为复杂的声明定义一个新的简单的别名。

方法是：在原来的声明里逐步用别名替换一部分复杂声明，如此循环，把带变量名的部分留到最后替换，得到的就是原声明的最简化版。举例： 

```c
//原声明
void (*b[10]) (void (*)());

//变量名为b，先替换右边部分括号里的，pFunParam为别名一：
typedef void (*pFunParam)();
//再替换左边的变量b，pFunx为别名二：
typedef void (*pFunx)(pFunParam);
//原声明的最简化版：
pFunx b[10];
```

这种用法是比较复杂的，出现的频率也不少，往往在看到这样的用法却不能理解，相信以上的解释能有所帮助。