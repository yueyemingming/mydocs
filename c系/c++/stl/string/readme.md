# stl string

```c++
#include <string>
```

## 1. 声明一个C++字符串

```text
string s                     生成一个空字符串s
string s(str)                拷贝构造函数生成str的复制品
string s(str,stridx)         将字符串str内"始于位置stridx"的部分当作字符串的初值
string s(str,stridx,strlen)  将字符串str内"始于stridx且长度顶多strlen"的部分作为字符串的初值
string s(cstr)               将C字符串作为s的初值
string s(chars,chars_len)    将C字符串前chars_len个字符作为字符串s的初值
string s(num,c)              生成一个字符串，包含num个c字符
string s(beg,end)            以区间beg;end(不包含end)内的字符作为字符串s的初值
s.~string()                  销毁所有字符，释放内存
```

## 2. 字符串操作函数

```text
=,assign()         赋以新值
swap()             交换两个字符串的内容
+=,append(),push_back()      在尾部添加字符
insert()           插入字符
erase()            删除字符
clear()            删除全部字符
replace()          替换字符
+                  串联字符串
==,!=,<,<=,>,>=,compare()    比较字符串  
size(),length()    返回字符数量
max_size()         返回字符的可能最大个数
empty()            判断字符串是否为空
capacity()         返回重新分配之前的字符容量
reserve()          保留一定量内存以容纳一定数量的字符
[ ], at()          存取单一字符
>>,getline()       从stream读取某值
<<                 将谋值写入stream
copy()             将某值赋值为一个C_string
c_str()            将内容以C_string返回
data()             将内容以字符数组形式返回
substr()           返回某个子字符串
begin() end()      提供类似STL的迭代器支持
rbegin() rend()    逆向迭代器
get_allocator()    返回配置器
```

## 3. 详细介绍

### 2.1 C++字符串和C字符串的转换

C++提供的由C++字符串得到对应的C_string的方法是使用data()、c_str()和copy()，其中

* data()以字符数组的形式返回字符串内容，但并不添加’\0’。
* c_str()返回一个以‘\0’结尾的字符数组
* copy()则把字符串的内容复制或写入既有的c_string或字符数组内。

C++字符串并不以’\0’结尾。

我的建议是在程序中能使用C++字符串就使用，除非万不得已不选用c_string。

### 2.2 大小和容量函数

一个C++字符串存在三种大小：

* `size()`, `length()`   现有的字符数，他们等效。
* `capacity()` 重新分配内存之前string所能包含的最大字符数。
* `max_size()` 这个大小是指当前C++字符串最多能包含的字符数，很可能和机器本身的限制或者字符串所在位置连续内存的大小有关系。我们一般情况下不用关心他，应该大小足够我们用的。但是不够用的话，会抛出length_error异常。

* reserve()函数，这个函数为string重新分配内存。重新分配的大小由其参数决定，默认为0，这时候会对string进行非强制性缩减。

### 2.3 元素存取

* 下标操作符[]并不检查索引是否有效，如果索引失效，会引起未定义的行为。
* at()会检查，如果使用at()的时候索引无效，会抛出out_of_range异常。

string Str("string");
Str[100];               //ok
Str[Str.length()]       //ok
Str.at(Str.length());   //ok
Str.at(100);            //未定义的行为, throw out_of_range

const string Cstr("const string");
Cstr[Cstr.length()]       //ok
Cstr.at(Cstr.length())    //未定义行为, 返回 ‘\0’, throw out_of_range

我不赞成类似于下面的引用或指针赋值：
char& r=s[2];
char* p= &s[3];
因为一旦发生重新分配，r,p立即失效。避免的方法就是不使用。

### 2.4 比较函数

C++字符串支持常见的比较操作符（>,>=,<,<=,==,!=），甚至支持string与C-string的比较(如 str<"hello")。
在使用>,>=,<,<=这些操作符的时候是根据"当前字符特性"将字符按字典顺序进行逐一得比较。字典排序靠前的字符小，比较的顺序是从前向后比较，遇到不相等的字符就按这个位置上的两个字符的比较结果确定两个字符串的大小。同时，string ("aaaa") <string(aaaaa)。
另一个功能强大的比较函数是成员函数compare()。他支持多参数处理，支持用索引值和长度定位子串来进行比较。他返回一个整数来表示比较结果，返回值意义如下：0-相等 〉0-大于 <0-小于。举例如下：
string s("abcd");
s.compare("abcd");  
s.compare("dcba");  
s.compare("ab");
s.compare(s);
s.compare(0,2,s,2,2);  
s.compare(1,2,"bcx",2);     //返回0
//返回一个小于0的值
//返回大于0的值
//相等
//用"ab"和"cd"进行比较小于零
//用"bc"和"bc"比较。
怎么样？功能够全的吧！什么？还不能满足你的胃口？好吧，那等着，后面有更个性化的比较算法。先给个提示，使用的是STL的比较算法。什么？对STL一窍不通？靠，你重修吧！

