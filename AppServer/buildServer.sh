# /usr/bin/env sh
CORES=$(nproc --all)
CORES=$[$CORES + 1]
mkdir build
cd build
cmake ..
make -j$CORES
cp AppServer ../bin/appserver
mv AppServer ../AppServer
mv UnitTest ../UnitTest
cd ..
#rm -r ./build
