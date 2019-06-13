package com.example.demo;

import feign.Contract;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class MyConfiguration1 {
    @Bean
    public Contract feignContract() {
        return new feign.Contract.Default();
    }
}
