# future

std::future可以从异步任务中获取结果，一般与std::async配合使用，std::async用于创建异步任务，实际上就是创建一个线程执行相应任务。

```c++
#include <future>
#include <iostream>
#include <stout/stringify.hpp>

bool is_prime(int x)
{
  for (int i=0; i<x; i++)
  {
    if (x % i == 0)
      return false;
  }
  return true;
}

int main()
{
  std::future<bool> fut = std::async(is_prime, 700020007);
  std::cout << "please wait";
  std::chrono::milliseconds span(100);
  while (fut.wait_for(span) != std::future_status::ready)
    std::cout << ".";
  std::cout << std::endl;

  bool ret = fut.get();
  std::cout << "final result: " << stringify(ret) << std::endl;
  return 0;
}
```

std::async会首先创建线程执行is_prime(700020007)， 任务创建之后，std::async立即返回一个std::future对象。

主线程既可使用std::future::get获取结果，如果调用过程中，任务尚未完成，则主线程阻塞至任务完成。

主线程也可使用std::future::wait_for等待结果返回。

wait_for可设置超时时间

- 如果在超时时间之内任务完成，则返回std::future_status::ready状态；
- 如果在超时时间之内任务尚未完成，则返回std::future_status::timeout状态。