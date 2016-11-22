FROM ubuntu

RUN apt-get update

RUN apt-get install -y gcc-4.9 g++-4.9 make cmake autoconf git wget libcurl3 libcurl4-openssl-dev

RUN mkdir -p /AppServer

WORKDIR /AppServer

COPY . /AppServer

RUN bash install_libs.sh

RUN bash buildServer.sh

EXPOSE 8888

#Para usar con el servicio de Firebase Cloud Messaging
#CMD ./bin/AppServer

#Para usar en modo test
CMD ["./bin/AppServer", "--test"]

#Pruebas unitarias
#CMD ./bin/UniTest
