# auto

c++11中auto不再是类型指示符(比如static, register, mutable等)，而是自动类型推导符。

auto变量在声明时必须马上初始化。

```cpp
auto a = 5;				// int
auto a = new auto(10);	// int*
const auto *pa=&a;		// const int*
const auto a=6;			// const int
auto a = 0.0;			// double

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
```
