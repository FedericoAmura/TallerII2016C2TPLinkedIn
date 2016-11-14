# /usr/bin/env sh
echo "----- Installing app server dependencies"
if [ $0 != ./install-dependencies.sh ]
then
	echo "AppServer/install-dependencies.sh: Ejecute este script desde su ubicación."
	exit 1
fi

sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
sudo apt-get update -q
sudo apt-get install gcc-4.9 g++-4.9 -y
sudo apt-get install cpp build-essential cmake git libcurl3 libcurl4-openssl-dev

if [ ! -d ./lib ]; then mkdir lib; fi

#Posible optimización: Checkear y compilar las librerias en paralelo. Problema: Se puede "perder" el output

#gmock y gtest
if [ ! -r ./lib/libgmock.a ] || [ ! -r ./lib/libgtest.a ]
then
	if [ ! -d ./gtestsrc ]; then mkdir gtestsrc; fi
	cd gtestsrc
	git clone --depth=1 https://github.com/google/googletest.git googletest
	cd googletest
	cmake .
	make
	mv ./googlemock/libgmock.a ../../lib/libgmock.a
	mv ./googlemock/gtest/libgtest.a ../../lib/libgtest.a
	cd ../..
	rm -f -r ./gtestsrc	
fi

#leveldb y memenv
if [ ! -r ./lib/libleveldb.a ] || [ ! -r ./lib/libmemenv.a ]
then
	if [ ! -d ./leveldbsrc ]; then mkdir leveldbsrc; fi
	cd leveldbsrc
	git clone --depth=1 https://github.com/google/leveldb.git leveldb
	cd leveldb
	make
	mv ./out-static/libleveldb.a ../../lib/libleveldb.a
	mv ./out-static/libmemenv.a ../../lib/libmemenv.a
	cd ../..
	rm -f -r ./leveldbsrc	
fi

#log4cpp
if [ ! -r ./lib/liblog4cpp.a ]
then
	if [ ! -d ./log4cppsrc ]; then mkdir log4cppsrc; fi
	cd log4cppsrc
	git clone --depth=1 git://git.code.sf.net/p/log4cpp/codegit log4cpp
	cd log4cpp
	./autogen.sh
	./configure --enable-doxyen=no
	make
	mv ./src/.libs/liblog4cpp.a ../../lib/liblog4cpp.a
	cd ../..
	rm -f -r ./log4cppsrc	
fi

#libjpeg
if [ ! -r ./lib/libjpeg.a ]  || [ ! -r ./include/jpeg/jconfig.h ]
then
	if [ ! -d ./srcjpeg ]; then mkdir srcjpeg; fi
	cd srcjpeg 
	wget http://www.ijg.org/files/jpegsrc.v9b.tar.gz
	tar -zxvf jpegsrc.v9b.tar.gz
	cd jpeg-9b
	./configure
	make
	mv .libs/libjpeg.a ../../lib/libjpeg.a
	mv jconfig.h ../../include/jpeg/jconfig.h
	cd ../..
	rm -f -r ./srcjpeg
fi


