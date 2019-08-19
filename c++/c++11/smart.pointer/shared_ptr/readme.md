# shared_ptr

shared_ptr使用引用计数，每一个shared_ptr的拷贝都指向相同的内存。每使用他一次，内部的引用计数加1，每析构一次，内部的引用计数减1，减为0时，自动删除所指向的堆内存。shared_ptr内部的引用计数是线程安全的，但是对象的读取需要加锁。

* 初始化。智能指针是个模板类，可以指定类型，传入指针通过构造函数初始化。也可以使用make_shared函数初始化。不能将指针直接赋值给一个智能指针，一个是类，一个是指针。例如`std::shared_ptr<int> p4 = new int(1);`的写法是错误的
* 拷贝和赋值。拷贝使得对象的引用计数增加1，赋值使得原对象引用计数减1，当计数为0时，自动释放内存。后来指向的对象引用计数加1，指向后来的对象。
* get函数获取原始指针
* 注意不要用一个原始指针初始化多个shared_ptr，否则会造成二次释放同一内存
* 注意避免循环引用，shared_ptr的一个最大的陷阱是循环引用，循环，循环引用会导致堆内存无法正确释放，导致内存泄漏。循环引用在weak_ptr中介绍。

---

* 简短小例子

```c++
#include <iostream>
#include <memory>

int main() {
    int a = 10;
    std::shared_ptr<int> ptra = std::make_shared<int>(a);
    std::shared_ptr<int> ptra2(ptra);            //拷贝
    std::cout << ptra.use_count() << std::endl;  //数量变为2

    int b = 20;
    int *pb = &a;
    //std::shared_ptr<int> ptrb = pb;            //直接赋值，错误
    std::shared_ptr<int> ptrb = std::make_shared<int>(b);
    ptra2 = ptrb;             //赋值
    pb = ptrb.get();          //获取原始指针

    std::cout << ptra.use_count() << std::endl;
    std::cout << ptrb.use_count() << std::endl;
    return 0;
}
```

* 语法比较全的小例子

```c++
#include <iostream>
#include <memory>
using namespace std ;


// 创建
void test1(){
    int a=10;
    std::shared_ptr<int> p = std::make_shared<int>(a);
    cout << __FUNCTION__ << ":" << *p << endl ;
}


// 赋值
void test2() {
    std::shared_ptr<int> p(new int(10));
    std::shared_ptr<int> p1 = p;
    int* a = p.get();                   //得到原始指针
    cout << __FUNCTION__ << ":" << "p1=" << *p1 << "; p=" << *p << "; a=" << *a << endl ;
    cout << p.use_count() << endl ;
}


// 重置
void test3() {
    cout << __FUNCTION__ << ":" << endl;
    shared_ptr<int> ptr;
    ptr.reset(new int(1));
    // ptr = new int(1) ; // error
}


// 设置销毁函数
void mydelete_int_sptr(int *p) {
    cout << __FUNCTION__ << ":" ;
    cout << *p << " " << endl ;
    delete p;
}

void test4(){
    std::shared_ptr<int> p1(new int, mydelete_int_sptr) ;
}


//lamba销毁函数
void test5(){
    cout << __FUNCTION__ << ":" ;
    std::shared_ptr<int> p3(new int(100), [](int* p){
        cout << __FUNCTION__ << ":" ;
        cout << *p << endl ;
        delete p ;
    });
}


// 指向数组的共享指针，及lambada销毁函数
void test6() {
    cout << __FUNCTION__ << ":" ;
    std::shared_ptr<int> p4(new int[10]{1,2,3,4,5}, [](int* p) {
        cout << __FUNCTION__ << ":" ;
        for( int i = 0 ; i < 10 ; i++ )
            cout << p[i] << " " ;
        delete[] p;
        cout << endl ;
    });
}


// 指向数组的共享指针，及默认的销毁函数
void test7() {
    cout << __FUNCTION__ << ":" << endl;
    std::shared_ptr<int> p(new int[10]{6,7,8,9,0}, std::default_delete<int[]>() );

    for(int i = 0 ; i < 10 ; i++)
        cout << p.get()[i] << endl ;
}


//智能指针数组及默认销毁函数
template<typename T>
shared_ptr<T> make_shared_array(size_t size) {
    return shared_ptr<T>(new T[size]{1,2,3}, std::default_delete<int[]>());
}

void test8() {
    cout << __FUNCTION__ << ":" ;
    std::shared_ptr<int> p = make_shared_array<int>(10);
    for(int i = 0 ; i < 10 ; i++)
        cout << p.get()[i] << endl ;

}


int main() {
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
    test8();

    return 0;
}
```