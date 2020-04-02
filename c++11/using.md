# using代替typedef

- using替换typedef所有功能
- 类型替换
```cpp
// 函数指针
void f() {}
int main() {
    //相当于 typedef void (*FunctionPtr)();
    using FunctionPtr = void (*)();
    FunctionPtr ptr = f;
}
```
- 函数指针
```c++
// 类型
typedef unsigned char u1;
typedef unsigned short u2;

using u4 = uint32_t;
using u8 = uint64_t;

using line_no = std::vector<string>::size_type;
// typedef vector<string>::size_type line_no;

using UPtrMapSS = std::unique_ptr<std::unordered_map<std::string, std::string>>;
//typedef std::unique_ptr<std::unordered_map<std::string, std::string>> UPtrMapSS;
```
