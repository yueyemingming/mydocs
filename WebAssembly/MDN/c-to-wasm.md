# 编译 C/C++ 为 WebAssembly

当你在用C/C++之类的语言编写模块时，你可以使用Emscripten来将它编译到WebAssembly。让我们来看看它是如何工作的。

## 1. Emscripten 环境安装

首先，让我们来配置所需要的开发环境。

### 1.1 所需条件

```bash
apt -y install python2.7 nodejs cmake default-jre git-core

git clone https://github.com/juj/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest

source ./emsdk_env.sh --build=Release
```

> 注意: 每当您想要使用Emscripten时，尝试从远程更新最新的emscripten代码是个很好的习惯（运行 git pull）。如果有更新，重新执行 install 和 activate 命令。这样就可以确保您使用的Emscripten一直保持最新。

## 2. 编译样例代码

现在环境配置完毕，让我们看看如何使用它把C代码编译到Emscripten。当使用Emscripten来编译的时候有很多种不同的选择，我们介绍其中主要的2种：

* 编译到 wasm 并且生成一个用来运行我们代码的HTML，将所有 wasm 在web环境下运行所需要的 “胶水” JavaScript代码都添加进去。
* 编译到 wasm 然后仅仅生成 JavaScript。

让我们一个一个看看。

### 2.1 生成 HTML 和 JavaScript

我们先来看一个最简单的例子，通过这个，你可以使用Emscripten来将任何代码生成到WebAssembly，然后在浏览器上运行。

```c++
#include <stdio.h>

int main(int argc, char ** argv) {
  printf("Hello World\n");
}
```

```bash
emcc hello.c -s WASM=1 -o hello.html
```

下面列出了我们命令中选项的细节：

* -s WASM=1 — 指定我们想要的wasm输出形式。如果我们不指定这个选项，Emscripten默认将只会生成asm.js。
* -o hello.html — 指定这个选项将会生成HTML页面来运行我们的代码，并且会生成wasm模块以及编译和实例化wasm模块所需要的“胶水”js代码，这样我们就可以直接在web环境中使用了。

这个时候在您的源码文件夹应该有下列文件:

* hello.wasm   二进制的wasm模块代码
* hello.js     一个包含了用来在原生C函数和JavaScript/wasm之间转换的胶水代码的JavaScript文件
* hello.html   一个用来加载，编译，实例化你的wasm代码并且将它输出在浏览器显示上的一个HTML文件

### 2.2 运行你的例子

现在使用一个支持 WebAssembly 的浏览器加载生成的 hello.html 。

如果一切顺利，你应该可以在页面上的 Emscripten 控制台和浏览器控制台中看到 "Hello World" 的输出。恭喜！你已经成功将 C 代码编译成 JavaScript 并且在浏览器中执行了！

### 3. 使用自定义HTML模板

有些时候你可能想要使用一个自定义的 HTML 模板。让我们看看怎么实现。

1. 首先，在一个新文件夹中保存以下 C 代码到 hello.c 中：

```c++
#include <stdio.h>

int main(int argc, char ** argv) {
    printf("Hello World\n");
}
```

```bash
mkdir html_template
cp ~/emsdk/emscripten/1.38.15/src/shell_minimal.html html_template
emcc -o hello.html hello.c -O3 -s WASM=1 --shell-file html_template/shell_minimal.html
```

* 我们使用了 -o hello.html ，这意味编译器将仍然输出 js 胶水代码 和 html 文件。
* 我们还使用了 --shell-file html_template/shell_minimal.html ，这指定了您要运行的例子使用 HTML 页面模板。

下面让我们来运行这个例子。上面的命令已经生成了 hello.html，内容和我们使用的模板非常相像，只不过多加了一些 js 胶水和加载wasm文件的代码。 在浏览器中打开它，你会看到与上一个例子相同的输出。

注意：通过用.js取代.htm(l)作为文件后缀名，你就可以得到只有JavaScript的输出文件，而不再是完整的HTML文件。例如：`emcc -o hello.js hello.c -O3 -s WASM=1`. 你可以完全从零开始创建你自己的HTML文件。尽管如此，不推荐这样做。因为Emscripten需要大量的JavaScript“胶水”代码从而能够 处理内存分配、内存泄漏以及大量的其他问题。这些问题都已经在提供的模板中得到了处理。使用模板要比自己编写模板要容易得多。不过，当对模板所做的事情越来越熟悉的时候，你就能够按照自己的需要创建定制化的模板了。

### 4. 调用一个定义在C中的自定义方法

如果需要按需调用一个在 C 语言文件中定义的函数，你可以使用  Emscripten 中的 ccall() 函数以及 EMSCRIPTEN_KEEPALIVE 声明 （将你的函数添加到导出的函数列表中。接下来让我们看看这是怎么实现的。

首先，将以下代码在新目录中保存为 hello.c ：

```c++
#include <stdio.h>
#include <emscripten/emscripten.h>

int main(int argc, char ** argv) {
    printf("Hello World\n");
}

#ifdef __cplusplus
extern "C" {
#endif

int EMSCRIPTEN_KEEPALIVE myFunction(int argc, char ** argv) {
  printf("我的函数已被调用\n");
}

#ifdef __cplusplus
}
#endif
```

默认情况下，Emscripten 生成的代码只会调用 main() 函数，其它的函数将被视为无用代码。在一个函数名之前添加 EMSCRIPTEN_KEEPALIVE 能够防止这样的事情发生。你需要导入 emscripten.h 库来使用 EMSCRIPTEN_KEEPALIVE。

注意: 为了保证万一你想在 C++ 代码中引用这些代码时代码可以正常工作，我们添加了 #ifdef 代码块。由于 C 与 C++ 中名字修饰规则的差异，添加的代码块有可能产生问题，但目前我们设置了这一额外的代码块以保证你使用 C++ 时，这些代码会被视为外部 C 语言函数。

为了方便起见，现在将 html_template/shell_minimal.html 也添加到这一目录（在实际开发环境中你肯定需要将其放到某一特定位置）。

现在使用你的Emscripten编译器环境的终端窗口进入你的新目录， 运行以下命令编译你的 C 语言代码：

```bash
emcc -o hello.html hello.c -O3 -s WASM=1 --shell-file html_template/shell_minimal.html
```

如果你在浏览器中在此加载实例，你将看到和之前相同的结果。

现在我们需要运行新的 myFunction() JavaScript 函数。首先，按照以下实例添加一个`<button>` ，就在 `<script type='text/javascript'>` 开头标签之前。

```html
<button class="mybutton">运行我的函数</button>
```

现在在最后一个 `<script>` 元素 （就在 `</script>` 关闭标签之前）中添加以下代码：

```js
document.querySelector('.mybutton').addEventListener('click', function(){
  alert('检查控制台');
  var result = Module.ccall('myFunction', // name of C function
                             null, // return type
                             null, // argument types
                             null); // arguments
});
```

以上就是如何使用 ccall() 调用导出的函数的方式。