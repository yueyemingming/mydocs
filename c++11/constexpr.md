# constexpr

```cpp
constexpr int len_foo() {
    return 5 ;
}

int main()
{
    char str[len_foo()] ;

    int len = 5 ;
    char a[len];            //c++11可以

    return 0 ;
}
```
