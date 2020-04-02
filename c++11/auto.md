# auto

- c++11中auto不再是类型指示符(比如static, register, mutable等)，而是自动类型推导符。

- 变量在声明时必须马上初始化。

  ```cpp
  auto a = 5;				// a->int, auto->int
  const auto a=6;			// a->const int, auto->int
  const auto *pa=&a;		// a->const int*, auto->int
  auto a = new auto(10);	// a->int*, auto->int*
  auto* a=&x;				// a->int*, auto->int
  auto& a=x;				// a->int&, auto->int
  auto a = 0.0;			// a->double, auto->int
  
  int a[10] = {0};
  auto pa = a;			//pa->int*, auto->int*
  
  // 常用来替代iterator
  for (std::vector<char>::const_iterator itr = v.cbegin(); itr != v.cend(); ++itr);
  for (auto itr = v.cbegin(); itr != v.cend(); ++itr);
  ```

- 不能用作函数参数。
- 不能用作类或结构体中的非static成员变量。
- 赋值时去掉const属性，指针和引用会保留const属性(因为指向同一个对象)

```cpp
const auto a = 5;
auto b=a;				//赋值，b为新对象，const不保留
auto& c=a;				//const保留
auto* d=&a;				//const保留
```

