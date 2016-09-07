# /usr/bin/env sh
mkdir build
cd build
cmake ..
make
mv AppServer ../AppServer
#rm -R build
