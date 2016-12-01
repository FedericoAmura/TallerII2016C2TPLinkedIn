Implementación
==============
El Application Server realiza tres funciones fundamentales que consisten en:

.. contents::
   :local:


Recepción y proceso de peticiones HTTP
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Para el manejo de peticiones, su procesamiento y respuesta a los clientes, se utilizó `Mongoose Web Server <https://docs.cesanta.com/mongoose/master/>`_ .
Esta herramienta provee una gran abstracción a la hora de usarla. Durante la llegada
de una nueva petición, se dispará un nuevo thread para atender tal petición. Dependiendo del tipo
de verbo utilizado, un handler tomará la petición para procesarla y luego de procesarla
devolverá una respuesta HTTP. Si la petición tiene datos de formulario, sólo se permitirán
datos en formato json, haciendo uso de `Json11 <https://github.com/dropbox/json11>`_
para su procesamiento.

Almacenamiento de datos relacionado con el usuario
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
El AppServer cuenta con la funcionalidad de almacenamiento de datos, ya sea datos de usuarios,
conversaciones, notificaciones, solicitudes, etc. Para esto, se utilizó `LevelDB <https://github.com/google/leveldb>`_ (desarrollado por Google),
una base de datos basada en pares de clave-valor. Mediante el uso de DBRaw se manejan los
accesos y seteos contra la base de datos.

Conexión con el Shared Server
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Para realizar peticiones al Shared Server se utilizó la libería de  `cURL <https://curl.haxx.se/>`_ ,
abstrayendo su interfaz mediante el uso del patrón de diseño Adapter. Se utiliza cURL, porque sólo
se realizan peticiones simples desde el AppServer al Shared (sólo GETs). Por otro lado, los
datos recibidos por parte del Shared llegan en formato json y para eso se utilizó `Json11 <https://github.com/dropbox/json11>`_
para procesarlos de manera eficiente.
