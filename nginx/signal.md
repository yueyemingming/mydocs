# nginx信号控制

信号 | 解释
:--- | :---
TERM, INT | Quick shutdown
QUIT | 优雅的关闭进程,即等请求结束后再关闭<br>Graceful shutdown
HUP | 改变配置文件,平滑的重读配置文件<br>Configuration reload ,Start the new worker processes with a new configuration Gracefully shutdown the old worker processes
USR1 | 重读日志,在日志按月/日分割时有用<br>Reopen the log files
USR2 | 平滑的升级<br>Upgrade Executable on the fly
WINCH | 优雅关闭旧的进程(配合USR2来进行升级)<br>Gracefully shutdown the worker processes

