# /usr/bin/env sh
CORES=$(nproc --all)
CORES=$[$CORES + 1]
mkdir build
mkdir bin
cd build
cmake ..
make -j$CORES
mv AppServer ../bin/AppServer
mv UnitTest ../bin/UnitTest
cp ../default.jpg ../bin
cd ..
#rm -r ./build
