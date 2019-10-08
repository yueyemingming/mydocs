# 终端默认使用zsh

## 安装

```bash
apt -y install zsh fonts-powerline

sh -c "$(curl -fsSL https://raw.github.com/robbyrussell/oh-my-zsh/master/tools/install.sh)"
sh -c "$(wget https://raw.github.com/robbyrussell/oh-my-zsh/master/tools/install.sh -O -)"
```

## 修改为 agnoster

vim ~/.zshrc

! [agnoster](agnoster.png)

## zsh设置为系统默认终端

```bash
chsh -s `which zsh`
```

## 解决终端乱码

出现终端乱码的原因，大部分时因为字体设置问题，只要修改字体即可。

> 如果终端出现乱码，请将终端的字体更改为 "Meslo for Powerline" .
> 如果vscode中出现乱码，请将vscode的终端字体更改为 "Meslo for Powerline" .

! [vscode](vscode.png)


