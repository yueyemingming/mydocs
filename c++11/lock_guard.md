# lock_guard

当程序中有共享数据时，你肯定不想让程序其陷入条件竞争，或是出现不变量被破坏的情况，此时可使用std::mutex互斥量来解决数据共享的问题。

C++中通过实例化std::mutex创建互斥量实例，通过成员函数lock()对互斥量上锁，unlock()进行解锁。不过，实践中不推荐直接去调用成员函数lock()，调用lock()就意味着，必须在每个函数出口都要去调用unlock()，也包括异常的情况。

C++标准库为互斥量提供了一个[**RAII**](https://blog.csdn.net/datase/article/details/80075143)语法的模板类std::lock_guard，在构造时就能提供已锁的互斥量，并在析构的时候进行解锁，从而保证了一个已锁互斥量能被正确解锁。

在std::lock_guard对象构造时，传入的mutex对象(即它所管理的mutex对象)会被当前线程锁住。在lock_guard对象被析构时，它所管理的mutex对象会自动解锁，不需要程序员手动调用lock和unlock对mutex进行上锁和解锁操作。

```c++
#include <list>
#include <mutex>
#include <algorithm>
#include <iostream>

std::list<int> some_list;
std::mutex some_mutex;

void add_to_list(int new_value)
{
    std::lock_guard<std::mutex> mutx(some_mutex);
    some_list.push_back(new_value);
}

bool list_contains(int value_to_find)
{
    std::lock_guard<std::mutex> mutx(some_mutex);
    return std::find(some_list.begin(), some_list.end(), value_to_find) != some_list.end();
}


int main()
{
    add_to_list(33);
    std::cout << "contains(1)=" << list_contains(1) << ",contains(33)=" << list_contains(33) << std::endl;
    return 0；
}
```

std::lock_guard在构造时只被锁定一次，并且在销毁时解锁。

与std::lock_guard功能相同的有std::unique_lock，但是std::unique_lock也可以提供自动加锁、解锁功能，比std::lock_guard更加灵活。std::unique_lock需要付出更多的时间、性能成本。