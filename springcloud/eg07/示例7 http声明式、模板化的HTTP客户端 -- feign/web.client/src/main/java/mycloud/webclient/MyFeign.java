package mycloud.webclient;

import org.springframework.cloud.openfeign.FeignClient;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;

@FeignClient("webserver")
public interface MyFeign {

    @RequestMapping(
            value = "/hello",
            method = RequestMethod.GET
    )
    String get();

    @RequestMapping(
            value = "/hello",
            method = RequestMethod.POST
    )
    String post();
}
