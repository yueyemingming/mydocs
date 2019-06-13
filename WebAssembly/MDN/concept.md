# WebAssembly概念

本文解释了WebAssembly如何工作的概念，包括它的目标、它解决的问题以及它是如何在网络浏览器的渲染引擎中运行的。

## 1. WebAssembly是什么？

WebAssembly是一种运行在现代网络浏览器中的新型代码并且提供新的性能特性和效果。它设计的目的不是为了手写代码而是为诸如C、C++和Rust等低级源语言提供一个高效的编译目标。

对于网络平台而言，这具有巨大的意义——这为客户端app提供了一种在网络平台以接近本地速度的方式运行多种语言编写的代码的方式；在这之前，客户端app是不可能做到的。

而且，你可以在不知道如何编写WebAssembly代码的情况下就可以使用它。WebAssembly的模块可以被导入的到一个网络app（或Node.js）中，并且暴露出供JavaScript使用的WebAssembly函数。JavaScript框架不但可以使用WebAssembly获得巨大性能优势和新特性，而且还能使得各种功能保持对网络开发者的易用性。

## 2. WebAssembly的目标

作为 W3C WebAssembly Community Group中的一项开放标准，WebAssembly是为下列目标而生的：

* 快速、高效、可移植——通过利用常见的硬件能力，WebAssembly代码在不同平台上能够以接近本地速度运行。
* 可读、可调试——WebAssembly是一门低阶语言，但是它有确实有一种人类可读的文本格式（其标准即将得到最终版本），这允许通过手工来写代码，看代码以及调试代码。
* 保持安全——WebAssembly被限制运行在一个安全的沙箱执行环境中。像其他网络代码一样，它遵循浏览器的同源策略和授权策略。
* 不破坏网络——WebAssembly的设计原则是与其他网络技术和谐共处并保持向后兼容。

> 注：WebAssembly也用在网络和JavaScript环境之外（参考非网络嵌入）。

## 3. WebAssembly如何适应网络平台？

网络平台可以被想象成拥有两个部分：

* 一个运行网络程序（Web app）代码——比如，给你的程序提供能力的JavaScript——的虚拟机
* 一系列网络程序能够调用从而控制网络浏览器/设备功能并且能够让事物发生变化的网络API（DOM、CSSOM、WebGL、IndexedDB、Web Audio API等）。

从历史角度讲，虚拟机过去只能加载JavaScript。这对我们而言足够了，因为JavaScript足够强大从而能够解决人们在当今网络上遇到的绝大部分问题。尽管如此，当试图把JavaScript应用到诸如3D游戏、虚拟现实、增强现实、计算机视觉、图像/视频编辑以及大量的要求原生性能的其他领域的时候，我们遇到了性能问题。

而且，下载、解析以及编译巨大的JavaScript应用程序的成本是过高的。移动平台和其他资源受限平台进一步放大了这些性能瓶颈。

WebAssembly是一门不同于JavaScript的语言，但是，它不是用来取代JavaScript的。相反，它被设计为和JavaScript一起协同工作，从而使得网络开发者能够利用两种语言的优势：

* JavaScript是一门高级语言。对于写网络应用程序而言，它足够灵活且富有表达力。它有许多优势——它是动态类型的，不需要编译环节以及一个巨大的能够提供强大框架、库和其他工具的生态系统。
* WebAssembly是一门低级的类汇编语言。它有一种紧凑的二进制格式，使其能够以接近原生性能的速度运行, 并且为诸如C++和Rust等拥有低级的内存模型语言提供了一个编译目标以便它们能够在网络上运行。（注意，WebAssembly有一个在将来支持使用了垃圾回收内存模型的语言的高级目标。）

随着WebAssembly出现在了浏览器中，我们前面提到的虚拟机将会加载和运行两种类型的代码——JavaScript和WebAssembly。

不同类型的代码能够按照需要进行相互调用——WebAssembly的JavaScript API使用能够被正常调用的JavaScript函数, 封装了导出的WebAssembly代码, 并且WebAssembly代码能够导入和同步调用常规的JavaScript函数。事实上，WebAssembly代码的基本单元被称作一个模块并且WebAssembly的模块在很多方面都和ES2015的模块是等价的。

### 3.1 WebAssembly关键概念

为了理解WebAssembly如何在浏览器中运行，需要了解如何几个关键概念。所有这些概念都是一一映射到了WebAssembly的JavaScript API中。

