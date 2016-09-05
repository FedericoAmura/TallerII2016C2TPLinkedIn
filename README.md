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

###Compilación###

###Ejecucíón###

####Ejecución bajo docker####

##Shared Server##

###Dependencias###

-nodejs npm postgresql-9.5
-grunt-cli bower express pg ejs

###Compilación###

###Ejecucíón###

####Ejecución bajo docker####

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

