# overload, overwrite, override

- overload —— **重载**：类中的同名函数
  - 相同的范围（在一个类中）
  - 函数名字相同
  - 参数不同
  - virtual可有可无
- overwrite —— **覆盖**：派生类函数覆盖基类函数
  - 不同范围（分别位于派生类与基类）
  - 函数名字相同
  - 参数相同
  - 基类函数必须virtual
- override —— **重定义**：（派生类与基类）
  - 不同范围（分别位于派生类和基类）
  - 函数名字相同
  - 参数相同时，无virtual
  - 参数不同时，virtual可有可无