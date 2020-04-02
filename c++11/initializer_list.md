# 初始化列表

[TOC]

## 1. 统一初始化

- 统一初始化，即用 **{}** 来替代 **()** 来进行初始化。形式有如下两种:

```c++
Type obj {old_obj};
Type obj = {old_obj};
```

- 并且不受拷贝构造函数的影响，即拷贝构造函数是私有的，同样能够正确初始化。

```c++
//基础类型
int a{3};		int a = {3};
//对象
Foo o {123};	Foo o = {123};
//指针
int* a = new int{123};
//堆上动态分配数组
int* a = new int[3] {1, 2, 3};
//函数参数和返回值
struct Foo { int a;	double d; };
Foo func() { return {3, 5.5}; }
//stl对象
set<int> ss			 = {1,2,3};
vector<int> sv		 = {1,2,3,4,5};
map<int, string> mis = {{1,"a"}, {2,"b"}, {3,"c"}};
```

## 2. 非聚合类型不可使用统一初始化

**聚合类型** 是指完全由普通类型组合成的结构体或类或其数组等。内部无构造函数、无析构函数、基类、无虚函数、无私有或受保护的非静态成员、无{}或=的直接初始化的非静态成员等。就相当于c语言中可以加入普通函数的的struct。

> c++11允许在类声明时使用 {}或= 来初始化非静态数据成员。

```c++
struct Foo {	int a = 3 ;	}
```

- 非聚合类型调用统一初始化是非法的。

```c++
//函数参数和返回值
struct Foo {	int a=0;	double d=0.0;	};
Foo	f{3, 5.5};				//错误，因为类型中成员变量进行了初始化
```

## 3. 初始化列表实现

初始化列表的是通过 initializer_list 来实现的。

```c++
#include <initializer_list>

std::initializer_list<int> list = {1,2,3};

cout << list.size() << endl ;		//3
cout << *list.begin() << endl ;		//1
cout << *(list.begin()+1) << endl ;	//2
cout << *(list.begin()+2) << endl ;	//3
for(auto it = list.begin(); it != list.end() ; it++ )
cout << *it << endl ;				//1,2,3
```

### 3.1 类

```cpp
struct Foo {
    Foo(std::initializer_list<int> list) {}
};

Foo f({ 1, 2, 3, 4, 5 });
Foo f = { 1, 2, 3, 4, 5 };
```

### 3.2 函数参数

```c++
void foo(std::initializer_list<int> list) {}
foo({1,2,3}) ;
```

### 3.3 stl

stl容器中包含 size(), begin(), end()就可以通过 initializer_list来实现初始化列表功能了。

```c++
//stl对象
set<int> ss			 = {1,2,3};
vector<int> sv		 = {1,2,3,4,5};
map<int, string> mis = {{1,"a"}, {2,"b"}, {3,"c"}};
```

vector初始化列表的基本实现思路。

```c++
class FooVector {
    std::vector<int> content_;
public:
    FooVector(std::initializer_list<int> list) {}
};

FooVector f = {1,2,3,4,5};
```

## 4. 效率

initializer_list中存储的是引用，它不存储对象本身。因此它的效率比较高。对于stl的对象，它会把引用指向的真实对象，完全赋值一遍。

```c++
initializer_list<int> func() {
    int a = 1;
    int b = 3;
    return {a,b};					//此处只是返回a,b的引用，放到另外一个初始化列表中，并未进行a,b的值拷贝
}
initializer_list<int> l = func();	//编译能通过，但无法得到正确结果，因为func调用结束后，a,b空间都已经释放
```

解决办法：进行值拷贝。

```c++
vector<int> func() {
    int a = 1;
    int b = 3;
    return {a,b};					//对vector会进行a,b的值拷贝
}
vector<int> v = func();
```

## 5. 防止类型收窄

类型收窄，一个宽精度数值赋值给一个低精度变量时，引起了数据丢失，即类型收窄。

c99、c++98/03、c++11都可以编译通过，不会进行判断。但c++11可以通过初始化列表来防止此类事件发生。

```c++
int a = 1.1 ;	//ok, 无告警
int a = {1.1};	//error

char c = 1024;		//ok, warning
char c = {1024};	//error

char c = {100};					//ok, const常量
int i=100;	char c = {i};		//ok, warning
int i=1000;	char c = {i};		//不同的编译器，可能warning,可以error。
```

