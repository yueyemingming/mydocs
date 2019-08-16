# 前后端交互基本示例

* 环境 : ubuntu 16.04
* 前端 : javascript+jquery+ajax
* 后端 : java
* 方式 : post

## 1. 前端

### 1.1 安装启动nginx

```bash
apt -y install nginx
/etc/init.d/nginx start
```

### 1.2 前端代码

直接在 nginx 服务主页里编写代码

```bash
vim /var/www/html/index.html
```

```html
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<title>前后端测试代码</title>
<script src="https://cdn.staticfile.org/jquery/1.10.2/jquery.min.js">   //JavaScript加载解析jquery及ajax的包
</script>
<script>
$(document).ready(function(){
    $("button").click(function(){
        $.post("http://192.168.0.125:8080/hello",          //这是发送数据的地址
        "name=aaaaaaaaaaaa",                               //这是发送的数据
        function(data,status){
            alert("数据: \n" + data + "\n状态: " + status); //接收数据的返回信息
        });
    });
});
</script>
</head>
<body>

<button>发送post信息</button>

</body>
</html>
```

## 2. 后端

### 2.1 软件依赖 请参照 [java springboot helloworld](../../java/springboot/01.helloworld/readme.md#1-软件依赖)

### 2.2 开发代码

#### 2.2.1 主程同 [java springboot helloworld](../../java/springboot/01.helloworld/readme.md#21-主程)

#### 2.2.2 HelloController类

```java
@RestController
public class HelloController {
    @RequestMapping(
            value = "/hello",
            method = RequestMethod.POST  //feign这里只支持GET方法
    )
    public String hello(@RequestParam("name") String name) {
        System.out.println(name);
        return "success";
    }
}
```