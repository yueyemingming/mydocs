# decltype

[TOC]

## 1. auto和decltype区别

auto通过值计算来推导类型；而decltype通过类型计算来确定类型，因此它不需要赋初值。

## 2. decltype不是关键字，而是函数调用。

```c++
decltype(expression)	//注意，这里面可以是表达式
```

```c++
decltype(x+y);
decltype(fun());   
```

```cpp
int x;
decltype(x) y;		// y->int, decltype(x)->int
decltype(x+y) z;	// z->int, decltype(x+y)->int, decltype支持表达式计算推导类型
const decltype(z)* p=&z;	// p->const int*, decltype(z)->int

const int& i=x;
decltype(i) j;		// j->const int&, decltype(i)->const int&, const保留
```

## 3. 替代const_iterartor

const_iterator，诸多容器都提供了const_iterator迭代器，但每种容器在实现时都声明const_iterator迭代器实现一遍，并不是一个很好的做法，因此此处使用了 decltype。

```c++
template <class ContainerT>
class Foo<const ContainerT>
{
    ContainerT::const_iterator it_;
public:
	void func(const ContainterT& container){
        it_ = container.begin();
    }
    
    //...
};
```

```c++

template <class ContainerT>
class Foo<const ContainerT>
{
    decltype(ContainerT().begin()) it_;		//改用decltype代替
public:
	void func(const ContainterT& container){
        it_ = container.begin();
    }
    
    //...
};
```

## 4. 某些标准库类型的定义

```c++
typedef decltype(nullptr)	nullptr_t;
typedef decltype(sizeof(0))	size_t;
```

## 5. auto和decltype推导函数返回值，后置推导

```c++
template<typename T, typename U>
auto add(T t, U t) -> decltype(t+u) {	// 放在函数名后面，成为后置，没办法去掉auto放前面，因为如果放前面，此时 t和u两个对象还未创建。
    return t+u;
}
```

之所以不使用前置推导，具体原因见《深入应用c++》p15。