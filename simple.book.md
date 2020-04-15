# python简明教程

## 1. hello world

```py
#!/usr/bin/python

print('hello world')
```

## 2. 基本概念

### 2.1 字面量——常量

一个字面意义上的常量的例子是如同5、1.23、9.25e-3这样的数，或者如同'This is a string'、"It's a string!"这样的字符串。

### 2.2 数

- 2是一个整数的例子。
- 长整数不过是大一些的整数。
- 3.23和52.3E-4是浮点数的例子。E标记表示10的幂。在这里，52.3E-4表示52.3 * 10-4。
- (-5+4j)和(2.3-4.6j)是复数的例子。

### 2.3 字符串

#### 2.3.1 单引号（'）== 双引号（"）

```py
"What's your name?"
```

#### 2.3.2 三引号（'''或"""），多行的字符串。

```py
  '''This is a multi-line string. This is the first line.
  This is the second line.
  "What's your name?," I asked.
  He said "Bond, James Bond."
  '''
```
#### 2.3.3 转义符 \

##### 2.3.3.1 使用单引号（\'）

```python
'What\'s your name?'
```

等价于

```python
'What's your name?'
```

##### 2.3.3.2 使用\ (\\\\)

##### 2.3.3.3 行末反斜杠

行尾反斜杠表示字符串在下一行继续，而不是开始一个新的行。例如：

```python
"This is the first sentence.\
This is the second sentence."
```
等价于
```python
"This is the first sentence. This is the second sentence."
```

##### 2.3.3.4 自然字符串 —— 禁用转移 前缀r或R

```python
r"Newlines are indicated by \n"。
```

##### 2.3.3.5 Unicode字符串 —— 前缀u或U。

```python
u"This is a Unicode string."。
```
> 在你处理文本文件的时候使用Unicode字符串，特别是当你知道这个文件含有用非英语的语言写的文本。

##### 2.3.3.6 字符串是不可变的

##### 2.3.3.7 按字面意义级连字符串

把两个字符串按字面意义相邻放着，他们自动级连。

```python
'What\'s' 'your name?'
```
会被自动转为

```python
"What's your name?"
```

> 在Python中没有专门的char数据类型。

### 2.4 变量

标识符的命名
数据类型
对象
输出
它如何工作
逻辑行与物理行
缩进
概括