package mycloud.webserver;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class MyController {
    @Value("${server.port}")
    private String port;

    @RequestMapping("/hello")
    public String hello() {
        return "hello from 'webserver: " + port + "'";
    }
}
