# tuple 元组

tuple容器(元组), 是表示元组容器, 是不包含任何结构的,快速而低质(粗制滥造, quick and dirty)的, 可以用于函数返回多个返回值;

tuple容器, 可以使用直接初始化, 和"make_tuple()"初始化, 访问元素使用"get<>()"方法, 注意get里面的位置信息, 必须是常量表达式(const expression);

可以通过"std::tuple_size<decltype(t)>::value"获取元素数量; "std::tuple_element<0, decltype(t)>::type"获取元素类型;

如果tuple类型进行比较, 则需要保持元素数量相同, 类型可以比较, 如相同类型, 或可以相互转换类型(int&double);

无法通过普通的方法遍历tuple容器, 因为"get<>()"方法, 无法使用变量获取值;

以下代码包含一些基本的用法, 详见注释;

```cpp  
#include <iostream>  
#include <vector>  
#include <string>  
#include <tuple>  
  
using namespace std;  
  
std::tuple<std::string, int>  
giveName(void)  
{  
    std::string cw("Caroline");  
    int a(2013);  
    std::tuple<std::string, int> t = std::make_tuple(cw, a);  
    return t;  
}  
  
int main()  
{  
    std::tuple<int, double, std::string> t(64, 128.0, "Caroline");  
    std::tuple<std::string, std::string, int> t2 =  
            std::make_tuple("Caroline", "Wendy", 1992);  
  
    //返回元素个数  
    size_t num = std::tuple_size<decltype(t)>::value;  
    std::cout << "num = " << num << std::endl;  
  
    //获取第1个值的元素类型  
    std::tuple_element<1, decltype(t)>::type cnt = std::get<1>(t);  
    std::cout << "cnt = " << cnt << std::endl;  
  
    //比较  
    std::tuple<int, int> ti(24, 48);  
    std::tuple<double, double> td(28.0, 56.0);  
    bool b = (ti < td);  
    std::cout << "b = " << b << std::endl;  
  
    //tuple作为返回值  
    auto a = giveName();  
    std::cout << "name: " << get<0>(a)  
            << " years: " << get<1>(a) << std::endl;  
  
    return 0;  
}
```

输出结果：

```text
num = 3  
cnt = 128  
b = 1  
name: Caroline years: 2013  
```
