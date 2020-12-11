# staticmethod,classmethod

## staticmethod 绑定到类

```python
class Fruit:
    price = 10
    color = "red"
    def __init__(self):
        pass

    # 方式1，装饰符方式实现
    @staticmethod
    def getPrice():
        print(Fruit.price)

    # 方式2，函数方式实现
    def __getColor():
        print(Fruit.color)
    getColor = staticmethod(__getColor)

# 类调用
Fruit.getPrice()    # 调用方式1
Fruit.getColor()    # 调用方式2
```

## classmethod 绑定到实例

```python
class Fruit:
    price = 10
    color = "red"
    def __init__(self):
        pass

    # 方式1，装饰符方式实现
    @ classmethod
    def getPrice(self):
        print(self.price)

    # 方式2，函数方式实现
    def __getColor(self):
        print(self.color)
    getColor = classmethod(__getColor)

# 类调用
Fruit.getPrice()    # 调用方式1
Fruit.getColor()    # 调用方式1

# 实例调用
apple = Fruit()
apple.getPrice()  # 调用方式1
apple.getColor()  # 调用方式2
```