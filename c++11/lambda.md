# lambda表达式

lambda函数的语法定义：（采用了追踪返回类型的方式声明其返回值)

```cpp
[capture](parameters) mutable -> return-type
{
    statement;
}
```

- []，捕捉列表，捕捉上下文中的变量以供lambda函数使用
  - []    表示    空捕捉
  - [=]   表示值  传递方式捕捉所有父作用域的变量(包括this)
  - [&]   表示引用传递方式捕捉所有父作用域的变量(包括this)
  - [var] 表示值  传递方式捕捉变量var
  - [&var]表示引用传递方式捕捉变量var  
  - [this]表示值  传递方式捕捉当前this指针 (this。函数体内可以使用Lambda所在类中的成员变量。)

```cpp
class Test{
public:
    Test(int a, int b): m_a(a),m_b(b){}
    void f1(int t){
        auto f = [this] {return m_a + m_b;}; // 省略参数列表，返回值由编译器推断为int
        cout << f() + t <<endl;
    }
    int m_a;
    int m_b;
};

int main()
{
    // 最简lambda函数
    [] {};

    int a = 3;
    int b = 4;

    // [=]
    auto c = [=] {return a + b;};   // 省略参数列表，返回值由编译器推断为int
    cout << c() << endl;
    b++;                            // 值捕捉注意： 它只捕捉一次。如需关联变量，用引用
    cout << c() << endl;

    // [&]
    auto d = [&] {return a + b;};   // 省略参数列表，返回值由编译器推断为int
    cout << d() << endl;
    b++;                            // 值捕捉注意： 它只捕捉一次。如需关联变量，用引用
    cout << d() << endl;

    // [this]
    Test t(1,2);
    t.f1(9);

    return 0;
}
```
