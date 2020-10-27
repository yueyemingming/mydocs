# Dockerfile CMD与RUN的区别

dockerfile    ----------->     镜像image    -------------->    容器container

- 从Dockerfile到镜像阶段：RUN
- 从镜像到容器阶段：CMD, 有且仅有1次调用   (CMD可以有多个，单只有最后一个生效)