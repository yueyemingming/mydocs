# source, sh, exec, .号的区别

命令 | 含义
:--- | :---
source和.号 | shell中使用source run.sh，是直接运行run.sh的命令，不创建子shell，类似与html中include。  .号跟source基本相同，用来引用其他文件(一般定义了一些function，和变量)，跟source基本相同。
sh | sh是则创建子shell，子shell里面 的变量父shell无法使用，对环境变量的修改也不影响父shell。父shell中的局部变量，子shell也无法使用，只有父shell的环境变量， 子shell能够使用。
exec | 还有一个exec run.sh，这个与source类似，区别是exec执行完，不再执行后面的语句。
