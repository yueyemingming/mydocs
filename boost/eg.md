# 代码例子

```cpp
#include <boost/thread.hpp>
#include <iostream>

void wait(int seconds)
{
  boost::this_thread::sleep(boost::posix_time::seconds(seconds));
}

void thread()
{
  for (int i = 0; i < 5; ++i)
  {
    wait(1);
    std::cout << i << std::endl;
  }
}

int main()
{
  boost::thread t(thread);
  t.join();
}
```

```bash
g++ main.cpp -std=c++11 -I /usr/local/boost/include/ -L /usr/local/boost/lib -lboost_thread -lboost_system -pthread
```
