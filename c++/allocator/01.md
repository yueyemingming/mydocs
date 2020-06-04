# C++ allocator类



使用allocator类可以将内存的分配和对象构造分离，allocator是一个模板，为了定义一个allocator对象，我们必须指明这个allocator可以分配的对象类型。

construct成员函数接受一个指针和零个或多个额外参数，额外参数用来初始化构造的对象。

```c++
int main()
{
    allocator<string> strAlloc;
    //分配两个未初始化的string
    auto strs = strAlloc.allocate(2);
    //对两个string执行构造函数
    strAlloc.construct(&strs[0], "hello");
    strAlloc.construct(&strs[1], "world!");
    for (int i = 0; i < 2; i++) {
        cout << strs[i] << endl;
    }
    //对两个string执行析构函数
    for (int i = 0; i < 2; i++) {
        strAlloc.destroy(&strs[i]);
    }
    //释放内存
    strAlloc.deallocate(strs, 2);
    system("pause");
}
```

标准库还为allocator类定义了拷贝和填充未初始化内存的函数。

```c++
int main()
{
    allocator<string> strAlloc;
    //分配两个未初始化的string
    auto strs = strAlloc.allocate(2);

    //从迭代器指出的输入范围中拷贝元素到未初始化的内存中
    vector<string> strs2 = { "hello","world" };
    uninitialized_copy(strs2.begin(), strs2.end(), strs);
    for (int i = 0; i < 2; i++) {
        cout << strs[i] << endl;
    }


    //从迭代器指向的元素开始,拷贝2个元素到未初始化的内存中
    vector<string> strs3 = { "are","you" };
    uninitialized_copy_n(strs3.begin(), 2, strs);
    for (int i = 0; i < 2; i++) {
        cout << strs[i] << endl;
    }
    
    allocator<int> intAlloc;
    auto ints = intAlloc.allocate(2);
    
    //在迭代器指定的原始内存范围填充指定值
    uninitialized_fill(ints, &ints[2], 123);
    for (int i = 0; i < 2; i++) {
        cout << ints[i] << endl;
    }
    
    //从迭代器指向的元素开始,填充2个指定的值
    uninitialized_fill_n(ints, 2, 456);
    for (int i = 0; i < 2; i++) {
        cout << ints[i] << endl;
    }
    system("pause");
}
```



