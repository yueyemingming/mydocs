# helloworld示例

## 1. 写代码

`vim hello.c`

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
  printf("myFunction\n");
}

#ifdef __cplusplus
}
#endif
```

默认情况下，Emscripten 生成的代码只会调用 main() 函数，其它的函数将被视为无用代码。在一个函数名之前添加 EMSCRIPTEN_KEEPALIVE 能够防止这样的事情发生。你需要导入 emscripten.h 库来使用 EMSCRIPTEN_KEEPALIVE。

> 注意: 为了保证万一你想在 C++ 代码中引用这些代码时代码可以正常工作，我们添加了 #ifdef 代码块。由于 C 与 C++ 中名字修饰规则的差异，添加的代码块有可能产生问题，但目前我们设置了这一额外的代码块以保证你使用 C++ 时，这些代码会被视为外部 C 语言函数。

## 2. 编译

```bash
mkdir html_template
cp ~/emsdk/emscripten/1.38.15/src/shell_minimal.html html_template
emcc -o hello.html hello.c -O3 -s WASM=1 --shell-file html_template/shell_minimal.html
```

## 3. 编辑hello.html

`vim hello.html`

### 3.1 添加一个button

首先，按照以下实例添加一个 `<button>` ，就在 `<script type='text/javascript'>` 开头标签之前。

```html
<button class="mybutton">run my function</button>
```

### 3.2 调用

现在在最后一个 `<script>` 元素 （就在 </script> 关闭标签之前）中添加以下代码：

```html
document.querySelector('.mybutton').addEventListener('click', function(){
  alert('检查控制台');
  var result = Module.ccall('myFunction', // name of C function
                             null, // return type
                             null, // argument types
                             null); // arguments
});
```

## 4. 运行

在当前目录下启动http服务器

```bash
python -m SimpleHTTPServer 9000     # python的http服务器,其他服务也行
```

在浏览器中打开localhost:9000，按F12打开控制台，查看结果.
