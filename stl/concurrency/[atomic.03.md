# std::atomic (续)

[C++11 并发指南六(  类型详解二 std::atomic )](atomic.02.md) 介绍了基本的原子类型 std::atomic 的用法，本节我会给大家介绍C++11 标准库中的 std::atomic 针对整形(integral)和指针类型的特化版本做了哪些改进。

总地来说，C++11 标准库中的 std::atomic 针对整形(integral)和指针类型的特化版本新增了一些算术运算和逻辑运算操作。具体如下：

```c++
integral fetch_add(integral, memory_order = memory_order_seq_cst) volatile;
integral fetch_add(integral, memory_order = memory_order_seq_cst);
integral fetch_sub(integral, memory_order = memory_order_seq_cst) volatile;
integral fetch_sub(integral, memory_order = memory_order_seq_cst);
integral fetch_and(integral, memory_order = memory_order_seq_cst) volatile;
integral fetch_and(integral, memory_order = memory_order_seq_cst);
integral fetch_or(integral, memory_order = memory_order_seq_cst) volatile;
integral fetch_or(integral, memory_order = memory_order_seq_cst);
integral fetch_xor(integral, memory_order = memory_order_seq_cst) volatile;
integral fetch_xor(integral, memory_order = memory_order_seq_cst);
 
integral operator++(int) volatile;
integral operator++(int);
integral operator--(int) volatile;
integral operator--(int);
integral operator++() volatile;
integral operator++();
integral operator--() volatile;
integral operator--();
integral operator+=(integral) volatile;
integral operator+=(integral);
integral operator-=(integral) volatile;
integral operator-=(integral);
integral operator&=(integral) volatile;
integral operator&=(integral);
integral operator|=(integral) volatile;
integral operator|=(integral);
integral operator^=(integral) volatile;
integral operator^=(integral);
```

 下面我们来简单介绍以上的 std::atomic 特化版本的成员函数。

## fetch_add

| if T is integral (1) | T fetch_add (T val, memory_order sync = memory_order_seq_cst) volatile noexcept; <br />T fetch_add (T val, memory_order sync = memory_order_seq_cst) noexcept; |
| -------------------- | ------------------------------------------------------------ |
| if T is pointer (2)  | T fetch_add (ptrdiff_t val, memory_order sync = memory_order_seq_cst) volatile noexcept; <br />T fetch_add (ptrdiff_t val, memory_order sync = memory_order_seq_cst) noexcept; |

将原子对象的封装值加 val，并返回原子对象的旧值（适用于整形和指针类型的 std::atomic 特化版本），整个过程是原子的。sync 参数指定内存序：

|   Memory Order 值    |    Memory Order 类型    |
| :------------------: | :---------------------: |
| memory_order_relaxed |         Relaxed         |
| memory_order_consume |         Consume         |
| memory_order_acquire |         Acquire         |
| memory_order_release |         Release         |
| memory_order_acq_rel |     Acquire/Release     |
| memory_order_seq_cst | Sequentially consistent |

另外，如果第二个参数不指定（取默认参数 memory_order_seq_cst），则 fetch_add 相当与 std::atomic::operator+=。

## fetch_sub

| if T is integral (1) | T fetch_sub (T val, memory_order sync = memory_order_seq_cst) volatile noexcept; <br />T fetch_sub (T val, memory_order sync = memory_order_seq_cst) noexcept; |
| -------------------- | ------------------------------------------------------------ |
| if T is pointer (2)  | T fetch_sub (ptrdiff_t val, memory_order sync = memory_order_seq_cst) volatile noexcept; <br />T fetch_sub (ptrdiff_t val, memory_order sync = memory_order_seq_cst) noexcept; |

将原子对象的封装值减 val，并返回原子对象的旧值（适用于整形和指针类型的 std::atomic 特化版本），整个过程是原子的。sync 参数指定内存序：

|   Memory Order 值    |    Memory Order 类型    |
| :------------------: | :---------------------: |
| memory_order_relaxed |         Relaxed         |
| memory_order_consume |         Consume         |
| memory_order_acquire |         Acquire         |
| memory_order_release |         Release         |
| memory_order_acq_rel |     Acquire/Release     |
| memory_order_seq_cst | Sequentially consistent |

另外，如果第二个参数不指定（取默认参数 memory_order_seq_cst），则 fetch_sub 相当与 std::atomic::operator-=。

## fetch_and

```c++
T fetch_and (T val, memory_order sync = memory_order_seq_cst) volatile noexcept;
T fetch_and (T val, memory_order sync = memory_order_seq_cst) noexcept;
```

 将原子对象的封装值按位与 val，并返回原子对象的旧值（只适用于整型的 std::atomic 特化版本），整个过程是原子的。sync 参数指定内存序：

|   Memory Order 值    |    Memory Order 类型    |
| :------------------: | :---------------------: |
| memory_order_relaxed |         Relaxed         |
| memory_order_consume |         Consume         |
| memory_order_acquire |         Acquire         |
| memory_order_release |         Release         |
| memory_order_acq_rel |     Acquire/Release     |
| memory_order_seq_cst | Sequentially consistent |

另外，如果第二个参数不指定（取默认参数 memory_order_seq_cst），则 fetch_add 相当与 std::atomic::operator&=。

## fetch_or

```c++
T fetch_or (T val, memory_order sync = memory_order_seq_cst) volatile noexcept;
T fetch_or (T val, memory_order sync = memory_order_seq_cst) noexcept;
```

 将原子对象的封装值按位或 val，并返回原子对象的旧值（只适用于整型的 std::atomic 特化版本），整个过程是原子的。sync 参数指定内存序：

|   Memory Order 值    |    Memory Order 类型    |
| :------------------: | :---------------------: |
| memory_order_relaxed |         Relaxed         |
| memory_order_consume |         Consume         |
| memory_order_acquire |         Acquire         |
| memory_order_release |         Release         |
| memory_order_acq_rel |     Acquire/Release     |
| memory_order_seq_cst | Sequentially consistent |

另外，如果第二个参数不指定（取默认参数 memory_order_seq_cst），则 fetch_or 相当与 std::atomic::operator|=。

## fetch_xor

```c++
T fetch_xor (T val, memory_order sync = memory_order_seq_cst) volatile noexcept;
T fetch_xor (T val, memory_order sync = memory_order_seq_cst) noexcept;
```

将原子对象的封装值按位异或 val，并返回原子对象的旧值（只适用于整型的 std::atomic 特化版本），整个过程是原子的。sync 参数指定内存序：

|   Memory Order 值    |    Memory Order 类型    |
| :------------------: | :---------------------: |
| memory_order_relaxed |         Relaxed         |
| memory_order_consume |         Consume         |
| memory_order_acquire |         Acquire         |
| memory_order_release |         Release         |
| memory_order_acq_rel |     Acquire/Release     |
| memory_order_seq_cst | Sequentially consistent |

另外，如果第二个参数不指定（取默认参数 memory_order_seq_cst），则 fetch_xor 相当与 std::atomic::operator^=。

## operator++

| pre-increment (1)  | T operator++() volatile noexcept; <br />T operator++() noexcept; |
| ------------------ | ------------------------------------------------------------ |
| post-increment (2) | T operator++ (int) volatile noexcept; <br />T operator++ (int) noexcept; |

自增运算符重载， 第一种形式 (1) 返回自增后的值（即前缀++），第二种形式(2) 返回自增前的值（即后缀++），适用于整形和指针类型的 std::atomic 特化版本。

## operator--

自减运算符重载， 第一种形式 (1) 返回自减后的值（即前缀--），第二种形式(2) 返回自减前的值（即后缀--），适用于整形和指针类型的 std::atomic 特化版本。

## atomic::operator (comp. assign.)

复合赋值运算符重载，主要包含以下形式： 

| if T is integral (1) | T operator+= (T val) volatile noexcept; T operator+= (T val) noexcept;<br />T operator-= (T val) volatile noexcept; T operator-= (T val) noexcept; <br />T operator&= (T val) volatile noexcept; T operator&= (T val) noexcept; <br />T operator\|= (T val) volatile noexcept; T operator\|= (T val) noexcept;<br />T operator^= (T val) volatile noexcept; T operator^= (T val) noexcept; |
| -------------------- | ------------------------------------------------------------ |
| if T is pointer (2)  | T operator+= (ptrdiff_t val) volatile noexcept; <br />T operator+= (ptrdiff_t val) noexcept;<br />T operator-= (ptrdiff_t val) volatile noexcept;<br />T operator-= (ptrdiff_t val) noexcept; |

以上各个 operator 都会有对应的 **fetch_\*** 操作，详细见下表：

|  操作符  |      成员函数       |     支持类型      |          |          |      |
| :------: | :-----------------: | :---------------: | :------: | :------: | ---- |
| 复合赋值 |       等价于        |       整型        | 指针类型 | 其他类型 |      |
|   `+`    | atomic::operator+=  | atomic::fetch_add |    是    |    是    | 否   |
|   `-`    | atomic::operator-=  | atomic::fetch_sub |    是    |    是    | 否   |
|   `&`    | atomic::operator&=  | atomic::fetch_and |    是    |    否    | 否   |
|   `|`    | atomic::operator\|= | atomic::fetch_or  |    是    |    否    | 否   |
|   `^`    | atomic::operator^=  | atomic::fetch_xor |    是    |    否    | 否   |

好了，本节先介绍这里，下一节我会介绍 C++11 中 C 风格的原子操作 API。