# C++11新特性：unordered_map和map的对比

unordered_map和map类似，都是存储的key-value的值，可以通过key快速索引到value。

不同的是unordered_map不会根据key的⼤⼩进⾏排序，存储时是根据key的hash值判断元素是否相同，即unordered_map内部元素是无序的，⽽map中的元素是按照二叉搜索树存储，进⾏中序遍历会得到有序遍历。

所以使⽤时map的key需要定义operator<。⽽unordered_map需要定义hash_value函数并且重载operator==。

很多系统内置的数据类型都⾃带这些，那么如果是⾃定义类型，那么就需要⾃己重载operator<或者hash_value()了。

结论：如果需要内部元素自动排序，使⽤map，不需要排序使⽤unordered_map。

## 1. map使⽤案例

```cpp
#include<string>
#include<iostream>
#include<map>
using namespace std;

struct person
{
    string name;
    int age;
    person(string name,int age)
    {
        this->name=name;
        this->age=age;
    }
    bool operator<(const person&p)const
    {
        return this->age<p.age;
    }
};

int main()
{
    map<person,int> m;

    person p1("Tom1",20);
    person p2("Tom2",22);
    person p3("Tom3",22);
    person p4("Tom4",23);
    person p5("Tom5",24);

    m.insert(make_pair(p3,100));
    m.insert(make_pair(p4,100));
    m.insert(make_pair(p5,100));
    m.insert(make_pair(p1,100));
    m.insert(make_pair(p2,100));

    for(auto iter : m )
        cout<< iter.first.name << "\t" << iter.first.age << endl;

    return 0;
}
```

输出为：(根据age进⾏了排序的结果)

```result
Tom1 20
Tom3 22
Tom4 23
Tom5 24
```

因为Tom2和Tom3的age相同，由我们定义的operator<只是比较的age，所以Tom3覆盖了Tom2，结果中没有Tom2。

如果运算符 **<** 的重载是如下。

```cpp
bool operator<(const person&p) const {
    return this->name < p.name ;
}
```

输出结果：按照那么进⾏的排序，如果有那么相同则原来的那么会覆盖

```result
Tom1 20
Tom2 22
Tom3 22
Tom4 23
Tom5 24
```

## 2. unordered_map使⽤案例

```cpp
#include<string>
#include<iostream>
#include<unordered_map>
using namespace std;
struct person {
    string name;
    int age;
    person(string name,int age) {
        this->name=name;
        this->age=age;
    }

    bool operator==(const person&p)const {
        return name==p.name&&age==p.age;
    }
};

size_t hash_value(const person&p) {
    size_t seed=0;
    std::hash_combine(seed,std::hash_value(p.name));
    std::hash_combine(seed,std::hash_value(p.age));
    return seed;
}

int main() {
    typedef std::unordered_map<person,int>umap;
    umap m;
    person p1("Tom1",20);
    person p2("Tom2",22);
    person p3("Tom3",22);
    person p4("Tom4",23);
    person p5("Tom5",24);
    m.insert(umap::value_type(p3,100));
    m.insert(umap::value_type(p4,100));
    m.insert(umap::value_type(p5,100));
    m.insert(umap::value_type(p1,100));
    m.insert(umap::value_type(p2,100));

    for(auto iter : m )
        cout << iter.first.name << "\t" << iter.first.age << endl;

    return0;
}

对于hash_value的重载没有成功，在vs2013上报错; g++也提示没有hash_combine这个函数呀。操你大爷。
