HTTPRequestHandler
==========================================

.. cpp:class:: HTTPRequestHandler

   Clase encargada de gestionar las peticiones HTTP.

----------------
Miembros
----------------

.. cpp:member::  http_request* HTTPRequestHandler::request

    Descripción del miembro.

.. cpp:member::  DB_Handler* HTTPRequestHandler::db_handler

    Descripción del miembro.

.. cpp:member::  URI HTTPRequestHandler::uri

    Descripción del miembro.

----------------
Métodos
----------------

.. cpp:function:: HTTPRequestHandler(http_request* req)

    Constructor

    :param req: Petición HTTP que se quiere procesar.
    :returns: Devuelve un objeto de tipo HTTPRequestHandler.

.. cpp:function:: bool validToken()

    Validación del token de autenticación.

    :returns: Devuelve true si el token (si es que se encuentra en el authorization header)
              es válido, sino false.

.. cpp:function:: void sendReply(http_response* res)

    Envía una respuesta HTTP.

    :param res: Contiene una respuesta y un código de respuesta.
    :returns: void.

.. cpp:function:: void closeConnection()

    Cierra una conexión.

    :returns: void.
