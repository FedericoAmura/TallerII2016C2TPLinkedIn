# /usr/bin/env sh
mkdir build
cd build
cmake ..
make
mv AppServer ../AppServer
mv UnitTest ../UnitTest
cd ..
#rm -r ./build
