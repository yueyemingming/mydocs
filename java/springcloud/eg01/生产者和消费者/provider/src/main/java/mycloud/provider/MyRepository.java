package mycloud.provider;

import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface MyRepository extends CrudRepository<MyUser, Integer> {
}
