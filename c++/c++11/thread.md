# thread

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
