# /usr/bin/env sh
echo "----- Installing app server dependencies"
if [ $0 != ./install-dependencies.sh ]
then
	echo "AppServer/install-dependencies.sh: Ejecute este script desde su ubicación."
	exit 1
fi

sudo apt-get install cpp build-essential cmake unzip

if [ ! -d ./lib ]; then mkdir lib; fi

#Posible optimización: Checkear y compilar las librerias en paralelo. Problema: Se puede "perder" el output

#gmock y gtest
if [ ! -r ./lib/libgmock.a ] || [ ! -r ./lib/libgtest.a ]
then
	if [ ! -d ./gtestsrc ]; then mkdir gtestsrc; fi
	cd gtestsrc
	wget https://github.com/google/googletest/archive/master.zip
	unzip master.zip
	cd googletest-master
	cmake .
	make
	mv ./googlemock/libgmock.a ../../lib/libgmock.a
	mv ./googlemock/gtest/libgtest.a ../../lib/libgtest.a
	cd ../..
	rm -r ./gtestsrc	
fi

#leveldb y memenv
if [ ! -r ./lib/libleveldb.a ] || [ ! -r ./lib/libmemenv.a ]
then
	if [ ! -d ./leveldbsrc ]; then mkdir leveldbsrc; fi
	cd leveldbsrc
	wget https://github.com/google/leveldb/archive/master.zip
	unzip master.zip
	cd leveldb-master
	make
	mv ./out-static/libleveldb.a ../../lib/libleveldb.a
	mv ./out-static/libmemenv.a ../../lib/libmemenv.a
	cd ../..
	rm -r ./leveldbsrc	
fi
