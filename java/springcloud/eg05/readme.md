# 示例5 Ribbon客户端的负载均衡策略配置 -- 通过配置文件方式

![framework.png](framework.png)

1. `Web客户端` 和 `两台Web服务器` 的 `Euroka客户端` 和 `Ribbon客户端` 都分别注册到 `中心Euroka服务器` 上。
2. `Web客户端`访问`两台Web服务器`时，`Ribbon服务器`为它负载均衡。
3. 通过配置文件修改访问 `ribbon服务器`的负载均衡策略。

## 1. Eureka Server 端, 提供注册服务

* 请参照 [示例2 Eureka注册服务与服务发现 1. Eureka Server](../eg02/readme.md#1-Eureka-Server)

## 2. 两台Web服务端

* 请参照 [示例3 Ribbon的基本使用 2. 两台Web服务端](../eg03/readme.md#2-两台Web服务端)

## 3. Web客户端

### 3.1 pom.xml

* 请参照 [示例3 Ribbon的基本使用 3.1 pom.xml](../eg03/readme.md#31-pomxml)

### 3.2 application.yml

```yml
eureka:
  client:
    service-url:
      defaultZone: http://localhost:8761/eureka/

spring:
  application:
    name: webclient

webserver:    #这里定义的eureka的ribbon服务器的名称
  ribbon:
    NFLoadBalancerRuleClassName : com.netflix.loadbalancer.RandomRule
```

### 3.3 MyController.java

* 请参照 [示例3 Ribbon的基本使用 3.3 MyController.xml](../eg03/readme.md#33-MyControllerjava)

### 3.4 访问

* 请参照 [示例3 Ribbon的基本使用 3.4 访问](../eg03/readme.md#34-访问)

### 3.5 验证结果

* 访问 <http://localhost:8080/hello>，并且不挺的刷新。随机出现返回 `hello from 'webserver: 8002'`和`hello from 'webserver: 8001'` 代表负载均衡已经成功。