### 2.5 更改内容

这在字符串的操作中占了很大一部分。
首先讲赋值，第一个赋值方法当然是使用操作符=，新值可以是string(如：s=ns) 、c_string(如：s="gaint")甚至单一字符（如：s=’j’）。
还可以使用成员函数assign()，这个成员函数可以使你更灵活的对字符串赋值。还是举例说明吧
s.assign(str);
s.assign(str,1,3);
s.assign(str,2,string::npos);
s.assign("gaint");
s.assign("nico",5);
s.assign(5,’x’);     //不说
//如果str是"iamangel" 就是把"ama"赋给字符串
//把字符串str从索引值2开始到结尾赋给s
//不说
//把’n’ ‘I’ ‘c’ ‘o’ ‘\0’赋给字符串
//把五个x赋给字符串

把字符串清空的方法有三个：
s="";s.clear();s.erase();        (我越来越觉得举例比说话让别人容易懂！)。
string提供了很多函数用于插入（insert）、删除（erase）、替换（replace）、增加字符。
先说增加字符（这里说的增加是在尾巴上），函数有 +=、append()、push_back()。举例如下：
s+=str;
s+="my name is jiayp";
s+=’a’;     //加个字符串
//加个C字符串
//加个字符
s.append(str);
s.append(str,1,3);
s.append(str,2,string::npos)
s.append("my name is jiayp");
s.append("nico",5);
s.append(5,’x’);
//不解释了 同前面的函数参数assign的解释
//不解释了
s.push_back(‘a’);    //这个函数只能增加单个字符对STL熟悉的理解起来很简单。

也许你需要在string中间的某个位置插入字符串，这时候你可以用insert()函数，这个函数需要你指定一个安插位置的索引，被插入的字符串将放在这个索引的后面。
s.insert(0,"my name");
s.insert(1,str);
这种形式的insert()函数不支持传入单个字符，这时的单个字符必须写成字符串形式(让人恶心)。既然你觉得恶心，那就不得不继续读下面一段话：为了插入单个字符，insert()函数提供了两个对插入单个字符操作的重载函数：
insert(size_type index,size_type num,chart c) ,   insert(iterator pos,size_type num,chart c)
其中size_type是无符号整数，iterator是char*,所以，你这么调用insert函数是不行的：insert(0,1, ’j’);这时候第一个参数将转换成哪一个呢？所以你必须这么写：insert((string::size_type)0,1,’j’)！
第二种形式指出了使用迭代器安插字符的形式，在后面会提及。顺便提一下，string有很多操作是使用STL的迭代器的，他也尽量做得和STL靠近。

删除函数erase()的形式也有好几种（真烦！），替换函数replace()也有好几个。举例吧：
string s="il8n";
s.replace(1,2,"nternationalizatio");
s.erase(13);
s.erase(7,5);
//从索引1开始的2个替换成后面的C_string
//从索引13开始往后全删除
//从索引7开始往后删5个

### 2.6 提取子串和字符串连接

题取子串的函数是：substr(),形式如下：
s.substr();
s.substr(11);
s.substr(5,6);     //返回s的全部内容
//从索引11往后的子串
//从索引5开始6个字符
把两个字符串结合起来的函数是+。

### 2.7 输入输出操作

1．>> 从输入流读取一个string。
2．<< 把一个string写入输出流。
另一个函数就是getline(),他从输入流读取一行内容，直到遇到分行符或到了文件尾。

### 2.8 搜索与查找

查找函数很多，功能也很强大，包括了：
find(),  rfind(),  find_first_of(),  find_last_of(),  find_first_not_of(),  find_last_not_of()
这些函数返回符合搜索条件的字符区间内的第一个字符的索引，没找到目标就返回npos。所有的函数的参数说明如下：
第一个参数是被搜寻的对象。
第二个参数（可有可无）指出string内的搜寻起点索引。
第三个参数（可有可无）指出搜寻的字符个数。
最后再说说npos的含义，string::npos的类型是string::size_type,所以，一旦需要把一个索引与npos相比，这个索引值必须是string::size)type类型的，更多的情况下，我们可以直接把函数和npos进行比较（如：if(s.find("jia")== string::npos)）。
