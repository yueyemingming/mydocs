# ubuntu 安装nodejs

## 1. 安装

## 1.1 直接nodejs包安装方式

```bash
wget http://cdn.npm.taobao.org/dist/node/v10.0.0/node-v10.0.0-linux-x64.tar.gz
tar xzvf node-v10.0.0-linux-x64.tar.gz -c /opt
echo 'export PATH="/opt/node-v10.0.0-linux-x64/bin:$PATH"' >> ~/.bashrc
source ~/.bashrc
```

### 1.2 ubuntu apt 安装方式

安装说明链接
<https://nodejs.org/en/download/package-manager/#debian-and-ubuntu-based-linux-distributions>

```bash
curl -sL https://deb.nodesource.com/setup_10.x | sudo -E bash -
apt -y install nodejs
```

### 1.3 通过nvm安装方式

```bash
curl -o- https://raw.githubusercontent.com/creationix/nvm/v0.30.2/install.sh | bash
source ~/.bashrc
nvm install v6   #安装某个版本
nvm use v6       # 切换到某个版本
```

## 2. 设置安装源

### 2.1 命令设置安装源

```bash
npm config set registry https://registry.npm.taobao.org #设置淘宝源
npm config set registry https://registry.cnpmjs.org     #或者设置cnpm源
```

### 2.2 手动设置安装源

```bash
vim ~/.npmrc
------------------------------------------------------------
registry=https://registry.npm.taobao.org
sass_binary_site=https://npm.taobao.org/mirrors/node-sass/
phantomjs_cdnurl=http://npm.taobao.org/mirrors/phantomjs
ELECTRON_MIRROR=http://npm.taobao.org/mirrors/electron/
------------------------------------------------------------
```

## 3. 使用

```bash
node -v #查看nodejs版本
npm -v  #查看npm版本

#安装依赖包
npm install       #安装package.json所有依赖包到node_modules目录
npm install -g    #安装package.json所有依赖包到系统目录

npm install <package>       #安装单个依赖包到node_modules目录
npm install -g <package>    #安装单个依赖包到系统目录

npm i -g yarn  #安装yarn
yarn           #用yarn安装package.json所有依赖包到node_modules目录

#删除重装依赖包
rm -rf node_modules/
npm cache clean
```
