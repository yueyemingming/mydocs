# github

## 1. 免密push

```bash
git config --global credential.helper store
```

提交时第一次仍需要输入密码，后面再git push就不需要输入密码了。

git bash上，如果输入错误，windows也会保存凭证，此时只能不使用凭证，重新输入。

```
git config --system --unset credential.helper
```


## 2. 子模块

### 2.1 下载所有子模块

```bash
git submodule update --init --recursive
```

### 2.2 添加

```bash
git submodule add git://github.com/test.git test
```

* 查看变化

```bash
git status

# On branch master
# Changes to be committed:
#   (use "git reset HEAD <file>..." to unstage)
#
#      new file:   .gitmodules
#      new file:   test
#
```

```bash
cat .gitmodules

[submodule "test"]
path = test
url = git://github.com/test.git
```

```bash
cat .git/modules/test/HEAD

bd792d02c70e7686da2b27197eba4fd6df30477c    #这是某次提交
```

### 2.3 管理

* 如果子模块内容是空，初始化其本地配置文件。

```bash
git submodule init
```

* 如果子模块不为空，只拉取当前子模块本身数据

```bash
git pull
```

* 如果子模块不为空，从子模块拉取所有下层子模块数据

```bash
git submodule update
```

### 2.4 修改子模块

子模块可以切换到某次提交 或 跟踪某个分支，或者一些其他修改，修改后需要更新父模块的最当前子模块的引用。此处是更新到某次提交的修改及父模块的引用。

#### 2.4.1 进入子模块

```bash
cd test
```

#### 2.4.2 切换子模块到某一次提交

```bash
git reset --hard bd792d02c70e7686da2b27197eba4fd6df30477c
```

#### 2.4.3 在父模块中查看模块变化

```bash
cd ..
git diff
```

```java
diff --git a/test b/test
index 2ebfb2d..bd792d0 160000
--- a/test
+++ b/test
@@ -1 +1 @@
-Subproject commit 2ebfb2db2bf7225c50d55541a516c1f789d05ca8
+Subproject commit bd792d02c70e7686da2b27197eba4fd6df30477c
```

此处发现子模块应用的提交id发生了变化。

#### 2.4.4 提交

```bash
git add .
git commit -m "... submodule test"
```

#### 2.4.5 上传

```bash
#提交本地仓库到远端仓库
git push

#上传某个分支
git push --set-upstream origin liruigang

#强制远程回滚某个提交
git push origin <commit_id> --force
```

### 2.5 删除子模块

```bash
git rm --cached test
```
