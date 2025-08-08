```
cd ${HOME}
mkdir git
cd git
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./vcpkg install protobuf
./vcpkg install grpc
export VCPKG_ROOT=$HOME/git/vcpkg
export VCPKG_DEFAULT_TRIPLET=x64-linux
export CMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=$HOME/git/vcpkg/scripts/buildsystems/vcpkg.cmake .
make
```

## problem

Segmantation Fault

```
~/git/generic_record_server/build$ ./rpc_server
Server listening on 0.0.0.0:50051

~/git/generic_record_server/build$ ./plugin_loader libplugin_api.so
[tsurugi_create_generic_client_factory] Creating factory for service: Greeter
[tsurugi_create_generic_client_factory] Returning rpc_client_factory
[PluginLoader] Factory created: 0x596030104ad0
[main] Factory created: 0x596030104ad0
[rpc_client] RPC client initialized
[rpc_client] Greeter stub initialized
[rpc_client] Byer stub initialized
[main] SayHello connect
[rpc_client] call function_index: 0, 0
[rpc_client] case 0 arg1: hello
Segmentation fault (core dumped)
```

OK

```
~/git/generic_record_server/build$ ./rpc_server
Server listening on 0.0.0.0:50051

~/git/generic_record_server/build$ ./rpc_client
[tsurugi_create_generic_client_factory] Creating factory for service: Greeter
[tsurugi_create_generic_client_factory] Returning rpc_client_factory
[rpc_client] RPC client initialized
[rpc_client] Greeter stub initialized
[rpc_client] Byer stub initialized
SayHello connect
[rpc_client] call function_index: 0, 0
[rpc_client] case 0 arg1: hello
Greeter received: Hello hello
AddIntOne connect
[rpc_client] call function_index: 0, 1
[rpc_client] case 1 arg1: 42
Greeter received: 43
SayWolrd connect
[rpc_client] call function_index: 0, 2
[rpc_client] case 2 arg1: world
Greeter received: World world
DecDecimal connect
[rpc_client] call function_index: 0, 3
[rpc_client] case 3 arg1: Dec
[rpc_client] case 3 arg2: 3
Greeter received: 0

~/git/generic_record_server/build$ ./rpc_server
Server listening on 0.0.0.0:50051
Dec 3
```

OK
