# /usr/bin/env sh
echo "----- Installing project dependencies"
cd AppServer
./install-dependencies.sh
cd ..
./ClientApp/install-dependencies.sh
./SharedServer/install-dependencies.sh
./Tester/install-dependencies.sh
./StyleCheck/install-dependencies.sh
