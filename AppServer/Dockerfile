FROM ubuntu

#RUN apt-get -y update

#RUN apt-get install -y cpp build-essential make cmake git libcurl3 libcurl4-openssl-dev

RUN mkdir -p /AppServer

COPY . /AppServer

#RUN cd AppServer; bash ./buildServer.sh

EXPOSE 8888

CMD AppServer/AppServer
