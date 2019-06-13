# java

* [安装](install.md)

* **java程序的运行**
  
  ```bash
  java -jar xxx.jar
  ```

* [JNI技术总结](jni/readme.md)
* [NDK技术总结](ndk/readme.md)
* **maven工程打包**

  ```bash
  cd 项目跟目录（和pom.xml同级）
  mvn clean package
  ## 或者执行下面的命令
  ## 排除测试代码后进行打包
  mvn clean package  -Dmaven.test.skip=true
  ```

* [spring boot](springboot/readme.md)
* [spring cloud](springcloud/readme.md)
