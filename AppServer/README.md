# Docker

## Generación de imagen Docker
```sh
$ docker build -t docker-appserver .
```

## Ejecución del AppServer en Docker
```sh
$ docker run -i -t -p 8888:8888 docker-appserver
```

# Instalación de la aplicación
## Generación de paquete
Se provee el siguiente script, buildPackage.sh, para la generación del paquete de instalación.
Primeramente se deberá proveer los permisos de ejecución.
```sh
$ chmod +x buildPackage.sh
```
Luego, ejecutar:
```sh
$ ./buildPackage.sh
```
Esto generará un paquete de instalación llamado AppServer_<versión>-Linux.deb en el directorio actual.

## Instalación de paquete
Ejecutar el siguiente comando:
```sh
$ sudo dpkg -i AppServer_<versión>-Linux.deb
```

## Ver archivos instalados por el paquete
```sh
$ sudo dpkg -L appserver
```

## Ejecución de la aplicación
Para ejecutar la aplicación, deberá ubicarse en el directorio donde se instaló la aplicación (/usr/appserver) y ejecutar el siguiente comando:
```sh
$ ./AppServer
```

## Eliminación de paquete
Eliminación no completa, se mantendrán todos los archivos de configuración, scripts, registros y otros datos
de usuario que gestiona el paquete.
```sh
$ sudo dpkg -r appserver
```
Para eliminar completamente todo lo asociado con el paquete instalado, utilice el
siguiente comando:
```sh
$ sudo dpkg -P appserver
```
