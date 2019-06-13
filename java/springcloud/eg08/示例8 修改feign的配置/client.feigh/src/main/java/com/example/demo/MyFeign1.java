package com.example.demo;

import feign.RequestLine;
import org.springframework.cloud.openfeign.FeignClient;

@FeignClient( name = "web.ribbon", configuration = MyConfiguration1.class )
public interface MyFeign1 {
    @RequestLine("GET /hello")
    String test1();

    @RequestLine("GET /web.ribbon")
    String test2();
}
