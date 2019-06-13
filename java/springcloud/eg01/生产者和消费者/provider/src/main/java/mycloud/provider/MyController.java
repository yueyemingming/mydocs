package mycloud.provider;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import javax.annotation.Resource;
import java.util.Optional;

@RestController
@RequestMapping("/mycloud")
public class MyController {
    @Resource
    private MyRepository repo;

    @RequestMapping("/{id}")
    public Optional<MyUser> findById(@PathVariable Integer id) {
        return repo.findById(id);
    }
}
