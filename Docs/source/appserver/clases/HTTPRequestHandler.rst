HTTPRequestHandler
==========================================

.. cpp:class:: HTTPRequestHandler

   Clase encargada de gestionar las peticiones HTTP.

----------------
Miembros
----------------

.. cpp:member::  http_request* HTTPRequestHandler::request

    Petición HTTP que se será gestionada.

.. cpp:member::  DB_Handler* HTTPRequestHandler::db_handler

    Handler de las bases de datos para gestionar información sobre los usuarios
    como información personal, conversaciones, etc.

.. cpp:member::  URI HTTPRequestHandler::uri

    Uri de la petición HTTP.

----------------
Métodos
----------------

.. cpp:function:: HTTPRequestHandler(http_request* req)

    Constructor.

    :param req: Petición HTTP que será gestionada.
    :returns: Devuelve un objeto de tipo HTTPRequestHandler.

.. cpp:function:: bool validToken()

    Validación del token de autenticación.

    :returns: Devuelve true si el token (si es que se encuentra en la cabecera de Autorización)
              es válido, sino false.

.. cpp:function:: virtual void handleResquest()

    Método virtual.

.. cpp:function:: void sendReply(http_response* res)

    Envía una respuesta HTTP.

    :param res: Contiene una respuesta y un código de respuesta HTTP.
    :returns: void.

.. cpp:function:: void closeConnection()

    Cierra una conexión.

    :returns: void.
