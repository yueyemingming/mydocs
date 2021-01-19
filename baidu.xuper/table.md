# table智能合约

> protobuf 版本注意只能使用 3.7.1， fuck， 坑。

```bash
#生成协议源文件
cd src/
protoc --cpp_out=. mytable.proto

cd ..
xdev build


xchain-cli wasm deploy --account XC1234567890111111@xuper --cname mytable -a '{"id": "1", "name":"myname", "desc":"mydesc"}' --fee 5000000 --runtime c ../example/mytable/mytable.wasm

xchain-cli wasm invoke --fee 11000 mytable --method get -a '{"name":"myname"}'
xchain-cli wasm invoke --fee 11000 mytable --method scan -a '{"id": "1", "name":"myname"}'
xchain-cli wasm invoke --fee 11000 mytable --method set -a '{"id": "1", "name":"myname", "desc":"mydesc"}'
xchain-cli wasm invoke --fee 11000 mytable --method set -a '{"id": "2", "name":"myname2", "desc":"mydesc2"}'
xchain-cli wasm invoke --fee 11000 mytable --method del -a '{"id": "1", "name":"myname", "desc":"mydesc"}'
xchain-cli wasm invoke --fee 11000 mytable --method del -a '{"id": "2", "name":"myname2", "desc":"mydesc2"}'

xchain-cli wasm invoke --fee 11000 mytable --method size
xchain-cli wasm invoke --fee 11000 mytable --method clear



xchain-cli wasm query mytable --method get  -a '{"name":"myname"}'
xchain-cli wasm query mytable --method scan -a '{"id": "1", "name":"myname"}'
xchain-cli wasm query mytable --method set -a '{"id": "2", "name":"myname2", "desc":"mydesc2"}'
xchain-cli wasm query mytable --method set -a '{"id": "1", "name":"myname", "desc":"mydesc"}'
xchain-cli wasm query mytable --method del -a '{"id": "1", "name":"myname", "desc":"mydesc"}'

xchain-cli wasm upgrade --account XC1234567890111111@xuper --cname mytable --fee 5000000 ../example/mytable/mytable.wasm

xchain-cli wasm invoke --method get -a '{"address":"myaddress"}' --fee 11000 mytable
xchain-cli wasm invoke --method add -a '{"id": "1", "address":"myaddress"}' --fee 11000 mytable

```
