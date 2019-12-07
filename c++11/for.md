# for的使用

```cpp
int main() {
    // eg1
    int array[] = {1, 2, 3, 4, 5};
    for (auto &x : array)
        cout << x << endl;

    // eg2
    vector<int> v(5, 100); // 卧槽，还能这么初始化呀。5个100

    for (auto itr = v.begin(); itr != v.end(); ++itr)
        cout << *itr << endl;

    for (auto &i : v)
        cout << i << endl;

    return 0;
}
```
