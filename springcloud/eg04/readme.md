# 示例4 Ribbon客户端的负载均衡策略配置 -- 通过代码方式

![framework.png](framework.png)

1. `Web客户端` 和 `两台Web服务器` 的 `Euroka客户端` 和 `Ribbon客户端` 都分别注册到 `中心Euroka服务器` 上。
2. `Web客户端`访问`两台Web服务器`时，`Ribbon服务器`为它负载均衡。
3. 通过代码修改访问 `ribbon服务器`的负载均衡策略。

## 1. Eureka Server 端, 提供注册服务

* 请参照 [示例2 Eureka注册服务与服务发现 1. Eureka Server](../eg02/readme.md#1-Eureka-Server)

* `spring-cloud-starter-netflix-eureka-server`这个包中已经包含了ribbon服务包`spring-cloud-starter-netflix-ribbon` 。

## 2. 两台Web服务端

* 请参照 [示例3 Ribbon的基本使用 2. 两台Web服务端](../eg03/readme.md#2-两台Web服务端)

## 3. Web客户端

### 3.1 pom.xml

* 请参照 [示例3 Ribbon的基本使用 3.1 pom.xml](../eg03/readme.md#31-pomxml)

### 3.2 application.yml

* 请参照 [示例3 Ribbon的基本使用 3.2 application.xml](../eg03/readme.md#32-applicationyml)

### 3.3 MyController.java

```java
@RestController
@EnableEurekaClient
@RibbonClient(name = "webserver", configuration = MyConfiguration.class)   //这里使用到了ribbon的策略注解
public class MyController
{
    @Bean
    @LoadBalanced
    public RestTemplate restTemplate() {
        return new RestTemplate();
    }

    @Resource
    public RestTemplate restTemplate;

    @RequestMapping("/hello")
    public String hello() {
        return restTemplate.getForObject("http://webserver/hello", String.class);
    }
}
```

### 3.4 MyConfiguration.java

```java
@Configuration
public class MyConfiguration {  //策略类
    @Resource
    IClientConfig config;

    @Bean
    public IRule ribbonRule() {
        return new RandomRule();    //策略为随机方式
    }
}
}
```

### 3.5 访问

* 请参照 [示例3 Ribbon的基本使用 3.4 访问](../eg03/readme.md#34-访问)

### 3.5 验证结果

* 访问 <http://localhost:8080/hello>，并且不挺的刷新。随机出现返回 `hello from 'webserver: 8002'`和`hello from 'webserver: 8001'` 代表负载均衡已经成功。