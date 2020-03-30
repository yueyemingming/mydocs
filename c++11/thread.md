# thread

## 1.0 基础使用

```c++
#include <iostream>
#include <thread>
#include <mutex>

using namespace std ;

int g_i=0;
mutex g_i_mutex ;

void safe_increment()
{
    lock_guard<mutex> lock(g_i_mutex);
    ++g_i;

    cout << this_thread::get_id() << " : " << g_i << endl ;
}

int main()
{
    cout << __func__ << " : " << g_i << endl ;

    thread t1(safe_increment);
    thread t2(safe_increment);

    t1.join();
    t2.join();

    cout << __func__ << " : " << g_i << endl ;

    return 0 ;
}
```

```bash
g++ -std=c++11 thread.cpp -pthread
```

编译时必须带 **-pthread**


## 2.0 佩带参数及lambda

```c++
#include <iostream>
#include <map>
#include <string>
#include <thread>

using namespace std;

void global_fun() {
    cout << "global thread function ." << endl;
}

class Obj{
public :
    string m;
};

struct Test {
public:
    // global fun for thread
    void global_fun_test() {
        std::thread t(
                global_fun
        );
        t.detach();
    }


    // static fun in class for thread
    static void static_fun() {
        cout << "class static thread function ." << endl;
    }

    void class_static_fun_test() {
        std::thread t(static_fun);
        t.detach();
    }


    // static fun with args in class for thread
    static void static_fun_arg(int x) {
        cout << "class static thread with args(" << x << ") function ." << endl;
    }

    void class_static_fun_test_arg(int x) {
        std::thread t(static_fun_arg, x);
        t.detach();
    }


    // class fun, 非静态, 此处必须使用lamba表达式把this引进来
    void fun() {
        cout << "class thread function ." << endl;
    }

    void class_fun_test() {
        std::thread t(
                [&] {
                    fun();
                }
        );
        t.detach();
    }


    // class fun, 非静态, 此处必须使用lamba表达式把this引进来, 带参数
    void fun_arg(Obj& o) {
        cout << "class thread with arg(" << o.m << ") function ." << endl;
    }

    void class_fun_test_arg1(Obj& o) {
        std::thread t(
                [&] {
                    fun_arg(o);
                }
        );
        t.detach();
    }

    void class_fun_test_arg2(Obj& o) {
        std::string str ="bbbbbbbb";
        std::thread t(
                [&, str] {              //此处str值传递，而不是引用传递
                    fun_arg(o);
                    cout << str << endl ;
                }
        );
        t.detach();
    }

};

int main() {
    Test t;

//    t.global_fun_test();

//    t.class_static_fun_test();
//    t.class_static_fun_test_arg(100);

    t.class_fun_test();

    Obj o{"aaaaaaaa"};
    t.class_fun_test_arg1(o);
    t.class_fun_test_arg2(o);

    std::this_thread::sleep_for(std::chrono::seconds(2));

    return 0;
}
```