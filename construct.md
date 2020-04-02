# 拷贝构造函数和赋值构造函数

```
class Foo {
public:
    Foo() {}
    Foo(int) {}         //构造函数
    Foo& operator =(const Foo&) {};
private:
    Foo(const Foo&) {}  //拷贝构造函数
};

int main( ){
    Foo a1(123);
    //Foo a2=123;       //对象在初始化时，调用的时拷贝构造函数，而不是赋值构造函数 
    Foo a3; a3 = 123;   //赋值构造函数
```

