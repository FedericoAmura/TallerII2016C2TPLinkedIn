#Jobify#

Aplicación similar a Linkedin, consta de un app server, un shared server, y un cliente Android.

##Android##

###Dependencias###

-gradle

###Compilación###

###Ejecución###

##App Server##

###Dependencias###

-cpp build-essential cmake libcurl3 libcurl4-openssl-dev

En /lib: libleveldb.a liblog4cpp.a libcpr.a

Para tests, en /lib: libgtest.a libgmock.a

Se provee install-dependencies.sh

###Compilación###

Se provee ./buildServer.sh.

Sino, ejecutar cmake seguido de make.

###Parámetros de ejecución###

TODO: Nivel de log, archivos de log o consola

####Ejecución bajo docker####

###LevelDB###

Documentación de uso: https://rawgit.com/google/leveldb/master/doc/index.html

###Log4cpp###

Documentación de uso: https://log4cpp.sourceforge.net/

###JSON11###

Documentación de uso: https://github.com/dropbox/json11

###CPR: C++ Requests (Curl for People)###

Documentación de uso: https://github.com/whoshuu/cpr

###Tests Unitarios###

Se utiliza google test.

Correr el ejecutable UnitTests, que se autogenera al compilar el proyecto.

Documentación de uso: https://github.com/google/googletest/blob/master/googletest/docs/Primer.md

####Code Coverage####

Se provee el script AppServer/coverage.sh que al ser ejecutado corre los tests y deja el output en html en AppServer/coverage. 

##Shared Server##

###Dependencias###

-nodejs npm postgresql-9.5
-grunt-cli bower express pg ejs

###Compilación###

###Ejecución###

####Ejecución bajo docker####

##Estilo de código##

Consultar https://github.com/google/styleguide

Para c++ se provee un script que ejecuta cpplint sobre el código c++.

##Documentación Sphinx##

###Dependencias###

javasphinx

###Compilación###

Via make, con el makefile en /Docs, varios targets posibles.
Se provee el buildhtml.sh.

###Referencia de syntaxis para rst###

Syntaxis general: Consultar http://www.sphinx-doc.org/en/stable/rest.html 

Syntaxis para dominio c++: Consultar http://www.sphinx-doc.org/en/stable/domains.html#id2

Syntaxis para dominio java: Consultar https://bronto.github.io/javasphinx/

Syntaxis para dominio js: Consultar http://www.sphinx-doc.org/en/stable/domains.html#the-javascript-domain

##Integración Continua##

Travis CI: https://travis-ci.org/FedericoAmura/TallerII2016C2TPLinkedIn

Documentación de Travis: https://docs.travis-ci.com/
