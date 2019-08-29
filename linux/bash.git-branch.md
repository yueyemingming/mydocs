# 让bash显示git分支

```sh
vim ~/.bashrc
```

```sh
function git-branch-name {
  git symbolic-ref --short -q HEAD 2>/dev/null
}
function git-branch-prompt {
  local branch=`git-branch-name`
  if [ $branch ]; then printf "(%s)" $branch; fi
}
PS1="\u@\h:\[\033[0;36m\]\w\[\033[0m\]\[\033[0;32m\]\$(git-branch-prompt)\[\033[0m\]# "
```
