# /usr/bin/env sh
if [ $0 != ./install-dependencies.sh ]
then
	echo "AppServer/install-dependencies.sh: Ejecute este script desde su ubicación."
	exit 1
fi

sudo apt-get install cpp build-essential cmake

if [ ! -d ./lib ]; then mkdir lib; fi

#gmock y gtest
if [ ! -r ./lib/libgmock.a ] || [ ! -r ./lib/libgtest.a ]
then
	sudo apt-get install unzip
	if [ ! -d ./libsrc ]; then mkdir libsrc; fi
	cd libsrc
	wget https://github.com/google/googletest/archive/master.zip
	unzip master.zip
	cd googletest-master
	cmake .
	make
	mv ./googlemock/libgmock.a ../../lib/libgmock.a
	mv ./googlemock/gtest/libgtest.a ../../lib/libgtest.a
	cd ../..
	rm -r ./libsrc	
fi
