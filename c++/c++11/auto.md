# auto

```cpp
int main()
{
    // auto推导为int*
    auto a = 5;
    auto aptr = new auto(10);

    //数组地址的赋值
    int arr[10] = {0};
    int *arr_ptr = arr;
    auto arr_auto_ptr = arr;

    // auto替代iterator
    std::vector<char> v;
    v.push_back('a');
    v.push_back('b');
    v.push_back('c');
    for (std::vector<char>::const_iterator itr = v.cbegin(); itr != v.cend(); ++itr);
    for (auto itr = v.cbegin(); itr != v.cend(); ++itr);

    return 0;
}
```
