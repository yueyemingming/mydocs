# 示例1 生产者和消费者

## 1. 生产者 Provider, 也就是服务端

### 1.1 pom.xml

* jpa支持

```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-web</artifactId>
</dependency>

<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-data-jpa</artifactId>
</dependency>
```

### 1.2 创建表的脚本 `schema.sql` 和 `data.sql`

* schema.sql 创建表的脚本

```sql
drop table if exists my_user;   /*由于idea ide编辑器的编码格式不同，请不要再任何表名和字段名上加引号，包括单引号和双引号*/
CREATE TABLE my_user (
  id int(11) NOT NULL,
  user_age int(11) NOT NULL,
  user_name varchar(255) DEFAULT NULL,
  PRIMARY KEY (id)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
```

* data.sql 插入表数据的脚本

```sql
INSERT INTO my_user VALUES ('1', '31', 'xiaoming1');
INSERT INTO my_user VALUES ('2', '32', 'xiaoming2');
INSERT INTO my_user VALUES ('3', '33', 'xiaoming3');
INSERT INTO my_user VALUES ('4', '34', 'xiaoming4');
```

### 1.3 application.yml

* 我这里把原来的 `application.properties` 修改为了 `application.yml`
* yml格式感觉更好看一点

```yml
server:
  port: 7900

spring:
  jpa:
    generate-ddl: false     # 这里很重要，不自动将 Entity 的类翻译成数据库表，而是使用创建表的脚本
    show-sql: true
    hibernate:
      ddl-auto: none

  datasource:
    url: jdbc:mysql://10.100.194.211:3306/test
    username: root
    password: 123456
    driver-class-name: com.mysql.cj.jdbc.Driver
    tomcat:
      max-active: 20
      max-idle: 8
      min-idle: 8
      initial-size: 10
    schema: classpath:schema.sql    #上面定义的创建表的脚本
    data: classpath:data.sql        #上面定义的插入表数据的脚本
    initialization-mode: always     #使用这个代表我要去调用创建表和插入数据的脚本
```

### 1.4 java代码

#### 1.4.1 MyUser.java

```java
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;

@Entity
public class MyUser {
    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    private int id ;
    private int userAge;
    private String userName;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getUserAge() {
        return userAge;
    }

    public void setUserAge(int userAge) {
        this.userAge = userAge;
    }

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }
}
```

#### 1.4.2 MyRepository.java

```java
@Repository
public interface MyRepository extends CrudRepository<MyUser, Integer> {
}
```

#### 1.4.3 MyController.java

```java
@RestController
@RequestMapping("/mycloud")
public class MyController {
    @Resource
    private MyRepository repo;

    /**
     * 这里直接跳过了service, control类直接调用了repository类。
     * {id} 这里的id是从浏览器地址里输入来获取参数。
     * @PathVariable 代表这个是从注解里获取参数
     * http://localhost:7900/mycloud/1
     */
    @RequestMapping("/{id}")
    public Optional<MyUser> findById(@PathVariable Integer id) {
        return repo.findById(id);
    }
}
```

## 2. 消费者 customer, 也就是客户端

### 2.1 pom.xml

* 消费者调用生产者，不需要数据库支持，这里只需要web支持即可。

```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-web</artifactId>
</dependency>
```

### 2.2 application.yml

* 我这里把原来的 `application.properties` 修改为了 `application.yml`
* yml格式感觉更好看一点

```yml
server:
  port: 7901
```

### 2.3 java代码

#### 2.3.1 MyUser.java

* 这里的MyUser只需要匹配Provider的Api，它在这里不是注解

```java
public class MyUser {
    private int id ;
    private int userAge;
    private String userName;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getUserAge() {
        return userAge;
    }

    public void setUserAge(int userAge) {
        this.userAge = userAge;
    }

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }
}
```

#### 2.3.2 MyController.java

```java
@RestController
@RequestMapping("/mycloud")
public class MyController {
    /**
     * 这里只是访问Provider的api服务，因此不需要Respository对象
     * 这里需要通过RestTemplate对象提供api访问服务。
     * 新版本springboot没有实现RestTemplate，这里需要自己实现
     */
    @Bean
    public RestTemplate restTemplate() {
        return new RestTemplate();
    }

    /**
     * 定制RestTemplate对象，来访问Provider提供的http api服务。
     */
    @Resource
    public RestTemplate restTemplate;

    /**
     * http://localhost:7901/mycloud/1
     */
    @RequestMapping("/{id}")
    public MyUser findById(@PathVariable Integer id) {
        return restTemplate.getForObject("http://localhost:7900/mycloud/" + id, MyUser.class ) ;
    }
}
```