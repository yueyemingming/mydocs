package mycloud.webclient;

import org.springframework.cloud.openfeign.EnableFeignClients;
import org.springframework.web.bind.annotation.*;

import javax.annotation.Resource;

@RestController
@EnableFeignClients
public class MyController
{
    @Resource
    private MyFeign myFeign;

    @RequestMapping(
            value = "/get",
            method = RequestMethod.GET
    )
    public String get() {
        return myFeign.get();
    }

    @RequestMapping(
            value = "/post",
            method = RequestMethod.GET
    )
    public String post() {
        return myFeign.post();
    }
}
