# Boost线程池

### Boost线程池

[Boost 线程池](https://www.boost.org/doc/libs/1_69_0/doc/html/boost_asio/reference/thread_pool.html#boost_asio.reference.thread_pool.protected_member_functions)位于组件 asio 中，是一种固定大小的线程池。

```c
class thread_pool :
  public execution_context
```

*Types*

| Name                                                         | Description                                         |
| ------------------------------------------------------------ | --------------------------------------------------- |
| [executor_type](https://www.boost.org/doc/libs/1_69_0/doc/html/boost_asio/reference/thread_pool__executor_type.html) | Executor used to submit functions to a thread pool. |
| [fork_event](https://www.boost.org/doc/libs/1_69_0/doc/html/boost_asio/reference/thread_pool/fork_event.html) | Fork-related event notifications.                   |

*Member Functions*

| Name                                                         | Description                                                  |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [get_executor](https://www.boost.org/doc/libs/1_69_0/doc/html/boost_asio/reference/thread_pool/get_executor.html) | Obtains the executor associated with the pool.               |
| [join](https://www.boost.org/doc/libs/1_69_0/doc/html/boost_asio/reference/thread_pool/join.html) | Joins the threads.                                           |
| [notify_fork](https://www.boost.org/doc/libs/1_69_0/doc/html/boost_asio/reference/thread_pool/notify_fork.html) | Notify the execution_context of a fork-related event.        |
| [stop](https://www.boost.org/doc/libs/1_69_0/doc/html/boost_asio/reference/thread_pool/stop.html) | Stops the threads.                                           |
| [thread_pool()](https://www.boost.org/doc/libs/1_69_0/doc/html/boost_asio/reference/thread_pool/thread_pool.html) | Constructs a pool with an automatically determined number of threads. |
| [thread_pool(std::size_t num_threads)](https://www.boost.org/doc/libs/1_69_0/doc/html/boost_asio/reference/thread_pool/thread_pool.html) | Constructs a pool with a specified number of threads.        |
| [~thread_pool](https://www.boost.org/doc/libs/1_69_0/doc/html/boost_asio/reference/thread_pool/_thread_pool.html) | Destructor.                                                  |

*Protected Member Functions*

| Name                                                         | Description                             |
| ------------------------------------------------------------ | --------------------------------------- |
| [destroy](https://www.boost.org/doc/libs/1_69_0/doc/html/boost_asio/reference/thread_pool/destroy.html) | Destroys all services in the context.   |
| [shutdown](https://www.boost.org/doc/libs/1_69_0/doc/html/boost_asio/reference/thread_pool/shutdown.html) | Shuts down all services in the context. |

*Friends*

| Name                                                         | Description                                                  |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [add_service](https://www.boost.org/doc/libs/1_69_0/doc/html/boost_asio/reference/thread_pool/add_service.html) | (Deprecated: Use make_service().) Add a service object to the execution_context. |
| [has_service](https://www.boost.org/doc/libs/1_69_0/doc/html/boost_asio/reference/thread_pool/has_service.html) | Determine if an execution_context contains a specified service type. |
| [make_service](https://www.boost.org/doc/libs/1_69_0/doc/html/boost_asio/reference/thread_pool/make_service.html) | Creates a service object and adds it to the execution_context. |
| [use_service](https://www.boost.org/doc/libs/1_69_0/doc/html/boost_asio/reference/thread_pool/use_service.html) | Obtain the service object corresponding to the given type.   |

To submit functions to the thread_pool, use the [dispatch](https://www.boost.org/doc/libs/1_69_0/doc/html/boost_asio/reference/dispatch.html) , [post](https://www.boost.org/doc/libs/1_69_0/doc/html/boost_asio/reference/post.html) or [defer](https://www.boost.org/doc/libs/1_69_0/doc/html/boost_asio/reference/defer.html) free functions.

```cpp
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

/* 互斥锁 */
std::mutex mutex_iostream;

void my_task( void )
{
    std::lock_guard<std::mutex> lg(mutex_iostream);
    std::cout.flush();
    std::cout << "This is my task." << std::endl;
    std::cout.flush();
}

int main( int argc, const char **argv )
{
    /* 定义一个4线程的线程池 */
    boost::asio::thread_pool tp( 4 );

    /* 将函数投放到线程池 */
    for( int i=0; i<5; ++i ) {
        boost::asio::post( tp, my_task );
    }

    /* 将语句块投放到线程池 */
    for( int i=0; i<5; ++i ) {
        boost::asio::post(
            tp,
            [](){
                std::lock_guard<std::mutex> lg( mutex_iostream );
                std::cout.flush();
                std::cout << "This is lambda task." << std::endl;
                std::cout.flush();
            });
    }

    /* 退出所有线程 */
    tp.join();

    system("PAUSE");

    return 0;
}
```

