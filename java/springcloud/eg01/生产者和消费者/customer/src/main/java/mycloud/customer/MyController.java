package mycloud.customer;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.client.RestTemplate;

import javax.annotation.Resource;

@RestController
@RequestMapping("/mycloud")
public class MyController {

    @Bean
    public RestTemplate restTemplate() {
        return new RestTemplate();
    }

    @Resource
    public RestTemplate restTemplate;

    @Value("${userServicePath")
    private String userServicePath;

    @RequestMapping("/{id}")
    public MyUser findById(@PathVariable Integer id) {
        return restTemplate.getForObject( userServicePath + id, MyUser.class ) ;
    }
}
