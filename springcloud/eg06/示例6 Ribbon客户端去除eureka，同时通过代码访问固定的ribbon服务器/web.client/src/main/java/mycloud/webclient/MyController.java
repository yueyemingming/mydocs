package mycloud.webclient;

import org.springframework.cloud.client.ServiceInstance;
import org.springframework.cloud.client.loadbalancer.LoadBalanced;
import org.springframework.cloud.client.loadbalancer.LoadBalancerClient;
import org.springframework.context.annotation.Bean;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.client.RestTemplate;

import javax.annotation.Resource;

@RestController
public class MyController
{
    @Bean
    public RestTemplate restTemplate() {
        return new RestTemplate();
    }

    @Resource
    private RestTemplate restTemplate;

    @RequestMapping("/hello")
    public String hello() {
        return restTemplate.getForObject("http://webserver/hello", String.class);
    }

    @Resource
    private LoadBalancerClient lbc ;

    @RequestMapping("api")
    public String api() {
        ServiceInstance si = this.lbc.choose("webserver");
        System.out.println("-----------------------------------------------------");
        String address = "http://" + si.getHost() + ":" + si.getPort() + "/hello";
        System.out.println(address);
        System.out.println("-----------------------------------------------------");
        return restTemplate.getForObject(address, String.class);
    }
}
