# 类型转换函数

```cpp
struct object_id_type {
    operator string() {
        string s;
        s += i;
        return s;
    }

    int i;
};

int main()
{
    object_id_type a;
    a.i = 65;
    cout << string(a) << endl;
    return 0;
}
```
