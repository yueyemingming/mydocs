# 初始化列表

```cpp
#include <initializer_list>

// 1. class类---------------------------------
class A {
public:
    A(std::initializer_list<int> list) {}
};

void a_fun() {
    A a1({ 1, 2, 3, 4, 5 });
    A a2 = { 1, 2, 3, 4, 5 } ;
}


// 2. struct结构体-------------------------------
struct B {
    B(std::initializer_list<int> list) {}
};

void b_fun() {
    B b1({1,2});
    B b2 = {1, 2} ;
}


// 3. 函数参数---------------------------------
void foo(std::initializer_list<int> list) {}

void in_fun() {
    foo({1,2,3}) ;
}
```
