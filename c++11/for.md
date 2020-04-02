# for的使用

```cpp
int array[] = {1, 2, 3, 4, 5};
for (auto &x : array)
    cout << x << endl;
```
```c++
vector<int> v(5, 100); // 卧槽，还能这么初始化呀。5个100
for (auto &i : v)
    cout << i << endl;
```

