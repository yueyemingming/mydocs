# git

<!-- TOC -->
- [git](#git)
  - [1. git配置](#1-git配置)
  - [2. 仓库](#2-仓库)
  - [3. 暂存区](#3-暂存区)
  - [4. 提取与提交](#4-提取与提交)
  - [5. git中文支持](#5-git中文支持)
<!-- /TOC -->

## 1. git配置

```bash
git config xxx.xxx xxx             #.git/config
git config --global xxx.xxx xxx    #~/.gitconfig
git config --system xxx.xxx xxx    #/etc/gitconfig

git config user.name "rui"                     #配置提交者信息
git config user.email yueyemingming@163.com    #配置提交者邮箱信息
git config core.editor vim                     #配置编辑器
git config merge.tool kdiff3                   #配置merge diff工具 kdiff3、opendiff、tkdiff、meld、xxdiff、emerge、vimdiff、gvimdiff等
```

## 2. 仓库

```bash
git init            #仓库创建

git clone ssh://[user@]host.xz[:port]/path/to/repo.git  #ssh协议仓库克隆
git clone [user@]host.xz[:port]/path/to/repo.git        #ssh协议仓库克隆
git clone git://host.xz[:port]/path/to/repo.git         #git协议仓库克隆
git clone http[s]://host.xz[:port]/path/to/repo.git     #http\https协议仓库克隆
git clone http[s]://user@host.xz[:port]/path/to/repo.git     #http\https协议仓库克隆, 指定用户名登录尤其在clone私有仓库时最重要
git clone ftp[s]://host.xz[:port]/path/to/repo.git      #ftp\ftps协议仓库克隆
git clone rsync://host.xz/path/to/repo.git              #rsync协议仓库克隆
git clone file:///path/to/repo.git                      #file协议仓库克隆
```

* **带user的会让输入密码**

## 3. 暂存区

```bash
git add file            #添加单个文件或文件夹到暂存区，支持sh通配符
git add .               #添加所有文件到暂存区
echo "zh" > .gitignore  #设置不需要添加到暂存区的内容
git rm file             #从暂存区中删除单个文件或文件夹
git mv file1 file2      #从暂存区中改名
git status              #查看暂存区状态
```

## 4. 提取与提交

```bash
git commit                #提交暂存区到本地仓库, 此命令会调用默认编辑器编辑提交信息
git commit -m "提交信息"   #提交暂存区到本地仓库, 引号内信息不能为空

git push                                   #提交本地仓库到远端仓库
git push --set-upstream origin liruigang   #上传某个分支
git pull                                   #从远端仓库拉取数据到本地仓库
git push origin <commit_id> --force        #强制回滚某个提交

git log                                    #查看提取与提交记录
git log --stat --summary                   #查看概要提取与提交记录

git show <commit_id>                       #查看一次修改内容
git show dfb02e6e4f2f7b573337763e5c0013802e392818
git show dfb02                             #一般只使用版本号的前几个字符即可
git show HEAD                              #显示当前分支的最新版本的更新细节
git show HEAD^                             #查看HEAD 的父版本更新细节
git show HEAD^^                            #查看HEAD 的祖父版本更新细节
git show HEAD~4                            #查看HEAD 的祖父之祖父的版本更新细节

git reset --hard HEAD                      #重置到HEAD
git reset --hard                           #重置到HEAD
git reset --hard <commit_id>               #重置到某提交
```

## 5. git中文支持

```bash
git config --global gui.encotding utf-8
git config --global i18n.commitencoding utf-8
git config --global i18n.logoutputencoding gbk
git config --global core.quotepath false

vim /etc/inputrc

set output-meta on
set convert-meta off
```
