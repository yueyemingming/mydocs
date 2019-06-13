package com.example.demo;

import feign.RequestLine;
import org.springframework.cloud.openfeign.FeignClient;
import org.springframework.web.bind.annotation.RequestMapping;

@FeignClient(name = "x", url = "http://localhost:8000/", configuration = MyConfiguration2.class )
public interface MyFeign2 {
    @RequestLine("GET /eureka/apps/web.ribbon")
    public String test2();
}