* 模块：表示一个已经被浏览器编译为可执行机器码的WebAssembly二进制代码。一个模块是无状态的, 并且像一个二进制大对象（Blob）一样能够被缓存到IndexedDB中或者在windows和workers之间进行共享（通过postMessage()函数）。一个模块能够像一个ES2015的模块一样声明导入和导出。
* 内存：一个可变大小的ArrayBuffer。它包含了一个连续的字节数组, 并且WebAssembly的低级内存存取指令可对其进行读写操作。
* 表格：一个可变大小的包含引用类型（比如，函数）的带类型数组。它包含了不能作为原始字节存储在内存中的引用（为了安全和可移植性的原因）。
* 实例：一个模块及其在运行时使用的所有状态，包括内存、表格和一系列导入值。一个实例就像一个已经被加载到一个拥有一组特定导入的特定的全局变量的ES2015模块。

JavaScript API为开发者提供了创建模块、内存、表格和实例的能力。给定一个WebAssembly实例，JavaScript代码能够同步调用作为普通JavaScript函数暴露出来的导出代码。通过把JavaScript函数导入到WebAssembly实例中，任意的JavaScript函数都能被WebAssembly代码同步调用。

因为JavaScript能够完全控制WebAssembly代码如何下载、编译运行，所以，JavaScript开发者甚至可以把WebAssembly想象成一个高效地生成高性能函数的JavaScript特性。

将来，WebAssembly模块将会像ES2015模块那样加载（使用`<script type='module'>`)，这也就意味着JavaScript代码能够像轻松地使用一个ES2015模块那样来获取、编译和导入一个WebAssembly模块。

## 4. 如何在我的app中使用WebAssembly？

上面我们讨论了WebAssembly向网络平台增加的基本要素：代码的二进制格式和加载运行该二进制代码的API。现在让我们讨论如何在实践中使用这些基本要素。

WebAssembly生态系统处在初始阶段；更多的工具会毫无疑问得不断出现。当然，有两个主要的着手点：

* 使用Emscripten移植一个C/C++应用程序。
* 直接在汇编级别编写生成WebAssembly代码。

让我们讨论这几项：

### 4.1 从C/C++移植

Emscripten工具能够输入C/C++源代码然后将其编译为一个.wasm模块，必要的用来加载和运行该模块的JavaScript”胶水“代码以及一个用来展示代码运行结果的HTML文档。

![001.png](001.png)

简而言之，工作流程如下所示：

* Emscripten首先把C/C++提供给clang+LLVM——一个成熟的开源C/C++编译器工具链，比如，在OSX上是XCode的一部分。
* Emscripten将clang+LLVM编译的结果转换为一个.wasm二进制文件。
* 就自身而言，WebAssembly当前不能直接的存取DOM；它只能调用JavaScript，并且只能传入整形和浮点型的原始数据类型作为参数。这就是说，为了使用任何Web API，WebAssembly需要调用到JavaScript，然后由JavaScript调用Web API。因此，Emscripten创建了HTML和JavaScript胶水代码以便完成这些功能。

JavaScript胶水代码并不是像你想象的那么简单。首先，Emscripten实现了流行的C/C++库，比如，SDL、 OpenGL、 OpenAL以及部分POSIX。这些库以Web API的形式实现并且每个库需要一个JavaScript胶水代码来连接WebAssembly和低层的Web API。

因此，部分胶水代码实现了C/C++代码使用的库的功能。而且，胶水代码还包括调用前面提到的WebAssembly的JavaScript API来获取、加载和运行.wasm文件的逻辑。

生成的HTML文档加载JavaScript胶水文件, 并且将stdout输出到`<textarea>`。如果应用程序使用了OpenGL，HTML文档还会包含一个用来渲染目标的`<canvas>`标签。修改Emscripten的输出文件并将其转换为需要的web app是很容易的。

你可以在emscripten.org找到关于Emscripten的完整文档以及在[编译 C/C++ 为 WebAssembly](c-to-wasm.md)找到一个实现工具链并交叉编译你自己的C/C++应用程序为wasm的指南。

### 4.2 直接编写WebAssembly代码

你想构建自己的编译器或者你自己的工具或者创建一个能够在运行时生成WebAssembly代码的JavaScript库？

就像真实的汇编语言一样，WebAssembly的二进制格式也有文本表示——两者之间1:1对应。你可以手工书写或者生成这种格式然后使用这些工具（WebAssemby text-to-binary tools）中的任何一个把它转换为二进制格式。

这有一份如何做这些的简单指南，参考[将WebAssembly文本格式转换为wasm](text-to-wasm.md)。

## 5. 总结

本文给你解释了WebAssembly是什么，它为什么如此有用，它是如何适应网络的以及你如何才能使用它。