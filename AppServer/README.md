## Docker

#### 1. Generación de imagen Docker
```sh
$ docker build -t docker-appserver .
```

#### 2. Ejecución del AppServer en Docker
```sh
$ docker run -i -t -p 8888:8888 docker-appserver
```
