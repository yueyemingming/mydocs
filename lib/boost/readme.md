# boost

- [官方网址](https://dl.bintray.com/boostorg/release/) —— <https://dl.bintray.com/boostorg/release/>

## 手动编译安装boost

```bash
mkdir -p ~/software && cd ~/software
wget https://dl.bintray.com/boostorg/release/1.67.0/source/boost_1_67_0.tar.gz -O  boost_1_67_0.tar.gz
tar -zxvf boost_1_67_0.tar.gz && cd boost_1_67_0 && chmod +x bootstrap.sh
./bootstrap.sh --prefix=/usr
./b2 --buildtype=complete install
```

## 代码例子

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
