# nginx系统变量

变量 | 解释
:--- | :---
is_args | 如果有args参数，这个变量等于"?"，否则等于""，空值。
arg_name | 请求中的的参数名，即"?"后面的arg_name=arg_value形式的arg_name
arg_PARAMETER | 这个变量包含GET请求中，如果有变量PARAMETER时的值。
args | 这个变量等于请求行中(GET请求)的参数，如：foo=123&bar=blahblah;
binary_remote_addr | 客户端地址的二进制形式, 固定长度为4个字节。
body_bytes_sent | 响应时送出的body字节数数量。即使连接中断，这个数据也是精确的。
bytes_sent | 传输给客户端的字节数
connection | TCP连接的序列号
connection_requests | TCP连接当前的请求数量
content_length | 请求头中的Content-length字段。
content_type | 请求头中的Content-Type字段。
cookie_name | cookie名称
cookie_COOKIE | cookie COOKIE变量的值
url | 求中的当前URI(不带请求参数，参数位于$args)，可以不同于浏览器传递的$request_uri的值，它可以通过内部重定向，或者使用index指令进行修改，$uri不包含主机名，如”/foo/bar.html”。/stat.php (对比request_uri)
document_root | 当前请求在root指令中指定的值
document_uri | 与uri相同。
host | 请求主机头字段，否则为服务器名称。
hostname | 主机名
http_name | 匹配任意请求头字段；变量名中的后半部分"name"可以替换成任意请求头字段，如在配置文件中需要获取http请求头："Accept-Language"，那么将"－"替换为下划线，大写字母替换为小写，形如：$http_accept_language即可。
https | 如果开启了SSL安全模式，值为"on"，否则为空字符串。
http_user_agent | 客户端agent信息
http_cookie | 客户端cookie信息
limit_rate | 用于设置响应的速度限制
msec | 当前的Unix时间戳
nginx_version | nginx版本
pid | 工作进程的PID
pipe | 如果请求来自管道通信，值为"p"，否则为"."
proxy_protocol_addr | 获取代理访问服务器的客户端地址，如果是直接访问，该值为空字符串
realpath_root | 当前请求的文档根目录或别名的真实路径，会将所有符号连接转换为真实路径。
query_string | 与args相同。
remote_addr | 客户端的IP地址。
remote_port | 客户端的端口。
remote_user | 已经经过HTTP基础认证服务的用户名。
request | 代表客户端的请求地址
request_body_file | 客户端请求主体信息的临时文件名。
request_method | 客户端请求的动作，通常为GET或POST。
request_completion | 如果请求结束，设置为OK. 当请求未结束或如果该请求不是请求链串的最后一个时，为空(Empty)。
request_method | GET或POST
request_length | 请求的长度 (包括请求的地址, http请求头和请求主体)
request_time | 处理客户端请求使用的时间; 从读取客户端的第一个字节开始计时。
request_filename | 当前请求的文件路径，由root或alias指令与URI请求生成。
request_uri | 包含请求参数的原始URI，不包含主机名，如："/foo/bar.php?arg=baz"。不能修改。/stat.php?id=1585378&web_id=1585378
scheme | 请求使用的Web协议, “http” 或 “https”。
sent_http_name | 可以设置任意http响应头字段； 变量名中的后半部分“name”可以替换成任意响应头字段，如需要设置响应头Content-length，那么将“－”替换为下划线，大写字母替换为小写，形如：$sent_http_content_length 4096即可。
server_protocol | 服务器的HTTP版本, 通常为 “HTTP/1.0” 或 “HTTP/1.1”。
server_addr | 服务器地址，在完成一次系统调用后可以确定这个值, 需要注意的是：为了避免访问linux系统内核，应将ip地址提前设置在配置文件中。
server_name | 服务器名称, www.cnphp.info。
server_port | 请求到达服务器的端口号。
status | HTTP响应代码
tcpinfo_rtt, $tcpinfo_rttvar, $tcpinfo_snd_cwnd, $tcpinfo_rcv_space | 客户端TCP连接的具体信息
time_local | 服务器时间（LOG Format 格式）
time_iso8601 | 服务器时间的ISO 8610格式
