# 03 fastjson带时间示例

## 1. 软件依赖 请参照 [02 fastjson](../02.fastjson/readme.md#1-软件依赖)

## 2 开发代码

### 2.1 主程同 [01 hello world 2.1 主程](../01.helloworld/readme.md#21-主程)

### 2.2 FastJsonController类

```java
class FastJsonUser {
    private String name;

    @JSONField(format = "yyyy-MM-dd HH:mm")     //这个注解代表输出的时间格式
    private Date birthday;

    @JSONField(serialize = false)               //这个注解代表不序列化输出remark字段
    private String remark;

    public String getName() { return name; }
    public String getRemark() { return remark; }
    public Date getBirthday() { return birthday;}

    public void setName(String name) { this.name = name; }
    public void setBirthday(Date birthday) { this.birthday = birthday; }
    public void setRemark(String remark) { this.remark = remark; }
}

@RestController
public class FastJsonController {

    @RequestMapping("/fastjson")        //这个注解代表网址映射：http://127.0.0.1:8080/fastjson
    @ResponseBody
    public FastJsonUser getUser() {
        FastJsonUser user = new FastJsonUser();
        user.setName("fast json");
        user.setBirthday(new Date());
        user.setRemark("remark info");
        return user;
    }
}
```

网页结果：

```json
{ "birthday":"2019-02-12 20:36", "name":"fast json" }
```

### 2.3 也可以在main类中，添加如下代码，也可以转换时间显示格式

```java
@Bean               //这个注解代表这是是Bean
public HttpMessageConverters fastJsonConfigure() {
    FastJsonHttpMessageConverter converter = new FastJsonHttpMessageConverter();
    FastJsonConfig fastJsonConfig = new FastJsonConfig();
    fastJsonConfig.setSerializerFeatures(SerializerFeature.PrettyFormat);
    //日期格式化
    //fastJsonConfig.setDateFormat("yyyy-MM-dd HH:mm");     //这里去掉了秒的格式
    converter.setFastJsonConfig(fastJsonConfig);
    return new HttpMessageConverters(converter);
}
```
