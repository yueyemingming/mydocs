# Dockerfile CMD与ENTRYPOINT的区别

CMD 与 ENTRYPOINT 都是 Dockerfile 中执行最后一条命令的命令。

> **CMD相当于docker run时，命令行传递过来的参数。**

但

- docker run命令替代
  - CMD指定的命令可以被 docker run 命令行的命令所代替。
  - ENTRYPOINT 不可以。始终运行 ENTRYPOINT 指定的命令。如果想要覆盖ENTRYPOINT命令，需要再 docker run 命令行 执行参数 --entrypoint "xxxx"。
- 解析shell变量
  - 并且它指定的命令只有带有解析器 "/bin/sh" "-c" 时，shell变量才能被解析。
  - 并且ENTRYPOINT指定的命令，可以包含shell变量，可以被解析。



CMD与ENTRYPOINT结合使用的时候，用于为ENTRYPOINT提供参数

```dockerfile
CMD ["param1", "param2"]
ENTRYPOINT /bin/sh -c
```

>  **docker run命令行的上指定的命令，相当于时 /bin/sh -c 的参数。此时会覆盖掉 CMD 传过来的参数**



```dockerfile
ENTRYPOINT ["/bin/sh", "-c"]  # 这种是运行 /bin/sh -c 这个命令本身，而不是运行后面的命令。
```

