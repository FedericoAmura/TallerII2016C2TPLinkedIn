Docker
==================
Para llevar a cabo el despligue del servidor usando Docker, se enumeran los
siguiente pasos.

Contenidos:

.. contents::
   :local:

Instalación de Docker
^^^^^^^^^^^^^^^^^^^^^
Se deberá contar con la instalación de Docker. En caso de no tenerlo instalado,
se puede instalar siguiendo las instrucciones de instalación desde su página aqui.

Creación de imagen Docker
^^^^^^^^^^^^^^^^^^^^^^^^^
Una vez instalado Docker, deberá contar con el archivo Dockerfile (que contiene las
instrucciones para la creación de la imagen Docker del servidor, puede descargarlo
desde aqui) y los fuentes del proyecto y posicionarse en el mismo directorio donde se encuentre.
Para generar la imagen Docker del servidor, se deberá ejecutar el siguiente comando:

.. code-block:: bash

	docker build -t docker-appserver .

Corriendo el Servidor
^^^^^^^^^^^^^^^^^^^^^
Una vez creada la imagen Docker, se deberá correr dicha imagen utilizando el siguiente
comando:

.. code-block:: bash

 	docker run -i -t -p 8888:8888 docker-appserver

Los flags -i -t permiten ejecutar una terminal interactiva dentro del contenedor, con lo
cual si se desea detener el servidor puede ejecutar Ctrl + C y detenerlo.
