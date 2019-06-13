# my-graphene

```bash
docker run -itd --name node -p 31010:31010 -p 38090:38090 -p 38091:38091 -p 38092:38092 -v /etc/localtime:/etc/localtime:ro  ubuntu:16.04 /bin/bash

docker run -itd --name faucet -p 3000:3000 -p 3306:3306 -v /etc/localtime:/etc/localtime:ro ubuntu:16.04 /bin/bash

docker run -itd --name wallet -p 8080:8080 -v /etc/localtime:/etc/localtime:ro ubuntu:16.04 /bin/bash

apt update
apt -y install vim
vim /etc/apt/sources.list
apt update
```