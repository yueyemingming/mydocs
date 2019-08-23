# mongo cpp driver 的基本应用

- mongocxx::instance  连接实例

每个程序一个instance即可。

```cpp
mongocxx::instance inst{} ;
```

- mongocxx::uri 连接地址

```cpp
mongocxx::uri uri{"mongodb://127.0.0.1:27017/"};                        //没有指定数据库名称

mongocxx::uri uri{"mongodb://user1:password1@127.0.0.1:27017/test"};    //带用户名密码时，貌似只能连接单台主机
string uri.password() == "password1";
string uri.username() == "user1";
string uri.database() == "test" ;

mongocxx::uri uri{"mongodb://127.0.0.1:27017/,192.168.1.3:27017/"};     //多台时，貌似不能指定用户名密码

vector<host> hosts == uri.hosts() ;

struct host {
    std::string name;       //127.0.0.1
    std::uint16_t port;     //27017
    std::int32_t family;    //不知道啥意思
};
```

- mongocxx::client 客户端

```cpp
mongocxx::client client{uri};
mongocxx::client client{mongocxx::uri{}};
mongocxx::client client{mongocxx::uri{"mongodb://127.0.0.1:27017"}};
```

- mongocxx::database 数据库

> 如果client的uri中没有指定数据库名，这里需要指定使用的数据库

```cpp
mongocxx::database db = client["test"];
```

- mongocxx::collection 集合

```cpp
mongocxx::collection col = db.collection("mycol");
mongocxx::collection col = client["test"].collection("mycol");

col.find...()
col.insert...()
col.write...()
col.drop...()
col.count...()
col.create_index*()
```

```cpp
//json std::string -> value
bsoncxx::document::value value = bsoncxx::from_json(bsoncxx::stdx::string_view{body});

// value -> view
bsoncxx::document::view view = value.view();

// view -> value
bsoncxx::document::value value = bsoncxx::document::value{ view };

// view[] -> eletment
bsoncxx::document::element element = view["act"];

// element -> sub_element
bsoncxx::document::element sub_element = element.get_document().value["account"];

// element -> string
std::string str = sub_element.get_utf8().value.to_string() ;
std::cout << str << std::endl ;

//std::cout << "key() = " << element.key() << std::endl ;

// view -> std::string
bsoncxx::to_json(view);

// value -> std::string
bsoncxx::to_json(value);

// document -> view
bsoncxx::document doc;
bsoncxx::document::view view= doc.view();
bsoncxx::document::view view(doc);

// document -> value
bsoncxx::document::value value = doc.extract();
bsoncxx::document::value value = make_document( kvp( "name", "fuck" ));
```