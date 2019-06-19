# repeated限定修饰符

## 限定修饰符说明

repeated 代表可重复，我们可以理解为数组：

## proto

```proto
syntax = "proto3";      //指定版本信息，不指定会报错

message Person          //message为关键字，作用为定义一种消息类型
{
    string name = 1;    //姓名
    int32 id = 2;       //id
    string email = 3;   //邮件
}

message AddressBook
{
    repeated Person people = 1;
}
```

## 字段API

而对于字段修饰符为repeated的字段生成的函数，则稍微有一些不同，如people字段，则编译器会为其产生如下的代码：

```cpp
int people_size() const;
void clear_people();
const ::Person& people(int index) const;
::Person* mutable_people(int index);
::Person* add_people();
::google::protobuf::RepeatedPtrField< ::Person >* mutable_people();
const ::google::protobuf::RepeatedPtrField< ::Person >& people() const;
```

## 测试程序

```cpp
void set_addressbook()
{
    AddressBook obj;

    Person *p1 = obj.add_people(); //新增加一个Person
    p1->set_name("mike");
    p1->set_id(1);
    p1->set_email("mike@qq.com");

    Person *p2 = obj.add_people(); //新增加一个Person
    p2->set_name("jiang");
    p2->set_id(2);
    p2->set_email("jiang@qq.com");

    Person *p3 = obj.add_people(); //新增加一个Person
    p3->set_name("abc");
    p3->set_id(3);
    p3->set_email("abc@qq.com");

    fstream output("pb.xxx", ios::out | ios::trunc | ios::binary);

    bool flag = obj.SerializeToOstream(&output);//序列化
    if (!flag)
    {
        cerr << "Failed to write file." << endl;
        return;
    }

    output.close();//关闭文件
}

void get_addressbook()
{
    AddressBook obj;
    fstream input("./pb.xxx", ios::in | ios::binary);
    obj.ParseFromIstream(&input);  //反序列化
    input.close(); //关闭文件

    for (int i = 0; i < obj.people_size(); i++)
    {
        const Person& person = obj.people(i);//取第i个people
        cout << "第" << i + 1 << "个信息\n";
        cout << "name = " << person.name() << endl;
        cout << "id = " << person.id() << endl;
        cout << "email = " << person.email() << endl << endl;
    }
}
```