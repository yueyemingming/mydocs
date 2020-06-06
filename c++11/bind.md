# bind

```c++
#include <iostream>
#include <functional>

using namespace std ;

void call_when_even( int x, const std::function<void(int)>& f) {
    if ( !(x&1) )
        f(x);
}

void output(int x) {
    cout << x << " " ;	//0 2 4 6 8
}

void output_add_2(int x) {
    cout << x+2 << " ";	//2 4 6 8 10
}

int main() {
    {
        function<void(int)> fr = bind(output, placeholders::_1);
        //auto fr = bind(output, placeholders::_1);
        for(int i=0;i<10;i++)
            call_when_even(i, fr);
    }
    cout << endl ;
    {
        function<void(int)> fr = bind(output_add_2, placeholders::_1);
        //auto fr = bind(output_add_2, placeholders::_1);
        for(int i=0;i<10;i++)
            call_when_even(i, fr);
    }
    cout << endl ;
}
```



bind后面带上括号，表示直接函数调用。

```c++
#include <iostream>
#include <functional>

using namespace std;

void output(int x, int y) {
    cout << x << " " << y << endl ;
}

int main() {
    bind(output, 1, 2)();       //1 2
    bind(output, placeholders::_1, 2)(1);   //1 2
    bind(output, 2, placeholders::_1)(1);   //2 1

//    bind(output, 2, placeholders::_2)(1); //error
    bind(output, 2, placeholders::_2)(1,2); //2 2
    
    bind(output, placeholders::_1, placeholders::_2)(1,2);  //1 2
    bind(output, placeholders::_2, placeholders::_1)(1,2);  //2 1

    bind(output, placeholders::_1, placeholders::_2)(1,2);  //1 2
    bind(output, placeholders::_2, placeholders::_1)(1,2);  //2 1

    return 0;
}
```

