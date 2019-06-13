# 理解WebAssembly文本格式

为了能够让人类阅读和编辑WebAssembly, wasm二进制格式提供了相应的文本表示.

本文用基本语法的方式解释了这种文本表示是如何工作的, 以及它是如何与它表示的底层字节码, 及在JavaScript中表示wasm的封装对象关联起来的.

## 1. S-表达式

不论是二进制还是文本格式, WebAssembly代码中的基本单元是一个模块. 在文本格式中, 一个模块被表示为一个大的S-表达式.

S-表达式是一个非常古老和非常简单的用来表示树的文本格式. 因此, 我们可以把一个模块想象为一棵由描述了模块结构和代码的节点组成的树. 不过, 与一门编程语言的抽象语法树不同的是, WebAssembly的树是相当平的, 也就是大部分包含了指令列表.

首先, 让我们看下S-表达式长什么样. 树上的每个一个节点都有一对括号 ( ... ) 包围. 括号内的第一个标签告诉你该节点的类型, 其后跟随的是由空格分隔的属性或孩子节点列表. 因此, 这意味着WebAssembly的S-表达式：

```text
(module (memory 1) (func))
```

表示一棵根节点为“模块(module)”的树, 并且该树有两个孩子节点, 分别是属性为1的 `内存(memory)` 节点和一个 `函数(func)` 节点.

### 1.1 最简单的模块

让我们从最简单最短的可能的wasm模块开始.

```text
(module)
```

这个模块完全是空的, 但是仍然是一个合法的模块.

如果我们现在把该模块转换为二进制([将WebAssembly文本格式转换为wasm](text-to-wasm.md)), 我们将会看到在二进制格式中描述的8字节的模块头：

```text
0000000: 0061 736d              ; WASM_BINARY_MAGIC
0000004: 0d00 0000              ; WASM_BINARY_VERSION
```

### 1.2 向你的模块中增加功能

好了, 那并不是很有趣, 让我们向模块中增加一些可执行代码.

WebAssembly模块中的所有代码都是划分到函数里面. 函数具有下列的伪代码结构：

```js
( func <signature> <locals> <body> )
```

* `签名`       声明函数需要的参数, 以及函数的返回值.
* `局部变量`   像JavaScript中的变量, 但是显式的声明了类型.
* `函数体`     是一个低级指令的线性列表.

因此, 这与其他语言的函数很像, 尽管由于它是一个S-表达式并由此看起来有点儿不同.

## 2. 签名和参数

签名是由一系列 `参数类型声明` 及其后面的 `返回值类型声明` 列表组成. 值得注意的是：

* 没有(result)意味着函数不返回任何东西.
* 在当前版本中, 最多拥有一个返回类型, 但是以后会放开这个限制到任意数量.

每一个参数都有一个显式声明的类型；wasm当前有四个可用类型：

* i32：32位整数
* i64：64位整数
* f32：32位浮点数
* f64：64位浮点数

一个参数写为 `(param i32)`, 返回值类型写为`(result i32)`. 因此, 一个接受两个32位整数并返回一个64位浮点数的二进制函数会写成这样：

```js
(func (param i32) (param i32) (result f64) ... )
```

在签名的后面是带有类型的局部变量, 例如 `(local i32)`. 总的来说, 参数就是使用了调用函数传递过来的实参值进行了初始化的局部变量.

## 3. 获取和设置局部变量和参数

局部变量和参数能够被函数体使用get_local和set_local指令进行读写.

get_local/set_local指令使用数字索引来指向将被存取的条目：按照它们的声明顺序, 参数在前, 局部变量在后. 因此, 给定下面的函数：

```js
(func (param i32) (param f32) (local f64)
  get_local 0
  get_local 1
  get_local 2)
```

指令get_local 0会得到i32类型的参数, get_local 1会得到f32类型的参数, 以及get_local 2会得到f64类型的局部变量.

