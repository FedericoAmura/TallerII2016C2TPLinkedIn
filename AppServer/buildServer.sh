# /usr/bin/env sh
CORES=$(nproc --all)
CORES=$[$CORES + 1]
echo $CORES
mkdir build
cd build
cmake ..
make -j$CORES
mv AppServer ../AppServer
mv UnitTest ../UnitTest
cd ..
#rm -r ./build
