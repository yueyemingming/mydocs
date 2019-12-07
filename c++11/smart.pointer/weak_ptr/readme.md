# weak_ptr

weak_ptr是为了配合shared_ptr而引入的一种智能指针，因为它不具有普通指针的行为，没有重载operator*和->,它的最大作用在于协助shared_ptr工作，像旁观者那样观测资源的使用情况。

weak_ptr可以从一个shared_ptr或者另一个weak_ptr对象构造，获得资源的观测权。但weak_ptr没有共享资源，它的构造不会引起指针引用计数的增加。

使用weak_ptr的成员函数use_count()可以观测资源的引用计数，另一个成员函数expired()的功能等价于use_count()==0,但更快，表示被观测的资源(也就是shared_ptr的管理的资源)已经不复存在。

weak_ptr可以使用一个非常重要的成员函数lock()从被观测的shared_ptr获得一个可用的shared_ptr对象，从而操作资源。但当expired()==true的时候，lock()函数将返回一个存储空指针的shared_ptr。

```c++
#include <iostream>
#include <memory>

int main() {
    std::shared_ptr<int> sh_ptr = std::make_shared<int>(10);
    std::cout << sh_ptr.use_count() << std::endl;

    std::weak_ptr<int> wp(sh_ptr);
    std::cout << wp.use_count() << std::endl;

    if(!wp.expired()){
        //get another shared_ptr
        std::shared_ptr<int> sh_ptr2 = wp.lock();
        *sh_ptr = 100;
        std::cout << wp.use_count() << std::endl;
    }
    //delete memory
}
```