这有一个问题——使用数字索引来指向某个条目让人困惑和烦恼, 所以, 文本格式允许你给参数、局部变量和其他大部分条目起个名字, 方法就是在类型声明的前面添加一个使用美元符号($)作为前缀的名字.

这样的话, 你可以像下面这样改写我们前面的签名：

```js
(func (param $p1 i32) (param $p2 f32) (local $loc i32) …)
```

然后, 使用get_local $p1代替get_local 0, 等等. (注意, 当文本转换为二进制后, 二进制中只包含整数. )

## 4. 栈式机器

在能够编写函数体之前, 我们需要再讨论一件事情：栈式机器. 虽然浏览器把wasm编译为某种更高效的东西, 但是, wasm的执行是以栈式机器定义的. 也就是说, 其基本理念是每种类型的指令都是在栈上执行特定数量的i32/i64/f32/f64类型值的入栈出栈操作.

例如, get_local被定义为把它读到的局部变量值压入到栈上, 然后i32.add从栈上取出两个i32类型值(它的含义是把前面压入栈上的两个值取出来)计算它们的和(以2^32求模), 最后把结果压入栈上.

当函数被调用的时候, 它是从一个空栈开始的. 随着函数体指令的执行, 栈会逐步填满和清空. 例如, 在执行了下面的函数之后：

```js
(func (param $p i32)
  get_local $p
  get_local $p
  i32.add)
```

栈上只包含一个i32类型值——表达式 ($p + $p)的结果, 该结果是由i32.add得到的. 函数的返回值就是栈上留下的那个最终值.

WebAssembly验证规则确保栈准确匹配：如果你声明了(result f32), 那么, 最终栈上必须包含一个f32类型值. 如果没有result类型, 那么栈必须是空的.

## 5. 我们的第一个函数体

正如前面提到的, 函数体就是函数被调用后执行的指令列表. 把已经学到的放在一起, 我们能够定义一个包含我们的简单函数的模块：

```js
(module
  (func (param $lhs i32) (param $rhs i32) (result i32)
    get_local $lhs
    get_local $rhs
    i32.add))
```

这个函数获取两个参数, 然后相加, 最后返回其结果.

