# /usr/bin/env sh
echo "----- Installing project dependencies"
./StyleCheck/install-dependencies.sh
./Tester/install-dependencies.sh
./SharedServer/install-dependencies.sh
cd AppServer
./install-dependencies.sh
cd ..
./ClientApp/install-dependencies.sh
