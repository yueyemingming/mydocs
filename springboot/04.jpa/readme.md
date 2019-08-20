# jpa 数据库接口

## 1. JPA

* Java Persistence API, java描述对象---关系表的映射关系，将运行期的实体对象持久化到数据库中。
* 持久化就是指把数据放到外存或数据库等等持久存储
* JPA是java的持久化标准规范
* Hibernate是对象关系映射框架，对JDBC进行了轻量级对象封装，是全自动的orm框架
* orm就是对象-关系映射。也可以简单理解为程序对象映射或者是转换到数据库表中
* spring data简便访问数据库的接口框架
* JPA是一种规范，Hibernate是它的一种实现。除了Hibernate，还有其他实现，如EclipseLink、OpenJPA等。

## 2. 示例步骤

1. pom.xml添加mysql, spring-data-jpa依赖
2. 在application.properties文件中配置mysql连接配置文件
3. 在application.properties文件中配置JPA配置信息
4. 编写测试例子

## 3. 软件依赖

### 3.1 配置 pom.xml , 添加mysql, spring-data-jpa依赖

```xml
<dependency>
    <groupId>mysql</groupId>
    <artifactId>mysql-connector-java</artifactId>
</dependency>

<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-data-jpa</artifactId>
</dependency>
```

### 3.2 配置 application.properties 文件

#### 3.2.1 配置mysql连接配置文件

```ini
########################################################
###datasource -- 指定mysql连接信息
########################################################
spring.datasource.url = jdbc:mysql://10.100.194.211:3306/test
spring.datasource.username = root
spring.datasource.password = 123456
spring.datasource.driverClassName = com.mysql.cj.jdbc.Driver
spring.datasource.max-active=20
spring.datasource.max-idle=8
spring.datasource.min-idle=8
spring.datasource.initial-size=10
```

#### 3.2.2 JPA配置信息

```ini
########################################################
### Java Persistence Api --  Spring jpa的配置信息
########################################################
# Specify the DBMS
spring.jpa.database = MYSQL
# Show or not log for each sql query
spring.jpa.show-sql = true
# Hibernate ddl auto (create, create-drop, update)
spring.jpa.hibernate.ddl-auto = update
# Naming strategy
#[org.hibernate.cfg.ImprovedNamingStrategy  #org.hibernate.cfg.DefaultNamingStrategy] 下面这句话说明了程序中的对象自动创建对应的表结构
spring.jpa.hibernate.naming-strategy = org.hibernate.cfg.ImprovedNamingStrategy
# stripped before adding them to the entity manager)
spring.jpa.properties.hibernate.dialect = org.hibernate.dialect.MySQL5Dialect
```

## 4. 编写测试例子

### 4.1 示例1 添加\删除\查询所有

1. MyUser.java
2. MyRepository.java
3. MyService.java
4. MyController.java

#### 4.1.1 MyUser.java

```java
import javax.persistence.Entity;        //注意这个导入，org.hibernate.annotations.Entity 这个已经丢弃。
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;

@Entity     //JPA检测实体类中的@Entity注解时，会在数据库中生成对应的表结构，以此达到持久化的目的
public class MyUser {   //根据大小写拆分下划线： MyUser 对应的表名为 my_user

    /**
     * @Id 这个注解用来指定主键，这个注解必须要添加
     * @GenerateValue 这个注解用来指定主键策略，默认是自增
     */
    @Id @GeneratedValue(strategy = GenerationType.AUTO)
    private int id;
    private String userName;   //根据大小写拆分下划线： userName 对应的表列名为 user_name
    private int userAge;       //根据大小写拆分下划线： userAge 对应的表列名为 user_age

    public int getId() { return id; }
    public String getUserName() {
        return userName;
    }
    public int getUserAge() {
        return userAge;
    }

    public void setId(int id) {
        this.id = id;
    }
    public void setUserName(String userName) {
        this.userName = userName;
    }
    public void setUserAge(int userAge) {
        this.userAge = userAge;
    }
}
```

#### 4.1.2 MyRepository.java

```java
/**
 * 这是个接口，不是类。
 * CrudRepository 已经帮助实现了 添加\删除\查询所有 这几个功能。
 * 实际上，可以通过 @RepositoryDefinition 来代替这个即可
 */
public interface MyRepository extends CrudRepository<MyUser, Integer> {
    //这里没有自定义一些方法，而是使用了CrudRepository的默认的方法
    //这里也可以自己定义一些方法
}
```

