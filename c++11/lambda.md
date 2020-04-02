# lambda表达式

也叫做 **闭包类型**，即匿名函数。

## 1. lambda函数的语法定义：（采用了追踪返回类型的方式声明其返回值)

```cpp
[capture](parameters) mutable -> return-type
{
    statement;
}
```

## 2. []，捕捉列表，捕捉上下文中的变量以供lambda函数使用

| 符号   | 含义                                       |
| ------ | ------------------------------------------ |
| []     | 空捕捉                                     |
| [=]    | 值拷贝，捕捉所有父作用域的变量(包括this)   |
| [&]    | 引用拷贝，捕捉所有父作用域的变量(包括this) |
| [var]  | 值拷贝，捕捉变量var                        |
| [&var] | 引用拷贝，捕捉变量var                      |
| [this] | 值拷贝，捕捉当前this指针                   |

## 3. 例子

```c++
[] {};	// 最简lambda函数
```

```c++
int a = 3;
int b = 4;

// [=]
auto c = [=] {return a + b;};	// 省略参数列表，返回值由编译器推断为int
cout << c() << endl;			//7
b++;							// 值捕捉注意： 它只捕捉一次。如需关联变量，用引用
cout << c() << endl;			//7

// [&]
auto d = [&] {return a + b;};   // 省略参数列表，返回值由编译器推断为int
cout << d() << endl;			//8
b++;                            // 值捕捉注意： 它只捕捉一次。如需关联变量，用引用
cout << d() << endl;			//8

```

```cpp
class Foo{
public:
    Foo(int a, int b): m_a(a),m_b(b){}
    void f1(int t){
        auto f = [this] {return m_a + m_b;}; // 省略参数列表，返回值由编译器推断为int
        cout << f() + t <<endl;
    }
    int m_a;
    int m_b;
};

int main()
{
    // [this]
    Foo t(1,2);
    t.f1(9);

    return 0;
}
```
