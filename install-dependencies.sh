# /usr/bin/env sh
cd AppServer
./install-dependencies.sh
cd ..
./ClientApp/install-dependencies.sh
./SharedServer1/install-dependencies.sh
./Tester/install-dependencies.sh
