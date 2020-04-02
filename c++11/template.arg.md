# 默认模板参数

## 1.0 类默认模板参数

```c++
template <typename T = int>
struct Foo {
  T i;  
};

Foo<float> f{3.0};		cout << f.i << endl;	//3
//Foo<> a{3.0};			cout << a.i << endl;    //警告
//Foo<string> m{3};		cout << m.i << endl;	//无法编译通过
Foo<string> s{"ss"};	cout << s.i << endl;	//ss
```

## 2.0 函数默认模板参数

c++98/03中，不允许；c++11中支持。

```c++
template <typename RET = int, typename VAL>
RET func(VAL value) {
    return value;
}

func<long>(123) ;	//指定返回值是long
```

## 3.0 禁用自动推导类型

```c++
template <typename T>
struct Foo {
typedef T type_t;
};

template <typename T = int>
void func(typename Foo<T>::type_t value) {	//这种写法的含义是，要么使用默认类型int，要么必须指定类型，禁止自动推导
cout << value << endl ;
}

func(123.0);						//123 隐式类型转换
//func("aaaaa");					//无法编译通过，需要指定类型
func<string>("aaaaaa");				//aaaaaa
func<string>(string("aaaaaa"));		//aaaaaa
```

  