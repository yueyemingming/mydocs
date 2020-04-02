# define和typedef的区别

## 1. #define是预处理指令
在编译预处理时进行简单的替换，不作正确性检查，不关含义是否正确照样带入，只有在编译已被展开的源程序时才会发现可能的错误并报错。例如：

```c
#define PI 3.1415926
```

程序中的：area=PI*r*r 会替换为3.1415926*r*r

如果你把#define语句中的数字9 写成字母g 预处理也照样带入。

## 2. typedef是在编译时处理的。

它在自己的作用域内给一个已经存在的类型一个别名，但是You cannot use the typedef specifier inside a function definition。

## 3. typedef int * int_ptr; 和 #define int_ptr int * 

作用都是用int_ptr代表 int * ,但是二者不同，正如前面所说 ，#define在预处理时进行简单的替换，而typedef不是简单替换 ，而是采用如同定义变量的方法那样来声明一种类型。也就是说;

```c
//refer to (xzgyb(老达摩))
#define int_ptr int *
int_ptr a, b; //相当于int * a, b; 只是简单的宏替换
 
typedef int* int_ptr;
int_ptr a, b; //a, b 都为指向int的指针,typedef为int* 引入了一个新的助记符
```

这也说明了为什么下面观点成立

```c
//QunKangLi(维护成本与程序员的创造力的平方成正比)
typedef int * pint ;
#define PINT int *
```

那么：

```c
const pint p ;//p不可更改，但p指向的内容可更改
const PINT p ;//p可更改，但是p指向的内容不可更改。
```

pint是一种指针类型 const pint p 就是把指针给锁住了 p不可更改

而const PINT p 是const int * p 锁的是指针p所指的对象。

## 4. 还应经注意到#define 不是语句 不要在行末加分号，否则 会连分号一块置换。


## 5. 补充

```c
#elif  
```

等同于

```c
#else
#if 
```