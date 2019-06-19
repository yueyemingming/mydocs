# git push 传文件时出错505的原因之一

## 错误

![505.png](505.png)

可能的一个原因是本地git缓存过小。

## 解决办法

加大本地缓存

```bash
git config --global http.postbuffer 524288000   #500M
```
