FROM ubuntu

RUN apt-get update

RUN apt-get install -y libcurl3 libcurl4-openssl-dev

RUN mkdir -p /AppServer/bin

WORKDIR /AppServer/bin

COPY . /AppServer

EXPOSE 8888

CMD ./appserver
