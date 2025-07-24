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
```
