
# 配置系统包

```bash
apt update

apt -y install vim git net-tools wget unzip automake mysql-server libmysqlclient-dev curl bzip2 make build-essential openssl libreadline-dev autoconf libtool doxygen libc++-dev cmake g++ libssl-dev pkg-config
apt -y mysql-server libmysqlclient-dev

apt remove -y libreoffice* rhythmbox* mpv smplayer smplayer-l10n smplayer-themes remmina remmina-common remmina-plugin-rdp remmina-plugin-vnc imagemagick imagemagick-6.q16 imagemagick-common xserver-xorg-input-wacom simple-scan transmission-common transmission-gtk ubuntu-kylin-software-center youker-assistant unity-scope-virtualbox unity-scope-calculator unity-scope-chromiumbookmarks unity-scope-colourlovers unity-scope-devhelp unity-scope-texdoc unity-scope-tomboy unity-scope-video-remote unity-scope-zotero unity-scope-gdrive unity-scope-firefoxbookmarks unity-scope-manpages unity-scope-openclipart unity-scope-yelp  chromium-browser chromium-browser-l10n chromium-codecs-ffmpeg-extra thunderbird thunderbird-gnome-support thunderbird-locale-zh-cn thunderbird-locale-zh-hans aisleriot checkbox-converged checkbox-gui plainbox-provider-checkbox python3-checkbox-support shotwell shotwell-common cheese cheese-common libcheese-gtk25 libcheese8 gnome-mines youker-assistant indicator-china-weather libgweather-3-6 libgweather-common gnome-mahjongg chinese-calendar
apt autoremove
```