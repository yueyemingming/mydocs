# 左值右值

## 1. 基本示例

```c++
++x ; //相当于 x=x+1, return x			// 所以x返回的是左值
x++ ; //相当于 return (x+1), x=x+1		// (x+1)是个临时对象，所以，x返回的是个右值

++x = 10;	//因为是左值，可以进行复制。

int* p = &++x;	//因为是左值，所以可以去地址
int* p = &x++;	//因为是右值，所以不能取地址或赋值，编译错误
```

## 2. 用法

| 名称     | 用法      | 示例                 | 备注                                     |
| -------- | --------- | -------------------- | ---------------------------------------- |
| 右引用   | T&&       | int& r = ++x;        |                                          |
| 左引用   | T&        | int&& r = x++;       | 右值引用，防止了临时变量拷贝，提高了效率 |
| 万能引用 | const T&  | const int& r = x++;  |                                          |
|          | const T&& | const int&& r = x++; | const T&&，没有实际意义，一般不这样用    |

## 3. 转移语义 move

有个右值引用，就可以实现对象转移，同时不需要担心“深拷贝”的问题出现，相当于:

```c++
static_cast<T&&>(t);
```

简要的 **转移构造函数** 和 **转移赋值函数** 示例：

```c++
class moveable {
private:
    int x ;
public :
    moveable() {
        cout << "moveable()" << endl;
    }

    moveable(moveable&& other) {
        cout << "moveable(&&)" << endl ;
        std::swap(x, other.x);
    }

    moveable&operator=(moveable&& other) {
        cout << "operator=(&&)" << endl ;
        std::swap(x, other.x);
        return *this;
    }

    static moveable create() {
        cout << "create()" << endl ;
        moveable obj;
        return obj;		//返回临时对象时，就是右值，不需要用 std::move
    }
};


moveable m1;						//moveable()
moveable m2(std::move(m1));			//moveable(&&)
moveable m3 = moveable::create();	//create(),moveable()
moveable m4(moveable::create());	//create(),moveable()
moveable m5 = std::move(m2);		//moveable(&&)
moveable&& m6 = std::move(m5);		//无输出
```

> 目前string、vector、deque等组件都实现了转移构造函数和转移赋值函数，可以进行转移语义，因此目前的c++在函数中返回一个大容器对象是非常高效的。

## 4. 完美转发

完美转发用于把函数参数原封不动的转发给其他函数。

```c++
void check(int&) {
    cout << "left value" << endl ;
}

void check(int&&) {
    cout << "rigit value" << endl;
}

template<typename T>
void print(T&& v) {             //必须声明成&&，才能保证原状转发
    check(std::forward<T>(v));  //对于指针，引用，值类型都通用
}

int main() {
    int x = 10;
    print(x);               //left value
    print(std::move(x));    //right value
}
```

