# vim

* [vim命令快速参考.PDF](vim命令快速参考.PDF)

## 常用

```vim
set cindent
set autoindent
set hlsearch
set incsearch
```

## 窗口

```vim
:vsplit        水平分割窗口，新窗口文件为原文件
:vsplit a.txt  水平分割窗口，新窗口文件为a.txt
ctrl+w w       各个窗口中切换文件

:close         关闭当前窗口文件,但不退出vim
:only          关闭其他窗口文件
:new           分割形式新建文件
:new a.txt     分割形式新建文件a.txt
```

## vim tab设置为4个空格

```vim
set tabstop=4        "TAB的宽度被设置为4个空格。但仍然是\t. 只是vim把它解释成4个空格宽度，用别的编辑器还是\t符号
set shiftwidth=4     "缩进使用4个空格的宽度。
set softtabstop=4    "设置tab所占的列数，当输入tab时，设为4个空格的宽度。
"set expandtab        "扩展tab为空格。
```