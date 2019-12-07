# 函数对象

```cpp
class print {
public:
    print() {
        std::cout << "construct()" << std::endl;
    }

    void operator()(){
        std::cout << "operator()" << std::endl;
    }
};

int main()
{
    print p;
    p();
}
```

```cpp
//1. 原始方式
void fun(const int i)
{
    std::cout << i << std::endl;
}

void test1()
{
    std::vector<int> v(10, 1);

    std::vector<int>::iterator iter = v.begin();
    for( ; iter != v.end() ; ++iter) {
        fun(*iter);
    }
}


//2. 实现for_each模板
template<class Iter, class T>
void for_each(Iter first, Iter last, T f) {     // T的类型是 void (*)(const int); 模板不支持这种类型
    for( ; first != last ; ++first ){
        f(*first);
    }
}

void test2()
{
    std::vector<int> v(10, 1);
    for_each(iter.begin(), iter.end(), fun) ;   // fun的类型是 void (*)(const int); 模板不支持这种类型
}



//3. 函数对象
struct print {
    void operator()(int x) {                    //()这个是函数对象的关键
        std::cout << x << std::endl;
    }
};

void test3()
{
    std::vector<int> a = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10} ;
    const int length = a.size();
    for_each(a.begin(), a.end(), print());
}

int main()
{
    test1();
    test2();
    test3();

    return 0;
}
```

```cpp

//结构体函数对象定义
struct aaa {
public:
    void operator()(int i) {
        std::cout << i << " struct function object" << std::endl;
    }
};

//类函数对象定义
class print {
public:
    void operator()() {
        std::cout << "class function object" << std::endl; 
    }
} ;


int main()
{
    //函数对象调用
    aaa a;
    a(3);

    //函数对象调用
    print p;
    p();
    return 0 ;
}
```