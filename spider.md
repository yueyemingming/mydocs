# 爬虫基本代码

## 01. 需要的包

```python
import requests
from lxml import etree
```

## 02. 爬页解码

```python
header ={'User-Agent':'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/40.0.2214.115 Safari/537.36'}
doc = requests.get('http://.../',headers=header)    # 网站拒绝请求是加上header
```

## 03. 解编码

```python
print(doc.encoding)         # 查看网页的编码格式，实际上这个是不准确的
page = doc.content.decode('gbk', errors='ignore') # 在确定已知编码的情况下，ignore强制忽略错误
```

## 04. 解析网页

```python
html = etree.HTML(path)     # 在网页中解析网页
# html = etree.fromstring(str)     #从字符串中解码网页
```

## 05. 解析网页内容

```python
# 提取属性
propety = doc.xpath('//span/@class')
print(propety)

# 提取筛选后的内容
value = doc.xpath('//span[@class="rating_nums"]/text()')    
print(value)
```

## 06. 解析json

```python
import json
j=doc.json()
print(j['server_version'])
print(j['chain_id'])
print(json.dumps(j,indent=4))   # 格式化对齐
```
