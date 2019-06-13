# 加载和运行WebAssembly代码

为了在JavaScript中使用WebAssembly，在编译/实例化之前，你首先需要把模块放入内存。比如，通过 [XMLHttpRequest](https://developer.mozilla.org/zh-CN/docs/Web/API/XMLHttpRequest) 或 [Fetch](https://developer.mozilla.org/zh-CN/docs/Web/API/Fetch_API) ，模块将会被初始化为带类型数组；不过，将来会开发更多的方式。本文提供了一篇关于获取WebAssembly字节码的不同机制以及如何编译/实例化并运行它的参考。

## 1. 这里的主题是什么？

WebAssembly还没有和 `<script type='module'>` 或ES6的import语句集成，也就是说，当前还没有内置的方式让浏览器为你获取模块。当前唯一的方式就是创建一个包含你的WebAssembly模块二进制代码的 [ArrayBuffer](https://developer.mozilla.org/zh-CN/docs/Web/API/ArrayBuffer) , 并且使用[WebAssembly.instantiate()](https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Global_Objects/WebAssembly/instantiate) 编译它。这与new Function(string)类似，除了使用一个包含了WebAssembly源代码的数组缓存替换掉包含了JavaScript源代码的字符串。

那么，我们该如何获取这些字节并存入到一个数组缓存并编译它呢？下面进行解释。

## 2. 使用Fetch

Fetch是一个用来获取网络资源的方便现代的API。

假设网络上有一个叫做simple.wasm的WebAssembly模块：

* 我们可以使用 [fetch()](https://developer.mozilla.org/zh-CN/docs/Web/API/WindowOrWorkerGlobalScope/fetch) 全局函数来轻松地获取它，该函数返回一个可以解析为Response对象的promise。
* 我们可以使用 [arrayBuffer()](https://developer.mozilla.org/en-US/docs/Web/API/Body/arrayBuffer) 函数把响应（response）转换为带类型数组，该函数返回一个可以解析为带类型数组的promise。
* 最后，我们使用 [WebAssembly.instantiate()](https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Global_Objects/WebAssembly/instantiate) 函数一步实现编译和实例化带类型数组。

代码块看起来像这样：

```js
fetch('module.wasm').then(response =>
  response.arrayBuffer()
).then(bytes =>
  WebAssembly.instantiate(bytes, importObject)
).then(results => {
  // Do something with the compiled results!
});
```

### 2.1 顺便说一下instantiate()重载

[WebAssembly.instantiate()](https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Global_Objects/WebAssembly/instantiate) 函数有两种重载形式——一种是前面展示的那样，接受待编译的字节码作为参数, 并且返回一个promise, 并且该promise可以解析为一个包含已编译的模块对象及其实例的对象。

```js
{
  module : Module // 新编译的WebAssembly.Module对象，
  instance : Instance // 新的模块对象实例
}
```

> 注： 通常，我们只关心实例，但是，当我们想缓存模块，使用 [postMessage()](https://developer.mozilla.org/zh-CN/docs/Web/API/MessagePort/postMessage) 与另外一个worker或window共享模块或者只是创建更多的实例的时候，拥有模块对象是很有用的。
>
> 注： 这二种重载形式接受一个 [WebAssembly.Module](https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Global_Objects/WebAssembly/Module) 对象作为参数，, 并且返回一个包含了一个实例对象的promise。参考 [第二种重载示例](https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Global_Objects/WebAssembly/instantiate#Second_overload_example) 。

### 2.2 获取及实例化的实用函数

上面的代码样式可以工作，但是，每次都重新编写它们就显得啰嗦了，特别是当你想要加载多个模块的时候。为了简单起见，我们创建了一个叫做fetchAndInstantiate()的实用函数，它在后台工作并返回一个promise。你可以在wasm-utils.js中找到这个函数。它看起来像这样：

```js
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

把这个函数加入到HTML中，你就可以使用一行简单代码做到获取和实例化WebAssembly模块, 并且得到一个实例。

```js
fetchAndInstantiate('module.wasm', importObject).then(function(instance) {
  ...
})
```

> 注： 在我们的文档中，你可以看到许多这么用的例子（例如，参考[index.html](https://mdn.github.io/webassembly-examples/js-api-examples/)）——这是我们推荐的加载模块的标准方式。

### 2.3 运行你的WebAssembly代码

一旦在JavaScript中得到了可用的WebAssembly实例，你就可以开始使用那些通过 [WebAssembly.Instance.exports](https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Global_Objects/WebAssembly/Instance/exports) 属性导出的特性了。你的代码可能看起来像这样：

```js
fetchAndInstantiate('myModule.wasm', importObject).then(function(instance) {
  // 调用导出函数：
  instance.exports.exported_func();

  // 或者获取导出内存的缓存内容：
  var i32 = new Uint32Array(instance.exports.memory.buffer);

  // 或者获取导出表格中的元素：
  var table = instance.exports.table;
  console.log(table.get(0)());
})
```

> 注：关于从WebAssembly模块导出是如何工作的更多信息，请阅读使用WebAssembly的 [JavaScript API](https://developer.mozilla.org/zh-CN/docs/WebAssembly/Using_the_JavaScript_API) 和 [理解WebAssembly文本格式](understand.md)。

## 3. 使用XMLHttpRequest

[XMLHttpRequest](https://developer.mozilla.org/zh-CN/docs/Web/API/XMLHttpRequest) 在一定程度上而言要比Fetch老旧一些，但是，仍然可以很好地被用来获取带类型数组。仍然假设我们的模块叫做simple.wasm：

1. 创建一个 [XMLHttpRequest()](https://developer.mozilla.org/zh-CN/docs/Web/API/XMLHttpRequest) 实例，然后使用它的 [open()](https://developer.mozilla.org/zh-CN/docs/Web/API/XMLHttpRequest/open) 方法来开启一个请求——设置请求方法为GET, 并且声明我们想要获取的文件路径。
2. 关键之处在于使用 [responseType](https://developer.mozilla.org/zh-CN/docs/Web/API/XMLHttpRequest/responseType) 属性设置响应类型为'arraybuffer'。
3. 接下来使用 [XMLHttpRequest.send()](https://developer.mozilla.org/zh-CN/docs/Web/API/XMLHttpRequest/send) 发送请求。
4. 当响应已经完成下载之后，我们使用onload事件处理器来调用一个函数——在这个函数中，我们从 [response](https://developer.mozilla.org/zh-CN/docs/Web/API/XMLHttpRequest/response) 属性中得到数组缓存然后就像使用Fetch那样把它传递给[WebAssembly.instantiate()](https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Global_Objects/WebAssembly/instantiate) 。

最终代码看起来像这样：

```js
request = new XMLHttpRequest();
request.open('GET', 'simple.wasm');
request.responseType = 'arraybuffer';
request.send();

request.onload = function() {
  var bytes = request.response;
  WebAssembly.instantiate(bytes, importObject).then(results => {
    results.instance.exports.exported_func();
  });
};
```

> 注：你可以在 [xhr-wasm.html](https://mdn.github.io/webassembly-examples/js-api-examples/xhr-wasm.html) 看到实际使用的例子。