#Jobify#

Aplicación similar a Linkedin, consta de un app server, un shared server, y un cliente Android.

##Android##

###Dependencias###

-gradle

###Compilación###

###Ejecucíón###

##App Server##

###Dependencias###

-cpp build-essential cmake
En carpeta /lib: libgtest.a libgmock.a libleveldb.a
Se provee install-dependencies.sh

###Compilación###

Se provee ./buildServer.sh.
Sino, ejecutar cmake seguido de make. 

###Ejecucíón###

Ejecutar AppServer

####Ejecución bajo docker####

###LevelDB###

Documentación de uso: https://rawgit.com/google/leveldb/master/doc/index.html

###Tests Unitarios###

Se utiliza google test.

Correr el ejecutable UnitTests, que se autogenera al compilar el proyecto.

Para documentación sobre uso de gTest mirar https://github.com/google/googletest/blob/master/googletest/docs/Primer.md

##Shared Server##

###Dependencias###

-nodejs npm postgresql-9.5
-grunt-cli bower express pg ejs

###Compilación###

###Ejecucíón###

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
