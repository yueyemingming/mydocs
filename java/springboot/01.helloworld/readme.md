# 01 hello world 示例

## 1. 软件依赖

### 1.1 pom.xml中设置parent，表示对项目使用了springboot。

```xml
<parent>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-parent</artifactId>
    <version>2.1.2.RELEASE</version>
    <relativePath/> <!-- lookup parent from repo -->
</parent>
```

### 1.2 pom.xml中设置java及spring framework版本

```xml
<properties>
    <java.version>1.8</java.version>
    <springframework.version>5.1.4.RELEASE</springframework.version>
</properties>
```

### 1.3 pom.xml中为web添加启动依赖

```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-web</artifactId>
</dependency>
```

### 1.4 pom.xml中添加web依赖

```xml
<dependency>
    <groupId>org.springframework</groupId>
    <artifactId>spring-web</artifactId>
    <version>${springframework.version}</version>
</dependency>
```

### 1.5 pom.xml中设置为maven编译插件

```xml
<build>
    <plugins>

        <plugin>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-maven-plugin</artifactId>
        </plugin>

    </plugins>
</build>
```

## 2 开发代码

### 2.1 主程

```java
@SpringBootApplication      //这个注解代表当前项目是springboot项目
public class HelloApplication {

    public static void main(String[] args) {
        SpringApplication.run(HelloApplication.class, args);
    }
}
```

### 2.2 HelloController类

```java
@RestController                     //这个注解代表这个类是个Controller类。
public class HelloController {

    //这部分也可以直接放到main函数的类中
    @RequestMapping("/hello")       //这个注解代表网址映射：http://127.0.0.1:8080/hello
    public String Hello() {
        System.out.println("hello world 测试代码"); //web终端上能看到这个日志打印输出
        return "hello world";
    }
}
```