# 安装 google-chrome

```bash
dpkg -i google-chrome-stable_current_amd64.deb
vim /usr/bin/google

#exec -a "$0" "$HERE/chrome" "$@"
exec -a "$0" "$HERE/chrome" "$@" --user-data-dir  --no-sandbox

```