#### 4.1.3 MyService.java

```java
@Service      //这个注解代表这里是个服务类
public class MyService {

    //这个接口可以通过 @RepositoryDefinition 来代替
    @Resource    //这个注解代表引入资源，这里引入了Repository接口
    private MyRepository repo;

    @Transactional  //这个注解用来进行事务绑定
    public void save(MyUser user) {
        repo.save(user);
    }

    @Transactional
    public void delete(int id) {
        repo.deleteById(id);
    }

    //查询不是事务，不需要 @Transactional 注解
    public Iterable<MyUser> getAll() {
        return repo.findAll();
    }
}
```

#### 4.1.4 MyController.java

```java
@RestController
@RequestMapping("/eg1")
public class MyController {

    @Resource    //这个注解代表引入资源，这里引入了服务
    private MyService svc;

    @RequestMapping("/save")
    public String save() {
        MyUser user = new MyUser();
        user.setName("xiaoming");
        user.setAge(3);
        svc.save(user);
        return "save xiaoming ok .";
    }

    @RequestMapping("/delete")
    public String delete() {
        svc.delete(1);
        return "delete xiaoming ok";
    }

    @RequestMapping("/getAll")
    public Iterable<MyUser> getAll() {
        return svc.getAll();
    }
}
```

### 4.2 示例2 带条件查询

1. MyUser.java [代码请见4.1.1 MyUser.java](#3511-MyUserjava)
2. MyRepository.java
3. MyService.java
4. MyController.java

#### 4.2.1 MyRepository.java

```java
/**
 * 这里直接继承 Repository接口，它是个完全接口类，没实现任何操作。
 * 大部分项目都不会直接继承这个接口，而是继承已经实现了部分功能的子接口，比如
 * CrudRepository, PagingAndSortingRepository等。
 * CrudRepository 实现了基本的添加删除查找
 * PagingAndSortingRepository 还实现了排序和分页功能，具体看源代码
 * MyRepository接口 自己实现了一个条件查询功能
 */
public interface MyRepository extends Repository<MyUser, Integer> {
    public MyUser findByUserName(String userName);
}
```

#### 4.2.2 MyService.java

```java
@Service
public class MyService {
    @Resource
    private MyRepository repo;

    public JPAUser findByUserName(String userName) {
        return repo.findByUserName(userName);
    }
}
```

#### 4.2.3 修改MyController.java

```java
@RestController
@RequestMapping("/jpa2")
public class MyController {

    @Resource
    private MyService svc;

    /**
    * 查询方法 以 get | find | read 开头.
    * 查询连接：http://localhost:8080/jpa2/findByUserName?userName=xiaoming
    * userName对应这里的函数参数 String userName
    * 涉及查询条件时，条件的属性用条件关键字连接，要注意的是条件属性以首字母大写。
    */
    @RequestMapping("/findByUserName")
    public JPAUser findByUserName(String userName) {
        return svc.findByUserName(userName);
    }
}
```

### 4.3 示例3 JPQL带条件查询

1. MyUser.java [代码请见4.1.1 MyUser.java](#3511-MyUserjava)
2. MyRepository.java
3. MyService.java
4. MyController.java

#### 4.3.1 MyRepository.java

```java
public interface MyRepository extends Repository<MyUser, Integer> {
    /**
     * 如何编写JPQL语句
     * JPQL与Hibernate的HQL语句类型
     */
    @Query("from MyUser where userName=:cn")
    MyUser findByUserName(@Param("cn") String userName);
}
```

#### 4.3.2 MyService.java

```java
@Service
public class MyService {
    @Resource
    private MyRepository repo;

    public JPAUser findByUserName(String userName) {
        return repo.findByUserName(userName);
    }
}
```

#### 4.4 MyController.java

```java
@RestController
@RequestMapping("/jpa3")
public class MyController {

    @Resource
    private MyService svc;

    /**
    * 查询方法 以 get | find | read 开头.
    * 查询连接：http://localhost:8080/jpa2/findByUserName?userName=xiaoming
    * userName对应这里的函数参数 String userName
    * 涉及查询条件时，条件的属性用条件关键字连接，要注意的是条件属性以首字母大写。
    */
    @RequestMapping("/findByUserName")
    public JPAUser findByUserName(String userName) {
        return svc.findByUserName(userName);
    }
}
```