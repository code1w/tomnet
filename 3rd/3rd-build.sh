
#!/bin/bash
dir=$(cd -P -- "$(dirname -- "$0")" && pwd -P)
echo $dir
cd $dir
echo "shell build protobuf ...."
if [ ! -d "protobuf-3.12.0" ]; then
    tar zxf protobuf-all-3.12.0.tar.gz
fi
cd protobuf-3.12.0
./configure
make -j4

cd ..

echo "shell build jemalloc ...."
if [ ! -d "jemalloc-5.2.1" ]; then
    tar zxf jemalloc-5.2.1.tar.gz
fi
cd jemalloc-5.2.1
./autogen.sh
./configure --with-jemalloc-prefix=je_
make -j4




