package mycloud.ribbonconfig;

import com.netflix.client.config.IClientConfig;
import com.netflix.loadbalancer.IRule;
import com.netflix.loadbalancer.RandomRule;
import org.springframework.cloud.netflix.ribbon.RibbonClient;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import javax.annotation.Resource;

@Configuration
public class MyConfiguration {
    @Resource
    IClientConfig config;

    @Bean
    public IRule ribbonRule() {
        return new RandomRule();
    }
}
