# libcurl库的坑：Expect100-continue

在使用curl做POST的时候, 出现如下错误：

```txt
Host: kimnote.com
Accept: */*
Content-Length: 50
Content-Type: application/x-www-form-urlencoded
Expect: 100-continue
```

这是因为，当POST的数据大于1024字节的时候, curl并不会直接就发起POST请求, 而是会分为俩步:

* 发送一个请求, 包含一个Expect:100-continue, 询问Server使用愿意接受数据.
* 接收到Server返回的100-continue应答以后, 才把数据POST给Server.

解决办法如下：

```c++
    struct curl_slist * headers = 0;
    headers = curl_slist_append(headers, "Expect:");    // Disable "Expect: 100-continue"
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_slist_free_all(headers);
```
