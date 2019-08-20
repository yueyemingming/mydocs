# 02 fastjson示例

程序基本构建，请参见 [01 hello world](../01.helloworld/readme.md)

## 1. 软件依赖

### 1.1 pom.xml中添加fastjson依赖

```xml
<dependency>
    <groupId>com.alibaba</groupId>
    <artifactId>fastjson</artifactId>
    <version>1.2.46</version>
</dependency>
```

## 2 开发代码

### 2.1 主程同 [01 hello world 2.1 主程](../01.helloworld/readme.md#21-主程)

### 2.2 JsonController类

```java
class JsonUser {
    private int id;
    private String name;

    public int getId() { return id; }
    public String getName() { return name; }

    public void setId(int id) { this.id = id; }
    public void setName(String name) { this.name = name; }
}

@RestController
public class JsonController {
    @RequestMapping("/json")                //这个注解代表网址映射：http://127.0.0.1:8080/json
    public JsonUser getUser() {
        JsonUser user = new JsonUser();
        user.setId(1);
        user.setName("json");
        return user;
    }
}
```

网页结果：

```json
{ "id":1, "name":"json" }
```