有很多东西都可以放在函数体里面, 但是, 现在我们从简单的开始, 然后随着逐步前进, 你会看到更多的例子. 访问[webassembly.org语义手册](http://webassembly.org/docs/semantics/)获取可用操作码的完整列表.

### 5.1 调用函数

我们的函数自己不会做什么——现在, 我们需要调用它. 我们该如何做呢？正如在一个ES2015模块里面一样, wasm函数必须通过模块里面的export语句显式地导出.

像局部变量一样, 函数默认也是通过索引来区分的, 但是为了方便, 可以给它们起个名字. 让我们由此开始——首先, 在关键字func的后面增加一个美元符号开头的名字：

```js
(func $add … )
```

现在, 我们需要增加一个导出声明——看起来像下面这样：

```js
(export "add" (func $add))
```

这里的add是JavaScript中用来区别这个函数的名字, 而$add则是指出模块中的哪个WebAssembly函数将会被导出：

所以, 我们最终的模块(当前)看起来像下面这样：

```js
(module
  (func $add (param $lhs i32) (param $rhs i32) (result i32)
    get_local $lhs
    get_local $rhs
    i32.add)
  (export "add" (func $add))
)
```

如果你想继续研究这个例子, 那么把我们上面的模块保存到一个名叫add.wat的文件中, 然后使用 `wabt` (参考[将WebAssembly文本格式转换为wasm](text-to-wasm.md))将其转换为名叫 `add.wasm` 的二进制文件.

接下来, 我们把二进制文件加载到叫做 `addCode` 的带类型数组(获取WebAssembly字节码), 编译并实例化它, 然后在JavaScript中执行我们的add函数(现在, 我们可以在实例的[exports](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/WebAssembly/Instance/exports)属性中找到add()).

```js
fetchAndInstantiate('add.wasm').then(function(instance) {
   console.log(instance.exports.add(1, 2));  // "3"
});

// fetchAndInstantiate() found in wasm-utils.js
function fetchAndInstantiate(url, importObject) {
  return fetch(url).then(response =>
    response.arrayBuffer()
  ).then(bytes =>
    WebAssembly.instantiate(bytes, importObject)
  ).then(results =>
    results.instance
  );
}
```

> 注：你可以在GitHub上找到这个例子 [add.html](https://github.com/mdn/webassembly-examples/blob/master/understanding-text-format/add.html) (实时运行). 另外, 参考 [WebAssembly.instantiate()](https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Global_Objects/WebAssembly/instantiate) 来获取关于实例化函数的更多细节, 以及 [wasm-utils.j](https://github.com/mdn/webassembly-examples/blob/master/wasm-utils.js) 来获取 `fetchAndInstantiate()` 的源代码.

## 6. 探索基本原则

现在, 我们已经讨论了基本概念, 让我们继续看看更高级的特性.

### 6.1 在同一模块里的函数调用其他函数成员

为函数给定一个索引或名字, call指令可以调用它. 例如, 下面的模块包含两个函数——一个返回值42, 另一个返回, 第一个函数结果加1.

```js
(module
  (func $getAnswer (result i32)
    i32.const 42)
  (func (export "getAnswerPlus1") (result i32)
    call $getAnswer
    i32.const 1
    i32.add))
```

在这个例子中, 你注意到一个 `(export "getAnswerPlus1")` 代码段, 并且它声明在第二个函数的func语句之后——这声明我们想导出这个函数, 以及定义导出的名字的简便方法.

从功能上来说, 这与我们前面做过的那样——在函数外面即模块的其他地方包括一个独立的函数语句——是等价的. 例如：

```js
(export "getAnswerPlus1" (func $functionName))
```

调用我们前面模块的JavaScript看起来像这样：

```js
fetchAndInstantiate('call.wasm').then(function(instance) {
  console.log(instance.exports.getAnswerPlus1());  // "43"
});
```

> 注：你可以在GitHub上找到这个例子 [call.wasm](https://github.com/mdn/webassembly-examples/blob/master/understanding-text-format/call.html) (或实时运行). 再提一次, 查看 [wasm-utils.js](https://github.com/mdn/webassembly-examples/blob/master/wasm-utils.js) 来了解 `fetchAndInstantiate()` 的源代码.

### 6.2 从JavaScript导入函数

我们已经见过JavaScript调用WebAssembly函数, 但是WebAssembly如何调用JavaScript函数呢？事实上, WebAssembly对JavaScript没有任何了解, 但是, 它有一个可以导入JavaScript或wasm函数的通用方法. 让我们看一个例子：

```js
(module
  (import "console" "log" (func $log (param i32)))
  (func (export "logIt")
    i32.const 13
    call $log))
```

WebAssembly使用了两级命名空间, 所以, 这里的导入语句是说我们要求从console模块导入log函数. 另外, 你可以看到导出的logIt函数使用我们前面介绍的call指令调用了导入的函数.

导入的函数就像普通函数一样：它们拥有一个WebAssembly验证机制会静态检查的签名, 并且可以被设置一个索引, 以及能够被命名和被调用.

JavaScript函数没有签名的概念, 因此, 无论导入的声明签名是什么, 任何JavaScript函数都可以被传递过来. 一旦一个模块声明了一个导入,   [WebAssembly.instantiate()](https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Global_Objects/WebAssembly/instantiate) 的调用者必须传递一个拥有相应属性的导入对象.

就上面而言, 我们需要一个(让我们称之为importObject的)对象, 并且importObject.console.log是一个JavaScript函数.

这看起来像下面这样：

```js
var importObject = {
  console: {
    log: function(arg) {
      console.log(arg);
    }
  }
};

fetchAndInstantiate('logger.wasm', importObject).then(function(instance) {
  instance.exports.logIt();
});
```

> 注：你可以在GitHub上找到这个例子 [logger.html](https://github.com/mdn/webassembly-examples/blob/master/understanding-text-format/logger.html) (实时运行).

### 6.3 WebAssembly内存

上面的例子是一个相当简单的日志函数：它只是打印一个整数！要是我们想输出一个文本字符串呢？为了处理字符串及其他复杂数据类型, WebAssembly提供了内存. 按照WebAssembly的定义, 内存就是一个随着时间增长的字节数组. WebAssembly包含诸如i32.load和i32.store指令来实现对线性内存的读写.

从JavaScript的角度来看, 内存就是一个包括一切的大的可变大小的ArrayBuffer. 从字面上来说, 这也是asm.js所做的(除了它不能改变大小；参考 [asm.js编程模型](http://asmjs.org/spec/latest/#programming-model)).

因此, 一个字符串就是位于这个线性内存某处的字节序列. 让我们假设我们已经把一个合适的字符串字节写入到了内存中；那么, 我们该如何把那个字符串传递给JavaScript呢？

关键在于JavaScript能够通过 [WebAssembly.Memory()](https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Global_Objects/WebAssembly/Memory) 接口创建WebAssembly线性内存实例, 并且能够通过相关的实例方法获取已经存在的内存实例(当前每一个模块实例只能有一个内存实例). 内存实例拥有一个 [buffer](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/WebAssembly/Memory/buffer) 获取器, 它返回一个指向整个线性内存的ArrayBuffer.

内存实例也能够增长. 举例来说, 在JavaScript中可以调用 [Memory.grow()](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/WebAssembly/Memory/grow) 方法. 由于ArrayBuffer不能改变大小, 所以, 当增长产生的时候, 当前的ArrayBuffer会被移除, 并且一个新的ArrayBuffer会被创建并指向新的、更大的内存. 这意味着为了向JavaScript传递一个字符串, 我们所需要做的就是把字符串在线性内存中的偏移量, 以及某种表示其长度的方法传递出去.

虽然有许多不同的方法在字符串自身当中保存字符串的长度(例如, C字符串)；但是, 这里为了简单起见, 我们仅仅把偏移量和长度都作为参数：

```js
(import "console" "log" (func $log (param i32) (param i32)))
```

在JavaScript端, 我们可以使用 [文本解码器](https://developer.mozilla.org/en-US/docs/Web/API/TextDecoder) API从而轻松地把我们的字节解码为一个JavaScript字符串. (这里, 我们使用utf8, 不过, 许多其他编码也是支持的. )

```js
consoleLogString(offset, length) {
  var bytes = new Uint8Array(memory.buffer, offset, length);
  var string = new TextDecoder('utf8').decode(bytes);
  console.log(string);
}
```

这个谜题的最后一部分就是consoleLogString从哪里获得WebAssembly的内存(memory)实例. 这里, WebAssembly给我们很大灵活性：我们既可以使用JavaScript创建一个 [内存](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/WebAssembly/Memory) 对象并让WebAssembly模块导入这个内存或者我们让WebAssembly模块创建这个内存并把它导出给JavaScript.

为了简单起见, 让我们用JavaScript创建它, 然后把它导入到WebAssembly. 我们的导入语句编写如下：

```js
(import "js" "mem" (memory 1))
```

1表示导入的内存必须至少有1页内存. (WebAssembly定义一页为64KB. )

因此, 让我们看一个完整的打印字符串“Hi”的模块. 在一个常规的已编译的C程序, 你会调用一个函数来为字符串分配一段内存, 但是, 因为我们正在编写自己的汇编, 并且我们拥有整个线性内存, 所以, 我们可以使用数据(data)段把字符串内容写入到一个全局内存中. 数据段允许字符串字节在实例化时被写在一个指定的偏移量. 而且, 它与原生的可执行格式中的数据(.data)段是类似的.

我们最终的wasm模块看起来像这样：

```js
(module
  (import "console" "log" (func $log (param i32 i32)))
  (import "js" "mem" (memory 1))
  (data (i32.const 0) "Hi")
  (func (export "writeHi")
    i32.const 0  ;; pass offset 0 to log
    i32.const 2  ;; pass length 2 to log
    call $log))
```

> 注： 注意上面的双分号语法, 它允许在WebAssembly文件中添加注释.

现在, 我们可以从JavaScript中创建一个1页的内存(Memory )然后把它传递进去. 这会在控制台输出"Hi".

```js
var memory = new WebAssembly.Memory({initial:1});

var importObj = { console: { log: consoleLogString }, js: { mem: memory } };

fetchAndInstantiate('logger2.wasm', importObject).then(function(instance) {
  instance.exports.writeHi();
});
```

> 注：你可以在GitHub上找到完整源代码 [logger2.html](https://github.com/mdn/webassembly-examples/blob/master/understanding-text-format/logger2.html) (或者实时运行).

### 6.4 WebAssembly表格

总的来说, 表格是从WebAssembly代码中通过索引获取的可变大小的引用数组.

为了了解为什么表格是必须的, 我们首先需要观察前面看到的call指令, 它接受一个静态函数索引, 并且只调用了一个函数——但是, 如果被调用者是一个运行时值呢？

* 在JavaScript中, 我们总是看到：函数是一等值.
* 在C/C++中, 我们看到了函数指针.
* 在C++中, 我们看到了虚函数.

WebAssembly需要一种做到这一点的调用指令, 因此, 我们有了接受一个动态函数操作数的call_indirect指令. 问题是, 在WebAssembly中, 当前操作数的仅有的类型是i32/i64/f32/f64.

WebAssembly可以增加一个 `anyfunc`类型("any"的含义是该类型能够持有任何签名的函数), 但是, 不幸的是, 由于安全原因, 这个anyfunc类型不能存储在线性内存中. 线性内存会把存储的原始内容作为字节暴露出去, 并且这会使得wasm内容能够任意的查看和修改原始函数地址, 而这在网络上是不被允许的.

解决方案是在一个表格中存储函数引用, 然后作为 代替, 传递表格索引——它们只是i32类型值. 因此, call_indirect的操作数可以是一个i32类型索引值.

#### 在wasm中定义一个表格

那么, 我们该如何在表格中放置wasm函数呢？就像数据段能够用来通过字节初始化线性内存区域一样, 元素(elem)段能够用来通过函数初始化表格区域：

```js
(module
  (table 2 anyfunc)
  (elem (i32.const 0) $f1 $f2)
  (func $f1 (result i32)
    i32.const 42)
  (func $f2 (result i32)
    i32.const 13)
  ...
)
```

* 在(table 2 anyfunc)中, 数字2表示的是表格的初始大小(也就是它将存储两个引用), 并且anyfunc声明的是这些引用的元素类型是“一个具有任何签名的函数”. 在当前的WebAssembly版本中, 这是唯一被允许的元素类型, 但是在将来, 更多的元素类型会加入进来.
* 函数(func)部分跟任何其他声明的wasm函数没有什么两样. 她们是我们将会在表格中引用的函数(作为例子, 每一个只是返回一个静态值). 值得注意的是, 函数部分声明的顺序并不重要——你可以在任何地方声明你的函数, 然后在你的元素段(elem section)中引用它们.
* 元素段(elem section)能够将一个模块中的任意函数子集以任意顺序列入其中, 并允许出现重复. 列入其中的函数将会被表格引用, 并且引用顺序是其出现的顺序.
* 元素段(elem section)中的(i32.const 0)值是一个偏移量——它需要在元素段开始的位置声明, 其作用是表明函数引用是在表格中的什么索引位置开始存储的. 这里我们指定的偏移量是0, 表格大小是2(参考上面), 因此, 我们可以在索引0和1的位置填入两个引用. 如果想在偏移量1的位置开始写入引用, 那么, 我们必须使用(i32.const 1), 并且表格大小必须是3.

> 注： 未初始化的元素会被设定一个默认的调用即抛出(throw-on-call)值.

在JavaScript中, 可以创建这样一个表格实例的等价的函数调用看起来如下所示：

```js
function() {
  // table section
  var tbl = new WebAssembly.Table({initial:2, element:"anyfunc"});

  // function sections:
  var f1 = function() { … }
  var f2 = function() { … }

  // elem section
  tbl.set(0, f1);
  tbl.set(1, f2);
};
```

#### 使用表格

接着继续. 现在, 表格已经定义好了, 我们需要用某种方法使用它. 让我们使用下面的代码段来做到这一点：

```js
(type $return_i32 (func (result i32))) ;; if this was f32, type checking would fail
(func (export "callByIndex") (param $i i32) (result i32)
  get_local $i
  call_indirect $return_i32)
```

* (type $return_i32 (func (param i32)))代码块使用一个引用名字定义了一个类型. 该类型被用来在后续的表格函数引用调用时进行类型检查. 这里, 我们声明的是该引用是一个返回值为i32类型的函数.
* 接下来, 我们定义了一个导出名字为callByIndex的函数. 它有一个接受i32类型的参数`$i`.
* 在函数里面, 我们在栈顶压入一个值——该值就是传递给参数`$i`的值.
* 最后, 我们使用call_indirect指令调用表格中的函数——这隐含的意思是$的值从栈顶出栈. 最终的结果就是callByIndex函数会调用表格中的第`$i`个函数.

你也可以在命令调用的时候显式地声明call_indirect的参数, 就像下面这样：

```js
(call_indirect $return_i32 (get_local $i))
```

在更高层面, 像JavaScript这样更具表达力的语言, 你可以设想使用一个数组(或者更有可能的是对象)来完成相同的事情. 伪代码看起来像这样：`tbl[i]()`.

回到类型检查. 因为WebAssembly是带有类型检查的, 并且anyfunc的含义是任何函数签名, 所以, 我们必须在调用点提供假定的被调用函数签名. 这里, 我们包含了一个 `$return_i32` 类型来告诉程序期望的是一个返回值为i32类型的函数. 如果被调用函数没有一个匹配的签名(比如说返回值是f32类型的), 那么, 程序会抛出[WebAssembly.RuntimeError](https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Global_Objects/WebAssembly/RuntimeError) 异常.

那么, 是什么把call_indirect指令和我们要是用的表格联系起来的呢？答案是, 现在每一个模块实例只允许唯一一个表格存在, 这也就是call_indirect指令隐式地使用的表格. 在将来, 当多表格被允许了, 我们需要在代码行中指明一个某种形式的表格标识符：

```js
call_indirect $my_spicy_table $i32_to_void
```

完整的模块看起来如下所示, 并且能够在我们的 [wasm-table.wat](https://github.com/mdn/webassembly-examples/blob/master/understanding-text-format/wasm-table.wat) 示例文件中找到：

```js
(module
  (table 2 anyfunc)
  (func $f1 (result i32)
    i32.const 42)
  (func $f2 (result i32)
    i32.const 13)
  (elem (i32.const 0) $f1 $f2)
  (type $return_i32 (func (result i32)))
  (func (export "callByIndex") (param $i i32) (result i32)
    get_local $i
    call_indirect $return_i32)
)
```

我们使用下面的JavaScript把它加载到一个网页中：

```js
fetchAndInstantiate('wasm-table.wasm').then(function(instance) {
  console.log(instance.exports.callByIndex(0)); // 返回42
  console.log(instance.exports.callByIndex(1)); // 返回13
  console.log(instance.exports.callByIndex(2));
  // 返回一个错误, 因为在表格中没有索引值2
});
```

> 注：你可以在GitHub上找到这个例子 [wasm-table.html](https://github.com/mdn/webassembly-examples/blob/master/understanding-text-format/wasm-table.html) (实时查看).
> 注：就像内存一样, 表格也能够从JavaScript中创建 (参考 [WebAssembly.Table()](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/WebAssembly/Table)), 并且能够导入和导出到其他wasm模块.

### 6.5 改变表格和动态链接

因为JavaScript对于函数引用有完全的存取权限, 所以, 从JavaScript中通过[grow()](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/WebAssembly/Table/grow)、[get()](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/WebAssembly/Table/get) 和 [set()](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/WebAssembly/Table/set) 方法能够改变表格对象.

因为表格是可变的, 所以, 它们能够用来实现复杂的加载时和运行时[动态链接](http://webassembly.org/docs/dynamic-linking). 当程序被动态地链接, 多个实例共享相同的内存和表格. 这与原生应用程序的多个.dll共享一个进程地址空间是等价的.

为了看看实际情况, 我们会创建一个包含一个内存对象和一个表格对象的导入对象, 并且把这个导入对象传递到多个 [instantiate()](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/WebAssembly/instantiate) 调用中去.

我们的.wat看起来像这样：

`shared0.wat`:

```js
(module
  (import "js" "memory" (memory 1))
  (import "js" "table" (table 1 anyfunc))
  (elem (i32.const 0) $shared0func)
  (func $shared0func (result i32)
   i32.const 0
   i32.load)
)
```

`shared1.wat`:

```js
(module
  (import "js" "memory" (memory 1))
  (import "js" "table" (table 1 anyfunc))
  (type $void_to_i32 (func (result i32)))
  (func (export “doIt”) (result i32)
   i32.const 0
   i32.const 42
   i32.store  ;; store 42 at address 0
   i32.const 0
   call_indirect $void_to_i32)
)
```

运行逻辑如下：

1. 函数shared0func在shared0.wat中定义并存储在我们的导出表格对象(table)中.
2. 该函数先创建一个常量值为0, 然后执行i32.load指令. 用给定的内存索引, 去加载存储到内存对象中的值, 给定的索引值为0. —— 这样, 会隐式地将之前的值出栈. 所以, shared0func加载并返回了存储在内存对象索引0处的值.
3. 在shared1.wat中, 我们导出了一个名为doIt的函数——这个函数创建了两个常量值, 分别为0和42, 然后使用i32.store指令把给定的值存储在指定索引位置的内存对象中. 同样的, 该指令会把这些值出栈, 所以, 结果就是把42存储在内存索引0处.
4. 在这个函数的最后一部分, 我们创建了常量值0, 然后调用表格中索引0处的函数, 该函数正是我们之前在shared0.wat中的使用元素代码段(elem block)存储的shared0func.
5. shared0func在被调用之后会加载我们在shared1.wat中使用i32.store指令存储在内存中的42.

> 注：上面的表达式会隐式地把这些值出栈, 但是, 你可以在使用指令的时候进行显式地声明. 例如：
>
> ```js
> (i32.store (i32.const 0) (i32.const 42))
> (call_indirect $void_to_i32 (i32.const 0))
> ```

在转换为汇编之后, 我们可以在JavaScript中通过下面的代码使用shared0.wasm和shared1.wasm：

```js
var importObj = {
  js: {
    memory : new WebAssembly.Memory({ initial: 1 }),
    table : new WebAssembly.Table({ initial: 1, element: "anyfunc" })
  }
};

Promise.all([
  fetchAndInstantiate('shared0.wasm', importObj),
  fetchAndInstantiate('shared1.wasm', importObj)
]).then(function(results) {
  console.log(results[1].exports.doIt());  // prints 42
});
```

每一个将被编译的模块都可以导入相同的内存和表格对象, 这也就是共享相同的线性内存和表格的“地址空间”.

> 注：你可以在GitHub上找到这个例子 [shared-address-space.html](https://github.com/mdn/webassembly-examples/blob/master/understanding-text-format/shared-address-space.html) (或者实时运行).

## 7. 总结

以上我们概括浏览了, 关于WebAssembly文本格式的主要部分, 以及它们是如何映射到WebAssembly JS API中的.