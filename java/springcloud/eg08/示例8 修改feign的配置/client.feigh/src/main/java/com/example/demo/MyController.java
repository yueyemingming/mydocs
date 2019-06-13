package com.example.demo;

import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import javax.annotation.Resource;

@RestController
public class MyController {
    @Resource
    private MyFeign1 myFeign1;

    @RequestMapping("/test1")
    public String test1() {
        return myFeign1.test1();
    }

    @Resource
    private MyFeign2 myFeign2;

    @RequestMapping("/test2")
    public String test2() {
        return myFeign2.test2();
    }
}
