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
se puede instalar siguiendo las instrucciones de instalación desde su página
`aqui <https://docs.docker.com/engine/installation/>`_.

Creación de la imagen Docker
^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Una vez instalado Docker, se deberá contar con el `proyecto <https://github.com/FedericoAmura/TallerII2016C2TPLinkedIn/tree/master/AppServer>`_ 
descargado y luego proceder a la instalación de dependencias (mediante el script
install-dependencies.sh) ya que el Dockerfile realiza una copia del directorio
raíz del proyecto. Luego, posicionarse en el directorio raíz del proyecto, que
cuenta con el archivo Dockerfile (que contiene las instrucciones para la
creación de la imagen Docker del servidor).
Para generar la imagen Docker del servidor, se deberá ejecutar el siguiente comando:

.. code-block:: bash

	$ docker build -t docker-appserver .

Corriendo el Servidor
^^^^^^^^^^^^^^^^^^^^^
Una vez creada la imagen Docker, se deberá correr dicha imagen utilizando el siguiente
comando:

.. code-block:: bash

 	$ docker run -i -t -p 8888:8888 docker-appserver

Los flags -i -t permiten ejecutar una terminal interactiva dentro del contenedor, con lo
cual si se desea detener el servidor puede ejecutar Ctrl + C y detenerlo.
