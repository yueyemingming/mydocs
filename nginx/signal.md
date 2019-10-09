# nginx信号控制

信号 | 解释
:--- | :---
TERM, INT | Quick shutdown
QUIT | Graceful shutdown  优雅的关闭进程,即等请求结束后再关闭
HUP | Configuration reload ,Start the new worker processes with a new configuration Gracefully shutdown the old worker processes<br>改变配置文件,平滑的重读配置文件
USR1 | Reopen the log files 重读日志,在日志按月/日分割时有用
USR2 | Upgrade Executable on the fly 平滑的升级
WINCH | Gracefully shutdown the worker processes 优雅关闭旧的进程(配合USR2来进行升级)

