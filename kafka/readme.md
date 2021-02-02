# kafka

- [linux下直接安装](linux.kafka.install.md)
- [docker安装kafka](docker.kafka.install.md)
- 概念
  - topic，主题，相关联的数据主题
  - partition，分区，将数据离散的分散在各个分区里，保证数据不丢失，对应文件系统的文件夹，可以配置每个partition的数据存储量大小。
  - segment，段，在分区里，对应分区里的各个文件。可配置每个segment的数据存储量大小。
- 数据清除规则——**清除数据时，通过直接删除符合条件的segment的文件。**
  - 根据时间：可以配置数据的保存时间，超过即清除
  - 根据存储量：可以配置partition的存储量大小，超过即清除。
