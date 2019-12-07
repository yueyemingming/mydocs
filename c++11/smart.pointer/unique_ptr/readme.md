# unique_ptr

unique_ptr“唯一”拥有其所指对象，同一时刻只能有一个unique_ptr指向给定对象（通过禁止拷贝语义、只有移动语义来实现）。
相比与原始指针unique_ptr用于其RAII的特性，使得在出现异常的情况下，动态资源能得到释放。

unique_ptr指针本身的生命周期：从unique_ptr指针创建时开始，直到离开作用域。离开作用域时，若其指向对象，则将其所指对象销毁(默认使用delete操作符，用户可指定其他操作)。

在智能指针生命周期内，可以改变智能指针所指对象，如创建智能指针时通过构造函数指定、通过reset方法重新指定、通过release方法释放所有权、通过移动语义转移所有权。

```c++
#include <iostream>
#include <memory>

int main() {
    std::unique_ptr<int> uptr(new int(10));   //绑定动态对象
    //std::unique_ptr<int> uptr2 = uptr;      //不能赋值
    //std::unique_ptr<int> uptr2(uptr);       //不能拷贝
    std::unique_ptr<int> uptr2 = std::move(uptr);    //转换所有权
    uptr2.release();                          //释放所有权
    //超过uptr的作用域，内存释放

    return 0 ;
}
```

```cpp
//基本使用
void test1() {
    std::unique_ptr<int> uptr(new int(10));
    //std::unique_ptr<int> uptr2 = uptr ;
    //std::unique_ptr<int> uptr2(uptr);
    std::unique_ptr<int> uptr2 = std::move(uptr);
    uptr2.release();
}


//自动析构，再也不用delete了
class Test {
public:
    Test() { std::cout << "Test()" << std::endl ; }
    ~Test() { std::cout << "~Test()" << std::endl ; }
};

void test2() {
    std::unique_ptr<Test> pTest(new Test) ;
}


int main() {
    test1();
    test2();

    return 0;
}
```

```cpp
//测试在map, queue, vector等中，自动释放智能指针
class Test
{
public:
    Test() {
        cout << "Test()" << endl ;
    }
    ~Test() {
        cout << "~Test()" << endl ;
    }
};

int main()
{
    map<int, shared_ptr<Test>> m;
    std::shared_ptr<Test> pTest(new Test) ;
    m[0] = pTest ;

    queue<std::unique_ptr<Test>> q;
    q.push(std::unique_ptr<Test>(new Test));

    return 0;
}
```
