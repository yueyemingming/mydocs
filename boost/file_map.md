# boost::interprocess 内存映射文件的用法

## 1. 闲聊

今天项目中要读写一个将近40M的文件，发现使用fstream读文件时，预先分配一个40M的缓冲区buffer时，随机性的会申请内存失败，因此查了下相关资料，发现使用内存映射文件可以解决此类读写大文件的问题。

内存文件映射其实是内存中分配了一块区域映射到文件所在的物理磁盘上，内存和物理磁盘的数据交换是以页大小处理的，因此在操作过程中，不会将整个文件缓存到内存上，因此使用的内存特别的小，但对于程序员来说，映射的指针就如操作一块普通的内存一样方便，所有的页交换都由操作系统来处理了。

## 2. 代码测试

boost::interprocess 提供的文件映射类特别的简单，主要是 `file_mapping` 和 `mapped_region`，具体的构造参数说明，可以去官网查看，这里只是提供一个例子。

```c++
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
using namespace boost::interprocess;

void main()
{
	file_mapping m_file("test.txt", read_write);
	mapped_region region(m_file, read_write);

	void * addr       = region.get_address();
	std::size_t size  = region.get_size();
}
```

- file_mapping类

  - 第一个参数是文件名，该文件必须存在，不然会抛出“找不到系统文件”的异常，它是不会帮你自动生成文件的。

  - 第二个参数是打开模式，目前只有read_only和read_write两种模式，一个是只读，一个是读写都可以。

- mapped_region类
  - 第一个参数是file_mapping的对象。
  - 第二个参数是打开模式，最好file_mapping的打开模式一致。

- get_address()就是取得文件的首地址了。

- get_size()是取得文件的长度。

## 3. 代码测试

程序里为了保证文件操作的原子性，因此需要加锁，而boost也提供了这种文件锁的机制file_lock，其实它就是拿一个文件来当互斥量，那我们的代码就变成了这样。

```c++
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/sharable_lock.hpp>
using namespace boost::interprocess;
void main()
{
	file_lock f_lock("test.txt");
	sharable_lock<file_lock> sh_lock(f_lock);
	file_mapping m_file("test.txt", read_write);
	mapped_region region(m_file, read_write);

	void * addr       = region.get_address();
	std::size_t size  = region.get_size();
}
```

- file_lock需要一个文件名的参数，如果该文件不存在也会抛异常，映射文件和锁文件可以是同一个。

- sharable_lock是共享锁，也可以使用独占锁scoped_lock，共享锁和独占锁的区别这里不细讲，可以问度娘。