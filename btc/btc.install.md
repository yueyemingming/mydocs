# bitcoin节点安装

```bash
apt -y install build-essential libtool autotools-dev automake pkg-config libssl-dev libevent-dev bsdmainutils python3 libboost-system-dev libboost-filesystem-dev libboost-chrono-dev libboost-program-options-dev libboost-test-dev libboost-thread-dev libboost-all-dev

apt -y install software-properties-common
add-apt-repository ppa:bitcoin/bitcoin
apt update
apt -y install libdb4.8-dev libdb4.8++-dev libminiupnpc-dev libzmq3-dev libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools libprotobuf-dev protobuf-compiler libqrencode-dev

git clone https://github.com/rui-space/bitcoin


cd bitcoin
./autogen.sh
./configure
make -4
make install
```