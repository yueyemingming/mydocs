# vector\<char\>与string的相互转换

## string -> vector\<char\>

```cpp
vector<char> vec(str.begin(), str.end());
```

## vector\<char\> -> string

```cpp
string str(vec.begin(), vec.end());
```

## char* -> vector\<char\>

```cpp
copy( str, str+size, back_inserter(vec)) ;
```
