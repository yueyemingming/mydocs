# map的操作

```c++
#include <iostream>
#include <map>
#include <string>

using namespace std;

struct Test {
    string s;
};

int main() {
    map<int, Test> m;       //此处注意不要使用引用
    Test t{string("aaaaaaaaa")};

    m[1]= t;

    Test& p = m[1];             //可以直接引用map中的数据
    p.s=std::string("bbbb");    //可以修改

    std::cout << p.s << std::endl;      //bbbb
    std::cout << m[1].s << std::endl;   //bbbb

    return 0;
}
```