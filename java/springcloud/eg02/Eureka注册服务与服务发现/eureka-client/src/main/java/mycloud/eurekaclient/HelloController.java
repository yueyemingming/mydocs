package mycloud.eurekaclient;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.cloud.netflix.eureka.EnableEurekaClient;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@RestController
@EnableEurekaClient
public class HelloController
{
     @Value("${spring.application.name}")
    private String serverName;

    @RequestMapping("/info")
    public String getInfo(@RequestParam String name) {
        return "Hello " + name + ", I am " + serverName ;
    }

    @RequestMapping("/hello")
    public String hello() {
        return "hello";
    }
}
