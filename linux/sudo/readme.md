# 解决sudo时的“sudo: unable to resolve host xxxx”的问题

```bash
sudo vim /etc/hosts
```

```text
127.0.0.1       localhost xxxx   #这后面添上这个主机名就行
```