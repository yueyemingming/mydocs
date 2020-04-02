# 函数对象

## 1. 函数对象种类

### 1.1 函数指针

```c++
void func() {}

void (*p)(void) = &func;	//加不加&都行
p();						//p() == func()
```

### 1.2 函数对象

```cpp
struct Foo {
    void operator()(){}
};

Foo f;
f();
```

### 1.3 可被转换为函数指针的的类对象

```c++
void func() {}

struct Foo {
	using p = void(*)(void);
    operator f(void) { return func; }
};

Foo f;
f();		//f() == func()
```

## 2. std::function

```c++
#include <functional>
```

### 2.1 绑定普通函数

```c++
void func() {}
function<void()> p = func;
p();
```

### 2.2 绑定类静态成员函数

```c++
class Foo {
public:
    static void func() {}
};

function<void()> p = Foo::func;
p();
```

### 2.3 绑定函数对象

```c++
class Foo {
public :
    void operator ()() {}
};

Foo foo
function<void()> p = foo;
p();
```

  ## 3. std::function的主要作用，回调

```c++
void output(int x) { cout << x << endl; }

//void print(int x, function<void(int)> o) {
void print(int x, const function<void(int)> &o) {	//这两种方式都可以，但const&的方式效率更高，避免函数拷贝
    cout << "msg -> ";
    o(x);
}

int main() {
    int a[] = {1, 2, 3, 4, 5};

    for (auto i : a)
        print(i, output);
}

//-------------
msg -> 1
msg -> 2
msg -> 3
msg -> 4
msg -> 5
```

